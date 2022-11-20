# Forest Fire Cellular Automata Simulation

This project implements the cellular automata method to simulate a forest fire.
The c++ source code utilizes OpenMP to parallelize the simulation. 
The animation is created with matplotlib.

Dependencies required to run this project:
- C++
  - OpenMP
  - g++
- Python
  - matplotlib
  - pillow | ffmpeg | imagemagick (matplotlib animation dependency)
  - NumPy

Directories
- bin: contains the compile c++ program
- data: contains the simulation output and an animation of the simulation
- src: contains source files for simulating and animating the forest fire

Files
- Makefile: 
  - Run `make all` to compile, execute, and animate the simulation.
  - Run `make simulate_forest_fire` to compile and execute the simulation.
  - Run `create_animation` to create the animated gif.
