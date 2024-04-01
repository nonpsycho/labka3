#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "task2.h"

static EventType StringToEvenType(int numb)
{
	switch (numb)
	{
	case 1:
		return ENROLLMENT;
	case 2:
		return EXPULSION;
	case 3:
		return RECOVERY;
	default:
		return ERRORTYPE;
	}
}
const char* EventTypeToString(EventType type)
{
	switch (type)
	{
	case ENROLLMENT:
		return "Enrollment";
	case EXPULSION:
		return "Expulsion";
	case RECOVERY:
		return "Recovery";
	case ERRORTYPE:
		return "Unknown";
	default:
		return "Unknown";
	}
}
static void ReplaceSpaces(char* str)
{
	size_t len = strlen(str);
	for (int i = 0; i < len; i++)
	{
		if (str[i] == ' ')
		{
			str[i] = ',';
		}
	}
}
//static EventType StringToEvenType(char* str)
//{
//	const char* eventNames[] = { "ENROLLMENT", "EXPULSION", "RECOVERY" };
//	for (int i = 0; i < 3; i++)
//	{
//		if (strcmp(str, eventNames[i]) == 0)
//		{
//			return (EventType) i;
//		}
//	}
//	return ERRORTYPE;
//}
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

static Student ReadStudent()
{
	Student student = { .marks = ConstructVec(sizeof(Discipline)), .events = ConstructVec(sizeof(Event)) };
	char buffer[256];
	int numMarks;
	int numEvent;

	printf("Enter the ID of the student: \n");
	scanf("%d", &student.id);
	printf("Enter student name: \n");
	int c;
	while ((c = getchar()) != '\n' && c != EOF) {}
	fgets(buffer, sizeof(buffer), stdin);
	size_t length = strlen(buffer); 
	if (buffer[length - 1] == '\n')
	{
		buffer[length - 1] = '\0';
	}
	ReplaceSpaces(buffer);
	student.name = malloc(strlen(buffer) + 1);
	strcpy(student.name, buffer);
	
	printf("Enter the number of marks: \n");
	scanf("%d", &numMarks);
	for (int i = 0; i < numMarks; i++)
	{
		Discipline discipline;
		printf("Enter the name of discipline: \n");
		scanf("%s", buffer);
		
		discipline.name = malloc(strlen(buffer) + 1);
		strcpy(discipline.name, buffer);

		printf("Enter the mark: \n");
		scanf("%d", &discipline.mark);

		PushBackVec(&student.marks, &discipline);
	}
	printf("Enter the number of events: \n");
	scanf("%d", &numEvent); 
	for (int i = 0; i < numEvent; i++)
	{
		Event event;
		int type;
		printf("Enter event type: 1- Enrollment; 2 - Expulsion; 3 - Recovery\n");
		scanf("%d",&type);

		event.type = StringToEvenType(type);

		printf("Enter the date of the event (yyyy.mm.dd): \n");
		int c;
		while ((c = getchar()) != '\n' && c != EOF) {}
		fgets(event.date, 11, stdin);

		PushBackVec(&student.events, &event);
	}

	return student;
}

static void Serialize(Vector* uni /*const char* fname*/)
{
	FILE* inputFile = fopen("info.txt", "w");
	if (inputFile == NULL)
	{
		printf("File openning error\n");
		exit(-1);
	}
	for (int i = 0; i < uni->count; i++)
	{
		Student* student = (Student*)AtVec(uni, i);
		fprintf(inputFile, "%d,", student->id);
		fprintf(inputFile, "%s,", student->name);
		size_t numMarks = student->marks.count; 
		for (int j = 0; j < numMarks; j++)
		{
			Discipline* discipline = (Discipline*)AtVec(&student->marks, j);
			fprintf(inputFile, "%s:%d", discipline->name, discipline->mark);
			if (j < numMarks - 1)
			{
				fprintf(inputFile, ";");
			}
		}
		fprintf(inputFile, ","); 

		size_t numEvents = student->events.count;
		for (int j = 0; j < numEvents; j++)
		{
			Event* event = (Event*)AtVec(&student->events, j); 
			fprintf(inputFile, "%d:%s", event->type, event->date);
			if (j < numEvents - 1)
			{
				fprintf(inputFile, ";");
			}
		}
		fprintf(inputFile, "\n");
	}
	fclose(inputFile);
}

