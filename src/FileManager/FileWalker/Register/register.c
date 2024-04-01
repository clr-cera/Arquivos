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
typedef register_collection* RegisterCollection;


void write_register(FILE* fp, Register reg) {
  fwrite(&(reg->removido), sizeof(char), 1, fp);
  fwrite(&(reg->tamanhoRegistro), sizeof(int), 1, fp);
  fwrite(&(reg->prox), sizeof(long int), 1, fp);
  fwrite(&(reg->id), sizeof(int), 1, fp);
  fwrite(&(reg->idade), sizeof(int), 1, fp);

  fwrite(&(reg->tamNomeJog), sizeof(int), 1, fp);
  if (reg->tamNomeJog > 0)
    fwrite(reg->nomeJogador, sizeof(char), reg->tamNomeJog, fp); 

  fwrite(&(reg->tamNacionalidade), sizeof(int), 1, fp);
  if (reg->tamNacionalidade > 0)
    fwrite(reg->nacionalidade, sizeof(char), reg->tamNacionalidade, fp); 
  
  fwrite(&(reg->tamNomeClube), sizeof(int), 1, fp);
  if (reg->tamNomeClube > 0)
    fwrite(reg->nomeClube, sizeof(char), reg->tamNomeClube, fp); 
}

Register read_register(FILE* fp) {
  Register reg = (Register) malloc(sizeof(register_obj));
  fread(&(reg->removido), sizeof(char), 1, fp);
  fread(&(reg->tamanhoRegistro), sizeof(int), 1, fp);
  fread(&(reg->prox), sizeof(long int), 1, fp);
  fread(&(reg->id), sizeof(int), 1, fp);
  fread(&(reg->idade), sizeof(int), 1, fp);
  
  fread(&(reg->tamNomeJog), sizeof(int), 1, fp);
  if (reg->tamNomeJog > 0){
    reg->nomeJogador = (string) malloc((reg->tamNomeJog+1)* sizeof(char));
    fread(reg->nomeJogador, sizeof(char), reg->tamNomeJog, fp); 
    reg->nomeJogador[reg->tamNomeJog] = '\0';
  }
  else reg->nomeJogador = NULL; 

  fread(&(reg->tamNacionalidade), sizeof(int), 1, fp);
  if (reg->tamNacionalidade > 0){
    reg->nacionalidade = (string) malloc((reg->tamNacionalidade+1)* sizeof(char));
    fread(reg->nacionalidade, sizeof(char), reg->tamNacionalidade, fp); 
    reg->nacionalidade[reg->tamNacionalidade] = '\0';
  }
  else reg->nacionalidade = NULL;
  
  fread(&(reg->tamNomeClube), sizeof(int), 1, fp);
  if (reg->tamNomeClube > 0){
    reg->nomeClube = (string) malloc((reg->tamNomeClube+1)* sizeof(char));
    fread(reg->nomeClube, sizeof(char), reg->tamNomeClube, fp); 
    reg->nomeClube[reg->tamNomeClube] = '\0';
  }
  else reg->nomeClube = NULL;


  return reg;
}

void write_register_collection(FILE* fp, RegisterCollection regcol) {
  for(int i = 0; i < regcol->length; i++) {
    write_register(fp, regcol->vec[i]);
  }
}
