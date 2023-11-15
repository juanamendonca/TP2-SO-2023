#include "video.h"
#include <stdint.h>


#define REGISTERS_QTY 17

extern const uint64_t regs[18];
void zeroException();
void invalidOpException();
extern void reset();

void exceptionDispatcher(void *excepCode) {
  switch ((uint64_t)excepCode) {
  case 0:
    putLine();
    zeroException();
    break;
  case 6:
    putLine();
    invalidOpException();
    break;
  }

  printError("--REGISTERS AT TIME OF EXCEPTION--");
  putLine();
  char *registers[REGISTERS_QTY] = {"RAX", "RBX", "RCX", "RDX", "RSI", "RDI",
                                    "RBP", "R8 ", "R9 ", "R10", "R11", "R12",
                                    "R13", "R14", "R15", "RIP", "RSP"};
  for (int i = 0; i < REGISTERS_QTY; i++) {
    int j = 0;
    while (registers[i][j] != 0) {
      putLetterNext(registers[i][j], WHITE);
      j++;
    }
    putLetterNext(':', WHITE);
    putLetterNext(' ', WHITE);
    putLetterNext(' ', WHITE);
    putLetterNext('0', WHITE);
    putLetterNext('x', WHITE);
    printHexa(regs[i]);
    putLine();
  }
  printError("--RESTARTING--");
  putLine();
}

void zeroException() {
  printError("ERROR: DIVIDING BY ZERO");
  putLine();
}

void invalidOpException() {
  printError("ERROR: INVALID OPERATION CODE");
  putLine();
}
