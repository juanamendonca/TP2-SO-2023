#include "pipe.h"
#include "sys_calls.h"
#include "video.h"
#include "strings.h"
#include "video.h"

typedef struct
{
    uint64_t rIndex;
    uint64_t wIndex;
    int semRead;  // Semaphore ID for reading in this pipe
    int semWrite; // Semaphore ID for writing in this pipe
    char buffer[BUFFER_SIZE];
    char name[MAX_LEN];
    int amountProcesses; // Number of processes using the pipe
} PipeType;

typedef struct
{
    PipeType pipe;
    uint64_t available;
} Space;

uint64_t semPipeManager; // Semaphore ID for controlling access to the pipe array.

static Space pipes[MAX_PIPES];

char * pipeName = "Pipe Manager";

static uint64_t indexValid(uint64_t pipeIndex);
static uint64_t createPipe(char *name);
static uint64_t findPipe(char *name);
static uint64_t findAvailableSpace();
uint64_t writeChar(uint64_t pipeIndex, char c);
void printPipe(PipeType pipe);
void printSemsInvolved(PipeType pipe);
void printProcesses(PipeType pipe);

// Returns -1 in case of error and 0 otherwise.
uint64_t initPipes()
{
    if ((semPipeManager = sem_open(pipeName, 1)) == -1)
    {
        putArrayNext("Error in initPipes",0xFFFFFF);
        return -1;
    }
    for (int i = 0; i < MAX_PIPES; i++)
    {
        pipes[i].available = TRUE;
    }
    return 0;
}

// Returns -1 in case of error
uint64_t pipeOpen(char *name)
{
    if (sem_wait(semPipeManager))
    {
        print("Error sem_wait in pipeOpen\n");
        return -1;
    }
    // Check if a pipe with the given name exists in our data structure

    int id = findPipe(name);
    if (id == 0)
    {
        // If no pipe with that name exists
        id = createPipe(name);
    }
    if (id == -1)
    {
        print("Error in pipeOpen, id=-1\n");
        sem_post(semPipeManager);
        return -1;
    }
    pipes[id - 1].pipe.amountProcesses++;
    if (sem_post(semPipeManager))
    {
        print("Error sem_post in pipeOpen\n");
        return -1;
    }
    return id;
}

uint64_t pipeClose(uint64_t pipeIndex)
{
    if (!indexValid(pipeIndex))
        return -1;

    if (sem_wait(semPipeManager) == -1)
    {
        print("Error sem_wait in pipeClose\n");
        return -1;
    }

    int closeRead = sem_close(getSemName(pipes[pipeIndex - 1].pipe.semRead));
    int closeWrite = sem_close(getSemName(pipes[pipeIndex - 1].pipe.semWrite));

    if (closeRead == -1 || closeWrite == -1)
    {
        print("pipeClose: Error in semaphore close for the pipe\n");
        return -1;
    }
    pipes[pipeIndex - 1].available = TRUE;

    if (sem_post(semPipeManager) == -1)
    {
        print("Error sem_post in pipeClose\n");
        return -1;
    }
    return 1;
}

uint64_t writePipe(uint64_t pipeIndex, char *string)
{
    if (!indexValid(pipeIndex))
        return -1;

    while (*string != 0)
    {
        if ((writeChar(pipeIndex, *string++)) == -1)
            return -1;
    }
    return 0;
}

uint64_t writeChar(uint64_t pipeIndex, char c)
{
    if (!indexValid(pipeIndex))
        return -1;

    PipeType *pipe = &pipes[pipeIndex - 1].pipe;
    if (sem_wait(pipe->semWrite) == -1)
    {
        print("Error in sem_wait in writeChar\n");
        return -1;
    }
    pipe->buffer[pipe->wIndex % BUFFER_SIZE] = c;
    pipe->wIndex++;
    if (sem_post(pipe->semRead) == -1)
    {
        print("Error in sem_post in writeChar\n");
        return -1;
    }
    return 1;
}

char readPipe(uint64_t pipeIndex)
{
    if (!indexValid(pipeIndex))
        return -1;

    PipeType *pipe = &pipes[pipeIndex - 1].pipe;
    if (sem_wait(pipe->semRead) == -1)
    {
        print("Error in sem_wait in readPipe\n");
        return -1;
    }
    char c = pipe->buffer[pipe->rIndex % BUFFER_SIZE];
    pipe->rIndex++;
    if (sem_post(pipe->semWrite) == -1)
    {
        print("Error in sem_post in readPipe\n");
        return -1;
    }
    return c;
}

// Returns 0 if no pipe with the name exists, the position + 1 if it exists, and -1 in case of an error.
static uint64_t findPipe(char *name)
{
    for (int i = 0; i < MAX_PIPES; i++)
    {
        if (pipes[i].available == FALSE && strcmp(name, pipes[i].pipe.name))
        {
            return i + 1;
        }
    }
    return 0;
}

// Returns the ID of the pipe on success and -1 if there was an error.
static uint64_t createPipe(char *name)
{
    int len = strlen(name);
    if (len <= 0 || len >= MAX_NAME-1)
    {
        print("createPipe: Name is too long\n");
        return -1;
    }
    uint64_t pos;
    if ((pos = findAvailableSpace()) != -1)
    {
        PipeType *newPipe = &pipes[pos].pipe;
        // Initialize the structure
        memcpy(newPipe->name, name, len);
        newPipe->rIndex = 0;
        newPipe->wIndex = 0;
        char nameR[MAX_NAME];
        memcpy(nameR, name, len);
        nameR[len] = 'R';
        nameR[len + 1] = 0;
        uint64_t semRead = sem_open(nameR, 0);
        char nameW[MAX_NAME];
        memcpy(nameW, name, len);
        nameW[len] = 'W';
        nameW[len + 1] = 0;
        uint64_t semWrite = sem_open(nameW, BUFFER_SIZE);
        if (semRead == -1 || semWrite == -1)
        {
            print("pipeOpen: Error in pipe's semaphores\n");
            return -1;
        }
        newPipe->semRead = semRead;
        newPipe->semWrite = semWrite;
    }
    return pos + 1;
}

static uint64_t findAvailableSpace()
{
    for (int i = 0; i < MAX_PIPES; i++)
    {
        if (pipes[i].available == TRUE)
        {
            pipes[i].available = FALSE;
            return i;
        }
    }
    return -1; // No more space in the array to create another pipe
}

static uint64_t indexValid(uint64_t pipeIndex)
{
    return ((pipeIndex == 0 || pipeIndex > MAX_PIPES) && pipes[pipeIndex - 1].available == FALSE) ? 0 : 1;
}

void pipe()
{
    print("PIPE'S NAME  STATE  SEMAPHORES INVOLVED  BLOCKED PROCESSES\n");
    int i;
    for(i = 0; i < MAX_PIPES; i++)
    {
        if(!(pipes[i].available))
        {
            printPipe(pipes[i].pipe);
        }
    }
    print("\n");
}

void printPipe(PipeType pipe)
{
    print(pipe.name);
    print("    ");
    print("Active");
    printSemsInvolved(pipe);
    print(" ");
    printProcesses(pipe);
    print(" ");
}

void printSemsInvolved(PipeType pipe)
{
    print(getSemName(pipe.semRead));
    print(", ");
    print(getSemName(pipe.semWrite));
    print(", ");
    print(getSemName(semPipeManager));
}

void printProcesses(PipeType pipe)
{
    printProcessesSem(pipe.semRead);
    print(" ");
    printProcessesSem(pipe.semWrite);
    print(" ");
    printProcessesSem(semPipeManager);
}
