import math
import cmath
import array
import time
import machine
import gc

gc.collect()
print(gc.mem_free())

SAMPLE_RATE_HZ = 500
RECORD_TIME_S = 2

# Set up ADC
# SAMPLE_WIDTH_BITS = 12
# ADC_RESOLUTION = (2 ** SAMPLE_WIDTH_BITS) - 1
adc = machine.ADC(26)

# Set up array to hold audio samples
NUM_SAMPLES = SAMPLE_RATE_HZ * RECORD_TIME_S
samples = array.array('H', (0 for _ in range(NUM_SAMPLES)))  # H - unsigned short integer 0-65535

# Record audio samples
print("Recording...")
# start_time = time.time()
for i in range(NUM_SAMPLES):
    samples[i] = adc.read_u16()
    time.sleep(1/SAMPLE_RATE_HZ)
# print(f"Recording done in {time.time() - start_time} seconds.")
print(len(samples))

# FFT function
# def fft(x):
#     N = len(x)
#     if N == 1:
#         return x
#     M = 2
#     while M <= N:
#         M <<= 1
#     M >>= 1
#     xe = fft(x[0:N:2])
#     xo = fft(x[1:N:2])
#     d = [math.exp(-2*math.pi*1*k/N)*xo[k] for k in range(M)]
#     return [xe[k] + d[k] for k in range(M)] + \
#            [xe[k] - d[k] for k in range(M)]

##################################################################################################
# 
# Computes the discrete Fourier transform (DFT) of the given complex vector, returning the result as a new vector.
# Set 'inverse' to True if computing the inverse transform. This DFT does not perform scaling, so the inverse is not a true inverse.
# The vector can have any length. This is a wrapper function.
# 
def transform(vector, inverse=False):
    n = len(vector)
    if n > 0 and n & (n - 1) == 0:  # Is power of 2
        return transform_radix2(vector, inverse)
    else:  # More complicated algorithm for aribtrary sizes
        return transform_bluestein(vector, inverse)


# 
# Computes the discrete Fourier transform (DFT) of the given complex vector, returning the result as a new vector.
# The vector's length must be a power of 2. Uses the Cooley-Tukey decimation-in-time radix-2 algorithm.
# 
def transform_radix2(vector, inverse):
    # Initialization
    n = len(vector)
    levels = _log2(n)
    exptable = [cmath.exp((2j if inverse else -2j) * cmath.pi * i / n) for i in range(n//2)]  # n must be divisible of 2, I added // to replace error
    vector = [vector[_reverse(i, levels)] for i in range(n)]  # Copy with bit-reversed permutation
    
    # Radix-2 decimation-in-time FFT
    size = 2
    while size <= n:
        halfsize = size // 2
        tablestep = n / size
        for i in range(0, n, size):
            k = 0
            for j in range(i, i + halfsize):
                temp = vector[j + halfsize] * exptable[int(k)]
                vector[j + halfsize] = vector[j] - temp
                vector[j] += temp
                k += tablestep
        size *= 2
    return vector


# 
# Computes the discrete Fourier transform (DFT) of the given complex vector, returning the result as a new vector.
# The vector can have any length. This requires the convolution function, which in turn requires the radix-2 FFT function.
# Uses Bluestein's chirp z-transform algorithm.
# 
def transform_bluestein(vector, inverse):
    # Find a power-of-2 convolution length m such that m >= n * 2 + 1
    n = len(vector)
    m = 1
    while m < n * 2 + 1:
        m *= 2
    
    exptable = [cmath.exp((1j if inverse else -1j) * cmath.pi * (i * i % (n * 2)) / n) for i in range(n)]  # Trigonometric table
    a = [x * y for (x, y) in zip(vector, exptable)] + [0] * (m - n)  # Temporary vectors and preprocessing
    b = [(exptable[min(i, m - i)] if (i < n or m - i < n) else 0) for i in range(m)]
    # b = [(exptable[min(i, m - i)].conjugate() if (i < n or m - i < n) else 0) for i in range(m)]
    c = convolve(a, b, False)[:n]  # Convolution
    for i in range(n):  # Postprocessing
        c[i] *= exptable[i]
    return c


# 
# Computes the circular convolution of the given real or complex vectors, returning the result as a new vector. Each vector's length must be the same.
# realoutput=True: Extract the real part of the convolution, so that the output is a list of floats. This is useful if both inputs are real.
# realoutput=False: The output is always a list of complex numbers (even if both inputs are real).
# 
def convolve(x, y, realoutput=True):
    assert len(x) == len(y)
    n = len(x)
    x = transform(x)
    y = transform(y)
    for i in range(n):
        x[i] *= y[i]
    x = transform(x, inverse=True)
    
    # Scaling (because this FFT implementation omits it) and postprocessing
    if realoutput:
        for i in range(n):
            x[i] = x[i].real / n
    else:
        for i in range(n):
            x[i] /= n
    return x


# Returns the integer whose value is the reverse of the lowest 'bits' bits of the integer 'x'.
def _reverse(x, bits):
    y = 0
    for i in range(bits):
        y = (y << 1) | (x & 1)
        x >>= 1
    return y


# Returns the integer y such that 2^y == x, or raises an exception if x is not a power of 2.
def _log2(x):
    i = 0
    while True:
        if 1 << i == x:
            return i
        elif 1 << i > x:
            raise ValueError("Not a power of 2")
        else:
            i += 1

#########################################################################################

# Compute power spectrum
print("Computing power spectrum...")
# start_time = time.time()
mag = []
freqs = []
X = transform(samples)
for i in range(NUM_SAMPLES // 2):
    mag.append(math.sqrt(X[i].real**2 + X[i].imag**2))
    freqs.append(i * (SAMPLE_RATE_HZ / NUM_SAMPLES))
# print(f"Power spectrum computed in {time.time() - start_time} seconds.")

# Find index of frequency with highest power
print("Finding frequency with highest power...")
start_time = time.time()
max_mag = max(mag)
max_idx = mag.index(max_mag)
freq_hz = freqs[max_idx]
print(f"Frequency with highest power: {freq_hz} Hz.")
# print(f"Frequency found in {time.time() - start_time} seconds.")
