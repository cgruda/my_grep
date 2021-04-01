#!/bin/sh

# compile
gcc *.c -o my_grep
if [ $? -ne 0 ]; then
	exit 1
fi

# run python script
./test.py
