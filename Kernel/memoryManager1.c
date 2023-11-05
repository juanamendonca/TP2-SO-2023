#include <memoryManager.h>

void *nextAddress;
void *memStart;
void *memEnd;

void createMemoryManager(void *const restrict managedMemory, int heapSize) {
  nextAddress = managedMemory;
  memStart = managedMemory;
  memEnd = memStart + heapSize;
}

void *malloc(const size_t memoryToAllocate) {
  void *allocation;

  if (nextAddress + 0x1000 < memEnd) {
    allocation = nextAddress;
    nextAddress += 0x1000;
    return allocation;
  } else {
    return NULL;
  }
}

void free(void *memory) {
  nextAddress = memStart;
  return;
}