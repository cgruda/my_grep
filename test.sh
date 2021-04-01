#!/bin/sh

# compile
gcc *.c -o mygrep
if [ $? -ne 0 ]; then
	exit 1
fi

# run python script
/usr/bin/python3 /home/cgruda/LABC/Ex2/test.py
