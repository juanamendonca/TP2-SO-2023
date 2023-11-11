#define BLACK 0x000000
#define GREEN 0x00FF00
#define RED 0xFF0000
#define WHITE 0xFFFFFF
#define CORAL 0xFF7F50
#define PINK 0xFF69B4
#define PURPLE 0xB070D0
#define BLUE 0X007FFF

#include <stdint.h>

int strcmp(char *X, char *Y);
void strcpy(char * dest, const char * src);
void scanf(char * buffer, int bufferSize);
void deleteExtraSpaces(char * string);
void deleteInitialFinalSpace(char * string);
void intToString(int number, char *str);
char getKey();
void print(char * string);
char getChar();
int strlen(char * string);
void printBase(uint64_t value, uint32_t base);
void printHexa(uint64_t value);
uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base);
void fillHexa(int num, char* buf);
void enter();
int atoi2(char *str);
void printInt(int num);