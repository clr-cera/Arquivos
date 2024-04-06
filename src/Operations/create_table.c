#include "operations.h"

// Operação de número 1

void create_table(string input_path, string output_name, FileManager fm) {
  fm_create_empty_table(fm, output_name);
  fm_insert_csv(fm, output_name, input_path);
}

