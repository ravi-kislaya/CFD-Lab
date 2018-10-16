#!/bin/bash


########################## HELPER FUNCTIONS #########################
function setUpMesh {
	
	rm -f ./Code/Metis/*

	if [ ! -d "./Code/Mesh" ]; then
		echo "ERROR: the ./Code/Mesh folder doesn't exist"
	fi

	cp ./Converter/Output/* ./Code/Mesh/
	echo "MESH WAS SET UP"
}


function runMetis {

	local PARTIOTION_NUMBER=0
	while (( $PARTIOTION_NUMBER < 2 ))
	do

		printf "Please, specify the number of partions (more than 2): "
		read PARTIOTION_NUMBER
		
		if (( $PARTIOTION_NUMBER < 2 )); then
			echo "ERROR: the number of partiotioning is less than 2"
		fi

	done

	if [ ! -f ./Metis/gpmetis ]; then
		echo "ERROR: metis was not installed"
		echo "HINT: please have a look at the README file: clause 4"
		exit 1
	fi

	# remove all partiotioning file
	rm -f ./Metis/AdjacencyList.graph*

	# copy a new Adjacency List from the python script output
	cp ./Converter/Output/AdjacencyList.graph ./Metis
	
	# run metis
	echo "PARTITIONING IS RUNNING: "
	./Metis/gpmetis -contig ./Metis/AdjacencyList.graph $PARTIOTION_NUMBER
	
	if [ ! -f ./Metis/AdjacencyList.graph.part.$PARTIOTION_NUMBER ]; then
		echo "ERROR: partitioning stage failed"
		echo "HINT: please refer to the metis documentation"
		exit 1
	fi
	echo "PARTITIONING IS DONE: "

	# rename the metis output according to the StudentLB specification
	mv ./Metis/AdjacencyList.graph.part.$PARTIOTION_NUMBER ./Metis/CpuPartitioning.prt
	mv ./Metis/CpuPartitioning.prt ./Code/Mesh/


	return $PARTIOTION_NUMBER
}

function generateMesh {

	if [ ! -f "./polyMesh/owner" ]; then 
		echo "ERROR: cannot generate the mesh since ./polyMesh/owner is missed"
		exit 1
	elif [ ! -f "./polyMesh/points" ]; then 
		echo "ERROR: cannot generate the mesh since ./polyMesh/points is missed"
		exit 1
	elif [ ! -f "./polyMesh/faces" ]; then 
		echo "ERROR: cannot generate the mesh since ./polyMesh/faces is missed"
		exit 1
	elif [ ! -f "./polyMesh/boundary" ]; then 
		echo "ERROR: cannot generate the mesh since ./polyMesh/boundary is missed"
		exit 1
	elif [ ! -f "./polyMesh/neighbour" ]; then 
		echo "ERROR: cannot generate the mesh since ./polyMesh/neighbour is missed"
		exit 1
	fi


	if [ ! -d "./Converter" ]; then
		echo "ERROR: cannot convert OpenFoam mesh since the converter is not installed"
		exit 1
	fi


	echo "MESH CONVERTION IS RUNNING"
	cd ./Converter
	python ./Translator.py
	CONVERTER_STATUS=$?
	if [ "$CONVERTER_STATUS" != "0" ]
	then
		echo "ERROR: mesh converion faild"
		cd ../
		exit 1
	else
		echo "MESH CONVERTION IS DONE" 
	fi

	cd ../

	runMetis
	local PARTIOTION_NUMBER=$?
	return $PARTIOTION_NUMBER
}


###################### SCRIPT: START ######################
shopt -s nullglob

PERMISSION_LEVEL=755
# check whether all folders exists
if [ ! -d "./Code/build/" ]; then
	mkdir -m $PERMISSION_LEVEL ./Code/build
fi

if [ ! -d "./Code/Frames" ]; then
	mkdir -m $PERMISSION_LEVEL ./Code/Frames
fi

if [ ! -d "./Code/InletVelocity" ]; then
	mkdir -m $PERMISSION_LEVEL ./Code/InletVelocity
fi

if [ ! -d "./Code/Mesh" ]; then
	mkdir -m $PERMISSION_LEVEL ./Code/Mesh
fi

if [ ! -d "./polyMesh" ]; then
	mkdir -m $PERMISSION_LEVEL ./polyMesh
fi


# Check whether the inlet velocity file exists
VELOCITY_FILES=(./Code/InletVelocity/*)
VELOCITY_FILE_NAME="./Code/InletVelocity/aorta.inl"
if [ ${#VELOCITY_FILES[@]} == 0 ]; then
	# Abort the script if there is no file 
	echo "ERROR: you didn't provide the velocity inlet file: clause 8"
	exit 1
else
	# if the name of the inlet velocity file is different 
	# change the name to the standart one
	if [ "${VELOCITY_FILES[0]}" != "$VELOCITY_FILE_NAME" ]; then
		mv ${VELOCITY_FILES[0]} $VELOCITY_FILE_NAME
	fi	
fi


# Check whether there is mesh in the ./Code/Mesh folder
MESH_FILES=(Code/Mesh/*)
PARTIOTION_NUMBER=0

# Check whether all file names is ok
if [ ! -f "Code/Mesh/Coordinates.crd" ] 		\
	|| [ ! -f "Code/Mesh/CpuPartitioning.prt" ] \
	|| [ ! -f "Code/Mesh/FlagFields.fg" ] 		\
	|| [ ! -f "Code/Mesh/Neighbors.nb" ]		\
	|| [ ! -f "Code/Mesh/AdjacencyList.graph" ]; then 

		generateMesh
		PARTIOTION_NUMBER=$?
		setUpMesh
fi


#check whether there should we compile executable or not
if [ ! -f "./Code/lbsim" ]; then
	
	if [ ! -f "./Code/Makefile" ]; then
		echo "ERROR: there is no Makefile in the ./Code folder"
		echo "HINT: Please download the Makefile from the github repositiry, clause 9"
		exit 1
	fi

	cd ./Code
	make
	cd ../
fi

# Abort the execution if there is still no executable file 
if  [ ! -f "./Code/lbsim" ]; then
	echo "ERROR: it was not able to build up the executable"
	echo "HINT: Please, refer to the README file: clause 5"
	exit 1
fi


# set up the number of pertitions
if (( $PARTIOTION_NUMBER == 0 )); then
	# check whether the LBM was generated
	if [ ! -f "./Converter/Output/AdjacencyList.graph" ]; then
		# run LBM mesh generation in case if there in no AdjacencyList file
		generateMesh
		PARTIOTION_NUMBER=$?
		setUpMesh
	fi

	runMetis
	PARTIOTION_NUMBER=$?
fi


# set up boundary conditions
if [ ! -f "./Code/Mesh/BoundaryList.bc" ]; then
	# check whether there is BoundaryList.bc in the ./Code folder
	if  [ ! -f "./Code/BoundaryList.bc" ]; then
		echo "ERROR: there is no BoundaryList.bc file in the ./Code folder"
		echo "HINT: Please, download BoundaryList.bc file from the github repository: clause 6"
		exit 1
	fi
	cp ./Code/BoundaryList.bc ./Code/Mesh/
	vim ./Code/Mesh/BoundaryList.bc -O ./polyMesh/boundary
else
	if [ ! -f "./polyMesh/boundary" ]; then
		vim ./Code/Mesh/BoundaryList.bc
	else
		vim ./Code/Mesh/BoundaryList.bc -O ./polyMesh/boundary
	fi
fi


# set up simulation settings
if [ ! -f "./Code/Input.data" ]; then
	echo "ERROR: there is no Input.data file in the ./Code folder"
	echo "HINT: Please, download Input.data file from the github repository: clause 7"
	exit 1
else
	vim ./Code/Input.data
fi


# clean "Frames" folder
rm -f ./Code/Frames/*

# run the simulation
cd ./Code
echo "StudentLB IS RUNNUNG"
mpirun -n $PARTIOTION_NUMBER ./lbsim ./Input.data $PARTIOTION_NUMBER 
echo "StudentLB IS DONE"
echo "You can see your results in the ./Code/Frames folder"
cd ../

###################### SCRIPT: END ######################
