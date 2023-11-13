#include "strings.h"
#include "video.h"
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

int strcmp(char *s1, char *s2) {
  while (*s1 != '\0' && *s2 != '\0' && *s1 == *s2) {
    s1++;
    s2++;
  }

  return (*s1 - *s2);
}

char *strncpy(char *dest, const char *src, size_t n) {
  size_t i;
  for (i = 0; i < n && src[i] != '\0'; i++) {
    dest[i] = src[i];
  }
  for (; i < n; i++) {
    dest[i] = '\0';
  }
  return dest;
}