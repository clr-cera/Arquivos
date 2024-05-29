#include "operations.h"

// Operação de número 6

// Adiciona novos registros no registro de dados
int insert_into(string file_name, string index_file_name, int add_quantity, FileManager fm) {

  int returnal = 0;

  for (int i = 1; i <= add_quantity; i++){
    Register new = read_reg_from_keyboard();
    returnal = fm_insert_into(fm, file_name, new);
    if(returnal == -1){
      return returnal;
    }
  }

  //Após as inserções, cria o índice atualizado
  fm_create_index_table(file_name, index_file_name, fm, true);

  string binario_path = concat_string(DATA_PATH, file_name); 
  binarioNaTela(binario_path);
  free(binario_path);
  
  string index_path = concat_string(DATA_PATH, index_file_name); 
  binarioNaTela(index_path);
  free(index_path);

  return returnal;
}

