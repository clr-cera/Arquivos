#include "operations.h"

// Operação de número 8

// Essa operação busca por um registro apenas pelo ID utilizando a árvore B
int select_from_where_id_B(string file_name, string index_file_name, int rem_quantity, FileManager fm) {

  int counter = -1;
  long int *IDv = malloc(sizeof(long int) * rem_quantity);

  // Todos os IDs para busca são coletados
  for (int i = 0; i < rem_quantity; i++){
    int m;
    // Número do ID é lido
    scanf("%d", &m);
    //ID é lido
    IDv[m] = read_ID;
  }

  // Busca na árvore B todos os registros correspondentes aos IDs passados
  //To do: Implement fm_search_B_tree_by_ID
  //counter = fm_search_B_tree_by_ID(fm, file_name, index_file_name, IDv, rem_quantity);
  // Se for -1, o arquivo não existe
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

