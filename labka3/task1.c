#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <float.h>
#include "task1.h"


typedef union 
{
	float floating;
	struct 
	{
		unsigned int mantissa : 23;
		unsigned int exponent : 8;
		unsigned int sign : 1;
	}parts;
}FloatParts;

static int FindIntegerPart(float number)
{
	FloatParts numParts;
	numParts.floating = number;
	unsigned int mantissaMask = (1 << 23) - 1;
	unsigned int mantissaPart = numParts.parts.mantissa & mantissaMask;

	int exponentBias = 127;
	int exponent = numParts.parts.exponent - exponentBias;

	int integerPart = 0;

	if (exponent >= 0) 
	{
		integerPart = (1 << exponent) | (mantissaPart >> (23 - exponent));
	}
	if (numParts.parts.sign == 1)
	{
		integerPart *= -1;
	}

	return integerPart;
}
bool isFloat(const char* str, float* dest)
{
	if (str == NULL)
	{
		return false;
	}
	char* endptr;
	double value = strtod(str, &endptr);
	if (str == endptr)
	{
		return false;
	}
	while (isspace((unsigned char)*endptr))
		endptr++;
	if (*endptr != '\0')
	{
		return false;
	}
	if (value < -FLT_MAX || value > FLT_MAX)
	{
		return false;
	}
	*dest = (float)value;
	return true;
}
void FirstTask()
{
	char buffer[256];
	printf("Enter a floating point number: ");
	int c;
	while ((c = getchar()) != '\n' && c != EOF) {} 
	if (fgets(buffer, sizeof buffer, stdin))
	{
		size_t length = strlen(buffer);
		if (length > 0 && buffer[length - 1] == '\n')
		{
			buffer[length - 1] = '\0';
		}
		else
		{
			printf("Buffer overflow\n");
			return 1; 
		}
		float number;
		if (isFloat(buffer, &number))
		{
			printf("Correct input\n");
			int intPart = FindIntegerPart(number);
			printf("Integer part of the number: %d\n", intPart);
		}
		else
		{
			printf("Invalid input or value is too large\n");
		}
	}
	else
	{
		printf("Invalid input\n");
	}
	return 0;
}