#!/bin/bash

# this script checks if *.h and *.cpp files 
# are correctly formatted using clang-format

# list of directories
DIRS="../src ../include"

# check if clan-format is there
command -v clang-format > /dev/null 

if [ $? -ne 0 ]; then

	echo "[EE] clang-format not installed"
	exit 1
fi


# loop over directories
for DIR in $DIRS
do

	# check if directory exist
	if ! [ -d $DIR ]; then
	
		echo "[WW] directory $DIR does not exist"
		continue
	fi

	# loop over files
	for FILE in `find $DIR -name '*.h' -o -name '*.cpp'`
	do
		echo "[II] testing: $FILE"

		diff <(cat $FILE) <(clang-format $FILE) > /dev/null 

		if [ $? -ne 0 ]; then

			echo "[EE] failed (see previous line for file name)"
			exit 2
		fi
	done
done

#exit code 0
exit 0
