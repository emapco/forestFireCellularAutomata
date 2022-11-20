# @file Makefile
# @author Emmanuel Cortes
# @brief This makefile compiles the c++ cellular automata program
#		 and then creates an animated gif using python.
# @date 2022-11-19
# 
# @copyright Copyright (c) Emmanuel Cortes 2022
#
# Dependencies: g++, OpenMP, matplotlib, numpy, pillow

.PHONY: make_bin create_animation simulate_forest_fire all

# C++ related variables
CXX = g++
CXXFLAGS = -O3 -fopenmp

# directories
BIN = bin
SRC = src
DATA = data

# run all targets
all: make_bin $(BIN)/forest_fire_ca simulate_forest_fire create_animation
	
# executes the forest fire cellular automata
simulate_forest_fire: $(BIN)/forest_fire_ca
	$(BIN)/forest_fire_ca

# compiles the cellular automata program
$(BIN)/forest_fire_ca: $(SRC)/forest_fire_ca.cpp
	$(CXX) $(CXXFLAGS) $(OBJS) $< -o $@ 

# creates an animation using the cellular automata simulation output
create_animation:
	python $(SRC)/ca_animation.py
	mv forest_fire.gif $(DATA)/

# silently make a `bin` directory
make_bin:
	mkdir -p bin
