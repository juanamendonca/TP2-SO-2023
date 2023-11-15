#include "keyboard_driver.h"
#include "lib.h"
#include "naiveConsole.h"
#include "pipe.h"
#include "scheduler.h"
#include "video.h"
#include <stdbool.h>

#define BUFF_SIZE 256
#define TECLA_LIMITE_SUPERIOR 90
#define CONTROL 0x1D
#define STDIN 0

static char buff[BUFF_SIZE] = {0}; // Circular vector. When buff reaches max capacity older elements are
                                   // overwritten. Keyboard buffer
static int front = 0; // Position of the first element
static int rear = 0;  // Position of last added element
static int cantElems = 0;
bool savedRegs = false;
int blockedProcess = -1;

// Circular vector. When buff reaches max capacity older elements are
// overwritten.
char nextElement() {
  pcb *pcb = getCurrentPcb();
  if (pcb->fd[0] == STDIN) {

    if (cantElems == 0) {
      blockedProcess = pcb->pid;
      block(blockedProcess);
    }

    char c = buff[front];

    cantElems--;
    front++;

    if (front == BUFF_SIZE)
      front = 0;

    return c;
  } else {
    return readPipe(pcb->fd[0]);
  }
}

int cantElements() { return cantElems; }

void cleanBuffer() {
  cantElems = 0;
  front = 0;
  rear = 0;
}

void writeEOF() {
  pcb *pcb = getCurrentPcb();
  if (pcb->fd[0] == STDIN) {
    buff[rear++] = '\0';
    cantElems++;
  } else {
    writeChar(pcb->fd[0], '\0');
  }
}

void keyHandler(uint64_t scancode) {
  char tecla = scancode;

  if (tecla <= TECLA_LIMITE_SUPERIOR) {
    if (cantElems == BUFF_SIZE) {
      return;
    }


    if (tecla == CONTROL) {
      savedRegs = true;
      return;
    }

    // Both ifs are necessary to maintain buffer circularity
    if (rear == BUFF_SIZE)
      rear = 0;
    if (front == BUFF_SIZE)
      front = 0;

    if ( scancode == 0x2A) {        //left shift
        writeEOF();
    } else if (scancode == 0x36) {  //right shift
        killCurrentForeground();
    } else if (keyBoardTable[(int)tecla] != 0) {
      buff[rear++] = keyBoardTable[(int)tecla];
      cantElems++;
    }
  }
  if (cantElems > 0 && blockedProcess != -1) {
    int unblockP = blockedProcess;
    blockedProcess = -1;
    unblock(unblockP);
  }
}

bool isRegsSaved() { return savedRegs; }
