#include "../FileManager/file_manager.h"
#include "../Lib/lib.h"

//Este header é responsável pra ser usado como interface com todas 
//as possíveis operações

void create_table(char* input_path, char* output_name, FileManager fm);

void select_from(char* file_name, FileManager fm);

void select_from_where(char* file_name, int search_quantity, FileManager fm);
