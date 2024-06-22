#include <vector>
#include <string>
#include "instrument.h"
#include "envelope_adsr.h" // Asegúrate de incluir el archivo de la envolvente ADSR si lo necesitas

namespace upc
{
    class SenoFM : public upc::Instrument
    {
        EnvelopeADSR adsr, adsr2; // Envolturas ADSR para amplitud y modulación
        float index, N1, N2, setting; // Parámetros específicos de la síntesis FM
        int N, decay_count, decay_count_I, note_int; // Variables para control de ciclo y contadores
        float A, max_level; // Amplitud máxima y nivel máximo de la señal
        std::vector<float> tbl, x_tm; // Tabla de formas de onda y buffer para un período de la señal
        long double mod_phase, mod_phase_step; // Fase de modulación y paso de la fase
        float fm, I1, I2; // Frecuencia de modulación y índices de modulación

        // Definir parámetros de ADSR
        float adsr_a, adsr_d, adsr_s, adsr_r;
        float adsr_a2, adsr_d2, adsr_s2, adsr_r2;
        
        // Definir parámetros de índices
        float index_step;
        float index_sen;

    public:
        SenoFM(const std::string &param = ""); // Constructor
        void command(long cmd, long note, long velocity = 1); // Método para gestionar comandos (ataque, liberación, etc.)
        const std::vector<float> &synthesize(); // Método para sintetizar la señal
        bool is_active() const { return bActive; } // Método para verificar si el instrumento está activo

    private:
        bool bActive; // Flag para indicar si el instrumento está activo
    };
}
