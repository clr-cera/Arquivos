#include "index_header.h"
#include "stdlib.h"

// Este arquivo contém funções básicas para o header do arquivo de índices
//
// O header apenas possui um campo que é o status, o qual indica a consistência do arquivo
typedef struct index_header_ {
  char status;
}index_headerObj;

typedef index_headerObj* IndexHeader;


// Essa função cria um novo header para o arquivo
IndexHeader new_index_header() {
  IndexHeader index_header = (IndexHeader) malloc(sizeof(index_headerObj));

  index_header->status = '0';

  return index_header;
}

// Essa função lê o header do arquivo
IndexHeader get_index_header(FILE* fp) {
  IndexHeader index_header = (IndexHeader) malloc(sizeof(index_headerObj));

  fread(&index_header->status, sizeof(char),1,fp);

  return index_header;
}

// Essa função apaga o header da memória
void erase_index_header(IndexHeader* index_headerp) {
  free(*index_headerp);
  *index_headerp = NULL;
}

// Essa função booleana é verdadeira quando o header está inconsistente
bool is_inconsistent_index_header(IndexHeader index_header) {
  return index_header->status == '0';
}

// Essa função escreve o header em formato binário no arquivo informado
void write_index_header(FILE* fp, IndexHeader index_header) {
  fwrite(&index_header->status, sizeof(char),1,fp);
}

//Modifica o status informado pelo header para inconsistente
void index_header_set_status_incon(IndexHeader index_header) {
  index_header->status = '0';
}

//Modifica o status informado pelo header para consistente
void index_header_set_status_con(IndexHeader index_header) {
  index_header->status = '1';
}

