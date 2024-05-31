#include "FileWalker/file_walker.h"
#include "IndexWalker/index_walker.h"
#include "file_manager.h"
#include <stdlib.h>
#include <stdio.h>


int fm_create_file_walker(FileManager fm, string file_name, OpenType open_type);
void fm_close_file_walker(FileManager fm);
void fm_write_register_collection(FileManager fm, string file_name, RegisterCollection regcol);

int fm_create_index_walker(FileManager fm, string file_name, OpenType open_type);
void fm_close_index_walker(FileManager fm);

typedef struct file_manager_obj_ {
  FileWalker curr_fw;
  IndexWalker curr_iw;
}file_manager_obj;

typedef file_manager_obj* FileManager;

// Retorna um File Manager sem registro de arquivos
FileManager create_file_manager(void) {
  FileManager fm = (FileManager) malloc(sizeof(file_manager_obj));
  fm->curr_fw = NULL; 
  fm->curr_iw = NULL;
  return fm;
}

//Apaga o file manager liberando memória
void erase_file_manager(FileManager* fmp) {
  free(*fmp);
  *fmp = NULL;
}

int fm_create_index_walker(FileManager fm, string file_name, OpenType open_type) {
  string file_path = concat_string(DATA_PATH, file_name);
  
  if(open_type == WRITE) {
    fm->curr_iw = create_index_walker(file_path, "wb");
  }
  if(open_type == READ) {
    fm->curr_iw = create_index_walker(file_path, "rb");
  }
  if(open_type == READWRITE) {
    fm->curr_iw = create_index_walker(file_path, "rb+");
  }

  if (fm->curr_iw == NULL) return -1;
  else return 1;
}

//Fecha o file walker associado a um dado File Manager
void fm_close_index_walker(FileManager fm) {
  close_index_walker(&fm->curr_iw);
}

//Chama a função de transformar o arquivo csv em um vetor de registros e passa esse vetor de registros para ser inserido em um arquivo binário com nome "file_name"
int fm_create_with_csv(FileManager fm, string file_name, string csv_path) {
  RegisterCollection regcol = csv_to_register_vector(csv_path);
  //debug_register_collection(regcol);
  if (regcol == NULL) return -1;
  fm_write_register_collection(fm, file_name, regcol);
  
  free_register_collection(&regcol);
  return 1;
}

//Tendo recebido o vetor de registros, o utiliza para chamar as funções necessárias para inserir os registros no arquivo binário "file_name"
void fm_write_register_collection(FileManager fm, string file_name, RegisterCollection regcol) {
  fm_create_file_walker(fm, file_name, WRITE);
  fw_insert_all(fm->curr_fw, regcol);
  fm_close_file_walker(fm);
}

//cria um File Walker e o associa ao File Manager
int fm_create_file_walker(FileManager fm, string file_name, OpenType open_type) {
  string file_path = concat_string(DATA_PATH, file_name);

  if(open_type == WRITE) {
    fm->curr_fw = create_file_walker(file_path, "wb");
  }
  if(open_type == READ) {
    fm->curr_fw = create_file_walker(file_path, "rb");
  }
  if(open_type == READWRITE) {
    fm->curr_fw = create_file_walker(file_path, "rb+");
  }

  if (fm->curr_fw == NULL) {
    return -1;
  }
  else return 1;
}

//Fecha o file walker associado a um dado File Manager
void fm_close_file_walker(FileManager fm) {
  close_file_walker(&fm->curr_fw, true);
}

int fm_get_reg_number(FileManager fm, string data_file_name) {
  bool should_close = false;
  if(fm->curr_fw == NULL){
    int returnal = fm_create_file_walker(fm, data_file_name, READ);
    if(returnal == -1) {
      return returnal;
    }
    should_close = true;
  }

  int returnal = fw_get_reg_number(fm->curr_fw);

  if(should_close)
    fm_close_file_walker(fm);

  return returnal;
}

//Imprime todos os registros no arquivo informado
int fm_print_all(FileManager fm, string file_name) {
  int returnal;
  returnal = fm_create_file_walker(fm, file_name, READ);

  if (returnal != -1) {
    returnal = fw_print_all(fm->curr_fw);
    fm_close_file_walker(fm);
  }

  return returnal;
}

