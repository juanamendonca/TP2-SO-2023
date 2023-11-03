#include <queue.h>
#include <stdlib.h>
#include <stdint.h>

void initalizeScheduler()
{
    Queue * queue = createQueue();
    if(queue == NULL)
    {
        return;
    }
}


int initalizeProcess(void (*process)(int argc, char **argv), int argc, char **argv, int foreground, int *fd)
{
     if (process == NULL)
     {
        return -1;
     }
}
