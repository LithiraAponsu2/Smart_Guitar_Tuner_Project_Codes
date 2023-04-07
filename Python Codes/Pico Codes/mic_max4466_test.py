import utime
import ustruct
import machine
import math

def fft(samples):
    n = len(samples)
    assert n & (n - 1) == 0, "Sample count must be a power of 2"
    if n == 1:
        return [samples[0]]
    even = fft(samples[0::2])
    odd =  fft(samples[1::2])
    return [even[k] + math.e**(-2j*math.pi*k/n)*odd[k] for k in range(n//2)] + \
           [even[k] - math.e**(-2j*math.pi*k/n)*odd[k] for k in range(n//2)]

# Set up the ADC pin
mic_pin = machine.Pin(26, machine.Pin.IN, machine.Pin.PULL_DOWN)
mic = machine.ADC(mic_pin)

# Set up the recording parameters
duration = 5 # recording duration in seconds
samples_per_second = 1000
sample_count = duration * samples_per_second

print("Recording for %d seconds..." % duration)

# Start the recording
samples = bytearray(sample_count * 2) # each sample is 2 bytes (16 bits)
for i in range(sample_count):
    sample = mic.read_u16()
    samples[i*2] = sample & 0xff # low byte
    samples[i*2+1] = sample >> 8 # high byte

print("Recording complete. Performing FFT...")

# Perform FFT on the samples
complex_samples = [complex(sample - 32768) for sample in ustruct.unpack_from("<h", samples)]
spectrum = fft(complex_samples)

print("FFT complete. Finding peak frequency...")

# Find the peak frequency
max_magnitude = 0
peak_frequency = 0
frequencies = []
for i, freq_bin in enumerate(spectrum):
    magnitude = abs(freq_bin)
    frequency = i * samples_per_second / sample_count
    frequencies.append(frequency)
    if magnitude > max_magnitude:
        max_magnitude = magnitude
        peak_frequency = frequency

print("Peak frequency: %.1f Hz" % peak_frequency)
print("Frequencies present: ", frequencies)
