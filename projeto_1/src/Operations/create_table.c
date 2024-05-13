#include "operations.h"

// Operação de número 1

// Essa operação cria um arquivo e insere os dados de um csv
int create_table(string input_path, string output_name, FileManager fm) {
  // O arquivo é criado
  fm_create_empty_table(fm, output_name);
  // O csv é inserido
  int returnal = fm_insert_csv(fm, output_name, input_path);

  // Se for -1, o csv não existe e por isso não pôde ser inserido, assim o arquivo é apagado
  if (returnal != -1) {
    string binario_path = concat_string(DATA_PATH, output_name); 
    binarioNaTela(binario_path);
    free(binario_path);
  }
  else{
    fm_delete_file(fm, output_name);
  }

  return returnal;
}

