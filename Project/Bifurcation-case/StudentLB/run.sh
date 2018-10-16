#!/bin/bash

if [ ! -f "./lbsim" ]; then
    make
fi

mpirun -n 1 ./lbsim ./Input.data 1
