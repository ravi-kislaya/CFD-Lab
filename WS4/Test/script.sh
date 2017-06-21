# That is the script that can simplify your workflow.
# The script can take one perameter
#
# There is five modes that one can use:
#   1. the clean mode - the mode cleans the folder from *.vtk, *.o
#      files and REPORT::<day>-<month>-<time>.
#      Type the following command to run that mode: ./script.sh clean
#
#	2. the make mode - the mode include the 1 mode and run make program
#	   to generate executable
#	   Type the following command to run that mode: ./script.sh make
#
#   3. the run mode - that mode includes step 1. and 2. The script runs the
#	   executable after successful compilation
#      Type the following commnad to run that mode: ./script.sh make-and-run
#
#   4. the memory check mode - that mode includes steps 1, 2. and 3.
#      Additionally, it runs valgrind and write its output
#      into the file called MEMORY_LEAKS_REPORT
#      Typpe the following command to run that mode: ./script make mem-check
#
#   5. the make and depict mode - that mode includes steps 1 and 2. Moreover,
#      it runs paraview if there is at least one vtk file

#!/bin/bash


# define all necessary functions
function cleanProject {

	# clean the window
	clear

	# remove executable
	make clean
	if [ -f ./$EXECUTABLE ]; then
		rm -f ./sim
	fi

	cleanVisualizationData
}

function cleanVisualizationData {
	VTK_FOLDER=./Frames

	# remove all vtk files
	rm -f $VTK_FOLDER/*.vtk
}


function generateExecutable {

	# clean the project
	cleanProject

	# run make program
    make
}


function runExecutable {

	cleanVisualizationData

	# run simulation if the compilation process was successful
    # $1 - means the parameter passed to the function
    if [ -f ./$EXECUTABLE ]; then
        ./$EXECUTABLE $1
    fi
}


function runValgrind {

	if [ -f ./$EXECUTABLE ]; then

		# remove the old report
		rm -f ./$MEM_CHECK_FILE*

		# set file name according to the format above
		FILE_NAME=$MEM_CHECK_FILE

		# generate the report
		valgrind --tool=memcheck ./$EXECUTABLE $1  #> $FILE_NAME 2>&1
	fi
}


# this function checks if the certain file format exists
# at the project folder
# The function takes two parameters, namely: FILE_FORMAT and RESULT
# FILE_FORMAT is the first parameter ($1) passed to the function
# RESULT is the second parameter and it's the return value at the same time
# Example of calling the function: checkFileFormat vtk RESULT
function isFileFormatExists {
    FILE_FORMAT=./*.$1
    RESULT=$2

    # count the number of suitable files
    COUNTER=`ls -1 $FILE_FORMAT 2>/dev/null | wc -l`

    # if the counter is not equal to zero it means that there is at least
    # one file with given format in the project directory
    if [ $COUNTER != 0 ]; then
        RESULT=true
    else
       RESULT=false
    fi
}

# declare all modes
RUN_CLEAN=clean
RUN_MAKE=make
MAKE_AND_RUN=make-and-run
RUN_VALGRIND=mem-check
RUN_PARAVIEW=make-and-depict


#declare all variables
EXECUTABLE=lbsim
MEM_CHECK_FILE=MEM_CHECK_REPORT
NUMBER_OF_AVALIABLE_PARAMETERS=1
SIMULATION_INPUT_FILE=cube.data
SIMULATION_OUTPUT_FILE=Cube3D...vtk
VTK_FOLDER=./Frames


# process all modes

if [ "$#" = $NUMBER_OF_AVALIABLE_PARAMETERS ]; then


	if [ $1 = $RUN_CLEAN ]; then

		# clean project mode
		cleanProject

	elif [ $1 = $RUN_MAKE ]; then

		# make-run executable mode
		generateExecutable

	elif [ $1 = $MAKE_AND_RUN ]; then

		# make and run executable mode
		generateExecutable
		runExecutable $SIMULATION_INPUT_FILE

	elif [ $1 = $RUN_VALGRIND ]; then

		# compile the memory leaks report
		generateExecutable
		runValgrind $SIMULATION_INPUT_FILE

    elif [ $1 = $RUN_PARAVIEW ]; then

        # make and run executable mode
		generateExecutable
		runExecutable $SIMULATION_INPUT_FILE


        # run paraview if the compilation was successful
        VtkExistence=false
        isFileFormatExists vtk $VtkExistence
        if [ $VtkExistence  ]; then
            paraview $VTK_FOLDER/$SIMULATION_OUTPUT_FILE
        else
            echo
            echo "Cannot run paraview. There is no vtk files"
            echo "HINT: compilation probably failed"
            echo
        fi

    else

		# print the error message
		echo
		echo "ERROR: there is no such mode"
		echo "HINT: choose one of the following modes:" \
					"clean, make, make-and-run, mem-check, make-and-depict"
		echo

	fi

else
    # print the error message
	echo
	echo "ERROR: incorrect number of input parameters"
	echo

fi
