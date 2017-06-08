# include <stdint.h>
# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <unistd.h>
# include <sys/mman.h>
# include <errno.h>
# include "huffman.h"
# include "bv.h"

int main (int argc, char **argv)
{
	char *iFile = NULL; //Input file
	char *oFile = NULL; //Output file
	uint8_t *sFile; //Char array for memory mapping
	uint8_t magicN[4]; //Array of bytes for the magic number
	bool treePrint = false, verbose = false;
	//Breaks magic number into bytes and fills array
	for (uint32_t i = 0; i < 4; i++)
	{
		magicN[i] = (0xdeadd00d >> (i*8)) & 255;
	}
	int c;
	while ((c = getopt (argc , argv , "i:o:vp")) != -1)
	{
		switch (c)
		{
			case 'i':
			{
				iFile = optarg; //Sets input file name
				break;
			}
			case 'o':
			{
				oFile = optarg; //sets output file name
				break;
			}
			case 'v':
			{
				verbose = true;
				break;
			}
			case 'p':
			{
				treePrint = true;
				break;
			}
		}
	}
	int file = open(iFile, O_RDONLY);
	if(file == -1)
	{
		printf("%s: %s\n", iFile, strerror(errno));
		return errno;
	}
	//File information from fstat
	struct stat buf;
	fstat(file, &buf);
	uint64_t compressFileSize = buf.st_size; //Size of the compressed file
	//Memory maps the file to an array of bytes
	sFile = mmap(NIL, compressFileSize, PROT_READ, MAP_PRIVATE, file, 0);

	if(sFile == MAP_FAILED)
	{
		printf("%s\n", strerror(errno));
		return(errno);
	}

	//Checks to see if file is a compressed file with correct
	//magic number
	for (uint32_t i = 0; i < 4; i++)
	{
		if (sFile[i] != magicN[i] || iFile == NULL)
		{
			printf("Error: Not a compressed file\n");
			return 0;
		}
	}
	//Grabs fileSize one byte at a time and ORs the bytes
	uint64_t fileSize = ((uint64_t)sFile[11] << 56)|((uint64_t)sFile[10] << 48)|
						((uint64_t)sFile[9] << 40)|((uint64_t)sFile[8] << 32)|
						((uint64_t)sFile[7] << 24)|((uint64_t)sFile[6] << 16)|
						((uint64_t)sFile[5] << 8)|(sFile[4]);

	//Grabs tree size one byte at a time and ORs the bytes
	uint16_t treeSize = (((uint16_t)sFile[13]) << 8)|(sFile[12]);
	//Creates an array to store Tree information
	uint8_t savedTree[treeSize];
	//Fills array with tree info
	for (uint32_t i = 0; i < treeSize; i++)
	{
		savedTree[i] = sFile[14+i];
	}
	//Loads the tree into a tree node
	treeNode *tree = loadTree(savedTree, treeSize);
	//Pointer to tree that will be used to keep track of position
	treeNode *treeStepper = tree;
	//Output of bits read
	char fileOutput[fileSize];
	//Keeps track of how many bytes were read
	uint64_t counter = 0;
	//How many bytes of bits were read
	uint32_t bitLength = compressFileSize - 14 - treeSize;
	//Creates a bitvector to hold codes
	bitV *bv = newVec(bitLength*8);
	//Sets vector in bitvector to hold the bytes from sFile
	for (uint32_t i = 0; i < bitLength; i++)
	{
		bv->vector[i] = sFile[i + 14 + treeSize];
	}
	//For every bit, it will step through the tree and if it's a leaf,
	//it will add the symbol to the output array
	for (uint32_t i = 0; i < bitLength*8; i++)
	{
		int32_t result = stepTree(tree, &treeStepper, valBit(bv, i));
		if (result != -1 && counter < fileSize)
		{
			fileOutput[counter] = (char)result;
			counter++;
		}
	}
	int outFile = 0; //File descriptor
	//If no output file, print to stdout
	if (oFile == NULL)
	{
		outFile = 1;
	}
	//Checks if file already exists
	else if (!stat (oFile, &buf))
	{
		printf("%s: Success\n", oFile);
	}
	else
	{
		if (verbose)
		{
			printf("Original %lu bits: tree (%u)\n", fileSize*8, treeSize);
		}
		if (treePrint)
		{
			printTree(tree, 1);
		}
		//Opens and writes output array to output file
		outFile = open(oFile, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR | S_IWGRP | S_IRGRP | S_IROTH | S_IWOTH);
		if(outFile == -1)
		{
			printf("%s: %s\n", oFile, strerror(errno));
			return(errno);
		}
		write(outFile, fileOutput, fileSize);
	}
	delTree(tree);
	delVec(bv);
	return 0;
}
