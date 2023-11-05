#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <stdlib.h>

void createMemoryManager(void *const restrict managedMemory, int heapSize);

void *malloc(const size_t memoryToAllocate);

void free(void *memory);

#endif