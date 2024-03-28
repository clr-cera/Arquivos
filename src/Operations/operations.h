#include "../FileManager/file_manager.h"

//Este header é responsável pra ser usado como interface com todas 
//as possíveis operações

void create_table(char* input_path, char* output_path, FileManager fm);

void select_from(char* file_path, FileManager fm);

void select_from_where(char* file_path, int search_quantity, FileManager fm);
