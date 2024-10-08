#include "../Lib/lib.h"
#include "FileWalker/Filter/filter.h"

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

