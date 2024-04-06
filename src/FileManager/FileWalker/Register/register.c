#include "register.h"
#include <stdlib.h>

#define CSV_LINE_SIZE_BUFFER 256

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

void free_register(Register* regp) {
  free((*regp)->nomeJogador);
  free((*regp)->nacionalidade);
  free((*regp)->nomeClube);
  free(*regp);
  *regp = NULL;
}

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

RegisterCollection csv_to_register_vector(string file_path){
  FILE* fp = fopen(file_path, "r");

  //O primeiro registro é inválido, então ele é descartado
  Register first_register = csv_line_to_register(fp);
  free_register(&first_register);

  RegisterCollection new_collection = malloc(sizeof(RegisterCollection));
  new_collection->length = count_lines(fp);
  new_collection->vec = malloc(new_collection->length * sizeof(Register));

  for(int i = 0; i < new_collection->length && !feof(fp); i++){
    (new_collection->vec)[i] = csv_line_to_register(fp);
  }

  return new_collection;
}

Register csv_line_to_register(FILE* fp){
  Register line = malloc(sizeof(register_obj));
  //Os campos "removido" e "prox" não serão utilizados nessa função, então são definidos para um valor padrão
  line->removido = '0';
  line->prox = -1;

  char string[CSV_LINE_SIZE_BUFFER];
  fgets(string, CSV_LINE_SIZE_BUFFER, fp);

  int field = 0;
  for(int i = 0; string[i] != '\n' || string[i] != EOF || string[i] != '\0'; i++){
    if(string[i] == ','){
      field++;
      continue;
    }
    switch(field){
      case 0:
        line->id = string_to_int(string, &i);
        break;
      case 1:
        line->idade = string_to_int(string, &i);
        break;
      case 2:
        line->tamNomeJog = dist_to_target(string, i, ',');
        line->nomeJogador = string_slicer(string, i, line->tamNomeJog);
        i += line->tamNomeJog;
        break;
      case 3:
        line->tamNacionalidade = dist_to_target(string, i, ',');
        line->nacionalidade = string_slicer(string, i, line->tamNacionalidade);
        i += line->tamNacionalidade;
        break;
      case 4:
        line->tamNomeClube = dist_to_target(string, i, ',');
        line->nomeClube = string_slicer(string, i, line->tamNomeClube);
        i += line->tamNomeClube;
        break;
    }
  }
  line->tamanhoRegistro = line->tamNomeClube + line->tamNacionalidade + line->tamNomeJog + 5 * sizeof(int) + sizeof(char) + sizeof(long int);
  return line;
}

