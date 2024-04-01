#include "file_walker.h"
#include "Header/header.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct file_walker_ {
  string file_path;
  FILE* fp;

  Header header;
  Register current_register;

}file_walker_obj;
typedef file_walker_obj* FileWalker;


FileWalker create_file_walker(string file_path, bool load_header) {
  FileWalker fw = (FileWalker) malloc(sizeof(file_walker_obj));
  fw->fp = fopen(file_path, "rb+");
  
  fw->file_path = file_path;

  if (load_header) fw->header = get_header(fw->fp);
  else fw->header = new_header();

  return fw;
}

void fw_insert_all(FileWalker fw, RegisterCollection regcol) {
  write_register_collection(fw->fp, regcol);
}

void fw_insert(FileWalker fw, Register reg) {
  write_register(fw->fp, reg);
}
