
.PHONY: all clean test

clean:
	rm -f *.o my_grep

all: clean
	make my_grep

test:
	./test.py

my_grep: match.o my_grep.o
	gcc my_grep.o match.o -o my_grep

my_grep.o: match.h
	gcc -c my_grep.c -o my_grep.o

match.o: match.c match.h
	gcc -c match.c -o match.o