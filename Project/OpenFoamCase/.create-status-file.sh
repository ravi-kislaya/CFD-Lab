#!/bin/bash

currentDirectory=$(pwd)
geometryDirectory=$currentDirectory'/constant/triSurface'
projectSettingsDirectory=$currentDirectory'/system'
statusFileName='.Status'
utility='.change-stl-file-header'

# Getting the array of files consisting of the geometry directory
cd $geometryDirectory
list=(*.stl) 


# Updating stl-files headers if the mode --force is selected
key="$1"
if [ "$key"="--force" ]
   then
	for ((i=0; i<${#list[@]}; i++)); do
		./$utility ${list[$i]}
		chmod 777 ${list[$i]}
	done
#echo "fine!"
fi


# Writing .Status file
for ((i=0; i<${#list[@]}; i++)); do
	result=${list[$i]%.stl}
	echo "$result">>"$statusFileName"
done


# Moving .Status file to the system directory
if [ -a $statusFileName ]
     then
	mv $geometryDirectory/$statusFileName $projectSettingsDirectory
fi

#echo "end"
