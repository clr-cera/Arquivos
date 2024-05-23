#include "index_walker.h"
#include <stdio.h>

typedef struct index_walker_ {
  FILE* index_fp;
  string index_file_path;

  IndexHeader index_header;
}index_walkerObj;
typedef index_walkerObj* IndexWalker;

void iw_refresh_header(IndexWalker iw);

//Inicializa um Index Walker e o associa ao arquivo index_path
IndexWalker create_index_walker(string file_path, bool create_header) {
  IndexWalker iw = (IndexWalker) malloc(sizeof(index_walkerObj));
  iw->index_fp = fopen(file_path, "rb+");
  if (iw->index_fp == NULL) {
    free(iw);
    return NULL;
  }
  
  iw->index_file_path = file_path;

  if (create_header) {
    iw->index_header = new_index_header();
    write_index_header(iw->index_fp, iw->index_header);
  }
  else {
    iw->index_header = get_index_header(iw->index_fp);
    
    if(is_inconsistent_index_header(iw->index_header)){
      erase_index_header(&iw->index_header);
      free(iw);
      return NULL;
    }

    index_header_set_status_incon(iw->index_header);
    iw_refresh_header(iw);
  }


  return iw;
}

void close_index_walker(IndexWalker* iwp) {
  IndexWalker iw = *iwp;
  
  free(iw->index_file_path);
  index_header_set_status_con(iw->index_header);
  iw_refresh_header(iw);

  if (iw->index_header != NULL) 
    erase_index_header(&iw->index_header);

  fclose(iw->index_fp);

  free(iw);
  *iwp = NULL;
}

void insert_all_index(IndexWalker iw, Index* vector, int size) {
  for(int i = 0; i < size; i++) {
    write_index(iw->index_fp, vector[i]);
  }
}

// Retorna byte offset vinculado ao índice no arquivo de indexação
long int search_offset(IndexWalker iw, int id) {
  long int initial_position = ftell(iw->index_fp);
  fseek(iw->index_fp,0, SEEK_END);
  long int final_position = ftell(iw->index_fp);
  fseek(iw->index_fp, initial_position, SEEK_SET);

  while(ftell(iw->index_fp) < final_position) {
    Index index = read_index(iw->index_fp);
    if(get_index_id(index) == id) {
      long int offset = get_index_offset(index);
      erase_index(&index);
      return offset;      
    }
    erase_index(&index);
  }
  return -1;
}

// Atualiza o header do arquivo com os dados do header do IndexWalker
void iw_refresh_header(IndexWalker iw) {
  int initial_position = ftell(iw->index_fp);
  fseek(iw->index_fp, 0, SEEK_SET);
  write_index_header(iw->index_fp, iw->index_header);
  fseek(iw->index_fp, initial_position, SEEK_SET);
}
