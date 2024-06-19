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
int fm_print_all_filter(FileManager fm, string file_name, Filter* filterv, int times);


Index* fm_get_index_vector(FileManager fm,string data_file_name);
int fm_insert_all_index(FileManager fm, string index_file_name, Index* vector, int size);

int fm_get_reg_number(FileManager fm, string data_file_name);

long int fm_get_offset_by_id(Index* vector, int size, int id);

int fm_delete_all_filter(FileManager fm, string file_name, string index_name, Filter* filterv, int times);

Index* fm_create_index_table(string data_file_name, string index_file_name, FileManager fm, bool should_erase);

int fm_insert_into(FileManager fm, string file_name, string index_name, Register* regv, int times);

int fm_create_b_tree(string data_file_name, string b_file_name, FileManager fm);
int fm_print_id_B(FileManager fm, string file_name, string index_file_name, int* index_vector, int times);
int fm_print_all_filter_B(FileManager fm, string file_name, string B_name, Filter* filterv, int times);
int fm_insert_into_B(FileManager fm, string file_name, string b_name, Register* regv, int times);

#endif
