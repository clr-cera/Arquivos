#include "index.h"
#include "stdlib.h"

typedef struct index_ {
  int id;
  long int byteoffset;
} indexObj;

typedef indexObj* Index;
void quicksort_rec(Index* startp, Index* endp);

Index create_index(int id, long int byteoffset) {
  Index index = (Index) malloc(sizeof(indexObj));
  
  index->id = id;
  index->byteoffset = byteoffset;

  return index;
}

void erase_index(Index* indexp) {
  free(*indexp);
  *indexp = NULL;
}

void write_index(FILE* fp, Index index) {
  fwrite(&(index->id), sizeof(int), 1, fp);
  fwrite(&(index->byteoffset), sizeof(long int), 1, fp);
}

Index read_index(FILE* fp) {
  int id;
  long int byteoffset;

  fread(&id, sizeof(int), 1, fp);
  fread(&byteoffset, sizeof(long int), 1, fp);

  return create_index(id, byteoffset);
}

void print_index(Index index) {
  if (index == NULL)
    return;

  printf("%d, %ld\n", index->id, index->byteoffset);
}

int get_index_id(Index index) {
  return index->id;
}

long int get_index_offset(Index index) {
  return index->byteoffset;
}

// This function sorts a vector of indexes using quicksort
Index* sort_index_vector(Index* vector, int size) {
  quicksort_rec(vector, vector + size-1);

  return vector;
}

// This functions swaps indexes from two positions of memory
void swap(Index* a, Index* b) {
  Index temp = *a;
  *a = *b;
  *b = temp;
}

// This implements the logic for quicksort
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

void erase_index_vector(Index** vectorp, int size) {
  Index* vector = *vectorp;
  for(int i = 0; i < size; i++) {
    free(vector[i]);
    vector[i] = NULL;
  }
  free(vector);
  *vectorp = NULL;
}
