#include "operations.h"

// Operação de número 9

// Essa operação busca todos os registros que obedecem um filtro, são realizadas buscas conforme a quantidade informada
int select_from_where_B(string file_name, string index_file_name, int search_quantity, FileManager fm) {

  int counter = -1;
  Filter filterv[search_quantity];

  // Todos os filtros para busca são coletados
  for (int i = 0; i < search_quantity; i++){
    int m;
    // Cria-se o filtro
    scanf("%d", &m);
    filterv[i] = read_filter(m);
  }

  // Imprime todos os registros que passam no filtro
  counter = fm_print_all_filter_B(fm, file_name, index_file_name, filterv, search_quantity);
  if (counter == -1) return counter;

  // Apaga todos os filtros da memória
  for (int i = 0; i < search_quantity; i++){
    erase_filter(&(filterv[i]));
  }

  return counter;
}

