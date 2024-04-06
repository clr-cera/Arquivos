#include "lib.h"
#include <stdlib.h>

// lib_strings é responsável por conter funções relacionadas a strings
string read_string(void) {
  string str = (char *) malloc(50*sizeof(char));
  scanf(" %s", str);

  return str;
}


int string_len(string str) {
  int sum = 0;
  for(int i = 0;;i++){
    if (str[i] == '\0') return sum;
    sum++;
  }  
}
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

int string_to_int(char* str, int* start){
  int number = 0;
  for(; str[*start] > 47 && str[*start] < 58; (*start)++){
    number += str[*start] - 48;
    number *= 10;
  }
  return number / 10;
}

int dist_to_target(char* str, int start, char target){
  int dist = 0;
  for(int i = start; str[i] != target; i++){
    if(str[i] == '\n' || str[i] == EOF)
      break;
    dist++;
  }
  return dist;
}

char* string_slicer(char* str, int start, int size){
  char* new_string = malloc(size * sizeof(char));
  for(int i = 0; i < size; i++){
    new_string[i] = str[i + start];
  }
  return new_string;
}

int count_lines(FILE* fp){
  int counter;
  FILE* previous_position = fp;
  while(!feof(fp)){
    if(fgetc(fp) == '\n')
      counter++;
  }
  fp = previous_position;
  return counter;
}
