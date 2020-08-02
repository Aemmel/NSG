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
import re

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
time_steps = [float(re.search(r"nsg_p_((\d|\.)+).dat", f).group(1)) for f in file_names_p]

# should be sorted, but better to check
if not sorted(file_names_u) or not sorted(file_names_v) or not sorted(file_names_p) or not sorted(time_steps):
    print("ERROR: File names aren't sorted!")
    exit()

################################
# preliminary work
# fig = plt.figure()
# ax = fig.add_subplot(111)
#ax_3D = fig.add_subplot(111, projection="3d")

x_array = np.linspace(0, obj["cell_cnt_x"]*obj["dx"], obj["cell_cnt_x"] - 2)
y_array = np.linspace(0, obj["cell_cnt_y"]*obj["dy"], obj["cell_cnt_y"] - 2)

X_mesh, Y_mesh = np.meshgrid(x_array, y_array)

################################
# functions for animating
def animate_pressure(i):
    data_p = np.array(pd.read_csv(file_names_p[i], delimiter="\t", header=None))

    im.set_array(data_p)
    return [im]

def animate_velocity(i):
    data_u = np.array(pd.read_csv(file_names_u[i], delimiter="\t", header=None))
    data_v = np.array(pd.read_csv(file_names_v[i], delimiter="\t", header=None))

    ax.collections = [] # clear lines
    ax.patches = [] # clear arrowheads

    stream = ax.streamplot(X_mesh, Y_mesh, data_u, data_v, density=1.2, color="blue")
    return [stream]

def animate_both(i):
    return animate_pressure(i), animate_velocity(i)

################################
# variables to control what to plot

# options are "none", "p", "v", "pv"
ANIMATE_WHAT = "none"
# slow down factor for animation (1 would be print_every from options.json)
ANIMATE_SLOW_FACTOR = 2
# title of MP4, if p, v or pv is added is done automatically
ANIMATE_TITLE = ""

# plot velocity. Array of time values to plot (it takes the closest values)
# if it's -1, then don't plot
PLOT_FLUID = [ 1.0, 2.0 ]
# plot velocity or pressure or both for each PLOT_FLUID
PLOT_VELOCITY = True
PLOT_PRESSURE = True
# "streamplot (and/or) heat map: PLOT_TITLE, t=..."
PLOT_TITLE = ""
# plot save file (without timestep) or if pressure or velocity is plotted
PLOT_SAVE_NAME = ""
# save the plots?
PLOT_SAVE = True

################################
# plot regarding to variables above
animation_func = None
if ANIMATE_WHAT == "p":
    animation_func = animate_pressure
    ANIMATE_TITLE += "_p"
elif ANIMATE_WHAT == "v":
    animation_func = animate_velocity
    ANIMATE_TITLE += "_v"
elif ANIMATE_WHAT == "pv":
    animation_func = animate_both
    ANIMATE_TITLE += "_pv"

if ANIMATE_WHAT != "none":
    fig = plt.figure()
    ax = fig.add_subplot(111)
    im = plt.imshow(np.array(pd.read_csv(file_names_p[0], delimiter="\t", header=None)), animated=True, extent=(0, obj["cell_cnt_x"]*obj["dx"], 0, obj["cell_cnt_y"]*obj["dy"]), cmap="viridis")

    fps = obj["print_every"]*1000 * ANIMATE_SLOW_FACTOR # stored in s, convert to ms and slow down

    ani = animation.FuncAnimation(fig, animation_func, np.arange(0, len(file_names_u)), interval=fps)
    ani.save("plots/" + ANIMATE_TITLE + ".mp4", writer="imagemagick")

for t in PLOT_FLUID:
    closest_time_step = 0
    # technically not closest but ceiling... but I don't care
    for i in range(len(time_steps)):
        if time_steps[i] >= t:
            closest_time_step = i
            break

    data_u = np.array(pd.read_csv(file_names_u[closest_time_step], delimiter="\t", header=None))
    data_v = np.array(pd.read_csv(file_names_v[closest_time_step], delimiter="\t", header=None))
    data_p = np.array(pd.read_csv(file_names_p[closest_time_step], delimiter="\t", header=None))

    # plot it
    fig = plt.figure()
    ax = fig.add_subplot(111)
    if PLOT_VELOCITY == True:
        ax.streamplot(X_mesh, Y_mesh, data_u, data_v, density=1.5, color="blue")
    if PLOT_PRESSURE == True:
        im = ax.imshow(data_p, extent=(0, obj["cell_cnt_x"]*obj["dx"], 0, obj["cell_cnt_y"]*obj["dy"]), cmap="viridis")
        fig.colorbar(im)

    ax.set_xlabel("X")
    ax.set_ylabel("Y")

    title_plot_type = ""
    if PLOT_VELOCITY == True:
        title_plot_type += "streamplot"
    if PLOT_VELOCITY == True and PLOT_PRESSURE == True:
        title_plot_type += " and "
    if PLOT_PRESSURE == True:
        title_plot_type += "heat map"
    if len(PLOT_TITLE) > 0:
        title_plot_type += ": "
    ax.set_title(title_plot_type + PLOT_TITLE + ", t=" + str(time_steps[closest_time_step]))
    setup_ax(ax)
    
    if PLOT_SAVE == True:
        plot_type = "_"
        if PLOT_PRESSURE == True:
            plot_type += "p"
        if PLOT_VELOCITY == True:
            plot_type += "v"
        plt.savefig("plots/" + PLOT_SAVE_NAME + "_t=" + str(time_steps[closest_time_step]) + plot_type + ".pdf")

    plt.show()


#plt.imshow(np.array(pd.read_csv(file_names_p[5], delimiter="\t", header=None)), cmap="Greys", extent=(0, obj["cell_cnt_x"]*obj["dx"], 0, obj["cell_cnt_y"]*obj["dy"]))

#plt.show()