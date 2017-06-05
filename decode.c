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
	char *iFile;
	char *oFile;
	uint8_t sFile[14];
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
	read(file, sFile, 4);
	for (uint32_t i = 0; i < 4; i++)
	{
		if (sFile[i] != magicN[i])
		{
			printf("Error: Not a compressed file\n");
			return 0;
		}
	}
	read(file, sFile, 8);
	uint64_t fileSize = ((uint64_t)sFile[7] << 56)|((uint64_t)sFile[6] << 48)|
						((uint64_t)sFile[5] << 40)|((uint64_t)sFile[4] << 32)|
						((uint64_t)sFile[3] << 24)|((uint64_t)sFile[2] << 16)|
						((uint64_t)sFile[1] << 8)|(sFile[0]);
	read(file, sFile, 2);
	uint16_t treeSize = (((uint16_t)sFile[1]) << 8)|(sFile[0]);
	uint8_t savedTree[treeSize];
	read(file, savedTree, treeSize);
	//for (uint32_t i = 0; i < treeSize; i++) { printf("%c", savedTree[i]); }
	//treeNode *tree = loadTree(savedTree, treeSize);
	treeNode *a = newNode('a', true, 0);
	treeNode *b = newNode('b', true, 0);
	treeNode *tree = join(a,b);
	printTree(tree, 1);
	treeNode *treeStepper = tree;
	char fileOutput[fileSize], bytes[1024];
	uint64_t counter = 0;
	int red = read(file, bytes, 1024);
	for (int32_t i = 0; i < red; i++)
	{
		for (uint32_t j = 0; j < 8; j++)
		{
			int32_t result = stepTree(tree, &treeStepper, bytes[i]&(0x1));
			if (result != -1 && counter < fileSize)
			{
				fileOutput[counter] = (char)result;
				counter++;
			}
			bytes[i] = bytes[i] >> 1;
		}
		if (red == 1024 && i == 1023)
		{
			red = read(file, bytes, 1024);
			i = 0;
		}
	}
	int outFile = open(oFile, O_CREAT | O_WRONLY, S_IRWXU | S_IRWXG | S_IRWXO);
    if(outFile == -1)
    {
      printf("%s: %s\n", oFile, strerror(errno));
      return(errno);
    }
	for (uint64_t i = 0; i < fileSize; i++)
	{
		write(outFile, &fileOutput[i], 1);
	}
	return 0;
}
