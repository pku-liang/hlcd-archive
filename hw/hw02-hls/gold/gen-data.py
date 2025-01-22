import numpy as np

def gen(r, c):
    return np.trunc(np.random.rand(r, c) * (1<<8)) / (1<<3)

A = np.float64(gen(64, 64))
B = np.float64(gen(64, 64))
C = A @ B
data = np.concat([A, B, C], axis=0)
np.savetxt('data.txt', data, fmt='%.8e')
