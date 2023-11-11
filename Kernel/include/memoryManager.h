#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <stdlib.h>

void createMemoryManager();

void *malloc(const size_t size);

void free(void *memory);

//For testing porpouses
void printBitmap();

#endif