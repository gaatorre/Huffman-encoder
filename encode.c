# include <stdint.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <unistd.h>
# include <sys/mman.h>
# include "intStack.h"
# include "treeStack.h"
# include "queue.h"
# include "huffman.h"
# include "code.h"

# define MAGICNUM 0xdeadd00d

int main(void)
{
  // size of the file
  uint64_t fileSize = 0;
  // histogram array
  uint64_t histogram[256];
  // queue
  queue *q = newQueue(256);
  // leaf count
  uint16_t leafCount = 0;

  for(uint32_t x = 0; x < 256; x++)
  {
    histogram[x] = 0;
  }

  // Opening the file
  int fd = open("getty", O_RDONLY);
  struct stat buf;
  fstat(fd, &buf);
  fileSize = buf.st_size - 1; // updating with the size of the file, remember about eof character
  uint8_t *sFile = mmap(NIL, fileSize, PROT_READ, MAP_PRIVATE, fd, 0);
  if(sFile == MAP_FAILED)
  {
      printf("Mapping Failed\n");
      exit(1);
  }

  // Creating the histogram
  for(uint32_t x = 0; x < fileSize; x++)
  {
    histogram[sFile[x]]++;
  }
  histogram[0]++;
  histogram[255]++;

  // close the file
  close(fd);

  // adds nodes to the priority queue
  for(uint32_t x = 0; x < 256; x++)
  {
    if(histogram[x] > 0)
    {
      enqueue(q, newNode(x, true, histogram[x]));
      leafCount++;
    }
  }

  // making the huffman tree

  //trouble begins here
  // root is the huffman tree
  treeNode *root = NIL;
  treeNode *left = NIL;
  treeNode *right = NIL;
  while(!emptyQ(q))
  {
    dequeue(q, &left);
    if(emptyQ(q))
    {
      root = left;
      break;
    }
    dequeue(q, &right);
    enqueue(q, join(right, left));
  }

  printTree(root, 3);

  // code table
  code table[256];
  for(uint32_t x = 0; x < 256; x++)
  {
    table[x] = newCode();
  }
  // creates the codes
  code temp = newCode();
  buildCode(root, temp, table);

  printf("%u\n", MAGICNUM);
  printf("size of tree is %u\n", 3 * leafCount - 1);
  dumpTree(root);
  printf("\n");
  // fd = open("output", O_RDWR | O_CREAT, S_IRUSR | S_IRGRP | S_IROTH | S_IRWXU);
  // if(fd < 0)
  // {
  //   printf("Error in output\n");
  //   exit(1);
  // }
  //
  // // char magic = (char)MAGICNUM;
  // write(fd, sFile, 4);

  delQueue(q);
  return 0;
}
