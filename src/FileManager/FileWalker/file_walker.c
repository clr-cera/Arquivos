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

void fw_refresh_header(FileWalker fw);

FileWalker create_file_walker(string file_path, bool create_header) {
  FileWalker fw = (FileWalker) malloc(sizeof(file_walker_obj));
  fw->fp = fopen(file_path, "rb+");
  
  fw->file_path = file_path;

  if (create_header) {
    fw->header = new_header();
    write_header(fw->fp, fw->header);
  }
  else fw->header = get_header(fw->fp);

  fw->current_register = NULL;

  return fw;
}

void close_file_walker(FileWalker* fwp) {
  FileWalker fw = *fwp;
  
  free(fw->file_path);

  if (fw->header != NULL) 
    erase_header(&fw->header);
  if (fw->current_register != NULL)
    free_register(&fw->current_register);

  fclose(fw->fp);

  free(fw);
  *fwp = NULL;
}

void fw_insert_all(FileWalker fw, RegisterCollection regcol) {
  long int input_offset = write_register_collection(fw->fp, regcol);
  int input_quantity = size_register_collection(regcol);

  header_increse_offset_number(fw->header, input_offset);
  header_increse_register_number(fw->header, input_quantity);

  fw_refresh_header(fw);
}

void fw_refresh_header(FileWalker fw) {
  int initial_position = ftell(fw->fp);
  fseek(fw->fp, 0, SEEK_SET);
  write_header(fw->fp, fw->header);
  fseek(fw->fp, initial_position, SEEK_SET);

}

void fw_insert(FileWalker fw, Register reg) {
  write_register(fw->fp, reg);
}
