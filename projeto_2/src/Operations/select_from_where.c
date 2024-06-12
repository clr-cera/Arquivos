#include "operations.h"

// Operação de número 3

// Essa operação imprime todos os registros que obedecem um filtro, são realizadas buscas conforme a quantidade informada
int select_from_where(char* file_name, int search_quantity, FileManager fm) {
  int counter = -1;
  Filter filterv[search_quantity];

  for (int i = 0; i < search_quantity; i++){
    int m;

    // Cria-se o filtro
    scanf("%d", &m);
    filterv[i] = read_filter(m);
  }
  
  // Imprime-se todos os registros que passam nos filtros
  counter = fm_print_all_filter(fm, file_name, filterv, search_quantity);

  for (int i = 0; i < search_quantity; i++) {
    erase_filter(&(filterv[i]));
  }

  return counter;
}

