#include "operations.h"

// Operação de número 6

// Adiciona novos registros no registro de dados
int insert_into(string file_name, string index_file_name, int add_quantity, FileManager fm) {

  int returnal = 0;

  Register regvec[add_quantity];

  for (int i = 0; i < add_quantity; i++){
    regvec[i] = read_reg_from_keyboard();
  }

  returnal = fm_insert_into(fm, file_name, index_file_name, regvec, add_quantity);

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

