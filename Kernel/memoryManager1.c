#include <memoryManager.h>

char *nextAddress;

void createMemoryManager(void *const restrict managedMemory)
{
    nextAddress = managedMemory;
}

void *allocMemory(const size_t memoryToAllocate)
{
    char *allocation = nextAddress;

    nextAddress += memoryToAllocate;

    return (void *)allocation;
}