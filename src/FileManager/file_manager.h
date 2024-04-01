#include "../Lib/lib.h"

#define DATA_PATH "database/"

typedef struct file_manager_obj_* FileManager;

// file manager é um conjunto de structs e funções responsáveis a administrar e operar os arquivos
FileManager create_file_manager(void);
void erase_file_manager(FileManager* fmp);
void fm_create_empty_table(FileManager fm, string file_name);
