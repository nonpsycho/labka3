#include <stdio.h>
#include <stdlib.h>
#include "task1.h"
#include "ioput.h"

typedef union 
{
    float floating;
    struct 
    {
        unsigned int mantissa : 23;
        unsigned int exponent : 8;
        unsigned int sign : 1;
    } parts;
} FloatParts;

int FindIntegerPart(char* file)
{
    FILE* inputFile = fopen(file, "rb");
    if (inputFile == NULL)
    {
        printf("\n\tFile openning error\n");
        exit(-1);
    }

    FloatParts floatparts;
    unsigned int integerPart = 0;
    while (fread(&floatparts.floating, sizeof(float), 1, inputFile) == 1)
    {
        unsigned int integerPart = floatparts.parts.mantissa >> (23 - floatparts.parts.exponent);

        if (floatparts.parts.sign == 1)
        {
            integerPart = -integerPart; 
        }
    }
    fclose(inputFile);
    return integerPart; 
}

void FirstTask(char* file)
{
    FileFill(file);
    int integerPart = FindIntegerPart(file);
    printf("Integer part: %d\n", integerPart);
}