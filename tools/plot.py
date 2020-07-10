import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.gridspec import GridSpec
import matplotlib.animation as animation
from mpl_toolkits.mplot3d import Axes3D
from matplotlib import rcParams
import json
from os import listdir
from os.path import isfile, join

# configure pyplot
plt.rc("text", usetex=True)
plt.rc("font", family="serif")
rcParams["animation.convert_path"] = r"C:/Program Files/ImageMagick-7.0.8-Q16/magick.exe"

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
# store file names
file_names = [join("out", f) for f in listdir("out") if isfile(join("out", f))] # read in file names, ignore directories

file_names_u = [f for f in file_names if "nsg_u" in f]
file_names_v = [f for f in file_names if "nsg_v" in f]
file_names_p = [f for f in file_names if "nsg_p" in f]

# should be sorted, but better to check
if not sorted(file_names_u) or not sorted(file_names_v) or not sorted(file_names_p):
    print("ERROR: File names aren't sorted!")
    exit()

# p_dat = np.zeros(len(file_names_p))
# for i in range(len(file_names_p)):
#     data_p = np.array(pd.read_csv(file_names_p[i], delimiter="\t", header=None))

#     p_dat[i] = data_p[20][20]

# plt.plot(np.arange(0, len(p_dat)), p_dat)

# plt.show()
# exit()

################################
# preliminary work
fig = plt.figure()
ax = fig.add_subplot(111)
#ax_3D = fig.add_subplot(111, projection="3d")

x_array = np.linspace(0, obj["cell_cnt_x"]*obj["dx"], obj["cell_cnt_x"] - 2)
y_array = np.linspace(0, obj["cell_cnt_y"]*obj["dy"], obj["cell_cnt_y"] - 2)

X_mesh, Y_mesh = np.meshgrid(x_array, y_array)


################################
# functions for animating
def animate_pressure(i, plot):
    data_p = np.array(pd.read_csv(file_names_p[i], delimiter="\t", header=None))

    plot[0].remove()
    plot[0] = ax_3D.plot_surface(X_mesh, Y_mesh, data_p, color="cyan")

    return plot

def animate_velocity(i):
    data_u = np.array(pd.read_csv(file_names_u[i], delimiter="\t", header=None))
    data_v = np.array(pd.read_csv(file_names_v[i], delimiter="\t", header=None))

    ax.collections = [] # clear lines
    ax.patches = [] # clear arrowheads

    stream = ax.streamplot(X_mesh, Y_mesh, data_u, data_v, density=2, color="blue")
    return stream

################################
# read in data
#data_u = np.array(pd.read_csv("out/nsg_u_3.803066.dat", delimiter="\t", header=None))
#data_v = np.array(pd.read_csv("out/nsg_v_3.803066.dat", delimiter="\t", header=None))
#data_p = np.array(pd.read_csv("out/nsg_p_3.201932.dat", delimiter="\t", header=None))

#fig, ax = plt.subplots()

# quiver (vector-field)
#q = ax.quiver(X, Y, data_u, data_v, units="width")

# streamplot (integralcurve)
#ax.streamplot(X, Y, data_u, data_v, density=2)

#fig = plt.figure()
#ax = fig.add_subplot(111, projection="3d")

#ax.plot_surface(X, Y, data_p)
#Z = np.sin(4*X)*np.cos(4*Y)
#Z = np.sin(X+Y)
#Z = X*X*Y + Y*Y*X + X*Y
#Z = np.sin(X)
#ax.plot_wireframe(X, Y, Z, color="red", rcount=10, ccount=10)

#plt.legend(loc="best")

#plt.savefig("plots/")

#plot_pressure = [ax_3D.plot_surface(X_mesh, Y_mesh, X_mesh, color="cyan")] # first plot, so it knows what kind of object it is
# ax.collections = [] # clear lines
# ax.patches = [] # clear arrowheads

#ani = animation.FuncAnimation(fig, animate_pressure, np.arange(0, len(file_names_p)), fargs=(plot_pressure,), interval=250)
ani = animation.FuncAnimation(fig, animate_velocity, np.arange(0, len(file_names_u)), interval=250)

#writer_gif = animation.PillowWriter(fps=20)

#ani.save("plots/test_pressure.mp4", writer="imagemagick")

plt.show()