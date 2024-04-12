#include "operations.h"

// Operação de número 2

int select_from(char* file_name, FileManager fm) {
  int counter = fm_print_all(fm, file_name);
  // Se for 0, nenhum registro foi encontrado
  if (counter == 0) {
    printf("Registro inexistente.\n\n");
  }

  return counter;
}
