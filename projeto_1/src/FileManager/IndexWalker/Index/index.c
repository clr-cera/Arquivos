#include "index.h"
#include "stdlib.h"

typedef struct index_ {
  int id;
  long int byteoffset;
} indexObj;

typedef indexObj* Index;

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

// TODO make this worth of being in front of my eyes (I will do it don't worry Renan, is now a matter of honor)
Index* sort_index_vector(Index* vector, int size) {
  for(int i = 0; i < size; i++) {
    for(int j = 0; j < size-1; j++) {
      if (vector[j]->id > vector[j+1]->id) {
        Index temp = vector[j];
        vector[j] = vector[j+1];
        vector[j+1] = temp;
      }
    }
  }

  return vector;
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
