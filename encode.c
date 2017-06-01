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
# include "bv.h"

# define MAGICNUM 0xdeadd00d
# define ARRAY_SIZE 256
# define VECTOR_SIZE 8000

void createHistogram(uint64_t histogram[], uint8_t *sFile, uint64_t fileSize)
{
  // Creating the histogram
  for(uint32_t x = 0; x < fileSize; x++)
  {
    histogram[sFile[x]]++;
  }
  histogram[0]++;
  histogram[ARRAY_SIZE - 1]++;
}

// adds nodes to the priority queue
void addNodes(uint64_t histogram[], queue *q, uint16_t *leafCount)
{
  // adds nodes to the priority queue
  for(uint32_t x = 0; x < ARRAY_SIZE; x++)
  {
    if(histogram[x] > 0)
    {
      enqueue(q, newNode(x, true, histogram[x]));
      *leafCount += 1;
    }
  }
}

treeNode *createHuffTree(queue *q)
{
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
  return root;
}

int main(void)
{
  // size of the file
  uint64_t fileSize = 0;
  // histogram array
  uint64_t histogram[ARRAY_SIZE];
  // queue
  queue *q = newQueue(ARRAY_SIZE);
  // leaf count
  uint16_t treeSize = 0;
  // input file
  uint8_t *sFile;
  uint32_t magicNum = MAGICNUM;

  // zero out the histrogram
  for(uint32_t x = 0; x < ARRAY_SIZE; x++)
  {
    histogram[x] = 0;
  }

  // Opening the file
  int fd = open("getty", O_RDONLY);
  // check to make sure file exists
  struct stat buf;
  fstat(fd, &buf);
  fileSize = buf.st_size - 1; // updating with the size of the file, remember about eof character
  sFile = mmap(NIL, fileSize, PROT_READ, MAP_PRIVATE, fd, 0);
  if(sFile == MAP_FAILED)
  {
      printf("Mapping Failed\n");
      exit(1);
  }

  createHistogram(histogram, sFile, fileSize);

  // close the file
  close(fd);

  // adds nodes to the priority queue
  addNodes(histogram, q, &treeSize);

  // making the huffman tree
  // root is the huffman tree
  treeNode *root = createHuffTree(q);

  printTree(root, 1);

  // code table
  code table[256];

  //fill the table with empty codes
  for(uint32_t x = 0; x < 256; x++)
  {
    table[x] = newCode();
  }

  // creates the codes
  code temp = newCode();
  buildCode(root, temp, table);
  printf("\n");
  treeSize = 3 * treeSize - 1;

  Bitv *bv = newVec(INCREMENT);
  for (uint32_t i = 0; i < fileSize; i++)
  {
      code add = table[sFile[i]];
      appendCode(add, bv);
  }

  FILE *oFile = fopen("output", "w");
  if(oFile == NIL)
  {
    printf("Error in output\n");
    exit(1);
  }

  int numWrite = fwrite(&magicNum, sizeof(magicNum), 1, oFile);
  numWrite = fwrite(&fileSize, sizeof(fileSize), 1, oFile);
  numWrite = fwrite(&treeSize, sizeof(treeSize), 1, oFile);
  (void) numWrite;
  dumpTree(root, oFile);
  fclose(oFile);
  delQueue(q);
  return 0;
}
