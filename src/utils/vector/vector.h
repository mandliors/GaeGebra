#pragma once

#include <stdlib.h>
#include <stdbool.h>

typedef struct Vector {
	size_t capacity;
	size_t size;
	void** data;
} Vector;

Vector* vector_create(size_t capacity);
void vector_free(Vector* vector);
void* vector_get(Vector* vector, size_t idx);
void vector_push_back(Vector* vector, void* value);
void vector_pop_back(Vector* vector);
void vector_insert(Vector* vector, size_t idx, void* value);
bool vector_contains(Vector* vector, void* value);
void vector_remove_at(Vector* vector, size_t idx);
void vector_remove(Vector* vector, void* value);
void vector_reserve(Vector* vector, size_t capacity);
size_t vector_size(Vector* vector);
void vector_clear(Vector* vector);
