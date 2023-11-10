#include <stdint.h>
int my_getpid();
int my_create_process(void (*process)(unsigned int argc, char **argv), int argc,
                      char **argv, int foreground, int *fd);
void my_nice(int pid, int newPrio);
int my_kill(int pid);
int my_block(int pid);
int my_unblock(int pid);
int my_sem_open(char *sem_id, uint64_t initialValue);
int my_sem_wait(char *sem_id);
int my_sem_post(char *sem_id);
int my_sem_close(char *sem_id);
void my_yield();
void my_wait(int pid);
