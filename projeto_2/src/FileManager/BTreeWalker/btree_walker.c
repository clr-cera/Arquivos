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

void bw_refresh_header(BTreeWalker bw);

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

