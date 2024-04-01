#include <stdio.h>
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

void FirstTask()
{
	float number;
	printf("Enter a floating point number: \n");
	scanf_s("%f", &number);
	int integerPart = FindIntegerPart(number);
	printf("Integer part of number: %d\n", integerPart);
	return 0;
}