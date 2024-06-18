#include "operations.h"

// Operação de número 8

// Essa operação busca por um registro apenas pelo ID utilizando a árvore B
int select_from_where_id_B(string file_name, string index_file_name, int search_quantity, FileManager fm) {

  int counter = -1;
  int *IDv = malloc(sizeof(int) * search_quantity);

  // Todos os IDs para busca são coletados
  for (int i = 0; i < search_quantity; i++){
    char id_trash[5];
    scanf(" %s", id_trash);

    scanf("%d", &IDv[i]);
  }

  // Busca na árvore B todos os registros correspondentes aos IDs passados
  counter = fm_print_id_B(fm, file_name, index_file_name, IDv, search_quantity);
  if (counter == -1) return counter;

  // Libera memória alocada
  free(IDv);


  string binario_path = concat_string(DATA_PATH, file_name); 
  binarioNaTela(binario_path);
  free(binario_path);
  
  string index_path = concat_string(DATA_PATH, index_file_name); 
  binarioNaTela(index_path);
  free(index_path);

  return counter;
}

