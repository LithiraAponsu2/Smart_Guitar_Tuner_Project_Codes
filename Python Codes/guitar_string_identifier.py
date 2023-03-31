# 1. Record audio signal: Use a microphone and a Python library such as PyAudio to record the audio signal from the guitar.

# 2. Filter the signal: Apply a bandpass filter to the audio signal to remove any unwanted frequencies and isolate the frequencies that correspond to the guitar strings. The cutoff frequencies for the bandpass filter can be determined based on the frequency range of each guitar string.

# 3. Analyze the signal: Use a signal processing algorithm to analyze the filtered audio signal and determine which guitar string is being played. One approach is to use a spectral analysis algorithm such as the Fast Fourier Transform (FFT) to compute the frequency spectrum of the signal. The guitar string that is being played will correspond to the frequency component with the highest amplitude in the spectrum.

# 4. Determine the string: Once you have computed the frequency spectrum of the signal, you can determine which guitar string is being played by comparing the frequency component with the highest amplitude to the expected frequency range for each guitar string.

import numpy as np
from scipy.io import wavfile
from scipy.fft import fft

sample_rate, audio_data = wavfile.read('Python Codes\E_note.wav')
# print(sample_rate)
# print(audio_data)

# audio_data = np.mean(audio_data, axis=1)  # sterio to mono
frequencies = fft(audio_data)  # fft transform extract frequencies
frequencies = np.abs(frequencies)  # only take absolute among complex
# print(frequencies)
frequency = np.argmax(frequencies) 
# print(frequency)# get index of maximum value among them

frequency_in_hertz = frequency * sample_rate / len(audio_data)  # convert to herts

print(frequency_in_hertz)
# 329.63 Hz - Eh 
# 246.94 Hz - B
# 196.00 Hz - G
# 146.83 Hz - D
# 110.00 Hz - A
# 82.41 Hz - El