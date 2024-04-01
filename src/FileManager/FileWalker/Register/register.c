#include "register.h"
#include <stdio.h>
#include <stdlib.h>


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
  
  int tamNomeClub;
  string nomeClube;
}register_obj;
typedef register_obj* Register;

typedef struct register_collection_ {
  Register* vec;
  int length;
}register_collection;

Register create_empty_register(void) {
  
} 

