#include "../FileManager/file_manager.h"
#include "../Lib/lib.h"
#include "../Funcoes_fornecidas/funcoes_fornecidas.h"

//Este header é responsável pra ser usado como interface com todas 
//as possíveis operações

//Operações devem retornar -1 para a main caso tentem acessar um arquivo que não existe

int create_table(char* input_path, char* output_name, FileManager fm);

int select_from(char* file_name, FileManager fm);

int select_from_where(char* file_name, int search_quantity, FileManager fm);

int create_index_file(string data_file_name, string index_file_name, FileManager fm);

int delete_from_where(string file_name, string index_file_name, int rem_quantity, FileManager fm);

int insert_into(string file_name, string index_file_name, int add_quantity, FileManager fm);

int create_index_B(string data_file_name, string index_file_name, FileManager fm);

int select_from_where_id_B(string data_file_name, string index_file_name, FileManager fm);

int select_from_where_B(string data_file_name, string index_file_name, FileManager fm);

int insert_into_B(string file_name, string index_file_name, int add_quantity, FileManager fm);
