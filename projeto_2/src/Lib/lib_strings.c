#include "lib.h"
#include <stdlib.h>
// lib_strings é responsável por conter funções relacionadas a strings

//Lê uma string do teclado
string read_string(void) {
  string str = (char *) malloc(50*sizeof(char));
  scanf(" %s", str);

  return str;
}

//Dada uma string terminada em \0, retorna seu tamanho
int string_len(string str) {
  int sum = 0;
  for(int i = 0;;i++){
    if (str[i] == '\0') return sum;
    sum++;
  }  
}

//Concatena 2 strings
string concat_string(string s1, string s2) {
  int len1 = string_len(s1);
  int len2 = string_len(s2);
  int final_len = len1 + len2 + 1;
  string str = (string) malloc(final_len * sizeof(char));
  
  int str_index = 0;
  for (int i = 0; i < len1; i++) {
    str[str_index] = s1[i]; 
    str_index++;
  }

  for(int i = 0; i <len2; i++) {
    str[str_index] = s2[i];
    str_index++;
  }

  str[str_index] = '\0';

  return str;
}


//Dado um número escrito em base 10 em uma string, retorna o número como um inteiro. 
//Ao mesmo tempo, avança o inteiro start para que quem chamou a função possa continuar a leitura imediatamente depois do número acabar.
int string_to_int(char* str, int* start){
  int number = 0;
  for(; str[*start] > 47 && str[*start] < 58; (*start)++){
    number += str[*start] - 48;
    number *= 10;
  }

  return number / 10;
}

//Calcula a distancia em uma string da posição start até a posição contendo o caracter target
int dist_to_target(char* str, int start, char target){
  int dist = 0;
  for(int i = start; str[i] != target; i++){
    if(str[i] == '\n' || str[i] == EOF)
      break;
    dist++;
  }
  return dist;
}


//Cria uma nova string com os próximos "size" caracteres da string original
char* string_slicer(char* str, int start, int size){
  char* new_string = malloc(size * sizeof(char));
  for(int i = 0; i < size; i++){
    new_string[i] = str[i + start];
  }
  return new_string;
}

//Conta quantas linhas um arquivo possui e retorna o ponteiro do arquivo para a posição inicial
int count_lines(FILE* fp){
  int counter = 0;
  int initial_position = ftell(fp);
  while(!feof(fp)){
    if(fgetc(fp) == '\n')
      counter++;
  }
  fseek(fp, initial_position, SEEK_SET);
  return counter;
}

int read_age(){
  char c;
  while((c = getchar()) != EOF && isspace(c));

  if(c == 'N' || c == 'n') { // idade NULO
		getchar(); getchar(); getchar(); // ignorar o "ULO" de NULO.
		return -1; // -1 para idade nula
  }

  int n = 0;

  while(c >= 48 && c <= 57){
    n += c - 48;
    n *= 10;
    c = getchar();
  }

  return n / 10;

}

long int read_ID(){
  char c;

  //Pula qualquer coisa antes de chegar no valor do ID em si
  while((c = getchar()) != EOF && isspace(c));
  while(c<47 || c>57)
    c = getchar();

  long int n = 0;

  while(c >= 48 && c <= 57){
    n += c - 48;
    n *= 10;
    c = getchar();
  }

  return n / 10;

}