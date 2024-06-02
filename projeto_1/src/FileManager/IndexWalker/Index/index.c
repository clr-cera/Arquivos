#include "index.h"
#include "stdlib.h"

// Esse arquivo contém funções básicas para manipulação de índices (a estrutura básica da indexação) e outros tipos relacionados
//
// O índice relaciona o id de um registro com o seu byteoffset no arquivo
typedef struct index_ {
  int id;
  long int byteoffset;
} indexObj;

typedef indexObj* Index;
void quicksort_rec(Index* startp, Index* endp);

// Esta função cria o índice que contém o id informado e seu respectivo byteoffset
Index create_index(int id, long int byteoffset) {
  Index index = (Index) malloc(sizeof(indexObj));
  
  index->id = id;
  index->byteoffset = byteoffset;

  return index;
}

// Esta função apaga um índice da memória
void erase_index(Index* indexp) {
  free(*indexp);
  *indexp = NULL;
}

// Esta função escreve o índice em formato binário no arquivo informado
void write_index(FILE* fp, Index index) {
  fwrite(&(index->id), sizeof(int), 1, fp);
  fwrite(&(index->byteoffset), sizeof(long int), 1, fp);
}

// Esta função lê o índice em formato binário do arquivo informado
Index read_index(FILE* fp) {
  int id;
  long int byteoffset;

  fread(&id, sizeof(int), 1, fp);
  fread(&byteoffset, sizeof(long int), 1, fp);

  return create_index(id, byteoffset);
}

// Esta função imprime o índice no stdout, serve apenas para debug
void print_index(Index index) {
  if (index == NULL)
    return;

  printf("%d, %ld\n", index->id, index->byteoffset);
}

// Esta função retorna o id do índice
int get_index_id(Index index) {
  return index->id;
}

// Esta função retorna o byteoffset do índice
long int get_index_offset(Index index) {
  return index->byteoffset;
}

// Essa função ordena um vetor de índices, implementamos utilizando o quicksort
Index* sort_index_vector(Index* vector, int size) {
  quicksort_rec(vector, vector + size-1);

  return vector;
}

// Essa função troca dois índices de posição levando em consideração seu endereço na memória
void swap(Index* a, Index* b) {
  Index temp = *a;
  *a = *b;
  *b = temp;
}

// Essa função implementa a lógica para o quicksort de índices
void quicksort_rec(Index* startp, Index* endp) {
  if(startp >= endp) return;
  Index* i = startp;
  Index* j = endp;
  Index* pivot = startp + (endp-startp)/2; 

  while(1) {
    if (i == j) {
      quicksort_rec(startp, pivot-1);
      quicksort_rec(pivot+1, endp);
      return;
    }

    while (i != pivot) {
      if((*i)->id > (*pivot)->id) {
        swap(pivot, i);
        pivot = i;
        continue;
      }
      i++;
    }
    
    while (j != pivot) {
      if((*j)->id < (*pivot)->id) {
        swap(pivot, j);
        pivot = j;
        continue;
      }
      j--;
    }
  }
}

// Essa função apaga da memória um vetor de índices
void erase_index_vector(Index** vectorp, int size) {
  Index* vector = *vectorp;
  for(int i = 0; i < size; i++) {
    free(vector[i]);
    vector[i] = NULL;
  }
  free(vector);
  *vectorp = NULL;
}
