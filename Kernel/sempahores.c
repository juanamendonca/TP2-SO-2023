#include "semaphores.h"
#include "video.h"
#include "string.h"

typedef struct
{
    semaphore sem;
    uint64_t is_available;
} space;

static space semSpaces[SEM_LIMIT];

static int creates_semaphore(char *name, uint64_t initValue);

static uint64_t find_space_availability();

static uint64_t lock_semaphore; // To block when opening or close

static uint64_t find_semaphore(char *name);

static uint64_t enqeue_process(uint64_t pid, semaphore *sem);

static uint64_t dequeue_process(semaphore *sem);

void print_semaphore(semaphore sem);

void print_procceses_blocked(process *process);

void start_semaphores()
{
    for (int i = 0; i < SEM_LIMIT; i++)
    {
        semSpaces[i].is_available = TRUE;
    }
}

static uint64_t find_space_availability()
{
    for (int i = 0; i < SEM_LIMIT; i++)
    {
        if (semSpaces[i].is_available == TRUE)
        {
            semSpaces[i].is_available = FALSE;
            return i;
        }
    }
    return -1; // No more space, another semaphore can not be created
}

static int creates_semaphore(char *name, uint64_t initValue)
{
    int pos;
    if ((pos = find_space_availability()) != -1)
    {
        //We initialize the structure
        memcpy(semSpaces[pos].sem.name, name, strlen(name));
        semSpaces[pos].sem.value = initValue;
        semSpaces[pos].sem.lock = 0; 
        semSpaces[pos].sem.firstProcess = NULL;
        semSpaces[pos].sem.lastProcess = semSpaces[pos].sem.firstProcess;
        semSpaces[pos].sem.size = 0;
        semSpaces[pos].sem.sizeList = 0;
    }
    return pos;
}

uint64_t sem_open(char *name, uint64_t initValue)
{
    while (_xchg(&lock_semaphore, 1) != 0); // Waiting for lock to be available
    int semIndex = find_semaphore(name);
    if (semIndex == -1) //  If the semaphore did not exist, we create it. 
    {
        semIndex = creates_semaphore(name, initValue);
        if (semIndex == -1)
        {
            _xchg(&lock_semaphore, 0);
            return -1; // Our space was filled. No space for more semaphores.
        }
    }
    semSpaces[semIndex].sem.size++;
    _xchg(&lock_semaphore, 0);
    return semIndex; // return the index of the sem
}

static uint64_t find_semaphore(char *name)
{
    for (int i = 0; i < SEM_LIMIT; i++)
    {
        if (semSpaces[i].is_available == FALSE && strcmp(name, semSpaces[i].sem.name) == 0)
        {
            return i;
        }
    }
    return -1;
}

uint64_t sem_close(char *name)
{
    while (_xchg(&lock_semaphore, 1) != 0);
    int semIndex = find_semaphore(name);
    if (semIndex == -1)
    {
        return -1; // Semaphore was not found
    }
    if ((--semSpaces[semIndex].sem.size) <= 0)
        semSpaces[semIndex].is_available = TRUE;
    _xchg(&lock_semaphore, 0);
    return 1;
}

uint64_t enqeue_process(uint64_t pid, semaphore *sem)
{
    process * process = malloc(sizeof(process));
    if (process == NULL)
    {
        return -1;
    }
    process->pid = pid;
    if (sem->sizeList == 0)
    {
        sem->firstProcess = process;
        sem->lastProcess = sem->firstProcess;
        process->next = NULL;
    }
    else
    {
        sem->lastProcess->next = process;
        process->next = NULL;
        sem->lastProcess = process;
    }
    sem->sizeList++;
    return 0;
}

uint64_t dequeue_process(semaphore *sem)
{
    if (sem == NULL || sem->firstProcess == NULL)
        return -1;
    process *current = sem->firstProcess;
    int pid = current->pid;
    sem->firstProcess = current->next;
    if (sem->firstProcess == NULL)
    {
        sem->lastProcess = NULL;
    }
    free(current);
    sem->sizeList--;
    return pid;
}

uint64_t sem_wait(uint64_t semIndex)
{
    if (semIndex >= SEM_LIMIT)
        return -1;
    semaphore *sem = &semSpaces[semIndex].sem;

    while (_xchg(&sem->lock, 1) != 0);

    if (sem->value > 0)
    {
        sem->value--;
        _xchg(&sem->lock, 0);
    }
    else
    {
        //If the value is 0, put the process to sleep.
        uint64_t pid = getPid();
        if (enqeue_process(pid, sem) == -1)
        {
            _xchg(&sem->lock, 0);
            return -1;
        }

        _xchg(&sem->lock, 0);
        if (block(pid) == -1)
        {
            return -1;
        }
        sem->value--;
    }
    return 0;
}

uint64_t sem_post(uint64_t semIndex)
{
    if (semIndex >= SEM_LIMIT)
    {
        return -1;
    }

    semaphore *sem = &semSpaces[semIndex].sem;
    while (_xchg(&sem->lock, 1) != 0)
        ;
    sem->value++;
    int pid = 0;
    if (sem->sizeList > 0)
    {
        if ((pid = dequeue_process(sem)) == -1)
        {
            _xchg(&sem->lock, 0);
            return -1;
        }
    }
    _xchg(&sem->lock, 0);
    unblock(pid) ?: forceTimer();
    return 0;
}

void sem()
{
    print("SEM'S NAME       STATE       BLOCKED PROCESSES\n");
    for (int i = 0; i < SEM_LIMIT; i++)
    {
        int toPrint = !(semSpaces[i].is_available);
        printInt(toPrint);
        if (toPrint)
        {
            printSem(semSpaces[i].sem);
            print("Entro aca el semaforo");
        }
    }
}

void printSem(semaphore sem)
{
    print(sem.name);
    if (strlen(sem.name) > 10) {
        putTab();
        putTab();
    } else {
        putTab();
        putTab();
        putTab();
        putTab();
    }

    printInt(sem.value);
    putTab();
    putTab();
    putTab();
    print_procceses_blocked(sem.firstProcess);
    putLine();
}

void print_procceses_blocked(process *process)
{
    while (process != NULL)
    {
        printInt(process->pid);
        print(" ");
        process = process->next;
    }
    print("-");
}

char *getSemName(uint64_t semIndex)
{
    if (semIndex >= SEM_LIMIT)
    {
        print("Wrong Index in getSemName\n");
        return NULL;
    }
    return semSpaces[semIndex].sem.name;
}

void printProcessesSem(uint64_t semIndex)
{
    if (semIndex >= SEM_LIMIT)
    {
        print("Wrong Index in printProcessesSem\n");
        return;
    }
    semaphore sem = semSpaces[semIndex].sem;
    print_procceses_blocked(sem.firstProcess);
}
