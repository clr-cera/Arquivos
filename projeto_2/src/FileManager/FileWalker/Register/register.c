#include "register.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../../../Funcoes_fornecidas/funcoes_fornecidas.h"
#include "../../../Lib/lib.h"

#define CSV_LINE_SIZE_BUFFER 256
#define READ_FROM_KEYBOARD_BUFFER 100

// Todos os tam não devem contar o \0 da string
typedef struct register_{
  char removido;
  int tamanhoRegistro;
  long int prox;
  long int read_at; //Guarda onde um registro foi lido, utilizado apenas dentro do programa, não é escrito no arquivo

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

//Utilizado apenas para fins de testes
void debug_register(Register reg) {
  printf("tam: %d\n", reg->tamanhoRegistro);

  printf("id: %d\nidade: %d\n", reg->id, reg->idade);

  printf("nome jogador: ");
  debug_dinamic_field(reg->tamNomeJog, reg->nomeJogador);

  printf("nacionalidade: ");
  debug_dinamic_field(reg->tamNacionalidade, reg->nacionalidade);

  printf("nome clube: ");
  debug_dinamic_field(reg->tamNomeClube, reg->nomeClube);
}

//Imprime as informações de um registro
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

//Imprime um campo de tamanho dinâmico do registro
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

//Utilizado apenas para fins de testes
void debug_dinamic_field(int size, string str) {
  printf("%d ",size);
  for (int i =0; i < size; i ++)
    printf("%c", str[i]);
  printf("\n");
}

//Apaga um registro, liberando memória
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

//Retorna o tamanho de um dado registro
long int get_register_tamanho(Register reg) {
  return reg->tamanhoRegistro;
}

//Escreve o registro em um arquivo binário
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

//Lê um registro de um arquivo binário
Register read_register(FILE* fp) {
  Register reg = (Register) malloc(sizeof(register_obj));

  reg->read_at = ftell(fp);

  fread(&(reg->removido), sizeof(char), 1, fp);
  fread(&(reg->tamanhoRegistro), sizeof(int), 1, fp);
  fread(&(reg->prox), sizeof(long int), 1, fp);
  fread(&(reg->id), sizeof(int), 1, fp);
  fread(&(reg->idade), sizeof(int), 1, fp);
  
  read_dinamic_field(&reg->nomeJogador, &reg->tamNomeJog, fp);

  read_dinamic_field(&reg->nacionalidade, &reg->tamNacionalidade, fp);
  
  read_dinamic_field(&reg->nomeClube, &reg->tamNomeClube, fp);

  //Serve para pular os sifrões após o fim do registro, caso existam.
  if(ftell(fp) != (reg->read_at + reg->tamanhoRegistro))
    fseek(fp, reg->read_at + reg->tamanhoRegistro, SEEK_SET);

  return reg;
}

//Lê um campo de tamanho dinâmico de um arquivo binário
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

//Escreve um campo de tamanho dinâmico em um arquivo binário
void write_dinamic_field(string string_field, int string_size, FILE* fp) {
  // escreve tamanho
  fwrite(&string_size, sizeof(int), 1, fp);
  
  // escreve string  
  if (string_size > 0)
    fwrite(string_field, sizeof(char), string_size, fp); 

}

//Retorna se o registro consta como removido ou não
bool is_removed(Register reg) {
  return reg->removido == '1';
}

//Retorna o ID do registro
int get_id(Register reg) {
  return reg->id;
}

//Retorna a idade do jogador no registro
int get_idade(Register reg) {
  return reg->idade;
}

//Retorna o nome do jogador no registro
string get_nome_jogador(Register reg) {
  return reg->nomeJogador;
}

//Retorna a nacionalidade do jogador no registro
string get_nacionalidade(Register reg) {
  return reg->nacionalidade;
}

//Retorna o nome do clube do jogador no registro
string get_nome_clube(Register reg) {
  return reg->nomeClube;
}

//Lê uma linha de um dado arquivo csv e a converte para um registro
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

//Configura um registro como logicamente removido
bool set_removed(Register reg) {
  reg->removido = '1';
  return true;
}

//Altera o campo prox do registro
bool set_prox(Register reg, long int p){
  reg->prox = p;
  return true;
}

//Retorna o campo prox do registro
long int get_prox(Register reg){
  return reg->prox;
}

//Retorna a posição do registro no arquivo de dados
long int get_read_at(Register reg){
  return reg->read_at;
}

//Retorna um registro informado pelo stdin
//É utilizada na inserção de registros
Register read_reg_from_keyboard(){
  Register new = (Register) malloc(sizeof(register_obj));

  new->removido = '0';
  new->prox = -1;

  scanf("%d", &(new->id));
  new->idade = read_age();

  char Buffer[READ_FROM_KEYBOARD_BUFFER];

  scan_quote_string(Buffer);
  if(strcmp(Buffer, "NULO") == 0){
    new->tamNomeJog = 0;
    new->nomeJogador = NULL;
  } else {
    new->tamNomeJog = strlen(Buffer);
    new->nomeJogador = malloc(sizeof(char) * new->tamNomeJog);
    strncpy(new->nomeJogador, Buffer, new->tamNomeJog);
  }

  scan_quote_string(Buffer);
  if(strcmp(Buffer, "NULO") == 0){
    new->tamNacionalidade = 0;
    new->nacionalidade = NULL;
  } else {
    new->tamNacionalidade = strlen(Buffer);
    new->nacionalidade = malloc(sizeof(char) * new->tamNacionalidade);
    strncpy(new->nacionalidade, Buffer, new->tamNacionalidade);
  }

  scan_quote_string(Buffer);
  if(strcmp(Buffer, "NULO") == 0){
    new->tamNomeClube = 0;
    new->nomeClube = NULL;
  } else {
    new->tamNomeClube = strlen(Buffer);
    new->nomeClube = malloc(sizeof(char) * new->tamNomeClube);
    strncpy(new->nomeClube, Buffer, new->tamNomeClube);
  }

  new->tamanhoRegistro = new->tamNomeClube + new->tamNacionalidade + new->tamNomeJog + 6 * sizeof(int) + sizeof(char) + sizeof(long int);
  return new;
}

//Utilizada para atualizar um registro ou sobreescrever um registro antigo
//Mantém o tamanho do registro antigo e completa o espaço com o char '$'
void overwrite_register(FILE* fp, Register reg, Register old){

  if(reg->tamanhoRegistro > old->tamanhoRegistro){
    return;
  }

  //Apenas se necessário, posiciona o ponteiro na posição correta para sobrescrever o registro antigo
  if(ftell(fp) != old->read_at){
    fseek(fp, old->read_at, SEEK_SET);
  }
    
  
  //Se eles tem exatamente o mesmo tamanho, não há necessidade de inserir sifões
  if(reg->tamanhoRegistro == old->tamanhoRegistro){
    write_register(fp, reg);
    return;
  }

  //Escreve o novo registro com o tamanho do anterior e insere $ para sobrescrever o lixo que sobrar
  int lixo = old->tamanhoRegistro - reg->tamanhoRegistro;
  reg->tamanhoRegistro = old->tamanhoRegistro;
  write_register(fp, reg);
  for(int i = 0; i < lixo; i++)
    fputc('$', fp);
  return;
}
