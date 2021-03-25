#!/bin/sh

compile() {
	gcc *.c -o ex2
	if [ $? -ne 0 ]; then
		exit 1
	fi
}

single_test() {
	touch test_discription
	touch mygrep_output
	touch grep_output

	echo "./ex2 $1 $2 $3 $4 $5 $6 $7 $8 $9 ${10} ${11} ${12}" > test_discription

	./ex2 $1 $2 $3 $4 $5 $6 $7 $8 $9 ${10} ${11} ${12} > mygrep_output
	if [ $? -ne 0 ]; then
		echo "error=$?"
		exit 1
	fi

	grep $2 $3 $4 $5 $6 $7 $8 $9 ${10} ${11} ${12} $1 > grep_output
	
	diff mygrep_output grep_output
	if [ $? -ne 0 ]; then
		exit 1
	else
		rm test_discription
		rm mygrep_output
		rm grep_output
		echo "pass"
	fi
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
