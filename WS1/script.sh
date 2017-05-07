#!/bin/bash


# clean the window
clear


# declare all variables
COMMAND=make
EXECUTABLE=sim


# clean the project
make clean
if [ -f ./$EXECUTABLE ]; then
    rm ./sim
fi


# remove all vtk files
rm ./*.vtk
rm ./*.o


# generate the executable file
if [ "$#" -eq 1 ]; then
    if [ $1 = $COMMAND ]; then
        # run make program
        make
    
        # run simulation if the compilation process was successful
        if [ -f ./$EXECUTABLE ]; then
            ./$EXECUTABLE
        fi
    fi
fi
