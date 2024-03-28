#include "lib.h"
#include "stdio.h"

// lib_strings é responsável por conter funções relacionadas a strings
char* read_string(void) {
  char* string = (char *) malloc(50*sizeof(char));
  scanf(" %s", string);

  return string;
}

