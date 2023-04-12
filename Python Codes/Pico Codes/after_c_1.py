from ulab import numpy as np
from machine import Pin, ADC
import utime

MIC = ADC(Pin(26))

samples = 1024
sampling_frequency = 48000

wave = np.zeros(samples, dtype=np.int16)
vReal = np.zeros(samples, dtype=np.float)
vImag = np.zeros(samples, dtype=np.float)

def convert_to_voltage(raw_value):
    # return (raw_value - 2048) * 3.3 / 4096.0
    return (raw_value - 32768) * 3.3 / 65536

def hamming_window(arr):
    n = len(arr)
    hamming = 0.54 - 0.46 * np.cos(2 * np.pi * np.arange(n) / (n - 1))
    return arr * hamming

def compute_magnitude(vReal, vImag):
    # Compute the magnitude of the complex numbers represented by vReal and vImag
    magnitude = np.sqrt(vReal**2 + vImag**2)
    return magnitude

def dc_removal(arr):
    """
    Removes DC offset from a 1D NumPy array.
    """
    mean = np.mean(arr)
    arr -= mean
    return arr

while True:
    for i in range(samples):
        wave[i] = -MIC.read_u16()
        #print(wave[i])
        utime.sleep_us(int(1000000/sampling_frequency))
    #print(wave)
    
    for i in range(samples):
        vReal[i] = convert_to_voltage(wave[i])
        
    vImag = np.zeros(samples, dtype=np.float)
    
    #np.fft.dc_removal(vReal)
    #vReal = dc_removal(vReal)
    #np.fft.hamming(vReal)
    vReal = hamming_window(vReal)
    vReal, vImag = np.fft.fft(vReal, vImag)
    vReal = compute_magnitude(vReal, vImag)
    print(vReal)
    peak_index = np.argmax(vReal)
    peak_freq = peak_index * sampling_frequency / samples
    
    print("Peak frequency: {:.2f} Hz".format(peak_freq))
    
    indices = np.argsort(vReal, axis=0)[::-1] # sort indices in descending order
    for i in range(5):
        idx = indices[i]
        power = vReal[idx]
        freq = idx * sampling_frequency / samples
        print("Frequency {:.2f} Hz has power {:.2f}".format(freq, power))
        
    utime.sleep(5)
