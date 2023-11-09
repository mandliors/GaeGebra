#include "vector.h"
#include <stdio.h>

Vector* vector_create(size_t capacity)
{
	Vector* vector = malloc(sizeof(Vector));
	if (vector == NULL)
	{
		printf("failed to allocate memory for vector\n");
		exit(1);
	}
	if (capacity > 0)
	{
		vector->data = (void**)malloc(capacity * sizeof(void*));
		if (vector->data == NULL)
		{
			printf("failed to allocate memory for vector data\n");
			exit(1);
		}
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
	if (vector == NULL)
		return;
	free(vector->data);
	free(vector);
}

void* vector_get(Vector* vector, size_t idx)
{
	if (vector == NULL || idx >= vector->size)
		return NULL;
	return vector->data[idx];
}
void vector_push_back(Vector* vector, void* value)
{
	if (vector == NULL)
		return;
	if (vector->size == vector->capacity)
	{
		vector->capacity = vector->capacity == 0 ? 1 : vector->capacity * 2;
		vector->data = (void**)realloc(vector->data, sizeof(void*) * vector->capacity);
		if (vector->data == NULL)
		{
			printf("failed to allocate memory for vector data\n");
			exit(1);
		}
	}
	vector->data[vector->size++] = value;
}
void vector_pop_back(Vector* vector)
{
	if (vector == NULL || vector->size == 0)
		return;
	vector->size--;
}
void vector_insert(Vector* vector, size_t idx, void* value)
{
	if (vector == NULL || idx > vector->size)
		return;
	if (vector->size == vector->capacity)
	{
		vector->capacity = vector->capacity == 0 ? 1 : vector->capacity * 2;
		vector->data = (void**)realloc(vector->data, sizeof(void*) * vector->capacity);
		if (vector->data == NULL)
		{
			printf("failed to allocate memory for vector data\n");
			exit(1);
		}
	}
	for (size_t i = vector->size; i > idx; i--)
		vector->data[i] = vector->data[i - 1];
	vector->data[idx] = value;
	vector->size++;
}
bool vector_contains(Vector* vector, void* value)
{
	if (vector == NULL)
		return false;
	for (size_t i = 0; i < vector->size; i++)
		if (vector->data[i] == value)
			return true;
	return false;
}
void vector_remove_at(Vector* vector, size_t idx)
{
	if (vector == NULL || idx >= vector->size)
		return;
	for (size_t i = idx; i < vector->size - 1; i++)
		vector->data[i] = vector->data[i + 1];
	vector->size--;
}
void vector_remove(Vector* vector, void* value)
{
	if (vector == NULL)
		return;
	for (size_t i = 0; i < vector->size; i++)
	{
		if (vector->data[i] == value)
		{
			vector_remove_at(vector, i);
			return;
		}
	}
}
void vector_reserve(Vector* vector, size_t capacity)
{
	if (vector == NULL)
		return;
	if (capacity > vector->capacity)
	{
		vector->data = (void**)realloc(vector->data, sizeof(void*) * capacity);
		if (vector->data == NULL)
		{
			printf("failed to allocate memory for vector data\n");
			exit(1);
		}
		vector->capacity = capacity;
	}
}
size_t vector_size(Vector* vector)
{
	if (vector == NULL)
		return 0;
	return vector->size;
}
void vector_clear(Vector* vector)
{
	if (vector == NULL)
		return;
	free(vector->data);
	vector->capacity = 0;
	vector->size = 0;
}
