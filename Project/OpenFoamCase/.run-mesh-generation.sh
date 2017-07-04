#!/bin/bash

# Cleaning old data
	rm ./constant/polyMesh/*
	rm ./0/*
	#rm -r 0.*

# Generating block Mesh
	blockMesh

# Generating snappyHexMesh
	snappyHexMesh -overwrite

# Launching paraFoam to see the result of mesh generation
key="$1"
if [ $key="-v" ] 
   then
	paraFoam
fi
