#include "operations.h"

// Operação de número 3

void select_from_where(char* file_name, int search_quantity, FileManager fm) {
  for (int i = 1; i <= search_quantity; i++){
    printf("Busca %d\n\n", i);
    int m;
    Filter filter;

    scanf("%d", &m);
    filter = read_filter(m);
    //print_filter(filter); //DEBUG
    
    int counter = fm_print_all_filter(fm, file_name, filter);
    if (counter == 0) {
      printf("Registro inexistente.\n\n");
    }

    erase_filter(&filter);
  }

}

