#!/bin/bash

#Create .Status-file
key="$1"
if [ "$key"="--force" ]
    then
	./.create-status-file.sh $key
	#echo "$key"
    else
	./.create-status-file.sh
	#echo "usual mode"
fi

#Update SnappyHexMeshFile
./.adjust-snappyHexMeshDict-file system/

#Update SurfaceFeatureExtractFile
./.adjust-featureExtractDict-file system/
