#include "user_syscalls.h"
#include <stdint.h>

int my_getpid() { return sys_get_pid(); }

int my_create_process(void (*process)(unsigned int argc, char **argv), int argc,
                      char **argv, int foreground, int *fd) {
  return sys_create_process(process, argc, argv, foreground, fd);
}

void my_nice(uint64_t pid, uint64_t newPrio) {
  sys_nice((int)pid, (int)newPrio);
}

int my_kill(uint64_t pid) { return sys_kill_process((int)pid); }

int my_block(uint64_t pid) { return sys_block_process((int)pid); }

int my_unblock(uint64_t pid) { return sys_unblock_process((int)pid); }


int my_yield() {
  sys_giveup_cpu();
  return 0;
}

void my_wait(int64_t pid) { sys_waitpid((int)pid); }
