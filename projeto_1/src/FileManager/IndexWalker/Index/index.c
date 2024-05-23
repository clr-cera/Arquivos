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

int get_index_id(Index index) {
  return index->id;
}

long int get_index_offset(Index index) {
  return index->byteoffset;
}

// TODO make this worth of being in front of my eyes (I will do it don't worry Renan, is now a matter of honor)
Index* sort_index_vector(Index* vector, int size) {
  for(int i = 0; i < size; i++) {
    for(int j = 0; j < size; j++) {
      if (vector[i]->id > vector[j]->id) {
        Index temp = vector[i];
        vector[i] = vector[j];
        vector[j] = temp;
      }
    }
  }

  return vector;
}
