from machine import Pin, ADC
import utime
import gc
from ulab import numpy as np

# initialize ADC
MIC = ADC(0)

samples = 4096
sampling_frequency = 8000/2.83

# initialising arrays
vReal = np.zeros(samples, dtype=np.float)  # store ADC values, also store Real parts of FFT
vImag = np.zeros(samples, dtype=np.float)
max_10 = np.zeros(10, dtype=np.float)

# function to calculate magnitude of fft
def compute_magnitude(vReal, vImag):
    # Compute the magnitude of the complex numbers represented by vReal and vImag
    magnitude = np.sqrt(vReal**2 + vImag**2)
    return magnitude

# memorise sample_count
sample_count = 0

while True:
    print("Listning...")
    while sample_count < 4096:
        vReal[sample_count] = MIC.read_u16()
        utime.sleep_us(int(1000000/8000))
        sample_count+=1
    
    mean = np.mean(vReal)  # may need to expel to reduce computation by just assuming mean
    
    # remove DC shift
    for i in range(samples):
        vReal[i] = vReal[i] - mean

    vReal, vImag = np.fft.fft(vReal, vImag)
    
    # Compute the magnitude of the filtered signal
    vReal = compute_magnitude(vReal, vImag)
    
    # fft index array maximum 10 powers
    vReal = np.argsort(vReal, axis=0)
    max_10 = vReal[-10:]
    
    # frequencies of maximum 10 powersa
    for i in range(len(max_10)):
        max_10[i] = max_10[i] * sampling_frequency / samples    
        print(max_10[i], end="\n")
        
    # Trigger garbage collection
    gc.collect()
        
    utime.sleep(5)