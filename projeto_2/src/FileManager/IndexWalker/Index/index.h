#include "stdio.h"
typedef struct index_* Index;

Index create_index(int id, long int byteoffset);
void erase_index(Index* indexp);

void write_index(FILE* fp, Index index); 
Index read_index(FILE* fp);
void print_index(Index index);

int get_index_id(Index index);
long int get_index_offset(Index index);

Index* sort_index_vector(Index* vector, int size);
void erase_index_vector(Index** vectorp, int size);

