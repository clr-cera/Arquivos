#include <stdio.h>
#include "Operations/operations.h"
#include "Lib/lib.h"
#include "FileManager/file_manager.h"

// Main será responsável apenas por receber argumentos iniciais e chamar
// a operação requisitada

int main(void) {
  char c;
  FileManager file_manager = create_file_manager();

  // Parser da operação a ser realizada
  while (scanf(" %c", &c) != EOF){
    int returnal = 0;

    //Operação 1
    if (c == '1') {
      char* input_path = read_string();
      char* output_name = read_string();

      returnal = create_table(input_path, output_name, file_manager);
      free(input_path);
    }

    //Operação 2
    if (c == '2') {
      char* file_name = read_string();

      returnal = select_from(file_name, file_manager);
      free(file_name);
    }

    //Operação 3
    if (c == '3') {
      char* file_name = read_string();
      int search_quantity; scanf(" %d", &search_quantity);

      returnal = select_from_where(file_name, search_quantity, file_manager);
      free(file_name);
    }

    if (returnal == -1)
      printf("Falha no processamento do arquivo.\n");
  }

  erase_file_manager(&file_manager);

  return 0;
}
