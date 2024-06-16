#include "stdlib.h"
#include "btree_header.h"


typedef struct btree_header_ {
  char status;
  int noRaiz;
  int proxRRN;
  int nroChaves;
}btree_headerObj;

typedef btree_headerObj* BTreeHeader;

BTreeHeader new_b_header() {
  BTreeHeader b_header = (BTreeHeader) malloc(sizeof(btree_headerObj));
  
  b_header->status = 0;
  b_header->noRaiz = -1;
  b_header->proxRRN = 0;
  b_header->nroChaves = 0;

  return b_header;
}

BTreeHeader get_b_header(FILE* fp) {
  BTreeHeader b_header = (BTreeHeader) malloc(sizeof(btree_headerObj));
  
  fread(&b_header->status, sizeof(char), 1, fp);
  fread(&b_header->noRaiz, sizeof(int), 1, fp);
  fread(&b_header->proxRRN, sizeof(int), 1, fp);
  fread(&b_header->nroChaves, sizeof(int), 1, fp);

  fseek(fp, 47, SEEK_CUR);
  return b_header;
}

void erase_b_header(BTreeHeader* pb_header) {
  free(*pb_header);
  *pb_header = NULL;
}

bool is_inconsistent_b_header(BTreeHeader b_header) {
  return b_header->status == '0';
}

void write_b_header(FILE* fp, BTreeHeader b_header) {
  char trash = '$';

  fwrite(&b_header->status, sizeof(char), 1, fp);
  fwrite(&b_header->noRaiz, sizeof(int), 1, fp);
  fwrite(&b_header->proxRRN, sizeof(int), 1, fp);
  fwrite(&b_header->nroChaves, sizeof(int), 1, fp);

  for(int i = 0; i < 47; i++){
    fwrite(&trash, sizeof(char), 1, fp);
  }
}

void b_header_set_status_incon(BTreeHeader b_header) {
  b_header->status = '0';
}

void b_header_set_status_con(BTreeHeader b_header) {
  b_header->status = '1';
}

void b_header_increase_chaves(BTreeHeader b_header, int number) {
  b_header->nroChaves+=number;
}

void b_header_increase_prox(BTreeHeader b_header,int number) {
  b_header->proxRRN+=number;
}

void b_header_set_raiz(BTreeHeader b_header, int raiz) {
  b_header->noRaiz = raiz;
}

