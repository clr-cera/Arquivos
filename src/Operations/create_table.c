#include "operations.h"

// Operação de número 1

int create_table(string input_path, string output_name, FileManager fm) {
  fm_create_empty_table(fm, output_name);
  int returnal = fm_insert_csv(fm, output_name, input_path);

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

