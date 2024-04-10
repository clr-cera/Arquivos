#include "register.h"

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


void read_dinamic_field(string* string_field, int* string_size, FILE* fp);
void write_dinamic_field(string string_field, int string_size, FILE* fp);
void debug_dinamic_field(int size, string str);
void print_dinamic_field(int size, string str);

void debug_register(Register reg) {
  printf("id: %d\nidade: %d\n", reg->id, reg->idade);

  printf("nome jogador: ");
  debug_dinamic_field(reg->tamNomeJog, reg->nomeJogador);

  printf("nacionalidade: ");
  debug_dinamic_field(reg->tamNacionalidade, reg->nacionalidade);

  printf("nome clube: ");
  debug_dinamic_field(reg->tamNomeClube, reg->nomeClube);
}

void print_register(Register reg) {
  //printf("id: %d\nidade: %d\n", reg->id, reg->idade); // DEBUG
  
  printf("Nome do Jogador: ");
  print_dinamic_field(reg->tamNomeJog, reg->nomeJogador);

  printf("Nacionalidade do Jogador: ");
  print_dinamic_field(reg->tamNacionalidade, reg->nacionalidade);

  printf("Clube do Jogador: ");
  print_dinamic_field(reg->tamNomeClube, reg->nomeClube);
  
  printf("\n");
}

void print_dinamic_field(int size, string str) {
  if (size <= 0 || str == NULL) {
    printf("SEM DADO");
  }
  else {
    for (int i = 0; i < size; i ++)
      printf("%c", str[i]);
  }
  printf("\n");
}

void debug_dinamic_field(int size, string str) {
  printf("%d ",size);
  for (int i =0; i < size; i ++)
    printf("%c", str[i]);
  printf("\n");
}

void free_register(Register* regp) {
  Register reg= *regp;
  if (reg==NULL) return;

  if(reg->nomeJogador != NULL)
    free(reg->nomeJogador);

  if(reg->nacionalidade != NULL)
    free(reg->nacionalidade);
  
  if(reg->nomeClube != NULL)
    free(reg->nomeClube);
  
  free(reg);
  *regp = NULL;
}

long int get_register_tamanho(Register reg) {
  return reg->tamanhoRegistro;
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
    (*string_field)[*string_size] = '\0';
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

bool is_removed(Register reg) {
  return reg->removido == '1';
}

int get_id(Register reg) {
  return reg->id;
}

int get_idade(Register reg) {
  return reg->idade;
}

string get_nome_jogador(Register reg) {
  return reg->nomeJogador;
}

string get_nacionalidade(Register reg) {
  return reg->nacionalidade;
}

string get_nome_clube(Register reg) {
  return reg->nomeClube;
}


Register csv_line_to_register(FILE* fp){

  Register line = malloc(sizeof(register_obj));
  //Os campos "removido" e "prox" não serão utilizados nessa função, então são definidos para um valor padrão
  line->removido = '0';
  line->prox = -1;

  // Caso falte registro
  line->id = -1;
  line->idade = -1;

  char string[CSV_LINE_SIZE_BUFFER];
  fgets(string, CSV_LINE_SIZE_BUFFER, fp);

  //printf("%s\n", string);

  int field = 0;
  for(int i = 0; string[i] != '\n' && string[i] != '\0'; i++){
    //printf("char: %c\n", string[i]);
    if(string[i] == ','){
      field++;
      continue;
    }
    switch(field){
      case 0:
        line->id = string_to_int(string, &i);
        //printf("id: %d\n", line->id);
        i--;
        break;
      case 1:
        line->idade = string_to_int(string, &i);
        //printf("idade: %d\n", line->idade);
        i--;
        break;
      case 2:
        line->tamNomeJog = dist_to_target(string, i, ',');
        line->nomeJogador = string_slicer(string, i, line->tamNomeJog);
        i += line->tamNomeJog-1;
        break;
      case 3:
        line->tamNacionalidade = dist_to_target(string, i, ',');
        line->nacionalidade = string_slicer(string, i, line->tamNacionalidade);
        i += line->tamNacionalidade-1;
        break;
      case 4:
        line->tamNomeClube = dist_to_target(string, i, ',');
        line->nomeClube = string_slicer(string, i, line->tamNomeClube);
        i += line->tamNomeClube;
        break;
    }
  }
  line->tamanhoRegistro = line->tamNomeClube + line->tamNacionalidade + line->tamNomeJog + 6 * sizeof(int) + sizeof(char) + sizeof(long int);
  return line;
}

