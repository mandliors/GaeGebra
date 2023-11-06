#pragma once

#include <stdlib.h>

typedef struct Vector {
	size_t capacity;
	size_t size;
	void** data;
} Vector;

Vector* vector_create(size_t capacity);
void vector_free(Vector* vector);

void* vector_get(Vector* vector, size_t idx);
void vector_push_back(Vector* vector, void* value);
void vector_reserve(Vector* vector, size_t capacity);
size_t vector_size(Vector* vector);
void vector_clear(Vector* vector);
