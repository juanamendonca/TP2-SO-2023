#include <stddef.h>
#ifndef MYSTRING_H
#define MYSTRING_H

// Función para copiar una cadena de origen en una cadena de destino
void strcpy(char *dest, const char *src);

// Función para calcular la longitud de una cadena
int strlen(const char *str);

int strcmp(char *s1, char *s2);

char *strncpy(char *dest, const char *src, size_t n);

#endif