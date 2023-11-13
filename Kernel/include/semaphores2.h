typedef struct PNode {
  struct PNode *next;
  int pid;
} PNode;

typedef struct semaphore {
  int id;
  char name[30];
  PNode *firstP;
  PNode *lastP;
  int lock;
  int value;
  int sizeP;
  int cantP;
  struct semaphore *next;
} semaphore;

extern uint64_t _xchg(int *lock, int value);
void start_semaphores();
int sem_open(char *name, int value);
int sem_close(int id);
int sem_wait(int id);
int sem_post(int id);
void sem();
