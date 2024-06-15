import numpy as np
import matplotlib.pyplot as plt

# Constants
N = 40
PI = np.pi

# Generate the sinewave signal
phase = 0
step = 2 * PI / N
tbl = np.zeros(N)

for i in range(N):
    tbl[i] = np.sin(phase)
    phase += step

# Create another array with every other sample
x = tbl[::2]

# Plotting
plt.figure()
plt.plot(tbl, 'ro', label='tbl[n]')  # red dots
plt.plot(range(0, N, 2), x, 'bo', label='x[n]')  # blue dots
plt.xlabel('Muestras')
plt.ylabel('Amplitud')
plt.title('tbl[n] vs x[n] con f0 = 2*SamplingRate')
plt.legend()
plt.grid(True)
plt.show()
