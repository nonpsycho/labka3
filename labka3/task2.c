#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hashtable.h"
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
	float gpaA = FindGPA(studentA);
	float gpaB = FindGPA(studentB);

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

static void PrintGeeks(HashTable* uni)
{
	Vector copyUni = ConstructVec(sizeof(Student));
	for (int i = 0; i < uni->keys.count; i++)
	{
		float key = *(float*)AtVec(&uni->keys, i);
		Student* student = (Student*)HTFind(uni, &key);
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

static void KickLazy(HashTable* uni)
{
	int maxEvents = 0;

	for (int i = 0; i < uni->keys.count; i++)
	{
		float key = *(float*)AtVec(&uni->keys, i);
		Student* student = (Student*)HTFind(uni, &key);
		if (student->events.count > maxEvents)
		{
			maxEvents = student->events.count;
		}
	}
	for (int i = uni->keys.count - 1; i >= 0; i--)
	{
		float key = *(float*)AtVec(&uni->keys, i);
		Student* student = (Student*)HTFind(uni, &key);
		if (student->events.count == maxEvents)
		{
			///////////
			/*DestructStudent(student); 
			RemoveVec(uni, i); */
			HTRemove(uni, &key);
		}
	}
}

void FindStudentByGPA(HashTable* uni)
{
	float gpa = 0;
	printf("Enter the student's GPA: \n");
	scanf("%f", &gpa);
	Student* student = (Student*)HTFind(uni, &gpa);
	PrintStudent(student);
}

void SecondTask()
{
	HashTable uni = HTCreate(sizeof(Student), HTDestroy, CompareKeys);
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
		float gpa = FindGPA(&student);
		HTInsert(&uni, &gpa, &student);
		int stFill;
		printf("Add another student?\n1 - yes; 0 - no\n"); 
		scanf("%d", &stFill);
		while(stFill != 0)
		{
			Student student = ReadStudent(); 
			float gpa = FindGPA(&student);
			HTInsert(&uni, &gpa, &student);
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
		printf("2 - Print the most successful students\n");
		printf("3 - Delete a student with replete learning history\n");
		printf("4 - Search for a student by gpa\n");
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
			for (int i = 0; i < uni.keys.count; i++)
			{
				float key = *(float*)AtVec(&uni.keys, i);
				Student* student = (Student*)HTFind(&uni, &key);
				PrintStudent(student);
			}
			break;
		}
		case 2:
		{
			printf("You choose a task 2\n");
			PrintGeeks(&uni);
			break;
		}
		case 3:
		{
			printf("You choose a task 3\n");
			KickLazy(&uni);
			break;
		}
		case 4:
		{
			printf("You choose a task 4\n");
			FindStudentByGPA(&uni);
		}
		default:
		{
			printf("\nInvalid number(( try again!\n");
			break;
		}
		}

	} while (choice != 0);
	Serialize(&uni);
	////destVec
	for (int i = 0; i < uni.keys.count; i++)
	{
		float key = *(float*)AtVec(&uni.keys, i);
		Student* student = (Student*)HTFind(&uni, &key);
		DestructStudent(student);
	}
	HTDestroy(&uni);
}