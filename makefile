CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -Werror -std=c99 -D_GNU_SOURCE
E_OBJECTS = queue.o encode.o huffman.o intStack.o treeStack.o code.o

.PHONY: all
all: prog

prog:$(E_OBJECTS)
	$(CC) $(CFLAGS) $(E_OBJECTS) -o encode

encode.o: encode.c
	$(CC) $(CFLAGS) -c encode.c

huffman.o: huffman.c
	$(CC) $(CFLAGS) -c huffman.c

intStack.o: intStack.c
	$(CC) $(CFLAGS) -c intStack.c

queue.o: queue.c
	$(CC) $(CFLAGS) -c queue.c

treeStack.o: treeStack.c
	$(CC) $(CFLAGS) -c treeStack.c

code.o: code.c
	$(CC) $(CFLAGS) -c code.c

.Phony: clean
clean:
	rm -f $(E_OBJECTS) encode
