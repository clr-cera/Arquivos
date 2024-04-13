#include "header.h"
#include <stdlib.h>

//Guarda as informações do header do arquivo
typedef struct header_ {
  char status;
  long int topo;
  long int proxByteOffset;
  int nroRegArq;
  int nroRegRem;
}header_obj;
typedef header_obj* Header;

//Cria um novo header vazio
Header new_header(void) {
  Header header = (Header) malloc(sizeof(header_obj)); 

  header->status = '0';
  header->topo = -1;
  header->proxByteOffset = 25;
  header->nroRegArq = 0;
  header->nroRegRem = 0;

  return header;
}

//Lê o header de um arquivo
Header get_header(FILE* fp) {
  Header header = (Header) malloc(sizeof(header_obj));
  
  fread(&header->status, sizeof(char),1,fp);
  fread(&header->topo, sizeof(long int),1,fp);
  fread(&header->proxByteOffset, sizeof(long int),1,fp);
  fread(&header->nroRegArq, sizeof(int),1,fp);
  fread(&header->nroRegRem, sizeof(int),1,fp);

  return header;
}

//Escreve o header em um arquivo
void write_header(FILE* fp, Header header) {
  fwrite(&header->status, sizeof(char),1,fp);
  fwrite(&header->topo, sizeof(long int),1,fp);
  fwrite(&header->proxByteOffset, sizeof(long int),1,fp);
  fwrite(&header->nroRegArq, sizeof(int),1,fp);
  fwrite(&header->nroRegRem, sizeof(int),1,fp);
}

//Aumenta o número de registros informado pelo header
void header_increse_register_number(Header header, int number) {
  header->nroRegArq+=number;
}

//Aumenta o offset informado pelo header
void header_increse_offset_number(Header header, long int number) {
  header->proxByteOffset += number;
}

//Modifica o status informado pelo header
void header_set_status_incon(Header header) {
  header->status = '0';
}

//Modifica o status informado pelo header
void header_set_status_con(Header header) {
  header->status = '1';
}

//Retorna a quantidade de registros no arquivo
int get_reg_number(Header header) {
  return header->nroRegArq;
}

//Apaga um header, liberando memória
void erase_header(Header* headerp) {
  free(*headerp);
  *headerp = NULL;
}

