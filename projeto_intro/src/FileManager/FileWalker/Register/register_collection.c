#include "register.h"

//Register collection é um vetor de registros
typedef struct register_collection_ {
  Register* vec;
  int length;
}register_collection;
typedef register_collection* RegisterCollection;

//Utilizado apenas para fins de testes
void debug_register_collection(RegisterCollection regcol) {
  for(int i = 0; i < regcol->length; i++) {
    debug_register(regcol->vec[i]);
  }
}

//Apaga uma coleção de registros, liberando memória
void free_register_collection(RegisterCollection* regcolp) {
  RegisterCollection regcol = *regcolp;

  for (int i =0; i < regcol->length; i++) {
    if (regcol->vec[i] != NULL)
      free_register(&(regcol->vec[i]));
  }

  free(regcol->vec);
  free(regcol);
  *regcolp = NULL;
}

//Retorna quantos registros estão no register collection
int size_register_collection(RegisterCollection regcol) {
  return regcol->length;
}

//Escreve todos os registros na coleção dentro de um arquivo
long int write_register_collection(FILE* fp, RegisterCollection regcol) {
  long int offset = 0;
  for(int i = 0; i < regcol->length; i++) {
    write_register(fp, regcol->vec[i]);
      offset+=get_register_tamanho(regcol->vec[i]);
  }
  return offset;
}

//Lê todos os registros de um CSV e os guarda em um register collection
RegisterCollection csv_to_register_vector(string file_path){
  FILE* fp = fopen(file_path, "r");
  if (fp == NULL) return NULL;

  //O primeiro registro é inválido, então ele é descartado
  while (1) 
    if (fgetc(fp) == '\n')
      break;
  

  RegisterCollection new_collection = malloc(sizeof(RegisterCollection));
  new_collection->length = count_lines(fp);
  new_collection->vec = malloc(new_collection->length * sizeof(Register));

  for(int i = 0; i < new_collection->length; i++){
    (new_collection->vec)[i] = csv_line_to_register(fp);
  }

  fclose(fp);
  return new_collection;
}

