import pandas as pd
import numpy as np

def normL2(mat):
    res = 0
    for i in mat:
        for j in i:
            res += j**2
    res /= np.shape(mat)[0] * np.shape(mat)[1]

    return np.sqrt(res)

path_h = "out/Convergence_test_dx=5e-3/"
path_2h = "out/Convergence_test_dx=1e-2/"
path_4h = "out/Convergence_test_dx=2e-2/"
# path_h = "out/Convergence_test_dx=1e-2/"
# path_2h = "out/Convergence_test_dx=2e-2/"
# path_4h = "out/Convergence_test_dx=4e-2/"

print(path_h)

# path_h = "playground/h/"
# path_2h = "playground/2h/"
# path_4h = "playground/4h/"

# step_size h
u_h_l = np.array(pd.read_csv(path_h + "nsg_u.dat", delimiter="\t", header=None))[3:-3, 3:-3]
v_h_l = np.array(pd.read_csv(path_h + "nsg_v.dat", delimiter="\t", header=None))[3:-3, 3:-3]
p_h_l = np.array(pd.read_csv(path_h + "nsg_p.dat", delimiter="\t", header=None))[3:-3, 3:-3]

# step_size 2h
u_2h_l = np.array(pd.read_csv(path_2h + "nsg_u.dat", delimiter="\t", header=None))[1:-1, 1:-1]
v_2h_l = np.array(pd.read_csv(path_2h + "nsg_v.dat", delimiter="\t", header=None))[1:-1, 1:-1]
p_2h_l = np.array(pd.read_csv(path_2h + "nsg_p.dat", delimiter="\t", header=None))[1:-1, 1:-1]

# step_size 4h
u_4h = np.array(pd.read_csv(path_4h + "nsg_u.dat", delimiter="\t", header=None))
v_4h = np.array(pd.read_csv(path_4h + "nsg_v.dat", delimiter="\t", header=None))
p_4h = np.array(pd.read_csv(path_4h + "nsg_p.dat", delimiter="\t", header=None))

# create arrays with correct sizes
u_h  = np.zeros(np.shape(u_4h))
v_h  = np.zeros(np.shape(u_4h))
p_h  = np.zeros(np.shape(u_4h))
u_2h = np.zeros(np.shape(u_4h))
v_2h = np.zeros(np.shape(u_4h))
p_2h = np.zeros(np.shape(u_4h))


# u_2h by mean of 2x2 elements
for i in range(len(u_2h)):
    for j in range(len(u_2h[i])):
        u_2h[i][j] = np.mean(u_2h_l[2*i:(2*i+2), 2*j:(2*j+2)])
        v_2h[i][j] = np.mean(v_2h_l[2*i:(2*i+2), 2*j:(2*j+2)])
        p_2h[i][j] = np.mean(p_2h_l[2*i:(2*i+2), 2*j:(2*j+2)])

# u_h by mean of 4x4 elements
for i in range(len(u_h)):
    for j in range(len(u_h[i])):
        u_h[i][j] = np.mean(u_h_l[4*i:(4*i+4), 4*j:(4*j+4)])
        v_h[i][j] = np.mean(v_h_l[4*i:(4*i+4), 4*j:(4*j+4)])
        p_h[i][j] = np.mean(p_h_l[4*i:(4*i+4), 4*j:(4*j+4)])

print("for u: " + str(np.log2( normL2(u_4h - u_2h) / normL2(u_2h - u_h) )))
print("for v: " + str(np.log2( normL2(v_4h - v_2h) / normL2(v_2h - v_h) )))
print("for p: " + str(np.log2( normL2(p_4h - p_2h) / normL2(p_2h - p_h) )))