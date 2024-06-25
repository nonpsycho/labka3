#include "hashtable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define HASH_TABLE_SIZE 256

static unsigned int Hash(void* key)
{
	float f = *(float*)key;
	uint32_t bits = *(uint32_t*)&f;
	return (int)(bits % HASH_TABLE_SIZE);
}

int CompareKeys(void* lhs, void* rhs)
{
	float lKey = *(float*)lhs;
	float rKey = *(float*)rhs;
	if (lKey < rKey)
	{
		return -1;
	}	
	else if (lKey > rKey)
	{
		return 1;
	}	
	else
	{
		return 0;
	}
}

static HashRecord* GetRecord(HashTable* ht, void* key)
{
	if (FindVec(&ht->keys, key, ht->KeyCompareFunc))
	{
		HashRecord* record = &ht->data[Hash(key)];
		while (record)
		{
			if (record->key == key)
			{
				return record;
			}
			record = record->next;
		}
	}
	return NULL;
}

HashTable HTCreate(int itemSize, void (*itemDtor)(void*), int (*KeyCompareFunc)(void*, void*))
{
	HashTable ht = {
		.keys = ConstructVec(sizeof(float)),
		.data = calloc(HASH_TABLE_SIZE, sizeof(HashRecord*)),
		.itemSize = itemSize,
		.size = HASH_TABLE_SIZE,
		.itemDtor = itemDtor,
		.KeyCompareFunc = KeyCompareFunc
	};
	return ht;
}

void HTInsert(HashTable* ht, void* key, void* value)
{
	if (FindVec(&ht->keys, &key, ht->KeyCompareFunc) != NULL)
	{
		HTRemove(ht, key);
	}
	PushBackVec(&ht->keys, key);

	HashRecord* newRecord = calloc(1, sizeof(HashRecord));
	newRecord->value = malloc(ht->itemSize);
	if (newRecord->value == NULL) 
	{
		free(newRecord);
		return;
	}
	memcpy(newRecord->value, value, ht->itemSize);
	newRecord->key = key;

	HashRecord* bucket = ht->data[Hash(key)];
	if (bucket != NULL)
	{
		while (bucket->next)
		{
			bucket = bucket->next;
		}
		bucket->next = newRecord;
	}
	else
	{
		ht->data[Hash(key)] = newRecord;
	}
}

void HTRemove(HashTable* ht, void* key)
{
	HashRecord* record = GetRecord(ht, key);
	if (record)
	{
		if (ht->itemDtor != NULL)
		{
			ht->itemDtor(record->value);
		}
		free(record->value);

		HashRecord* bucket = ht->data[Hash(key)]->key;
		if (record == bucket)	// First item in the bucket
		{
			ht->data[Hash(key)] = record->next;
		}
		else
		{
			HashRecord* prev = bucket;
			while (prev->next != record)
			{
				prev = prev->next;
			}
			prev->next = record->next;
		}
		free(record);
		int keyIdx = FindIdxVec(&ht->keys, &key, ht->KeyCompareFunc);
		RemoveVec(&ht->keys, keyIdx);
	}
}

void HTDestroy(HashTable* ht)
{
	for (int i = 0; i < ht->keys.count; i++)
	{
		void* key = (void*)AtVec(&ht->keys, i);
		HTRemove(ht, key);
	}
	free(ht->data);
	DestructVec(&ht->keys);
}

void* HTFind(HashTable* ht, void* key)
{
	HashRecord *record = GetRecord(ht, key);
	if (record)
	{
		return record->value;
	}
	return NULL;
}
