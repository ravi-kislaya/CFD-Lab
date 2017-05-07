#!/bin/bash

# clean the window
clear

# clean the project
make clean
rm ./sim

# remove all vtk files
rm ./*.vtk

# generate the executable file
make

# run simulation
./sim
