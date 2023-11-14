#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <stdlib.h>

void createMemoryManager(void * heap);

void *malloc(const size_t size);

void free(void *memory);

/**
 * Prints the values of free used and largest free block of memory
 */
void printBitmap();

#endif