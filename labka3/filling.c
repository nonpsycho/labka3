#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "filling.h"


int static ValidateDate(const char* date)
{
	if (strlen(date) != 10)
	{
		return 0;
	}
	if (date[2] != '.' || date[5] != '.')
	{
		
		return 0;
	}
	int day, month, year;
	if (sscanf(date, "%2d.%2d.%4d", &day, &month, &year) != 3)
	{
	
		return 0;
	}
	if (day < 1 || day > 31 || month < 1 || month > 12 || year < 0 || year > 2024)
	{
	
		return 0;
	}

	return 1;
}
void static ReplaseLowercase(char* str)
{
	int length = strlen(str);
	str[0] = toupper(str[0]);
	for (int i = 1; i < length; i++)
	{
		if (!isalpha(str[i]) && !isalpha(str[i - 1]))
		{
			continue;
		}
		if (isalpha(str[i]) && !isalpha(str[i - 1]))
		{
			str[i] = toupper(str[i]);
		}
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
EventType StringToEvenType(char* str)
{
	const char* eventNames[] = { "Enrollment", "Expulsion", "Recovery" };
	for (int i = 0; i < 3; i++)
	{
		if (strcmp(str, eventNames[i]) == 0)
		{
			return (EventType)i;
		}
	}
	return ERRORTYPE;
}

Student ReadStudent()
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
	student.name = malloc(strlen(buffer) + 1);
	strcpy(student.name, buffer);
	ReplaseLowercase(student.name);

	printf("Enter the number of marks: \n");
	scanf("%d", &numMarks);
	for (int i = 0; i < numMarks; i++)
	{
		Discipline discipline;
		printf("Enter the name of discipline: \n");
		int c;
		while ((c = getchar()) != '\n' && c != EOF) {}
		fgets(buffer, sizeof(buffer), stdin);
		size_t length = strlen(buffer);
		if (buffer[length - 1] == '\n')
		{
			buffer[length - 1] = '\0';
		}
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
		printf("Enter event type: Enrollment, Expulsion, Recovery\n");
		scanf("%s", buffer);

		event.type = StringToEvenType(buffer);

		printf("Enter the date of the event (dd.mm.yyyy): \n");
		int c;
		while ((c = getchar()) != '\n' && c != EOF) {}
		fgets(event.date, 11, stdin);

		if (!ValidateDate(event.date))
		{
			printf("Invalid date format, setting date to 0.\n");
			strcpy(event.date, "00.00.0000");
		}

		PushBackVec(&student.events, &event);
	}
	return student;
}

void Serialize(Vector* uni)
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
			const char* type = EventTypeToString(event->type);
			fprintf(inputFile, "%s:%s", type, event->date);
			if (j < numEvents - 1)
			{
				fprintf(inputFile, ";");
			}
		}
		fprintf(inputFile, "\n");
	}
	fclose(inputFile);
}

Vector GetTokens(char* str)
{
	char* token = strtok(str, ",\n");
	Vector tokens = ConstructVec(sizeof(char*));
	PushBackVec(&tokens, &token);
	puts(token);
	while ((token = strtok(NULL, ",\n")) != NULL)
	{
		PushBackVec(&tokens, &token);
		puts(token);
	}
	return tokens;
}

typedef  void (*ReadProc)(const char* token, Student* st);
void ReadId(const char* token, Student* st)
{
	sscanf(token, "%d", &st->id);
}
void ReadName(const char* token, Student* st)
{
	size_t length = strlen(token) + 1;
	st->name = malloc(length);
	strncpy(st->name, token, length);
}
void ReadDisciplines(const char* token, Student* st)
{
	char* disciplineToken = strtok(token, ";");
	while (disciplineToken != NULL)
	{
		char* markToken = strchr(disciplineToken, ':');
		if (markToken != NULL)
		{
			*markToken = '\0';
			markToken++;
			Discipline discipline;
			discipline.mark = atoi(markToken);
			size_t length = strlen(disciplineToken) + 1;
			discipline.name = malloc(length);
			strncpy(discipline.name, disciplineToken, length);
			PushBackVec(&st->marks, &discipline);
		}
		disciplineToken = strtok(NULL, ";");
	}
}
void ReadEvents(const char* token, Student* st)
{
	char* eventToken = strtok(token, ";");
	while (eventToken != NULL)
	{
		char* dateToken = strchr(eventToken, ':');
		if (dateToken != NULL)
		{
			*dateToken = '\0';
			dateToken++;
			Event event = { .type = StringToEvenType(eventToken), .date = "" };
			strncpy(event.date, dateToken, DATE_LENGTH);
			PushBackVec(&st->events, &event);
		}
		eventToken = strtok(NULL, ";");
	}
}

Student ReadFromFile(char* str)
{
	static const ReadProc proc[] = { ReadId, ReadName, ReadDisciplines, ReadEvents };
	Student student = { .marks = ConstructVec(sizeof(Discipline)), .events = ConstructVec(sizeof(Event)) };

	Vector tokens = GetTokens(str);

	for (int i = 0; i < tokens.count; i++)
	{
		char* token = *(char**)AtVec(&tokens, i);
		proc[i](token, &student);
	}
	DestructVec(&tokens);
	return student;
}
void Deserialize(Vector* uni)
{
	FILE* file = fopen("test.txt", "r");
	if (file == NULL)
	{
		printf("File openning error\n");
		exit(-1);
	}
	char buffer[256];
	while (fgets(buffer, 256, file) != NULL)
	{
		Student student = ReadFromFile(buffer);
		PushBackVec(uni, &student);
	}
	fclose(file);
}
