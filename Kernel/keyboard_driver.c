#include <keyboard_driver.h>
#include <video.h>
#include <lib.h>
#include <naiveConsole.h>
#include <stdbool.h>
#define BUFF_SIZE 256
#define TECLA_LIMITE_SUPERIOR 90
#define CONTROL 0x1D

static char buff[BUFF_SIZE] = {0}; // Circular vector. When buff reaches max capacity older elements are overwritten. Keyboard buffer 
static int front = 0;              // Position of the first element
static int rear = 0;               // Position of last added element
static int cantElems = 0;
bool savedRegs = false; 

char nextElement()// Circular vector. When buff reaches max capacity older elements are overwritten.

{

    if (cantElems == 0) {
        return -1;
    }

    char c = buff[front];

    cantElems--;
    front++;

    if (front == BUFF_SIZE) front = 0;

    return c;

}

int cantElements(){
    return cantElems;
}

void cleanBuffer(){
    cantElems=0;
    front=0;
    rear=0;
}

void keyHandler(uint64_t scancode)
{
    char tecla = scancode;

    if (tecla <= TECLA_LIMITE_SUPERIOR)
    {
        if (cantElems == BUFF_SIZE)
        {
            return;
        }
       if(tecla == CONTROL){
            savedRegs = true;
            return;
        }
    
       // Both ifs are necessary to maintain buffer circularity
        if (rear == BUFF_SIZE)
            rear = 0;
        if (front == BUFF_SIZE)
            front = 0;
        buff[rear++] = keyBoardTable[tecla];
        cantElems++;
    }

}

bool isRegsSaved(){
    return savedRegs;
}
