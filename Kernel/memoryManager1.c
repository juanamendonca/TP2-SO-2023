#include <memoryManager.h>

void * nextAddress = (void *) 0x0000000000050000;
void * memStart = (void *) 0x0000000000050000;
void * memEnd   = (void *) 0x000000000009FFFF;



void createMemoryManager(void *const restrict managedMemory) {
    nextAddress = managedMemory;
}

void * malloc(const size_t memoryToAllocate) {
    void * allocation;

    if (nextAddress + 0x1000 < memEnd){
        allocation = nextAddress;
        nextAddress += 0x1000;
        return allocation;
    } else {
        return NULL;
    }


}

void free(void *memory){
    nextAddress = memStart;
    return;
}