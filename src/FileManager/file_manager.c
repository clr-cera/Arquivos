#include "FileWalker/file_walker.h"
#include "file_manager.h"
#include <stdlib.h>
#include <stdio.h>

void insert_name_in_registry(FileManager fm, string file_name);
void create_file(FileManager fm, string file_name);

typedef struct file_manager_obj_ {
  string* file_name_registry;
  int file_number;
  string data_path;
}file_manager_obj;

typedef file_manager_obj* FileManager;

FileManager create_file_manager(void) {
  FileManager fm = (FileManager) malloc(sizeof(file_manager_obj));
  fm->file_number = 0;
  fm->file_name_registry = (string *) malloc((fm->file_number+5) * sizeof(string));
  
  return fm;
}

void erase_file_manager(FileManager* fmp) {
  FileManager fm = *fmp;
  for(int i = 0; i < fm->file_number; i++) 
    if (fm->file_name_registry[i] != NULL) {
      free(fm->file_name_registry[i]);
      fm->file_name_registry[i] = NULL;
    }

  free(fm->file_name_registry);
  free(fm);
  fmp = NULL;
}

void fm_create_empty_table(FileManager fm, string file_name) {
  insert_name_in_registry(fm, file_name);
  create_file(fm, file_name);
}

void create_file(FileManager fm, string file_name) {
  string file_path = concat_string(DATA_PATH, file_name);

  fclose(fopen(file_path, "w")); 
}

void insert_name_in_registry(FileManager fm, string file_name) {
  if (fm->file_number != 0 && (fm->file_number % 5) == 0) {
    fm->file_name_registry = (string *) realloc(fm->file_name_registry,(fm->file_number+5) * sizeof(string));
  }

  fm->file_name_registry[fm->file_number] = file_name;
  fm->file_number+=1;
}


