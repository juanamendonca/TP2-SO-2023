#ifndef PIPE_H
#define PIPE_H
#define BUFFER_SIZE 1024
#define MAX_LEN 20
#define MAX_PIPES 10
#define MAX_NAME 10

#include "defs.h"
#include "lib.h"

uint64_t initPipes();
uint64_t pipeOpen(char *name);
uint64_t pipeClose(uint64_t pipeIndex);
uint64_t writePipe(uint64_t pipeIndex, char *string);
char readPipe(uint64_t pipeIndex);
void pipe();
uint64_t writeChar(uint64_t pipeIndex, char c);

#endif
