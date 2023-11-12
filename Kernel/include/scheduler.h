#include <stdbool.h>
#include <stdint.h>

#ifndef SCHEDULER_H
#define SCHEDULER_H

typedef enum {
  READY,
  BLOCKED,
  KILLED,
} state;

typedef struct pcb {
  int pid;
  int ppid;
  int foreground;
  state state;
  int priority;
  int quantum;
  char name[30];
  int fd[2];
  void *rsp;
  void *rbp;
  int argc;
  char **argv;
  int waitingPid; //semid?
  
} pcb;

typedef struct stackFrame {
  uint64_t r15;
  uint64_t r14;
  uint64_t r13;
  uint64_t r12;
  uint64_t r11;
  uint64_t r10;
  uint64_t r9;
  uint64_t r8;
  uint64_t rsi;
  uint64_t rdi;
  uint64_t rbp;
  uint64_t rdx;
  uint64_t rcx;
  uint64_t rbx;
  uint64_t rax;
  uint64_t rip;
  uint64_t cs;
  uint64_t flags;
  uint64_t rsp;
  uint64_t ss;
  uint64_t base;
} stackFrame;

void initalizeScheduler();
void *scheduler(void *rsp);
int initalizeProcess(void (*process)(int argc, char **argv), int argc,
                     char **argv, int foreground, int *fd);
void freePcb(pcb *process);
int killProcess(int pid);
void killCurrent();
int block(int pid);
int unblock(int pid);
int getPid();
int processesInfo(char *buffer);
int nice(int pid, int priority);
void giveUpCPU();
// void setUserland(int pid);
extern void forceTimer();
void waitpid(int pid);

#endif