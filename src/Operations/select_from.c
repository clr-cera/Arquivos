#include "operations.h"

// Operação de número 2

void select_from(char* file_name, FileManager fm) {
  int counter = fm_print_all(fm, file_name);
    if (counter == 0) {
      printf("Registro inexistente.\n\n");
    }
}
