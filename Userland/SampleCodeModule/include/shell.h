#define BUFFER_SIZE 100
#define MAX_ARGS 5

typedef struct {
  void (*process)(unsigned int argc, char **argv);
  char *name;
  char *description;
} command;

void entry(char *buffer, char **args);

int parseArgs(char *argString, char **args);

int findPipe(char **args, int argc);

void help();

void runCommand(char **args, int argc, int fd[], int com);

void help();