#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "task2.h"
#include "filling.h"

static void PrintStudent(Student* st)
{
	printf("%d,%s,", st->id, st->name);
	for (int i = 0; i < st->marks.count; i++)
	{
		Discipline* discipline = (Discipline*)AtVec(&st->marks, i);
		printf("%s:%d", discipline->name, discipline->mark);
		if (i < st->marks.count - 1)
		{
			printf(";");
		}
	}
	printf(",");
	for (int i = 0; i < st->events.count; i++)
	{
		Event* event = (Event*)AtVec(&st->events, i);
		const char* type = EventTypeToString(event->type);
		printf("%s:%s", type, event->date);
		if (i < st->events.count - 1)
		{
			printf(";");
		}
	}
	printf("\n");
}
static void DestructStudent(Student* st)
{
	free(st->name); 
	for (int i = 0; i < st->marks.count; i++)
	{
		Discipline* disc = (Discipline*)AtVec(&st->marks, i);
		free(disc->name);
	}
	DestructVec(&st->marks);
	DestructVec(&st->events);
}
static int CompareByGPA(const void* a, const void* b)
{
	Student* studentA = (Student*)a;
	Student* studentB = (Student*)b;
	int sumA = 0; 
	int sumB = 0;
	size_t numMarksA = studentA->marks.count;
	size_t numMarksB = studentB->marks.count;

	for (int i = 0; i < studentA->marks.count; i++)
	{
		Discipline* discipline = (Discipline*)AtVec(&studentA->marks, i);
		sumA += discipline->mark;
	}
	for (int i = 0; i < studentB->marks.count; i++)
	{
		Discipline* discipline = (Discipline*)AtVec(&studentB->marks, i);
		sumB += discipline->mark;
	}
	double gpaA = (double)sumA / numMarksA;
	double gpaB = (double)sumB / numMarksB;
	if (gpaA < gpaB)
	{
		return 1;
	}
	else if (gpaA > gpaB)
	{
		return -1;
	}
	else
	{
		return 0;
	}
}

static void PrintGeeks(Vector* uni)
{
	Vector copyUni = ConstructVec(sizeof(Student));
	for (int i = 0; i < uni->count; i++)
	{
		Student* student = (Student*)AtVec(uni, i);
		for (int i = 0; i < student->events.count; i++)
		{
			Event* event = (Event*)AtVec(&student->events, i);
			if (event->type != 1)
			{
				PushBackVec(&copyUni, student);
			}
		}
	}
	qsort(copyUni.data, copyUni.count, sizeof(Student), CompareByGPA);
	int numbGeeks =(int)(copyUni.count * 0.3);
	for (int i = 0; i <= numbGeeks; i++)
	{
		Student* student = (Student*)AtVec(&copyUni, i);
		PrintStudent(student);
	}
	for (int i = 0; i < copyUni.count; i++)
	{
		Student* student = (Student*)AtVec(&copyUni, i);
		DestructStudent(student);
	}
	DestructVec(&copyUni);
}

static void KickLazy(Vector* uni)
{
	int maxEvents = 0;

	for (int i = 0; i < uni->count; i++)
	{
		Student* student = (Student*)AtVec(uni, i);
		if (student->events.count > maxEvents)
		{
			maxEvents = student->events.count;
		}
	}
	for (int i = uni->count - 1; i >= 0; i--)
	{
		Student* student = (Student*)AtVec(uni, i);
		if (student->events.count == maxEvents)
		{
			DestructStudent(student); 
			RemoveVec(uni, i); 
		}
	}
}

void SecondTask()
{
	Vector uni = ConstructVec(sizeof(Student));
	int fill;
	printf("Choose a way to fill: 0 - user input; 1 - test data\n");
	scanf_s("%d", &fill);
	if (fill)
	{
		Deserialize(&uni);
	}
	else
	{
		Student student = ReadStudent();
		PushBackVec(&uni, &student);
		int stFill;
		printf("Add another student?\n1 - yes; 0 - no\n"); 
		scanf("%d", &stFill);
		while(stFill != 0)
		{
			Student student = ReadStudent(); 
			PushBackVec(&uni, &student); 
			printf("Add another student?\n1 - yes; 0 - no\n"); 
			scanf("%d", &stFill); 
		}

		Serialize(&uni);
	}
	int choice;
	do 
	{
		printf("\n\tMenu:\n");
		printf("1 - Print a list of students\n");
		printf("2 - Delete a student by ID\n");
		printf("3 - Print the most successful students\n");
		printf("4 - Delete a student with replete learning history\n");
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
			printf("List of student\n");
			for (int i = 0; i < uni.count; i++)
			{
				Student* student = (Student*)AtVec(&uni, i); 
				PrintStudent(student);
			}
			break;
		}
		case 2:
		{
			int idx;
			printf("Enter the index of the student you want to delete\n");
			scanf_s("%d", &idx);
			Student* student = (Student*)AtVec(&uni, idx - 1);
			RemoveVec(&uni, idx);
			DestructStudent(student);
			
			break;
		}
		case 3:
		{
			printf("You choose a task 3\n");
			PrintGeeks(&uni);
			break;
		}
		case 4:
		{
			printf("You choose a task 4\n");
			KickLazy(&uni);
			break;
		}
		default:
		{
			printf("\nInvalid number(( try again!\n");
			break;
		}
		}

	} while (choice != 0);
	Serialize(&uni);
	for (int i = 0; i < uni.count; i++)
	{
		Student* student = (Student*)AtVec(&uni, i);
		DestructStudent(student);
	}
	DestructVec(&uni);
}