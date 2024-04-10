#include "operations.h"

// Operação de número 3

int select_from_where(char* file_name, int search_quantity, FileManager fm) {
  int counter = -1;
  for (int i = 1; i <= search_quantity; i++){
    printf("Busca %d\n\n", i);
    int m;
    Filter filter;

    scanf("%d", &m);
    filter = read_filter(m);
    //print_filter(filter); //DEBUG
    
    counter = fm_print_all_filter(fm, file_name, filter);
    if (counter == -1) break;
    if (counter == 0) {
      printf("Registro inexistente.\n\n");
    }

    erase_filter(&filter);
  }

  return counter;
}

