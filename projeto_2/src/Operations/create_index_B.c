#include "operations.h"

// Operação de número 7

// Essa operação analisa um arquivo e a partir dele gera um arquivo de índice árvore B
int create_index_B(string data_file_name, string index_file_name, FileManager fm) {
  int returnal = fm_create_b_tree(data_file_name, index_file_name, fm);
  if (returnal == -1){
    return returnal;
  }

  string binario_path = concat_string(DATA_PATH, index_file_name); 
  binarioNaTela(binario_path);
  free(binario_path);

  return 1;
}


