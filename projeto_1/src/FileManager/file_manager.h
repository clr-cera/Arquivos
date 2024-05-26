#include "../Lib/lib.h"
#include "FileWalker/Filter/filter.h"
#include "IndexWalker/Index/index.h"

#define DATA_PATH "database/"

typedef struct file_manager_obj_* FileManager;

// file manager é um conjunto de structs e funções responsáveis a administrar e operar os arquivos
FileManager create_file_manager(void);
void erase_file_manager(FileManager* fmp);

void fm_create_empty_table(FileManager fm, string file_name);
int fm_insert_csv(FileManager fm, string file_name, string csv_path);

int fm_print_all(FileManager fm, string file_name);
int fm_print_all_filter(FileManager fm, string file_name, Filter filter);

void fm_delete_file(FileManager fm, string file_name);

void fm_create_empty_index(FileManager fm, string file_name);

Index* fm_get_index_vector(FileManager fm,string data_file_name);
int fm_insert_all_index(FileManager fm, string index_file_name, Index* vector, int size);

int fm_get_reg_number(FileManager fm, string data_file_name);

long int fm_get_offset_by_id(FileManager fm, string index_file_name, int id);

int fm_delete_all_filter(FileManager fm, string file_name, string index_name, Filter filter);