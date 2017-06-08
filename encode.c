# include <stdint.h>
# include <string.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <unistd.h>
# include <sys/mman.h>
# include <errno.h>
# include "treeStack.h"
# include "queue.h"
# include "huffman.h"
# include "code.h"
# include "bv.h"

# define MAGICNUMBER 0xdeadd00d  // identifies the file as compressed
# define HISTOGRAM_SIZE 256      // size of the histogram
# define KILOBYTE 8000           // size of the bitvector

// conducts a frequency analysis of the file to create a histogram
void createHistogram(uint64_t histogram[], uint8_t *sFile, uint64_t fileSize)
{
  // conducts the frequency analysis if the file is not empty
  if(sFile != NULL)
  {
    for(uint32_t x = 0; x < fileSize; x++)
    {
      histogram[sFile[x]]++;
    }
  }
  // increments the first and last values to cover the case where the file
  // is empty
  histogram[0]++;
  histogram[HISTOGRAM_SIZE - 1]++;
}

// adds nodes to the priority queue
void addNodes(uint64_t histogram[], queue *q, uint16_t *leafCount)
{
  // adds nodes to the priority queue
  for(uint32_t x = 0; x < HISTOGRAM_SIZE; x++)
  {
    // only adds nodes with symbols that are in the file
    if(histogram[x] > 0)
    {
      enqueue(q, newNode(x, true, histogram[x]));
      *leafCount += 1;
    }
  }
}

// creates the huffman tree and returns the root node
treeNode *createHuffTree(queue *q)
{
  treeNode *root = NIL;
  treeNode *left = NIL;
  treeNode *right = NIL;
  // pops two nodes with the highest priority, joins them, and pushes the joined nodes
  while(!emptyQ(q))
  {
    dequeue(q, &left);
    // when there is only one node left, it is the root of the tree
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

int main(int argc, char **argv)
{
  // size of the file
  uint64_t fileSize = 0;
  // input filepath
  char *inFile;
  // output filepath
  char *outFile = NULL;
  // verbose
  bool verbose = false;
  // histogram array
  uint64_t histogram[HISTOGRAM_SIZE];
  // queue
  queue *q = newQueue(HISTOGRAM_SIZE);
  // leaf count
  uint16_t leafCount = 0;
  // tree size
  uint16_t treeSize = 0;
  // input file memory map
  uint8_t *sFile = NULL;
  // Magic Number
  uint32_t magicNumber = MAGICNUMBER;
  // boolean for printing the flag or not
  bool dispalyTree = false;

  // handles the command line flags
  int c;
  while ((c = getopt (argc , argv , "i:o:vp")) != -1)
  {
    switch (c)
    {
      case 'i':
        inFile = optarg;
        break;
      case 'o':
        outFile = optarg;
        break;
      case 'v':
        verbose = true;
        break;
      case 'p':
        dispalyTree = true;
        break;
    }
  }

  // zero out the histrogram
  for(uint32_t x = 0; x < HISTOGRAM_SIZE; x++)
  {
    histogram[x] = 0;
  }

  // Opening the file
  int fd = open(inFile, O_RDONLY);
  // check to make sure file exists
  if(fd == -1)
  {
    printf("%s: %s\n", inFile, strerror(errno));
		return errno;
  }

  // use fstat to gather information about the file
  struct stat buf;
  fstat(fd, &buf);
  fileSize = buf.st_size; // update the size of the file
  // memory map the file if it is not empty
  if(fileSize)
  {
    sFile = mmap(NIL, fileSize, PROT_READ, MAP_PRIVATE, fd, 0);
  }
  // error checking the memory map
  if(sFile == MAP_FAILED)
  {
      printf("%s\n", strerror(errno));
      return(errno);
  }

  createHistogram(histogram, sFile, fileSize);

  // close the file
  close(fd);

  // adds nodes to the priority queue
  addNodes(histogram, q, &leafCount);
  treeSize = 3 * leafCount - 1; // update the treeSize

  // making the huffman tree
  treeNode *root = createHuffTree(q);

  // code table
  code table[256];
  //fill the table with empty codes
  for(uint32_t x = 0; x < HISTOGRAM_SIZE; x++)
  {
    table[x] = newCode();
  }

  // creates the codes
  code temp = newCode();
  buildCode(root, temp, table);

  // use a bitvector to contain the character's translation
  bitV *bv = newVec(KILOBYTE);
  uint32_t bitLength = 0; // how many bits are used in the bitvector for the codes
  // appends the code of the character to the bitvector
  for (uint32_t i = 0; i < fileSize; i++)
  {
      code add = table[sFile[i]];
      bitLength = appendCode(add, bv);
  }

  // rounds up the size of the bitlength to a multiple of 8
  if(bitLength % 8)
  {
    bitLength += 8 - (bitLength % 8);
  }

  // opens the output file
  int oFile;
  // if no output was specified, it will write to stdout
  if(outFile == NULL)
  {
    oFile = 1;
  }
  // else it will write to the specified file
  else
  {
    oFile = open(outFile, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR | S_IWGRP | S_IRGRP | S_IROTH | S_IWOTH);
    // opening file error checking
    if(oFile == -1)
    {
      printf("%s\n", strerror(errno));
      return(errno);
    }
  }

  // Writes the magic number
  write(oFile, &magicNumber, sizeof(magicNumber));
  // Writes the size of the file
  write(oFile, &fileSize, sizeof(fileSize));
  // Writes the size of the tree
  write(oFile, &treeSize, sizeof(treeSize));
  // Dumps the tree
  dumpTree(root, oFile);
  // write the bits
  write(oFile, bv->vector, bitLength / 8);

  if(dispalyTree)
  {
    dumpTree(root, 1);
    printTree(root, 1);
  }

  // stat tracking for the output file
  if(verbose)
  {                     // magic number       file size          treeSize
    uint64_t outFileSize = sizeof(uint32_t) + sizeof(uint64_t) + sizeof(uint16_t) + treeSize + bitLength;
    double compressPercent = ((double)bitLength / (fileSize * 8)) * 100;
    printf("Original %lu bits: leaves %u (%u bytes) encoding %lu bits (%1.4f%%)\n",
            fileSize * 8, leafCount, treeSize, outFileSize, compressPercent);
  }

  // Closes the file
  close(oFile);
  // Delete the bit vector
  delVec(bv);
  // Delete the queue
  delQueue(q);
  return 0;
}
