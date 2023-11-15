#include <memoryManager.h>
#include <video.h>
#include <bits/stdint-uintn.h>
#include <string.h>

void *nextAddress;
void *memStart;
void *memEnd;

#define HEAP_SIZE 320 * 1024// 320 KiB  320 * 1024
#define BLOCK_SIZE 4

// Calculate the number of bytes needed for the bitmap.
#define BLOCK_PER_BYTE (8 / 2) // 4 blocks per byte (00 00 00 00), 2 bits (FREE USED FIRST) for each block. 00 free, 01 used, 10 first
#define BITMAP_SIZE (HEAP_SIZE / BLOCK_SIZE / BLOCK_PER_BYTE)

// Initialize the bitmap and heap.
unsigned char bitmap[BITMAP_SIZE];
char first = 1;



void createMemoryManager(void * heap){
    for (int i = 0; i < BITMAP_SIZE; i++) {
        bitmap[i] = 0;
    }
    memStart    = heap;
    nextAddress = heap;
}

void *malloc(const size_t size) {

    size_t blocksQty = (size + BLOCK_SIZE - 1) / BLOCK_SIZE; // Blocks quantity to assign

    int contiguousFreeBlocks = 0;
    int firstBlockIndex = -1;

    for (int it_block = 0; it_block < BITMAP_SIZE; it_block++) {
        for (int it_bit = 0; it_bit < BLOCK_PER_BYTE; it_bit++) {

            int state = (bitmap[it_block] >> (it_bit * 2)) & 0x03; // Search for a free block

            //Search blocksQty blocks quantity (free ones) that will be assigned
            if (state == 0) {
                if (contiguousFreeBlocks == 0) {
                    firstBlockIndex = (it_block * BLOCK_PER_BYTE) + it_bit;
                }
                contiguousFreeBlocks++;

                if (contiguousFreeBlocks == blocksQty) {
                    //Mark block as FIRST USED USED... from the first free block
                    for (int i = firstBlockIndex; i < firstBlockIndex + blocksQty; i++) {
                        int byteIndex = i / BLOCK_PER_BYTE;
                        int bitOffset = (i % BLOCK_PER_BYTE) * 2;
                        if (i == firstBlockIndex){
                            bitmap[byteIndex] |= (0x02 << bitOffset); // FIRST (10).
                        }else{
                            bitmap[byteIndex] |= (0x01 << bitOffset); // USED (01).
                        }
                    }


                    //Calculates the address in function of bitmap position
                    return (void*)((uint64_t)memStart + (firstBlockIndex * BLOCK_SIZE));
                }
            } else {
                // If there is not enough free block, then reset
                contiguousFreeBlocks = 0;
                firstBlockIndex = -1;
            }
        }
    }


    return NULL;
}

void free(void *memory) {
    //Search for the block location in the bitmap 
    int firstBlockIndex = ((uint64_t)memory - (uint64_t)memStart) / BLOCK_SIZE;


    int i;

    //Iterate up to find the next FIRST
    for (i = firstBlockIndex; i < BITMAP_SIZE * BLOCK_PER_BYTE; i++) {
        int byteIndex = i / BLOCK_PER_BYTE;
        int bitOffset = (i % BLOCK_PER_BYTE) * 2;
        int state = (bitmap[byteIndex] >> bitOffset) & 0x03;

        if (state == 0x02 && i != firstBlockIndex) {
            break;
        }

        bitmap[byteIndex] &= ~(0x03 << bitOffset); 
    }
  return;
}


void printBitmap() {
    int freeBlocks = 0;
    int usedBlocks = 0;
    int largestFreeBlock = 0;

    int contiguousFreeBlocks = 0;

    for (int it_block = 0; it_block < BITMAP_SIZE; it_block++) {
        for (int it_bit = 0; it_bit < BLOCK_PER_BYTE * 2; it_bit += 2) {
            int state = (bitmap[it_block] >> it_bit) & 0x03;
            switch (state) {
                case 0:  // FREE
                    contiguousFreeBlocks++;
                    largestFreeBlock++;
                    break;
                case 1:  // USED
                    usedBlocks++;
                    contiguousFreeBlocks = 0;
                    break;
                case 2:  // FIRST
                    freeBlocks += contiguousFreeBlocks;
                    usedBlocks++;
                    contiguousFreeBlocks = 0;

                    if (contiguousFreeBlocks > largestFreeBlock){
                        largestFreeBlock = contiguousFreeBlocks;
                    }
                    break;
                default:
                    break;
            }
        }
    }

    if (contiguousFreeBlocks > 0) {
        freeBlocks += contiguousFreeBlocks * BLOCK_SIZE;
    }

    // Print the results (replace with your own print function)
    putArrayNext("Block size: 4 bytes\nFree blocks: ", WHITE);
    putDecNext(freeBlocks, WHITE);
    putArrayNext("\nUsed blocks: ", WHITE);
    putDecNext(usedBlocks, WHITE);
    putArrayNext("\nLargest Free block: ", WHITE);
    putDecNext(largestFreeBlock, WHITE);
    putArrayNext("\n", WHITE);

}
