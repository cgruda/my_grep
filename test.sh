#!/bin/sh

TEST_IDX=1
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

compile() {
	gcc *.c -o mygrep
	if [ $? -ne 0 ]; then
		exit 1
	fi
}

all_test() {
	/usr/bin/python3 /home/cgruda/LABC/Ex2/test.py
# 	touch mygrep_output
# 	touch grep_output

# 	printf ${YELLOW}"[%2d] "${NC} $TEST_IDX
# 	echo "./ex2 "$1" "$2" "$3" "$4" "$5" "$6" "$7" "$8" "$9" "${10}" "${11}" "${12}""

# 	# run mygrep and catch mygrep errors (return value non-zero)
# 	./ex2 $1 $2 $3 $4 $5 $6 $7 $8 $9 ${10} ${11} ${12} > mygrep_output
# 	STATUS=$?
# 	if [ $STATUS -ne 0 ]; then
# 		tput cuu 1
# 		tput cuf 45
# 		echo "${RED}[mygrep fail $STATUS]${NC}"
# 		exit 1
# 	fi

# 	# run grep and catch grep errors (return value 2)
# 	grep $1 $2 $3 $4 $5 $6 $7 $8 $9 ${10} ${11} ${12} > grep_output
# 	STATUS=$?
# 	if [ $STATUS -eq 2 ]; then
# 		tput sc
# 		tput cuu 2
# 		tput cuf 45
# 		echo "${RED}[grep fail $STATUS]${NC}"
# 		tput rc
# 		exit 1
# 	fi

# 	tput cuu 1
# 	tput cuf 45

# 	# compare grep output and mygrep output
# 	cmp -s mygrep_output grep_output
# 	if [ $? -ne 0 ]; then
# 		echo "${RED}[fail]${NC}"
# 		exit 1
# 	else
# 		rm mygrep_output
# 		rm grep_output
# 		echo "${GREEN}[pass]${NC}"
# 	fi

# 	TEST_IDX=$((TEST_IDX+1))
}

# compile
compile

# run tests
all_test
#  checkemptyfile file0
# single_test checkemptyfile file0
# single_test chaim file1
# single_test '"is chaim"' file1 
# single_test someshit file1
# single_test -v chaim file1
# single_test -v dontfindme file1
# single_test -A 2 add file1
# single_test -A 2 -v is file1
# single_test -A 2 -v dontfind file1
# single_test -c chaim file1
# single_test -c dontfind file1
# single_test -v -c is file1
# single_test -A 2 -c add file1
# single_test -A 2 -v -c is file1
# single_test -n chaim file1
# single_test -n -v chaim file1
# single_test -A 2 -n -v add file1
# single_test -n -c chaim file1
# single_test -n -v -c chaim file1
# single_test -A 2 -c -n -v add file1
# single_test -b chaim file1
# single_test -n -b chaim file1
# single_test -n -v -b chaim file1
# single_test -b -A 2 -n -v add file1
# single_test -n -c -b chaim file1
# single_test -n -b -v -c chaim file1
# single_test -A 2 -c -b -n -v add file1
# single_test -i ADd file1
# single_test -i CHAIM file1
# single_test -i -c ADd file1
# single_test -i -c -n ADd file1
# single_test -i -c -b ADd file1
# single_test -A 2 -c -b -n -i -v add file1
# single_test -x chaim file1
# single_test -b -A 2 -x -n -v add file1
# single_test -n -c -b -x chaim file1
# single_test -n -b -v -x -c chaim file1
# single_test -A 2 -c -b -n -v add file1
# single_test -i -x ADd file1
# single_test -i -x CHAIM file1
# single_test -i -c -x ADd file1
# single_test -i -x -n ADd file1
# single_test -i -x -b ADd file1
# single_test -A 2 -c -x -b -n -i -v add file1
# single_test -A 2 -x -b -n -i -v add file1


