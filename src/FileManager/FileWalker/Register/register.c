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

void read_dinamic_field(string* string_field, int* string_size, FILE* fp);
void write_dinamic_field(string string_field, int string_size, FILE* fp);


void write_register(FILE* fp, Register reg) {
  fwrite(&(reg->removido), sizeof(char), 1, fp);
  fwrite(&(reg->tamanhoRegistro), sizeof(int), 1, fp);
  fwrite(&(reg->prox), sizeof(long int), 1, fp);
  fwrite(&(reg->id), sizeof(int), 1, fp);
  fwrite(&(reg->idade), sizeof(int), 1, fp);

  write_dinamic_field(reg->nomeJogador, reg->tamNomeJog, fp);

  write_dinamic_field(reg->nacionalidade, reg->tamNacionalidade, fp);
  
  write_dinamic_field(reg->nomeClube, reg->tamNomeClube, fp);
}

Register read_register(FILE* fp) {
  Register reg = (Register) malloc(sizeof(register_obj));
  fread(&(reg->removido), sizeof(char), 1, fp);
  fread(&(reg->tamanhoRegistro), sizeof(int), 1, fp);
  fread(&(reg->prox), sizeof(long int), 1, fp);
  fread(&(reg->id), sizeof(int), 1, fp);
  fread(&(reg->idade), sizeof(int), 1, fp);
  
  read_dinamic_field(&reg->nomeJogador, &reg->tamNomeJog, fp);

  read_dinamic_field(&reg->nacionalidade, &reg->tamNacionalidade, fp);
  
  read_dinamic_field(&reg->nomeClube, &reg->tamNomeClube, fp);

  return reg;
}

void read_dinamic_field(string* string_field, int* string_size, FILE* fp) {
  // ler tamanho
  fread(string_size, sizeof(int), 1, fp);

  // ler string se ela existir
  if (*string_size > 0){
    *string_field = (string) malloc((*string_size+1)* sizeof(char));
    fread(*string_field, sizeof(char), *string_size, fp); 
    *string_field[*string_size] = '\0';
  }
  else *string_field = NULL;
}

void write_dinamic_field(string string_field, int string_size, FILE* fp) {
  // escreve tamanho
  fwrite(&string_size, sizeof(int), 1, fp);
  
  // escreve string  
  if (string_size > 0)
    fwrite(string_field, sizeof(char), string_size, fp); 

}

void write_register_collection(FILE* fp, RegisterCollection regcol) {
  for(int i = 0; i < regcol->length; i++) {
    write_register(fp, regcol->vec[i]);
  }
}
