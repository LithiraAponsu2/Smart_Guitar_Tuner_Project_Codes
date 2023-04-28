from machine import Pin, ADC
import utime
import gc
from ulab import numpy as np

# initialize ADC
MIC = ADC(0)

samples = 4096
sampling_frequency = 8000/8.23

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
    while sample_count < samples:
        vReal[sample_count] = MIC.read_u16()
        utime.sleep_us(int(1000000/8000))
        sample_count+=1
    
    mean = np.mean(vReal)  # may need to expel to reduce computation by just assuming mean
    
    # remove DC shift
    for i in range(samples):
        vReal[i] = vReal[i] - mean
        
    for n,i in enumerate(vReal):
        print(i, end=",")
        if n % 24 == 0:
            print()
        
    utime.sleep(10)
    print("==========================================================")

    # only take magnitude grater than 10000 values until more 2000 values
    for i in range(samples):
        if vReal[i] > 10000:
            vImag[i:i + 1000] = vReal[i:i + 1000]
            break
        
    vReal[:] = vImag[:]
    vImag[:] = 0
    
    for n,i in enumerate(vReal):
        print(i, end=",")
        if n % 24 == 0:
            print()
        
    utime.sleep(10)
    print("==========================================================")
    
    # memory save method chat gpt
    # copy_len = min(samples - i, 2000)
    # if copy_len > 0:
    #     vReal[i:i + copy_len] = vImag[i:i + copy_len]
    #     vImag[i:i + copy_len] = 0
    #     i += copy_len

    
    vReal, vImag = np.fft.fft(vReal, vImag)
    
    # Compute the magnitude of the filtered signal
    vReal = compute_magnitude(vReal, vImag)
    
    
    
    # fft index array maximum 10 powers
    vReal = np.argsort(vReal, axis=0)
    max_10 = vReal[-10:]
    
    print(f"max={vReal[-1]*sampling_frequency / samples}")
    
    # max 10 between 50-500Hz
    # count = 0
    # for j in reversed(vReal):
    #     if count < 10:
    #         if 25 <= j <= 256:
    #             max_10[count] = j
    #             count+=1
    
    # frequencies of maximum 10 powersa
    for i in range(len(max_10)):
        max_10[i] = max_10[i] * sampling_frequency / samples    
        print(max_10[i], end="\n")
        
    # Trigger garbage collection
    # gc.collect()
        
    utime.sleep(5)
