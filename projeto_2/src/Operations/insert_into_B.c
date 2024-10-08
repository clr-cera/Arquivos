#include "operations.h"

// Operação de número 10

// Adiciona novos registros no registro de dados e na árvore B
int insert_into_B(string file_name, string index_file_name, int add_quantity, FileManager fm) {

  int returnal = 0;

  Register regvec[add_quantity];

  // Coleta todos os registros a serem inseridos
  for (int i = 0; i < add_quantity; i++){
    regvec[i] = read_reg_from_keyboard();
  }

  returnal = fm_insert_into_B(fm, file_name, index_file_name, regvec, add_quantity);

  if(returnal == -1){
    return returnal;
  }


  string binario_path = concat_string(DATA_PATH, file_name); 
  binarioNaTela(binario_path);
  free(binario_path);
  
  string index_path = concat_string(DATA_PATH, index_file_name); 
  binarioNaTela(index_path);
  free(index_path);

  return returnal;
}

