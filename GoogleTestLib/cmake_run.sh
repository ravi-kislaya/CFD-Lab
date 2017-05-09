#!/bin/bash


# set all script variables 
SCRIPT="cmake_run.sh"
BUILD_DIRECTORY="./build"
LIBRARY_NAME="libGoogleTest.a"


# clean the building folder
rm -r $BUILD_DIRECTORY/*


# clean the window
clear


# run cmake
cd $BUILD_DIRECTORY
cmake ../
cd ../


# run make if the user explicitly ask about that
if [ "$#" -eq 1 ]; then
    if [ $1="make" ]; then
        # run make program
        cd $BUILD_DIRECTORY
        make
        cd ../

        # remove the existing library from the current directory
        if [ -f ./$LIBRARY_NAME ]; then
            rm ./$LIBRARY_NAME
        fi

        # copy a fresh compiled library from the build directory
        if [ -f $BUILD_DIRECTORY/$LIBRARY_NAME ]; then
            cp $BUILD_DIRECTORY/$LIBRARY_NAME ./$LIBRARY_NAME
        fi
    fi
fi
