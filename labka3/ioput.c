#include <stdio.h>
#include <stdlib.h>
#include "ioput.h"

void FileFill(char* file)
{
	FILE* inputFile = NULL;
	inputFile = fopen(file, "wb");
	if (inputFile == NULL)
	{
		printf("\n\tFile openning error\n");
		exit(-1);
	}
	float number;
	printf("Enter a floating point number: \n");
	scanf_s("%f", &number);
	fwrite(&number, sizeof(float), 1, inputFile);

	fclose(inputFile);
}
