#include <stdio.h>
#include <stdlib.h>
#include "task1.h"

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

static void FileFill(char* file)
{
    FILE* inputFile = NULL;
    inputFile = fopen(file, "wb+");
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

static int FindIntegerPart(char* file)
{
    FILE* inputFile = fopen(file, "rb");
    if (inputFile == NULL)
    {
        printf("\n\tFile openning error\n");
        exit(-1);
    }

    FloatParts floatparts;
    int integerPart = 0;
    while (fread(&floatparts.floating, sizeof(float), 1, inputFile) == 1)
    {
        int integerPart = floatparts.parts.mantissa >> (23 - floatparts.parts.exponent);

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