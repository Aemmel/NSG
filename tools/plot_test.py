import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.gridspec import GridSpec
import matplotlib.animation as animation
from mpl_toolkits.mplot3d import Axes3D
from matplotlib import rcParams
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
# preliminary work
fig = plt.figure()
#ax = fig.add_subplot(111)
ax_3D = fig.add_subplot(111, projection="3d")
ax_3D.view_init(12, -105)

x_array = np.linspace(0, obj["cell_cnt_x"]*obj["dx"], obj["cell_cnt_x"] - 2)
y_array = np.linspace(0, obj["cell_cnt_y"]*obj["dy"], obj["cell_cnt_y"] - 2)

X_mesh, Y_mesh = np.meshgrid(x_array, y_array)

def plot_test(file, save, show=True):
    ################################
    # read in data
    data = np.array(pd.read_csv("out/nsg_p_0.000000.dat", delimiter="\t", header=None))

    ax_3D.plot_surface(X_mesh, Y_mesh, data, cmap="plasma")
    # Z = np.sin(4*X)*np.cos(4*Y)
    Z = np.sin(X_mesh+Y_mesh)
    # Z = X*X*Y + Y*Y*X + X*Y
    # Z = np.sin(X)
    ax_3D.plot_wireframe(X_mesh, Y_mesh, Z, color="red", rcount=10, ccount=10)
    #ax_3D.plot_surface(X_mesh, Y_mesh, Z, cmap="plasma")

    #plt.legend(loc="best")

    if save == True:
        plt.savefig("plots/" + file + ".pdf")

    if show == True:
        plt.show()

plot_test("test_PDE_num", False, True)