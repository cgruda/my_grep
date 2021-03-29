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

