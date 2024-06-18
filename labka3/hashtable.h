#ifndef __HASHTABLE_H__
#define __HASHTABLE_H__

#include "vector.h"

typedef struct HashRecord
{
	struct HashRecord* next;
	void* key;
	int keySize;
	void* value;
}HashRecord;

typedef struct
{
	HashRecord** data;
	Vector keys;
	int size;
	int itemSize;
	void (*itemDtor)(void*);
	int (*KeyCompareFunc)(void*, void*);
}HashTable;

int CompareKeys(void* lhs, void* rhs);
HashTable HTCreate(int itemSize, void (*itemDtor)(void*), int (*KeyCompareFunc)(void*, void*));
void HTInsert(HashTable* ht, void* key, void* value);
void HTRemove(HashTable* ht, void* key);
void HTDestroy(HashTable* ht);
void* HTFind(HashTable* ht, void* key);


#endif
