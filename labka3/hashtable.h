#ifndef __HASHTABLE_H__
#define __HASHTABLE_H__

#include "vector.h"

typedef struct HashRecord
{
	struct HashRecord* next;
	int key;
	void* value;
}HashRecord;

typedef struct
{
	HashRecord** data;
	Vector keys;
	int size;
	int itemSize;
	void (*itemDtor)(void*);
}HashTable;

HashTable HTCreate(int itemSize, void (*itemDtor)(void*));
void HTInsert(HashTable* ht, int key, void* value);
void HTRemove(HashTable* ht, int key);
void HTDestroy(HashTable* ht);
void* HTFind(HashTable* ht, int key);


#endif
