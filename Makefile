CC = gcc
CFLAGS = -Wall -Wextra -g 

all: test

test: 
	$(CC) $(CFLAGS) -o test mymalloc.c test.c tree.c
