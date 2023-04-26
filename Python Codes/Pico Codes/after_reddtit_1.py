from machine import Pin, ADC
import utime
import gc
from ulab import numpy as np


# initialize ADC
MIC = ADC(0)
sample_count = 1
analog = []

samples = 4096
sampling_frequency = 8000/2.83

wave = []
vReal = np.zeros(samples, dtype=np.float)
vImag = np.zeros(samples, dtype=np.float)

def compute_magnitude(vReal, vImag):
    # Compute the magnitude of the complex numbers represented by vReal and vImag
    magnitude = np.sqrt(vReal**2 + vImag**2)
    return magnitude

while True:
    print("Listning...")
    while sample_count < 4097:
        wave.append(MIC.read_u16())
        utime.sleep_us(int(1000000/8000))
        sample_count+=1
    
    mean = np.mean(wave)
    
    for i in range(samples):
        vReal[i] = wave[i] - mean
        
    vImag = np.zeros(samples, dtype=np.float)

    vReal, vImag = np.fft.fft(vReal, vImag)
    
    # Compute the magnitude of the filtered signal
    vReal = compute_magnitude(vReal, vImag)
    
    peak_index = np.argmax(vReal)
    peak_freq = peak_index * sampling_frequency / samples
    
    print("Peak frequency: {:.2f} Hz".format(peak_freq))
    
    indices = np.argsort(vReal, axis=0)[::-1] # sort indices in descending order
    for i in range(5):
        idx = indices[i]
        power = vReal[idx]
        freq = idx * sampling_frequency / samples
        print("Frequency {:.2f} Hz has power {:.2f}".format(freq, power))
    
    print(f"finally free {gc.mem_free()}")
    print(f"=============================")
    for n,i in enumerate(wave):
        print(i, end=",")
        if n % 24 == 0:
            print()
            
    utime.sleep(5)