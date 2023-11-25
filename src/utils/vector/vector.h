#pragma once

#include <stdlib.h>
#include <stdbool.h>

/**
 * @brief A generic vector type (dynamic array for void pointers)
 */
typedef struct Vector {
	size_t capacity;
	size_t size;
	void** data;
} Vector;

/**
 * @brief Creates a new vector
 * 
 * @param capacity The initial capacity of the vector
 * @return Vector* The created vector (must be freed with vector_destroy)
 */
Vector* vector_create(size_t capacity);
/**
 * @brief Destroys a vector
 * 
 * @param vector The vector to destroy
 */
void vector_destroy(Vector* vector);
/**
 * @brief Gets the value at the specified index
 * 
 * @param vector The vector to get the value from
 * @param idx The index of the element to get
 * @return void* The value at the specified index
 */
void* vector_get(Vector* vector, size_t idx);
/**
 * @brief Sets the value at the specified index
 * 
 * @param vector The vector to set the value in
 * @param idx The index of the element to set
 * @param value The value to set
 */
void vector_set(Vector* vector, size_t idx, void* value);
/**
 * @brief Pushes a value to the back of the vector
 * 
 * @param vector The vector to push to
 * @param value The value to push
 */
void vector_push_back(Vector* vector, void* value);
/**
 * @brief Pops a value from the back of the vector
 * 
 * @param vector The vector to pop from
 * @return void* The popped value
 */
void* vector_pop_back(Vector* vector);
/**
 * @brief Inserts a value at the specified index
 * 
 * @param vector The vector to insert into
 * @param idx The index to insert at
 * @param value The value to insert
 */
void vector_insert(Vector* vector, size_t idx, void* value);
/**
 * @brief Checks if the vector contains a value
 * 
 * @param vector The vector to check
 * @param value The value to check for
 * @return true If the vector contains the value
 * @return false If the vector does not contain the value
 */
bool vector_contains(Vector* vector, void* value);
/**
 * @brief Returns the index of a value in the vector
 * 
 * @param vector The vector to search
 * @param value The value to search for
 * @return int The index of the value in the vector, or -1 if not found
 */
int vector_index_of(Vector* vector, void* value);
/**
 * @brief Removes the value at the specified index
 * 
 * @param vector The vector to remove from
 * @param idx The index of the value to remove
 */
void vector_remove_at(Vector* vector, size_t idx);
/**
 * @brief Removes the first occurence of a value from the vector
 * 
 * @param vector The vector to remove from
 * @param value The value to remove
 */
void vector_remove(Vector* vector, void* value);
/**
 * @brief Reserves a new capacity for the vector
 * 
 * @param vector The vector to reserve for
 * @param size The new capacity of the vector
 */
void vector_reserve(Vector* vector, size_t capacity);
/**
 * @brief Returns the size of the vector
 * 
 * @param vector The vector to get the size of
 * @return size_t The size of the vector
 */
size_t vector_size(Vector* vector);
/**
 * @brief Clears the vector
 * 
 * @param vector The vector to clear
 */
void vector_clear(Vector* vector);
