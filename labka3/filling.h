#ifndef __FILLING_H__
#define __FILLING_H__

#include "task2.h"
#include "hashtable.h"
#include "vector.h"

#define INVALID_DATE "00.00.0000"

const char* EventTypeToString(EventType type); 
EventType StringToEvenType(char* str);
float FindGPA(Student* student);
Student ReadStudent();
void Serialize(HashTable* uni);
Vector GetTokens(char* str);
typedef  void (*ReadProc)(const char* token, Student* st);
void ReadEvents(const char* token, Student* st);
Student ReadFromFile(char* str);
void Deserialize(HashTable* uni);

#endif

