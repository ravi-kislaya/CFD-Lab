#!/bin/bash


# clean the window
clear


# declare all variables
RUN_MAKE=make
RUN_VALGRIND=mem-check
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
if [ $1 = $RUN_MAKE ]; then
    # run make program
    make
    
    # run simulation if the compilation process was successful
    if [ -f ./$EXECUTABLE ]; then
        ./$EXECUTABLE
    fi
fi

# compile the memory leaks report
if [ $2 = $RUN_VALGRIND ]; then
    valgrind --tool=memcheck ./$EXECUTABLE > MEMORY_LEAKS_REPORT 2>&1
fi 


