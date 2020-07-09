import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.gridspec import GridSpec
from mpl_toolkits.mplot3d import Axes3D
import json

# configure pyplot
plt.rc("text", usetex=True)
plt.rc("font", family="serif")

def setup_ax(ax):
    ax.tick_params(direction="in")          # ticks inside
    ax.xaxis.set_ticks_position("both")     # ticks on lower and upper x axis
    ax.yaxis.set_ticks_position("both")     # ticks on left and right y axis
    ax.tick_params(grid_alpha=0.5)

################################
# read in json
with open("data/options.json", "r") as json_file:
    data = json_file.read()
obj = json.loads(data)


################################
# read in data
#data_u = np.array(pd.read_csv("out/nsg_u_3.000064.dat", delimiter="\t", header=None))
#data_v = np.array(pd.read_csv("out/nsg_v_3.000064.dat", delimiter="\t", header=None))
data_p = np.array(pd.read_csv("out/nsg_p_0.000000.dat", delimiter="\t", header=None), dtype=np.float64)

#data_test_func = np.transpose(np.array(pd.read_csv("out/nsg_test_file.dat", delimiter="\t", header=None)))
#data_test_func = np.array(pd.read_csv("out/nsg_test_file.dat", delimiter="\t", header=None))

x = np.linspace(0, obj["cell_cnt_x"]*obj["dx"], obj["cell_cnt_x"] - 2)
y = np.linspace(0, obj["cell_cnt_y"]*obj["dy"], obj["cell_cnt_y"] - 2)
X,Y = np.meshgrid(x, y)

#x_data = x
x_data = y

#plt.plot(x_data, data_test_func, label="numeric")
#plt.plot(x_data, np.sin(x_data)*np.cos(x_data), label="orig")
#plt.plot(x_data, np.cos(x_data)**2 - np.sin(x_data)**2, label="analytic")


#fig, ax = plt.subplots()

# quiver (vector-field)
#q = ax.quiver(X, Y, data_u, data_v, units="width")

# streamplot (integralcurve)
#ax.streamplot(X, Y, data_u, data_v, density=2)

fig = plt.figure()
ax = fig.add_subplot(111, projection="3d")

ax.plot_surface(X, Y, data_p)
Z = np.sin(X)*np.cos(Y)
#Z = X*X*Y + Y*Y*X + X*Y
#Z = np.sin(X)
ax.plot_wireframe(X, Y, Z, color="red", rcount=10, ccount=10)

plt.legend(loc="best")

#plt.savefig("plots/")

plt.show()