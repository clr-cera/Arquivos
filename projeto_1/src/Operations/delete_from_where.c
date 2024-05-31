#include "operations.h"

// Operação de número 5

// Essa operação remove todos os registros que obedecem um filtro, são realizadas buscas conforme a quantidade informada
int delete_from_where(string file_name, string index_file_name, int rem_quantity, FileManager fm) {

  int counter = -1;
  Filter filterv[rem_quantity];

  for (int i = 0; i < rem_quantity; i++){
    //printf("Busca %d\n\n", i);
    int m;
    // Cria-se o filtro
    scanf("%d", &m);
    filterv[i] = read_filter(m);
  }

  // Deleta todos os registros que passam no filtro
  counter = fm_delete_all_filter(fm, file_name, index_file_name, filterv, rem_quantity);
  // Se for -1, o arquivo não existe
  if (counter == -1) return counter;

  // Apaga todos os filtros da memória
  for (int i = 0; i < rem_quantity; i++){
    erase_filter(&(filterv[i]));
  }


  string binario_path = concat_string(DATA_PATH, file_name); 
  binarioNaTela(binario_path);
  free(binario_path);
  
  string index_path = concat_string(DATA_PATH, index_file_name); 
  binarioNaTela(index_path);
  free(index_path);

  return counter;
}

