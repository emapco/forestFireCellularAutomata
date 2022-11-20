"""
@file model.cpp
@author Emmanuel Cortes
@brief This module creates an animated gif of the
       forest fire cellular automata simulation
@date 2022-11-19

@copyright Copyright (c) Emmanuel Cortes 2022
"""

import matplotlib
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import numpy as np


# set forest fire cmap
CUSTOM_CMAP = matplotlib.cm.get_cmap("RdYlGn_r")

# simulation parameters
HEIGHT_PER_STATE = 300
WIDTH_PER_STATE = 300
# simulation configurations
STATES = np.loadtxt(
    "data/output.csv", dtype=np.uint8, delimiter=",", usecols=range(WIDTH_PER_STATE)
)
NUM_STATES = len(STATES) // HEIGHT_PER_STATE


def init():
    global im
    im = plt.imshow(
        STATES[
            :HEIGHT_PER_STATE,
        ],
        animated=True,
    )
    im.set_cmap(CUSTOM_CMAP)
    return (im,)


def animate(i):
    global im
    plt.title(f"Time Step = {i}")
    im.set_data(
        STATES[
            i * HEIGHT_PER_STATE : i * HEIGHT_PER_STATE + HEIGHT_PER_STATE,
        ]
    )
    return (im,)


if __name__ == "__main__":
    # style graph
    matplotlib.rcParams.update({"font.size": 18})
    # initializing the animation figure
    fig = plt.figure()
    fig.set_size_inches(12, 12, True)
    # create animation object
    anim = FuncAnimation(fig, animate, init_func=init, frames=NUM_STATES, interval=1)
    # animate and save it
    anim.save("forest_fire.gif", dpi=300)