static void Deserialize(Vector* uni)
{
	FILE* inputFile = fopen("test.txt", "r");
	if (inputFile == NULL)
	{
		printf("File openning error\n");
		exit(-1);
	}
	char buffer[256];
	while (fgets(buffer, sizeof(buffer), inputFile) != NULL)
	{
		Student student;

		memset(&student, 0, sizeof(Student));
		sscanf(buffer, "%d,", &student.id);

		char name[256];
		char* token = strtok(buffer, ",");
		int counter = 0;
		while (token != NULL)
		{
			if (counter == 1)
			{
				strcpy(name, token);
			}
			else if (counter == 2)
			{
				strcat(name, ",");
				strcat(name, token);
			}
			else if (counter == 3)
			{
				strcat(name, ",");
				strcat(name, token);
			}
			counter++;
			token = strtok(NULL, ",");
		}
		student.name = malloc(strlen(name) + 1);

		char* marksStart = strchr(buffer, ',') + 1;
		char* marksEnd = strchr(marksStart, ',');

		if (marksEnd != marksStart)
		{
			*marksEnd = '\0';
			char* markMarker = strtok(marksStart, ";");

			while (markMarker != NULL)
			{
				Discipline discipline;
				sscanf(markMarker, "%[^:]:%d", discipline.name, &discipline.mark);
				PushBackVec(&student.marks, &discipline);
				markMarker = strtok(NULL, ";");
			}
		}
		char* eventsStart = marksEnd + 1;
		if (eventsStart[0] != '\n')
		{
			char* eventsEnd = strchr(eventsStart, '\n');
			*eventsEnd = '\0';
			char* eventMarker = strtok(eventsStart, ";");
			while (eventMarker != NULL)
			{
				Event event;
				int type;
				sscanf(eventMarker, "%d:%s", &type, event.date);
				event.type = StringToEvenType(type);
				PushBackVec(&student.events, &event);
				eventMarker = strtok(NULL, ";");
			}
		}
		PushBackVec(uni, &student);
	}
	fclose(inputFile);
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
	qsort(uni->data, uni->count, sizeof(Student), CompareByGPA);
	int numbGeeks =(int)(uni->count * 0.3);
	for (int i = 0; i <= numbGeeks; i++)
	{
		Student* student = (Student*)AtVec(uni, i);
		PrintStudent(student);
	}
}

static void KickLazy(Vector* uni)
{
	int maxEvents = 0;
	int maxEventsIndex = -1;

	for (int i = 0; i < uni->count; i++)
	{
		Student* student = (Student*)AtVec(uni, i);
		if (student->events.count > maxEvents)
		{
			maxEvents = student->events.count;
			maxEventsIndex = i;
		}
	}

	if (maxEventsIndex != -1)
	{
		Student* student = (Student*)AtVec(uni, maxEventsIndex);
		DestructStudent(student);
		RemoveVec(uni, maxEventsIndex);
	}
}

void SecondTask()
{
	//const char* file = "info.txt";
	Vector uni = ConstructVec(sizeof(Student));
	int fill;
	printf("Choose a way to fill: 0 - user input; 1 - test data\n");
	scanf_s("%d", &fill);
	if (fill)
	{
		Deserialize(&uni /*"test.txt"*/);
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
		}

		Serialize(&uni /*"info.txt"*/);
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
			Serialize(&uni /*"info.txt"*/);
			break;
		}
		case 3:
		{
			printf("You choose a task 3\n");
			PrintGeeks(&uni);
			Serialize(&uni /*"info.txt"*/);
			break;
		}
		case 4:
		{
			printf("You choose a task 4\n");
			KickLazy(&uni);
			Serialize(&uni /*"info.txt"*/);
			break;
		}
		default:
		{
			printf("\nInvalid number(( try again!\n");
			break;
		}
		}

	} while (choice != 0);

	for (int i = 0; i < uni.count; i++)
	{
		Student* student = (Student*)AtVec(&uni, i);
		DestructStudent(student);
	}
	DestructVec(&uni);
}