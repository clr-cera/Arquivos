#include "operations.h"

// Operação de número 6

// Adiciona novos registros no registro de dados
int insert_into(string file_name, string index_file_name, int add_quantity, FileManager fm) {

  for (int i = 1; i <= add_quantity; i++){
    //Aqui vai a parte difícil
  }

  //Após as inserções, cria o índice atualizado
  fm_create_index_table(file_name, index_file_name, fm, true);

  string binario_path = concat_string(DATA_PATH, file_name); 
  binarioNaTela(binario_path);
  free(binario_path);
  
  string index_path = concat_string(DATA_PATH, index_file_name); 
  binarioNaTela(index_path);
  free(index_path);

  return add_quantity;
}

