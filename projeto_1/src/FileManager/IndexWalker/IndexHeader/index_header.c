#include "index_header.h"
#include "stdlib.h"

typedef struct index_header_ {
  char status;
}index_headerObj;

typedef index_headerObj* IndexHeader;


IndexHeader new_index_header() {
  IndexHeader index_header = (IndexHeader) malloc(sizeof(index_headerObj));

  index_header->status = '0';

  return index_header;
}

IndexHeader get_index_header(FILE* fp) {
  IndexHeader index_header = (IndexHeader) malloc(sizeof(index_headerObj));

  fread(&index_header->status, sizeof(char),1,fp);

  return index_header;
}

void erase_index_header(IndexHeader* index_headerp) {
  free(*index_headerp);
  *index_headerp = NULL;
}

bool is_inconsistent_index_header(IndexHeader index_header) {
  return index_header->status == '0';
}

void write_index_header(FILE* fp, IndexHeader index_header) {
  fwrite(&index_header->status, sizeof(char),1,fp);
}

//Modifica o status informado pelo header
void index_header_set_status_incon(IndexHeader index_header) {
  index_header->status = '0';
}

//Modifica o status informado pelo header
void index_header_set_status_con(IndexHeader index_header) {
  index_header->status = '1';
}

