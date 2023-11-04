#include "strings.h"
#include <stddef.h> // Para el tipo size_t

// Implementación de la función my_strcpy
void strcpy(char *dest, const char *src) {
  while ((*dest = *src) != '\0') {
    dest++;
    src++;
  }
}

// Implementación de la función my_strlen
int strlen(const char *str) {
  int length = 0;
  while (*str != '\0') {
    length++;
    str++;
  }
  return length;
}