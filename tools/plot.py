import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.gridspec import GridSpec
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
## read in json
with open("data/options.json", "r") as json_file:
    data = json_file.read()
obj = json.loads(data)

print(obj["cell_cnt_x"])

data_u = np.transpose(np.array(pd.read_csv("out/nsg_u_0.000000.dat", delimiter="\t")))
data_v = np.transpose(np.array(pd.read_csv("out/nsg_v_0.000000.dat", delimiter="\t")))
#x,y = np.meshgrid(np.linspace(0, obj["cell_cnt_x"]*obj["dx"], obj["cell_cnt_x"]), np.linspace(0, obj["cell_cnt_y"]*obj["dy"], obj["cell_cnt_y"]))
x,y = np.meshgrid(np.linspace(0, 50, 50), np.linspace(0, 50, 50))

fig, ax = plt.subplots()

#q = ax.quiver(x, y, data_u, data_v, units="width")
#ax.quiverkey(q, X=0.3, Y=1.1, U=1, label='Quiver key, length = 10', labelpos='E')
q = ax.quiver(data_u, data_v)

plt.show()