#ifndef __TASK2_H__
#define __TASK2_H__

#define DATE_LENGTH 11 //yyyy.mm.dd\0
#include "vector.h"

typedef enum
{
	ENROLLMENT,
	EXPULSION,
	RECOVERY,
	ERRORTYPE
}EventType;

typedef struct 
{
	EventType type;
	char date[DATE_LENGTH];
}Event;

typedef struct
{
	int mark;
	char* name;
}Discipline;

typedef struct
{
	int id;
	char* name;
	Vector marks;
	Vector events;
}Student;

#endif


