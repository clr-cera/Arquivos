#include "register.h"
#include <stdlib.h>

// Todos os tam não devem contar o \0 da string
typedef struct register_{
  char removido;
  int tamanhoRegistro;
  long int prox;
  
  int id;
  int idade;
  
  int tamNomeJog;
  string nomeJogador;
  
  int tamNacionalidade;
  string nacionalidade;
  
  int tamNomeClube;
  string nomeClube;
}register_obj;
typedef register_obj* Register;

typedef struct register_collection_ {
  Register* vec;
  int length;
}register_collection;



void write_register(FILE* fp, Register reg) {
  fwrite(&(reg->removido), sizeof(char), 1, fp);
  fwrite(&(reg->tamanhoRegistro), sizeof(int), 1, fp);
  fwrite(&(reg->prox), sizeof(long int), 1, fp);
  fwrite(&(reg->id), sizeof(int), 1, fp);
  fwrite(&(reg->idade), sizeof(int), 1, fp);

  fwrite(&(reg->tamNomeJog), sizeof(int), 1, fp);
  fwrite(reg->nomeJogador, sizeof(char), reg->tamNomeJog, fp); 

  fwrite(&(reg->tamNacionalidade), sizeof(int), 1, fp);
  fwrite(reg->nacionalidade, sizeof(char), reg->tamNacionalidade, fp); 
  
  fwrite(&(reg->tamNomeClube), sizeof(int), 1, fp);
  fwrite(reg->nomeClube, sizeof(char), reg->tamNomeClube, fp); 
}
