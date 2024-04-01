#include "FileWalker/file_walker.h"
#include "file_manager.h"
#include <stdlib.h>
#include <stdio.h>

void fm_insert_name_in_registry(FileManager fm, string file_name);
void fm_create_file(FileManager fm, string file_name);
void fm_delete_file(FileManager fm, string file_name);

typedef struct file_manager_obj_ {
  string* file_name_registry;
  int file_number;
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
      fm_delete_file(fm, fm->file_name_registry[i]);
    }

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
  fm_insert_name_in_registry(fm, file_name);
  fm_create_file(fm, file_name);
}

void fm_create_file(FileManager fm, string file_name) {
  string file_path = concat_string(DATA_PATH, file_name);

  fclose(fopen(file_path, "w")); 
  free(file_path);
}

void fm_delete_file(FileManager fm, string file_name) {
  string file_path = concat_string(DATA_PATH, file_name);
  
  remove(file_path);
  free(file_path);
}

void fm_insert_name_in_registry(FileManager fm, string file_name) {
  if (fm->file_number != 0 && (fm->file_number % 5) == 0) {
    fm->file_name_registry = (string *) realloc(fm->file_name_registry,(fm->file_number+5) * sizeof(string));
  }

  fm->file_name_registry[fm->file_number] = file_name;
  fm->file_number+=1;
}

