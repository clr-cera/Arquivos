#include <stdio.h>
#include <stdlib.h>
#include "Node/node.h"
#include "BTreeHeader/btree_header.h"
#include "btree_walker.h"

typedef struct btree_walker_ {
  string file_path;

  BTreeHeader b_header;
  FILE* fp;
}btree_walkerObj;

typedef btree_walkerObj* BTreeWalker;

typedef struct insert_returnal_ {
  Index promoted;
  int right_child;
  bool error;
}InsertReturnal;

void bw_refresh_header(BTreeWalker bw);
long int bw_search_rec(BTreeWalker bw, int rrn, int key);
InsertReturnal bw_insert_rec(BTreeWalker bw, int current_rrn, Index index);

BTreeWalker create_b_walker(string file_path, string mode) {
  BTreeWalker bw = (BTreeWalker) malloc(sizeof(btree_walkerObj));
  bw->fp = fopen(file_path, mode);
  if (bw->fp == NULL) {
    free(bw);
    return NULL;
  }

  
  bw->file_path = file_path;

  if (strcmp(mode, "wb") == 0) {
    bw->b_header = new_b_header();
    write_b_header(bw->fp, bw->b_header);
  }
  else {
    bw->b_header = get_b_header(bw->fp);

    if(is_inconsistent_b_header(bw->b_header)) {
      close_b_walker(&bw, false);
      return NULL;
    }

    b_header_set_status_incon(bw->b_header);
    bw_refresh_header(bw);
  }


  return bw;

}

void close_b_walker(BTreeWalker* bwp, bool is_con) {
  BTreeWalker bw = *bwp;
  
  free(bw->file_path);
  if (is_con) {
    b_header_set_status_con(bw->b_header);
    bw_refresh_header(bw);
  }


  if (bw->b_header != NULL) 
    erase_b_header(&bw->b_header);

  fclose(bw->fp);

  free(bw);
  *bwp = NULL;
}

void bw_refresh_header(BTreeWalker bw) {
  int initial_position = ftell(bw->fp);
  fseek(bw->fp, 0, SEEK_SET);
  write_b_header(bw->fp, bw->b_header);
  fseek(bw->fp, initial_position, SEEK_SET);
}

void jump_rrn(BTreeWalker bw, int rrn) {
  fseek(bw->fp, 60*(rrn+1), SEEK_SET);
}

void reset_position(BTreeWalker bw) {
  fseek(bw->fp, 60, SEEK_SET);
}

//Esta função retorna o byteoffset associado ao id informado na árvore B
long int bw_search_offset(BTreeWalker bw, int id) {
  long int offset = bw_search_rec(bw, b_header_get_raiz(bw->b_header), id);
  
  reset_position(bw);

  return offset;
}

// Esta é uma função auxiliar que implementa a lógica recursiva para a busca do byteoffset
long int bw_search_rec(BTreeWalker bw, int rrn, int key) {
  if(rrn == -1) {
    return -1;
  }

  jump_rrn(bw, rrn);
  Node node = read_node(bw->fp);

  SearchAnswer answer = search_offset_or_rrn(node, key);

  if(answer.is_offset) {
    erase_node(&node);
    return answer.value;
  }

  else {
    erase_node(&node);
    return bw_search_rec(bw, answer.value, key);
  }
}

int bw_insert(BTreeWalker bw, Index index) {
  InsertReturnal returnal = bw_insert_rec(bw, b_header_get_raiz(bw->b_header), index);

  reset_position(bw);

  if (returnal.error) {
    return 0;
  }
  else {
    return 1;
  }
}

InsertReturnal bw_insert_rec(BTreeWalker bw, int current_rrn, Index index) {
  InsertReturnal returnal;
  returnal.error = false;
  returnal.right_child = -1;
  returnal.promoted = NULL;

  if(current_rrn == -1) {
    returnal.promoted = index;
    return returnal;
  }

  jump_rrn(bw, current_rrn);
  Node node = read_node(bw->fp);

  SearchAnswer answer = search_offset_or_rrn(node, get_index_id(index));
  if(answer.is_offset) {
    erase_node(&node);
    returnal.error = true;
    return returnal;
  }

  InsertReturnal below = bw_insert_rec(bw, answer.value, index);
  
  if (below.error || below.promoted == NULL) {
    erase_node(&node);
    return below;
  }

  if(!node_is_full(node)) {
    insert_index_in_node(node, below.promoted, below.right_child); 
    jump_rrn(bw, current_rrn);
    write_node(bw->fp, node);
    erase_node(&node);

    return returnal;
  }
  
  else {
    SplitReturnal split_returnal = node_split(below.promoted, below.right_child, node);
    
    jump_rrn(bw, current_rrn);
    write_node(bw->fp, node);
    erase_node(&node);

    //TODO think about split returnal right child, shouldn it always be at prox rrn? So should it be a split returnal?
    jump_rrn(bw, split_returnal.right_child);
    write_node(bw->fp, split_returnal.new_node);
    erase_node(&split_returnal.new_node);

    returnal.promoted = split_returnal.promoted;
    returnal.right_child = split_returnal.right_child;

    return returnal;
  }
}


