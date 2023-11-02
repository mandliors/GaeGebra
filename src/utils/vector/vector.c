#include "vector.h"
#include <assert.h>

Vector* vector_create(size_t capacity)
{
	Vector* vector = malloc(sizeof(Vector));
	assert(vector); //couldn't allocate memory
	if (capacity > 0)
	{
		vector->data = (void**)malloc(capacity * sizeof(void*));
		assert(vector->data); //couldn't allocate memory
		vector->capacity = capacity;
		vector->size = 0;
	}
	else
	{
		vector->data = NULL;
		vector->capacity = 0;
		vector->size = 0;
	}
	return vector;
}
void vector_free(Vector* vector)
{
	assert(vector); //vector is NULL
	free(vector->data);
	free(vector);
}

void* vector_get(Vector* vector, size_t idx)
{
	assert(vector); //vector is NULL
	assert(idx < vector->size); //index out of range on vector
	return vector->data[idx];
}
void vector_push_back(Vector* vector, void* value)
{
	assert(vector); //vector is NULL
	if (vector->size == vector->capacity)
	{
		vector->capacity = vector->capacity == 0 ? 1 : vector->capacity * 2;
		vector->data = (void**)realloc(vector->data, sizeof(void*) * vector->capacity);
		assert(vector->data); //couldn't allocate memory
	}
	vector->data[vector->size++] = value;
}
void vector_reserve(Vector* vector, size_t capacity)
{
	assert(vector); //vector is NULL
	if (capacity > vector->capacity)
	{
		vector->data = (void**)realloc(vector->data, sizeof(void*) * capacity);
		assert(vector->data); //couldn't allocate memory
		vector->capacity = capacity;
	}
}
void vector_clear(Vector* vector)
{
	assert(vector); //vector is NULL
	free(vector->data);
	vector->capacity = 0;
	vector->size = 0;
}
