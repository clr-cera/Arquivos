#include "header.h"
#include <stdlib.h>

typedef struct header_ {
  char status;
  long int topo;
  long int proxByteOffset;
  int nroRegArq;
  int nroRegRem;
}header_obj;
typedef header_obj* Header;

Header new_header(void) {
  Header header = (Header) malloc(sizeof(header_obj)); 

  header->status = 0;
  header->topo = -1;
  header->proxByteOffset = 0;
  header->nroRegArq = 0;
  header->nroRegRem = 0;

  return header;
}

Header get_header(FILE* fp) {
  Header header = (Header) malloc(sizeof(header_obj));
  fread(header, sizeof(header_obj), 1, fp);

  return header;
}

void write_header(FILE* fp, Header header) {
  fwrite(header, sizeof(header_obj), 1, fp);
}

void erase_header(Header* headerp) {
  free(*headerp);
  *headerp = NULL;
}
