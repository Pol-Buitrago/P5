import os
import numpy as np
import matplotlib.pyplot as plt
from scipy.io import wavfile
from scipy import signal

# Ruta del archivo de audio procesado con el efecto Fuzz
file_path = os.path.join('work', 'audio_con_fuzz.wav')

# Función para visualizar la forma de onda y el espectrograma del archivo de audio
def visualize_audio_effects(file_path):
    # Cargar archivo de audio
    sampling_rate, audio_data = wavfile.read(file_path)

    # Generar eje de tiempo en segundos
    total_samples = len(audio_data)
    total_time = total_samples / sampling_rate
    time_axis = np.linspace(0, total_time, total_samples)

    # Plotear forma de onda
    plt.figure(figsize=(15, 6))
    plt.subplot(2, 1, 1)
    plt.plot(time_axis, audio_data, color='b')
    plt.title('Audio con Efecto Fuzz - Forma de Onda')
    plt.xlabel('Tiempo (s)')
    plt.ylabel('Amplitud')

    # Plotear espectrograma
    plt.subplot(2, 1, 2)
    f, t, Sxx = signal.spectrogram(audio_data, fs=sampling_rate)
    plt.pcolormesh(t, f, 10 * np.log10(Sxx), shading='gouraud')
    plt.title('Audio con Efecto Fuzz - Espectrograma')
    plt.ylabel('Frecuencia (Hz)')
    plt.xlabel('Tiempo (s)')
    plt.colorbar().set_label('Intensidad (dB)')

    plt.tight_layout()
    plt.show()

# Llamar a la función para visualizar el archivo de audio con efecto Fuzz
visualize_audio_effects(file_path)
