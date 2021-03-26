#!/bin/sh

TEST_IDX=0
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

compile() {
	gcc *.c -o ex2
	if [ $? -ne 0 ]; then
		exit 1
	fi
}

single_test() {
	touch mygrep_output
	touch grep_output

	#echo "TET_IDX=${TEST_IDX}"
	echo "./ex2 $1 $2 $3 $4 $5 $6 $7 $8 $9 ${10} ${11} ${12}"

	./ex2 $1 $2 $3 $4 $5 $6 $7 $8 $9 ${10} ${11} ${12} > mygrep_output
	if [ $? -ne 0 ]; then
		echo "error=$?"
		exit 1
	fi

	grep $2 $3 $4 $5 $6 $7 $8 $9 ${10} ${11} ${12} $1 > grep_output
	
	tput cuu 1 
	tput cuf 45
	
	cmp -s mygrep_output grep_output
	if [ $? -ne 0 ]; then
		echo "${RED}[fail]${NC}"
		exit 1
	else
		rm mygrep_output
		rm grep_output
		echo "${GREEN}[pass]${NC}"
	fi

	TEST_IDX=$((TEST_IDX+1))
}

# compile
compile

# run tests
single_test file0 checkemptyfile
single_test file1 chaim
single_test file1 someshit
single_test file1 -v chaim
single_test file1 -v dontfindme
single_test file1 -A 2 add
single_test file1 -A 2 -v is
single_test file1 -A 2 -v dontfind
single_test file1 -c chaim
single_test file1 -c dontfind
single_test file1 -v -c is
single_test file1 -A 2 -c add
single_test file1 -A 2 -v -c is
single_test file1 -n chaim
single_test file1 -n -v chaim
single_test file1 -A 2 -n -v add
single_test file1 -n -c chaim
single_test file1 -n -v -c chaim
single_test file1 -A 2 -c -n -v add
single_test file1 -b chaim
single_test file1 -n -b chaim
single_test file1 -n -v -b chaim
single_test file1 -b -A 2 -n -v add
single_test file1 -n -c -b chaim
single_test file1 -n -b -v -c chaim
single_test file1 -A 2 -c -b -n -v add
single_test file1 -i ADd
single_test file1 -i CHAIM
single_test file1 -i -c ADd
single_test file1 -i -c -n ADd
single_test file1 -i -c -b ADd
single_test file1 -A 2 -c -b -n -i -v add
single_test file1 -x chaim
single_test file1 -b -A 2 -x -n -v add
single_test file1 -n -c -b -x chaim
single_test file1 -n -b -v -x -c chaim
single_test file1 -A 2 -c -b -n -v add
single_test file1 -i -x ADd
single_test file1 -i -x CHAIM
single_test file1 -i -c -x ADd
single_test file1 -i -x -n ADd
single_test file1 -i -x -b ADd
single_test file1 -A 2 -c -x -b -n -i -v add
single_test file1 -A 2 -x -b -n -i -v add


