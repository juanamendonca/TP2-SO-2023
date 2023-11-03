#ifndef SCHEDULER_H
#define SCHEDULER_H

typedef enum
{
  READY,
  BLOCKED,
  TERMINATED,
} state;

typedef struct p_info
{
  int pid;
  int ppid;
  int foreground;
  state state;
  char name[30];
  int fileDescriptors[2];
  void *rsp;
  void *rbp;
  int argc;
  char **argv;
} p_info;

void initalizeScheduler();

#endif