#include "senoFM.h"
#include "keyvalue.h"
#include <cmath>    // Para funciones matemáticas como sin(), pow()
#include <iostream> // Para la salida estándar, si es necesario

using namespace upc;
using namespace std;

SenoFM::SenoFM(const std::string &param)
    : adsr(SamplingRate), adsr2(SamplingRate),
      adsr_a(0.0f), adsr_d(0.0f), adsr_s(0.0f), adsr_r(0.0f),
      adsr_a2(0.0f), adsr_d2(0.0f), adsr_s2(0.0f), adsr_r2(0.0f),
      index_step(0.0f), index_sen(0.0f)
{
    bActive = false;
    x.resize(BSIZE);

    // Parsear los parámetros utilizando KeyValue si es necesario
    KeyValue kv(param);

    // Asignar valores por defecto si los parámetros no están presentes
    if (!kv.to_int("N", N))
        N = 40; // Valor por defecto para N

    if (!kv.to_float("max_level", max_level))
        max_level = 0.02; // Valor por defecto para max_level

    // Inicializar otros parámetros de síntesis FM según sea necesario
    // Por ejemplo: N1, N2, I1, I2, setting, etc.

    // Configurar las envolventes ADSR con los parámetros correspondientes
    adsr.set(adsr_a, adsr_d, adsr_s, adsr_r, 1.5F);
    adsr2.set(adsr_a2, adsr_d2, adsr_s2, adsr_r2, 1.5F);

    // Inicializar otras variables de estado según sea necesario
}

void SenoFM::command(long cmd, long note, long vel)
{
    if (cmd == 9)
    { //'Key' pressed: attack begins
        bActive = true;
        adsr.start();
        adsr2.start();

        // Calcular la frecuencia fundamental de la nota a partir del número de semitono
        float f0note = pow(2, ((float)note - 69) / 12) * 440;

        // Calcular el período de la nota en muestras
        float Nnote = 1.0f / f0note * SamplingRate;

        // Calcular el paso de índice para relacionar el período de la tabla y el de la nota
        index_step = (float)N / Nnote;

        // Reiniciar contadores y fases
        index = 0;
        index_sen = 0;
        mod_phase = 0;
        decay_count = 0;
        decay_count_I = 0;

        // Calcular la frecuencia de modulación
        fm = f0note * N2 / N1;

        // Calcular el paso de la fase de modulación
        mod_phase_step = 2 * M_PI * fm / SamplingRate;

        // Redimensionar el buffer para un período de la señal
        note_int = round(Nnote);
        x_tm.resize(note_int);

        // Ajustar la amplitud según la velocidad de la nota
        if (vel > 127)
            vel = 127;
        A = vel / 127.0f;
    }
    else if (cmd == 8)
    { //'Key' released: sustain ends, release begins
        adsr.stop();
        adsr2.stop();
    }
    else if (cmd == 0)
    { //'Key' released: release faster, but don't end it abruptly
        adsr.set(adsr_s, adsr_a, adsr_d, adsr_r / 4, 1.5F);
        adsr.stop();
        adsr2.set(adsr_s2, adsr_a2, adsr_d2, adsr_r2 / 4, 1.5F);
        adsr2.stop();
    }
}

const vector<float> &SenoFM::synthesize()
{
    if (!adsr.active())
    {
        x.assign(x.size(), 0);
        bActive = false;
        return x;
    }
    else if (!bActive)
    {
        return x;
    }

    unsigned int index_floor, next_index; // Índices de interpolación
    float weight, weight_fm;              // Pesos de interpolación
    std::vector<float> I_array(x.size()); // Arreglo para almacenar el valor de I para cada muestra

    // Llenar el arreglo con el valor I constante del usuario
    for (unsigned int i = 0; i < x.size(); i++)
    {
        I_array[i] = (I2 - I1);

        // Aplicar la envolvente exponencial si está seleccionada
        if (setting > 0)
            I_array[i] = I_array[i] * pow(setting, decay_count_I);
    }

    // Aplicar la función variable en el tiempo al arreglo
    if (setting <= 0)
        adsr2(I_array);

    // Ajustar el valor de I para cada muestra
    for (unsigned int i = 0; i < x.size(); i++)
    {
        I_array[i] = (I1 + I_array[i]);
    }

    // Llenar x_tm con un período de la nueva señal
    for (unsigned int i = 0; i < (unsigned int)note_int; ++i)
    {
        // Verificar si el índice flotante está fuera de los límites
        if ((long unsigned int)floor(index) > tbl.size() - 1)
            index = index - floor(index);

        // Obtener el índice como entero
        index_floor = (int)floor(index);
        weight = index - index_floor;

        // Ajustar los índices de interpolación si es necesario
        if (index_floor == (unsigned int)N - 1)
        {
            next_index = 0;
            index_floor = N - 1;
        }
        else
        {
            next_index = index_floor + 1;
        }

        // Interpolar los valores de la tabla
        x_tm[i] = ((1 - weight) * tbl[index_floor] + (weight)*tbl[next_index]);

        // Actualizar el índice real
        index = index + index_step;
    }

    // Modular la señal
    for (unsigned int i = 0; i < x.size(); ++i)
    {
        // Verificar si el índice de modulación está fuera de los límites
        if (index_sen < 0)
        {
            index_sen = note_int + index_sen;
        }
        if ((int)floor(index_sen) > note_int - 1)
        {
            index_sen = index_sen - (note_int - 1);
        }

        // Obtener el índice como entero
        int index_floor_fm = floor(index_sen);
        float weight_fm = index_sen - index_floor_fm;

        // Ajustar los índices de interpolación si es necesario
        if (index_floor_fm == note_int - 1)
        {
            next_index = 0;
            index_floor_fm = note_int - 1;
        }
        else
        {
            next_index = index_floor_fm + 1;
        }

        // Interpolar los valores de la tabla
        x[i] = A * ((1 - weight_fm) * x_tm[index_floor_fm] + weight_fm * (x_tm[next_index]));

        // Actualizar el índice real (fase) y la fase modulada
        index_sen = index_sen + 1 - I_array[i] * sin(mod_phase);
        mod_phase = mod_phase + mod_phase_step;
    }

    // Asegurarse de que la fase modulada esté dentro del rango [-pi, pi]
    while (mod_phase > M_PI)
        mod_phase -= 2 * M_PI;

    // Aplicar la envolvente exponencial o ADSR
    for (unsigned int i = 0; i < x.size(); i++)
    {
        if (setting != 0)
        {
            if (setting > 0) // Decrecer exponencialmente según el setting
            {
                x[i] = x[i] * max_level * pow(setting, decay_count);
                decay_count++;
            }
            else if (adsr.active() && setting == -1)
            {
                x[i] = x[i] * adsr_s; // Ajustar el nivel para evitar que el ataque supere el umbral de sustain
            }
        }
        else
            x[i] = x[i] * max_level; // Ajustar el nivel máximo para evitar la saturación al superponer múltiples señales
    }

    // Aplicar la envolvente ADSR a x y actualizar el estado interno de ADSR si es necesario
    if (setting <= 0)
        adsr(x);

    return x;
}
