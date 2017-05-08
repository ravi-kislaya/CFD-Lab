# That is the script that can simplify your workflow.
#
# There is three modes that one can use:
#   1. the basic mode - the mode cleans the folder from *.vtk and *.o files
#      Type the following command to run that mode: ./script.sh
#
#   2. the run mode - that mode includes step 1. and runs the make program
#      to generate the executable. The script runs the executable after 
#      successful compilation 
#      Type the following commnad to run that mode: ./script.sh make
#
#   3. the memory check mode - that mode includes steps 1 and 2. 
#      Additionally, it runs valgrind and write its output
#      into the file called MEMORY_LEAKS_REPORT
#      Typpe the following command to run that mode: ./script make mem-check


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


