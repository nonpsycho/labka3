#ifndef __VECTOR_H__ 
#define __VECTOR_H__ 


typedef struct
{
	void* data;
	int count;
	int capacity;
	int type; 
} Vector;

Vector ConstructVec(int el_size);
void PushBackVec(Vector* str, void* el);
void* BackVec(Vector* v);
void* AtVec(Vector* v, int idx);
void* FindIdxVec(Vector* v, void* ref, int(*cmp)(void*, void*));
void* FindVec(Vector* v, void* ref, int(*cmp)(void*, void*));
void RemoveVec(Vector* v, int idx);
void DestructVec(Vector* str);

#endif


