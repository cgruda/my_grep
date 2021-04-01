#!/bin/sh

# compile
gcc *.c -o mygrep
if [ $? -ne 0 ]; then
	exit 1
fi

# run python script
./test.py
