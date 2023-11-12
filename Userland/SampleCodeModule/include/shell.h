typedef struct {
  void (*command)(int argSize, char *args[]);
  char *name;
  char *description;
  int builtIn;
} command;

int parseArgs(char *argString, char **args);

void help();

void runCommand(char *entry);