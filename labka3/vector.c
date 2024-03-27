#include "vector.h"
#include <stdlib.h>
#include <string.h>


Vector ConstructVec(int el_size)
{
	Vector v = { .count = 0, .data = malloc(el_size), .capacity = el_size, .type = el_size };
	return v;
}

void* BackVec(Vector* v)
{
	return (char*)v->data + (v->count - 1) * v->type;
}

void* AtVec(Vector* v, int idx)
{
	if (idx < 0 || idx >= v->count)
	{
		return NULL;
	}
	return (char*)v->data + idx * v->type;
}

void PushBackVec(Vector* v, void* el)
{
	if (v->capacity <= v->count * v->type)
	{
		v->data = realloc(v->data, v->capacity * 2);
		v->capacity *= 2;
	}
	memcpy((unsigned char*)v->data + v->count * v->type, el, v->type);
	v->count++;
}

void RemoveVec(Vector* v, int idx)
{
	for (int i = idx; idx < v->count - 1; i++)
	{
		memcpy((unsigned char*)v->data + i * v->type, (unsigned char*)v->data + (i + 1) * v->type, v->type);
	}
	v->count--;
}

void DestructVec(Vector* v)
{
	free(v->data);
	v->capacity = 0;
	v->count = 0;
	v->type = 0;
	v->data = NULL;
}