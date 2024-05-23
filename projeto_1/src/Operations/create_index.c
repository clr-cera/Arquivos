#include "operations.h"

// Operação de número 4

// Essa operação analisa um arquivo e a partir dele gera um arquivo de índice
int create_index_file(string data_file_name, string index_file_name, FileManager fm) {
  fm_create_empty_index(fm, index_file_name);
  Index* vector = fm_get_index_vector(fm, data_file_name); 
  if (vector == NULL) {
    return -1;
  }

  int size = fm_get_reg_number(fm, data_file_name);
  vector = sort_index_vector(vector, size);
  fm_insert_all_index(fm, index_file_name, vector, size);

  string binario_path = concat_string(DATA_PATH, index_file_name); 
  binarioNaTela(binario_path);
  free(binario_path);

  return 1;
}


