import math
import cmath
import array
from ulab import numpy as np

arr = [0.0000 + 1.0000j, -0.8660 - 0.5000j, 0.8660 - 0.5000j, -0.0000 + 1.0000j]

a = [0.0000, -0.8660, 0.8660 , -0.0000]
a_ = np.array(a)
b = [1.0000,-0.5000,-0.5000, 1.0000]
b_ = np.array(b)

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



print(transform(arr))

c, d = np.fft.fft(a_, b_)
print(c)
print(d)
