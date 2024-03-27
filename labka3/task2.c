#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "task2.h"

static EventType StringToEvenType(char* str)
{
	const char* eventNames[] = { "ENROLLMENT", "EXPULSION", "RECOVERY" };
	for (int i = 0; i < 3; i++)
	{
		if (strcmp(str, eventNames[i]) == 0)
		{
			return (EventType) i;
		}
	}
	return ERRORTYPE;
}
void PrintStudent(const Student* st)
{
	printf("%d,%s,", st->id, st->name);
	for (int i = 0; i < st->marks.count; i++)
	{
		Discipline* discipline = (Discipline*)AtVec(&st->marks, i);
		printf("%s:%d,", discipline->name, discipline->mark);
		if (i < st->marks.count - 1)
		{
			printf(";");
		}
	}
	printf(",");
	for (int i = 0; i < st->events.count; i++)
	{
		Event* event = (Event*)AtVec(&st->events, i);
		printf("%s:%s", event->type, event->date);
		if (i < st->events.count - 1)
		{ 
			printf(";");
		}
	}
	printf("\n");
}

Student ReadStudent()
{
	Student student = { .marks = ConstructVec(sizeof(Discipline)), .events = ConstructVec(sizeof(Event)) };
	char buffer[256];
	int numMarks;
	int numEvent;

	printf("id: \n");
	scanf("%d", &student.id);
	printf("name: \n");
	scanf("%s", buffer);

	student.name = malloc(strlen(buffer) + 1);
	strcpy(student.name, buffer);
	
	printf("num of marks: \n");
	scanf("%d", &numMarks);
	for (int i = 0; i < numMarks; i++)
	{
		Discipline discipline;
		printf("dis: \n");
		scanf("%s", buffer);
		
		discipline.name = malloc(strlen(buffer) + 1);
		strcpy(discipline.name, buffer);

		printf("mark: \n");
		scanf("%d", &discipline.mark);

		PushBackVec(&student.marks, &discipline);
	}
	printf("num of event: \n");
	scanf("%d", &numEvent); 
	for (int i = 0; i < numEvent; i++)
	{
		Event event;
		printf("event type: \n");
		scanf("%s", buffer);

		event.type = StringToEvenType(buffer);

		printf("date yyyy.mm.dd: \n");
		fgets(event.date, 11, stdin);

		PushBackVec(&student.events, &event);
	}

	return student;
}

void Serialize(const Vector* uni, const char* fname)
{
	FILE* inputFile = fopen(fname, "w");
	if (inputFile == NULL)
	{
		printf("File openning error\n");
		exit(-1);
	}
	for (int i = 0; i < uni->count; i++)
	{
		Student* student = (Student*)AtVec(uni, i);
		fprintf(inputFile, "%d,%s,", student->id, student->name);
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
			fprintf(inputFile, "%s:%s", event->type, event->date);
			if (j < numEvents - 1)
			{
				fprintf(inputFile, ";");
			}
		}
		fprintf(inputFile, "\n");
	}
	fclose(inputFile);
}

void Deserialize(Vector* uni, const char* fname)
{
	FILE* inputFile = fopen(fname, "r");
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
		sscanf(buffer, "%d,%[^,],", &student.id, student.name);

		char* marksStart = strchr(buffer, ",") + 1;
		char* marksEnd = strchr(marksStart, ",");

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
				char type[20];
				sscanf(eventMarker, "%[^:]:%s", type, event.date); // mb %[] not s
				event.type = StringToEvenType(type);
				PushBackVec(&student.events, &event);
				eventMarker = strtok(NULL, ";");
			}
		}
		PushBackVec(uni, &student);
	}
	fclose(inputFile);
}

void PrintGeeks(Vector* uni)
{

}

void KickLazy(Vector* uni, int count)
{
	int IdxLazy = 0;
	for (int i = 0; i < uni->count - 1; i++)
	{
		Student* student1 = (Student*)AtVec(uni, i);
		for (int j = i; j < uni->count; j++)
		{
			Student* student2 = (Student*)AtVec(uni, j);
			int events1 = student1->events.count;
			int events2 = student2->events.count;
			int maxEvents = 0;
			if (events1 < events2)
			{
				IdxLazy = j;
			}
			else
			{
				IdxLazy = i;
			}
		}
		

	}
}

void SecondTask(char* file)
{
	Vector uni = ConstructVec(sizeof(Student));
	do {

	} while ();



	DestructVec(&uni);
}