#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include "../Lib/lib.h"
#include "FileWalker/Filter/filter.h"
#include "IndexWalker/Index/index.h"

#define DATA_PATH "./"

typedef enum open_type_ {
  WRITE,
  READ,
  READWRITE,  
}OpenType;

typedef struct file_manager_obj_* FileManager;

// file manager é um conjunto de structs e funções responsáveis a administrar e operar os arquivos
FileManager create_file_manager(void);
void erase_file_manager(FileManager* fmp);

int fm_create_with_csv(FileManager fm, string file_name, string csv_path);

int fm_print_all(FileManager fm, string file_name);
int fm_print_all_filter(FileManager fm, string file_name, Filter filter);


Index* fm_get_index_vector(FileManager fm,string data_file_name);
int fm_insert_all_index(FileManager fm, string index_file_name, Index* vector, int size);

int fm_get_reg_number(FileManager fm, string data_file_name);

long int fm_get_offset_by_id(FileManager fm, string index_file_name, int id);

int fm_delete_all_filter(FileManager fm, string file_name, string index_name, Filter filter);

int fm_create_index_table(string data_file_name, string index_file_name, FileManager fm, bool is_update);
#endif
