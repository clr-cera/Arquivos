#include <stdio.h>
#include "Operations/operations.h"
#include "Lib/lib.h"
#include "FileManager/file_manager.h"

// Main será responsável apenas por receber argumentos iniciais e chamar
// a operação requisitada

int main(void) {
  int op = 0;
  FileManager file_manager = create_file_manager();

  // Parser da operação a ser realizada
  while (true){
    int returnal = 0;
    op = read_op();

    //Operação 1
    if (op == 1) {
      char* input_path = read_string();
      char* output_name = read_string();

      returnal = create_table(input_path, output_name, file_manager);
      free(input_path);
      free(output_name);
    }

    //Operação 2
    if (op == 2) {
      char* file_name = read_string();

      returnal = select_from(file_name, file_manager);
      free(file_name);
    }

    //Operação 3
    if (op == 3) {
      char* file_name = read_string();
      int search_quantity; scanf(" %d", &search_quantity);

      returnal = select_from_where(file_name, search_quantity, file_manager);
      free(file_name);
    }

    //Operação 4
    if (op == 4) {
      char* data_file_name = read_string();
      char* index_file_name = read_string();

      returnal = create_index_file(data_file_name, index_file_name, file_manager);

      free(data_file_name);
      free(index_file_name);
    }

    //Operação 5
    if (op == 5) {
      char* file_name = read_string();
      char* index_name = read_string();
      int rem_quantity; scanf(" %d", &rem_quantity);

      returnal = delete_from_where(file_name, index_name, rem_quantity, file_manager);
      free(file_name);
      free(index_name);
    }

    //Operação 6
    if (op == 6) {
      char* file_name = read_string();
      char* index_name = read_string();
      int add_quantity; scanf(" %d", &add_quantity);

      returnal = insert_into(file_name, index_name, add_quantity, file_manager);
      free(file_name);
      free(index_name);
    }

    //Operação 7
    if (op == 7) {
      char* data_file_name = read_string();
      char* index_file_name = read_string();

      returnal = create_index_B(data_file_name, index_file_name, file_manager);

      free(data_file_name);
      free(index_file_name);
    }

    //Operação 8
    if (op == 8) {
      char* file_name = read_string();
      char* index_name = read_string();
      int search_quantity; scanf(" %d", &search_quantity);

      returnal = select_from_where_id_B(file_name, index_name, search_quantity, file_manager);
      free(file_name);
      free(index_name);
    }

    //Operação 9
    if (op == 9) {
      char* file_name = read_string();
      char* index_name = read_string();
      int search_quantity; scanf(" %d", &search_quantity);

      returnal = select_from_where_B(file_name, index_name, search_quantity, file_manager);
      free(file_name);
      free(index_name);
    }

    //Operação 10
    if (op == 10) {
      char* file_name = read_string();
      char* index_name = read_string();
      int add_quantity; scanf(" %d", &add_quantity);

      returnal = insert_into_B(file_name, index_name, add_quantity, file_manager);
      free(file_name);
      free(index_name);
    }

    if (returnal == -1)
      printf("Falha no processamento do arquivo.\n");
    
    if (op == -1)
      break;
  }

  erase_file_manager(&file_manager);
  
  return 0;
}
