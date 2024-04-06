#include "operations.h"

// Operação de número 1

void create_table(string input_path, string output_name, FileManager fm) {
  fm_create_empty_table(fm, output_name);
  fm_insert_csv(fm, output_name, input_path);

  string binario_path = concat_string(DATA_PATH, output_name); 
  binarioNaTela(binario_path);
  free(binario_path);
}

