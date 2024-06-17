#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

#define HASH_TABLE_SIZE 256

static int Hash(int key)
{
	return key % HASH_TABLE_SIZE;
}

static int CompareKeys(void* lhs, void* rhs)
{
	int lKey = *(int*)(lhs);
	int rKey = *(int*)(rhs);
	return lKey - rKey;
}

static HashRecord* GetRecord(HashTable* ht, int key)
{
	if (FindVec(&ht->keys, &key, CompareKeys))
	{
		HashRecord* record = &ht->data[Hash(key)];
		while (record)
		{
			if (record->key == key)
			{
				return record;
			}
			record == record->next;
		}
	}
	return NULL;
}

HashTable HTCreate(int itemSize, void (*itemDtor)(void*))
{
	HashTable ht = {
		.keys = ConstructVec(sizeof(int)),
		.data = calloc(sizeof(HashRecord*), HASH_TABLE_SIZE),
		.itemSize = itemSize,
		.size = HASH_TABLE_SIZE,
		.itemDtor = itemDtor
	};
	return ht;
}

void HTInsert(HashTable* ht, int key, void* value)
{
	PushBackVec(&ht->keys, key);

	HashRecord* newRecord = calloc(sizeof(HashRecord), 1);
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

void HTRemove(HashTable* ht, int key)
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
		int keyIdx = FindIdxVec(&ht->keys, &key, CompareKeys);
		RemoveVec(&ht->keys, keyIdx);
	}
}

void HTDestroy(HashTable* ht)
{
	for (int i = 0; i < ht->keys.count; i++)
	{
		int key = *(int*)AtVec(&ht->keys, i);
		HTRemove(ht, key);
	}
	free(ht->data);
	DestructVec(&ht->keys);
}

void* HTFind(HashTable* ht, int key)
{
	HashRecord *record = GetRecord(ht, key);
	if (record)
	{
		return record->value;
	}
	return NULL;
}