//Imprime todos os registros no arquivo informado que respeitem as condições de um dado filtro
int fm_print_all_filter(FileManager fm, string file_name, Filter* filterv, int times) {
  int returnal;
  returnal = fm_create_file_walker(fm, file_name, READ);

  if (returnal == -1){
    return returnal;
  }
  for(int i = 0; i < times; i++) {
    printf("Busca %d\n\n", i+1);

    Filter filter = filterv[i];
    returnal = fw_print_all_filter(fm->curr_fw, filter);
  
    if (returnal == 0) {
      printf("Registro inexistente.\n\n");
    }
  }

  fm_close_file_walker(fm);
  return returnal;
}

Index* fm_get_index_vector(FileManager fm,string data_file_name){
  bool has_to_close = false;
  if(fm->curr_fw == NULL) {
    int returnal = fm_create_file_walker(fm, data_file_name, READ);

    if (returnal == -1){
      return NULL;
    }
    has_to_close = true;
  }

  Index* vector = data_to_index_vector(fm->curr_fw); 

  if (has_to_close)
    fm_close_file_walker(fm);

  return vector;
}

int fm_insert_all_index(FileManager fm, string index_file_name, Index* vector, int size) {
  bool has_to_close = false;
  
  if(fm->curr_iw == NULL){ 
    int returnal = fm_create_index_walker(fm, index_file_name, WRITE);
    if (returnal == -1){
      return returnal;
    }
    has_to_close = true;
  }

  iw_insert_all_index(fm->curr_iw, vector, size);

  if(has_to_close)
    fm_close_index_walker(fm);

  return 1;
}

long int fm_get_offset_by_id(FileManager fm, string index_file_name, int id) {
  long int returnal = fm_create_index_walker(fm, index_file_name, READ);

  if(returnal == -1)
    return returnal;

  returnal = search_offset(fm->curr_iw, id);
  fm_close_index_walker(fm);

  return returnal;
}

int fm_create_index_table(string data_file_name, string index_file_name, FileManager fm, bool is_update) {
  Index* vector = fm_get_index_vector(fm, data_file_name); 
  if (vector == NULL) {
    return -1;
  }

  int size = fm_get_reg_number(fm, data_file_name);
  vector = sort_index_vector(vector, size);
  fm_insert_all_index(fm, index_file_name, vector, size);
  erase_index_vector(&vector, size);

  return 1;

}

int fm_delete_all_filter(FileManager fm, string file_name, string index_name, Filter* filterv, int times) {
  int returnal;
  returnal = fm_create_file_walker(fm, file_name, READWRITE);

  if(returnal == -1) {
    return returnal;
  }
  
  for (int i = 0; i < times; i++) {
    if (filter_unique(filterv[i])) {
      //Cria o índice para busca
      fm_create_index_table(file_name, index_name, fm, false);
      break;
    }
  }

  for (int i = 0; i < times; i++) {
    Filter filter = filterv[i];
    if(filter_unique(filter)) {
      long int offset = fm_get_offset_by_id(fm, index_name, filter_get_id(filter));
      // printf("%ld\n", offset); // DEBUG
      if (offset != -1)
        fw_delete_with_offset(fm->curr_fw, filter, offset);
    }

    else {
      returnal = fw_delete_all_filter(fm->curr_fw, filter);
    }
  }

  //Após a remoção, cria o índice novamente, para que ele esteja atualizado sem os valores removidos pelas operações anteriores
  fm_create_index_table(file_name, index_name, fm, true);

  fm_close_file_walker(fm);
  return returnal;
}

int fm_insert_into(FileManager fm, string file_name, string index_name, Register* regv, int times){
  int returnal;
  returnal = fm_create_file_walker(fm, file_name, READWRITE);
  if(returnal == -1) {
    return returnal;
  }


  for(int i = 0; i < times; i++) {
    Register reg = regv[i];
    fw_insert_into(fm->curr_fw, reg);
  }

  //Após as inserções, cria o índice atualizado
  fm_create_index_table(file_name, index_name, fm, true);

  fm_close_file_walker(fm);
  return returnal;
}
