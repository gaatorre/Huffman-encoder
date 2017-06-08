CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -Werror -std=c99 -g -D_GNU_SOURCE
E_OBJECTS = queue.o huffman.o treeStack.o code.o bv.o

.PHONY: all
all: prog

prog:$(E_OBJECTS) encode.o decode.o
	$(CC) $(CFLAGS) $(E_OBJECTS) encode.o -o encode
	$(CC) $(CFLAGS) $(E_OBJECTS) decode.o -o decode

bv.o: bv.c
	$(CC) $(CFLAGS) -c bv.c

encode.o: encode.c
	$(CC) $(CFLAGS) -c encode.c

decode.o: decode.c
	$(CC) $(CFLAGS) -c decode.c

huffman.o: huffman.c
	$(CC) $(CFLAGS) -c huffman.c

queue.o: queue.c
	$(CC) $(CFLAGS) -c queue.c

treeStack.o: treeStack.c
	$(CC) $(CFLAGS) -c treeStack.c

code.o: code.c
	$(CC) $(CFLAGS) -c code.c

.Phony: clean
clean:
	rm -f $(E_OBJECTS) encode decode
