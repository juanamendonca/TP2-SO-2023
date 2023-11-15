#include <stddef.h>
#ifndef MYSTRING_H
#define MYSTRING_H

// Function to copy an array from a source to a dest
void strcpy(char *dest, const char *src);

// Calculates array lenght
int strlen(const char *str);

int strcmp(char *s1, char *s2);

char *strncpy(char *dest, const char *src, size_t n);

#endif
