#include "operations.h"

// Operação de número 3

void select_from_where(char* file_name, int search_quantity, FileManager fm) {
  for (int i = 1; i <= search_quantity; i++){
    int m;
    Filter filter;

    scanf("%d", &m);
    filter = read_filter();
    //print_filter(filter); //DEBUG

    erase_filter(&filter);
  }

}

