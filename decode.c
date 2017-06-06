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
# include "intStack.h"
# include "treeStack.h"
# include "queue.h"
# include "huffman.h"
# include "code.h"
# include "bv.h"

int main (int argc, char **argv)
{
	char *iFile = NULL; //Input file
	char *oFile = NULL; //Output file
	uint8_t *sFile;
	uint8_t magicN[4];
	for (uint32_t i = 0; i < 4; i++)
	{
		magicN[i] = (0xdeadd00d >> (i*8)) & 255;
	}

	int c;
    while ((c = getopt (argc , argv , "i:o:v")) != -1)
    {
      switch (c)
      {
        case 'i':
          iFile = optarg;
          break;
        case 'o':
         oFile = optarg;
          break;
        // case 'v':
        //   verbose = false;
        //   break;
      }
    }
	int file = open(iFile, O_RDONLY);
	if(file == -1)
    {
      printf("%s: %s\n", iFile, strerror(errno));
  		return errno;
    }
	struct stat buf;
	fstat(file, &buf);
	uint64_t compressFileSize = buf.st_size;
	sFile = mmap(NIL, compressFileSize, PROT_READ, MAP_PRIVATE, file, 0);

	if(sFile == MAP_FAILED)
	{
		printf("%s\n", strerror(errno));
		return(errno);
	}

	for (uint32_t i = 0; i < 4; i++)
	{
		if (sFile[i] != magicN[i] || iFile == NULL)
		{
			printf("Error: Not a compressed file\n");
			return 0;
		}
	}

	read(file, sFile, 8);
	uint64_t fileSize = ((uint64_t)sFile[11] << 56)|((uint64_t)sFile[10] << 48)|
						((uint64_t)sFile[9] << 40)|((uint64_t)sFile[8] << 32)|
						((uint64_t)sFile[7] << 24)|((uint64_t)sFile[6] << 16)|
						((uint64_t)sFile[5] << 8)|(sFile[4]);

	uint16_t treeSize = (((uint16_t)sFile[13]) << 8)|(sFile[12]);
	uint8_t savedTree[treeSize];
	for (uint32_t i = 0; i < treeSize; i++)
	{
		savedTree[i] = sFile[14+i];
	}
	//for (uint32_t i = 0; i < treeSize; i++) { printf("%c", savedTree[i]); }
	treeNode *tree = loadTree(savedTree, treeSize);
	//printTree(tree, 1);
	treeNode *treeStepper = tree;
	char fileOutput[fileSize];
	uint64_t counter = 0;
	uint32_t bitLength = compressFileSize - 14 - treeSize;
	bitV *bv = newVec(bitLength*8);
	for (uint32_t i = 0; i < bitLength; i++)
	{
		bv->vector[i] = sFile[i + 14 + treeSize];
	}
	for (uint32_t i = 0; i < bitLength*8; i++)
	{
			int32_t result = stepTree(tree, &treeStepper, valBit(bv, i));
			if (result != -1 && counter < fileSize)
			{
				fileOutput[counter] = (char)result;
				counter++;
			}
	}
	int outFile = 0;
	if (oFile == NULL)
	{
		outFile = 1;
	}
	else
	{
		outFile = open(oFile, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR | S_IWGRP | S_IRGRP | S_IROTH | S_IWOTH);
	}
    if(outFile == -1)
    {
      printf("%s: %s\n", oFile, strerror(errno));
      return(errno);
    }
	for (uint64_t i = 0; i < fileSize; i++)
	{
		write(outFile, &fileOutput[i], 1);
	}
	delTree(tree);
	delVec(bv);
	return 0;
}
