#include <stdio.h>
#include "task1.h"

int main(int argc, char* argv[])
{
	char* file = "pupa.bin";
	int choice;
	do
	{
		printf("\n\tMenu:\n");
		printf("1 - Task 1: \n");
		printf("2 - Task 2: \n");
		printf("0 - Finish the program\n");
		printf("Choose a task: \n");
		scanf_s("%d", &choice);

		switch (choice)
		{
		case 0:
		{
			printf("The program is completed!");
			break;
		}
		case 1:
		{
			printf("You choose a task 1\n");
			FirstTask(file);
			break;
		}
		case 2:
		{
			printf("You choose a task 2\n");

			break;
		}
		default:
		{
			printf("\nInvalid number(( try again!\n");
			break;
		}
		}

	} while (choice != 0);

	return 0;
}