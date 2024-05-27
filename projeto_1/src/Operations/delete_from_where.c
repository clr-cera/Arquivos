#include "operations.h"

// Operação de número 3

// Essa operação imprime todos os registros que obedecem um filtro, são realizadas buscas conforme a quantidade informada
int delete_from_where(char* file_name, string index_file_name, int search_quantity, FileManager fm) {

  //Cria o índice para busca
  fm_create_index_table(file_name, index_file_name, fm, false);

  int counter = -1;
  for (int i = 1; i <= search_quantity; i++){
    //printf("Busca %d\n\n", i);
    int m;
    Filter filter;

    // Cria-se o filtro
    scanf("%d", &m);
    filter = read_filter(m);

    // Imprime-se todos os registros que passam no filtro
    counter = fm_delete_all_filter(fm, file_name, index_file_name, filter);
    // Se for -1, o arquivo não existe
    if (counter == -1) break;

    erase_filter(&filter);
  }

  //Após a remoção, cria o índice novamente, para que ele esteja atualizado sem os valores removidos pelas operações anteriores
  fm_create_index_table(file_name, index_file_name, fm, true);

  string binario_path = concat_string(DATA_PATH, file_name); 
  binarioNaTela(binario_path);
  free(binario_path);
  
  string index_path = concat_string(DATA_PATH, index_file_name); 
  binarioNaTela(index_path);
  free(index_path);

  return counter;
}

