
.PHONY: all clean test

clean:
	-rm *.o my_grep

all: my_grep

test:
	./test.sh

my_grep: match.o my_grep.o
	gcc my_grep.o match.o -o my_grep

my_grep.o: my_grep.c
	gcc -c my_grep.c -o my_grep.o

match.o: match.c match.h
	gcc -c match.c -o match.o