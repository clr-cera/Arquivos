#include "lib.h"
#include <stdio.h>
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
