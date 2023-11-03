#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <stdlib.h>

void createMemoryManager(void *const restrict managedMemory);

void *allocMemory(const size_t memoryToAllocate);

#endif