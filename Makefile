CC = gcc
CFLAGS = -Wall -Wextra -g 

all: test

test: test.o tree.o mymalloc.o
	$(CC) $(CFLAGS) -o $@ $^

test.o: test.c tree.h
tree.o: tree.c tree.h
mymalloc.o: mymalloc.c mymalloc.h

.PHONY: run
run: test
	./test
