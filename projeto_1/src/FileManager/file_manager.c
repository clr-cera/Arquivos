#include "FileWalker/file_walker.h"
#include "file_manager.h"
#include <stdlib.h>
#include <stdio.h>

void fm_insert_name_in_registry(FileManager fm, string file_name);

void fm_create_file(FileManager fm, string file_name);
void fm_delete_file(FileManager fm, string file_name);

int fm_create_file_walker(FileManager fm, string file_name, bool new_header);
void fm_close_file_walker(FileManager fm);
void fm_write_register_collection(FileManager fm, string file_name, RegisterCollection regcol);

typedef struct file_manager_obj_ {
  string* file_name_registry;
  int file_number;
  FileWalker curr_fw;
}file_manager_obj;

typedef file_manager_obj* FileManager;

// Retorna um File Manager sem registro de arquivos
FileManager create_file_manager(void) {
  FileManager fm = (FileManager) malloc(sizeof(file_manager_obj));
  fm->file_number = 0;
  fm->file_name_registry = (string *) malloc((fm->file_number+5) * sizeof(string));
  fm->curr_fw = NULL;
  
  return fm;
}

//Apaga o file manager liberando memória
void erase_file_manager(FileManager* fmp) {
  FileManager fm = *fmp;

  for(int i = 0; i < fm->file_number; i++)
    if (fm->file_name_registry[i] != NULL) {
      fm_delete_file(fm, fm->file_name_registry[i]);
      free(fm->file_name_registry[i]);
      fm->file_name_registry[i] = NULL;
    }


  free(fm->file_name_registry);
  free(fm);
  *fmp = NULL;
}

//Cria um arquivo vazio
void fm_create_empty_table(FileManager fm, string file_name) {
  fm_insert_name_in_registry(fm, file_name);
  fm_create_file(fm, file_name);

  fm_create_file_walker(fm, file_name, true); 
  fm_close_file_walker(fm);
}

//Chama a função de transformar o arquivo csv em um vetor de registros e passa esse vetor de registros para ser inserido em um arquivo binário com nome "file_name"
int fm_insert_csv(FileManager fm, string file_name, string csv_path) {
  RegisterCollection regcol = csv_to_register_vector(csv_path);
  //debug_register_collection(regcol);
  if (regcol == NULL) return -1;
  fm_write_register_collection(fm, file_name, regcol);
  
  free_register_collection(&regcol);
  return 1;
}

//Tendo recebido o vetor de registros, o utiliza para chamar as funções necessárias para inserir os registros no arquivo binário "file_name"
void fm_write_register_collection(FileManager fm, string file_name, RegisterCollection regcol) {
  fm_create_file_walker(fm, file_name, false);
  fw_insert_all(fm->curr_fw, regcol);
  fm_close_file_walker(fm);
}

//cria um File Walker e o associa ao File Manager
int fm_create_file_walker(FileManager fm, string file_name, bool new_header) {
  string file_path = concat_string(DATA_PATH, file_name);
  fm->curr_fw = create_file_walker(file_path, new_header);
  if (fm->curr_fw == NULL) return -1;
  else return 1;
}

//Fecha o file walker associado a um dado File Manager
void fm_close_file_walker(FileManager fm) {
  close_file_walker(&fm->curr_fw);
}

//Cria um arquivo binário de nome "file_name" no diretório "DATA_PATH"
void fm_create_file(FileManager fm, string file_name) {
  string file_path = concat_string(DATA_PATH, file_name);

  fclose(fopen(file_path, "w")); 
  free(file_path);
}

//deleta um arquivo de nome "file_name" no diretório "DATA_PATH"
void fm_delete_file(FileManager fm, string file_name) {
  string file_path = concat_string(DATA_PATH, file_name);
  
  remove(file_path);
  free(file_path);
}

//Insere um novo arquivo no registro do file manager
void fm_insert_name_in_registry(FileManager fm, string file_name) {
  if (fm->file_number != 0 && (fm->file_number % 5) == 0) {
    fm->file_name_registry = (string *) realloc(fm->file_name_registry,(fm->file_number+5) * sizeof(string));
  }

  fm->file_name_registry[fm->file_number] = file_name;
  fm->file_number+=1;
}

//Imprime todos os registros no arquivo informado
int fm_print_all(FileManager fm, string file_name) {
  int returnal;
  returnal = fm_create_file_walker(fm, file_name, false);

  if (returnal != -1) {
    returnal = fw_print_all(fm->curr_fw);
    fm_close_file_walker(fm);
  }

  return returnal;
}

//Imprime todos os registros no arquivo informado que respeitem as condições de um dado filtro
int fm_print_all_filter(FileManager fm, string file_name, Filter filter) {
  int returnal;
  returnal = fm_create_file_walker(fm, file_name, false);

  if (returnal != -1){
    returnal = fw_print_all_filter(fm->curr_fw, filter);
    fm_close_file_walker(fm);
  }

  return returnal;
}

