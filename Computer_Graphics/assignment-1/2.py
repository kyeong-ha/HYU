import numpy as np

M = np.arange(2,27,1)
print(M)

M = np.reshape(M, (5, 5))
print(M)

M[:, 0] = 0
print(M)

M **= 2
print(M)

v = M[0, :]
print(np.sqrt(v@v))