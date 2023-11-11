#include <memoryManager.h>
#include <video.h>
#include <bits/stdint-uintn.h>
#include <string.h>

void *nextAddress;
void *memStart;
void *memEnd;

#define HEAP_SIZE 320 * 1024 // 320 KiB
#define BLOCK_SIZE 4

// Calculate the number of bytes needed for the bitmap.
#define BLOCK_PER_BYTE (8 / 2) // 4 blocks per byte (00 00 00 00), 2 bits (FREE USED FIRST) for each block. 00 free, 01 used, 10 first
#define BITMAP_SIZE (HEAP_SIZE / BLOCK_SIZE / BLOCK_PER_BYTE)

// Initialize the bitmap and heap.
unsigned char bitmap[BITMAP_SIZE];
void* heap = (void*) 0x0000000000050000;
char first = 1;

void createMemoryManager(){
    for (int i = 0; i < BITMAP_SIZE; i++) {
        bitmap[i] = 0;
    }
    nextAddress = heap;
}

void *malloc(const size_t size) {

    size_t blocksQty = (size + BLOCK_SIZE - 1) / BLOCK_SIZE; // Cant de bloques a asignar

    int contiguousFreeBlocks = 0;
    int firstBlockIndex = -1;

    for (int it_block = 0; it_block < BITMAP_SIZE; it_block++) {
        for (int it_bit = 0; it_bit < BLOCK_PER_BYTE; it_bit++) {

            int state = (bitmap[it_block] >> (it_bit * 2)) & 0x03; //busco un bloque libre 00

            //Busca blocksQty cantidad de bloques libres que son los q voy a asignar
            if (state == 0) {
                if (contiguousFreeBlocks == 0) {
                    firstBlockIndex = (it_block * BLOCK_PER_BYTE) + it_bit;
                }
                contiguousFreeBlocks++;

                if (contiguousFreeBlocks == blocksQty) {
                    //Marco los bloques como FIRST USED USED... desde el primer bloque libre
                    for (int i = firstBlockIndex; i < firstBlockIndex + blocksQty; i++) {
                        int byteIndex = i / BLOCK_PER_BYTE;
                        int bitOffset = (i % BLOCK_PER_BYTE) * 2;
                        if (i == firstBlockIndex){
                            bitmap[byteIndex] |= (0x02 << bitOffset); // FIRST (10).
                        }else{
                            bitmap[byteIndex] |= (0x01 << bitOffset); // USED (01).
                        }
                    }


                    //calculo el address en funcion de la posicion del bitmap
                    return (void*)((uint64_t)heap + (firstBlockIndex * BLOCK_SIZE));
                }
            } else {
                // Si no alcanzan los bloques libres resetear
                contiguousFreeBlocks = 0;
                firstBlockIndex = -1;
            }
        }
    }

    return NULL;
}

void free(void *memory) {
    //Busco la ubicacion del bloque en el bitmap
    int firstBlockIndex = ((uint64_t)memory - (uint64_t)heap) / BLOCK_SIZE;


    int i;

    //Iteramos hasta encontrar el siguiente FIRST
    for (i = firstBlockIndex; i < BITMAP_SIZE * BLOCK_PER_BYTE; i++) {
        int byteIndex = i / BLOCK_PER_BYTE;
        int bitOffset = (i % BLOCK_PER_BYTE) * 2;
        int state = (bitmap[byteIndex] >> bitOffset) & 0x03;

        if (state == 0x02 && i != firstBlockIndex) {
            break;
        }

        bitmap[byteIndex] &= ~(0x03 << bitOffset); // seteo en 00 los bits a medida q hago el ciclo
    }
  return;
}

void printBitmap(){
    putArrayNext("Bitmap:\n", WHITE);
    for (int it_block = 0; it_block <  2 ; it_block++) {
        putArrayNext("BYTE ", WHITE);
        putDecNext(it_block, WHITE);
        putArrayNext(": ", WHITE);

        for (int it_bit = 0; it_bit < BLOCK_PER_BYTE * 2; it_bit += 2) {
            int state = (bitmap[it_block] >> it_bit) & 0x03;
            switch (state) {
                case 0:
                    putArrayNext("FREE ", WHITE);
                    break;
                case 1:
                    putArrayNext("USED ", WHITE);
                    break;
                case 2:
                    putArrayNext("FIRST ", GREEN);
                    break;
                default:
                    putArrayNext("UNKNOWN ", WHITE);
            }
        }
        putArrayNext("\n", WHITE);
    }
}