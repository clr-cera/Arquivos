#include "file_walker.h"
#include "Header/header.h"
#include <stdio.h>
#include <stdlib.h>

//Responsável pela manipulação interna de um arquivo
typedef struct file_walker_ {
  string file_path;
  FILE* fp;

  Header header;
  Register current_register;

}file_walker_obj;
typedef file_walker_obj* FileWalker;

void fw_refresh_header(FileWalker fw);

//Inicializa um File Walker e o associa ao arquivo file_path
FileWalker create_file_walker(string file_path, bool create_header) {
  FileWalker fw = (FileWalker) malloc(sizeof(file_walker_obj));
  fw->fp = fopen(file_path, "rb+");
  if (fw->fp == NULL) {
    free(fw);
    return NULL;
  }
  
  fw->file_path = file_path;

  if (create_header) {
    fw->header = new_header();
    write_header(fw->fp, fw->header);
  }
  else {
    fw->header = get_header(fw->fp);
    header_set_status_incon(fw->header);
    fw_refresh_header(fw);
  }

  fw->current_register = NULL;

  return fw;
}

//Encerra um file walker, liberando memória e fechando o arquivo com fclose
void close_file_walker(FileWalker* fwp) {
  FileWalker fw = *fwp;
  
  free(fw->file_path);
  header_set_status_con(fw->header);
  fw_refresh_header(fw);

  if (fw->header != NULL) 
    erase_header(&fw->header);
  if (fw->current_register != NULL)
    free_register(&fw->current_register);

  fclose(fw->fp);

  free(fw);
  *fwp = NULL;
}

//Insere todos os registros de um vetor de registro no arquivo associado ao File Walker
void fw_insert_all(FileWalker fw, RegisterCollection regcol) {
  long int input_offset = write_register_collection(fw->fp, regcol);
  int input_quantity = size_register_collection(regcol);

  header_increse_offset_number(fw->header, input_offset);
  header_increse_register_number(fw->header, input_quantity);

  fw_refresh_header(fw);
}

//Imprime todos os registros dentro do arquivo associado ao File Walker
int fw_print_all(FileWalker fw) {
  
  long int initial_pos = ftell(fw->fp);
  fseek(fw->fp, 0, SEEK_END);
  long int final_pos = ftell(fw->fp);
  fseek(fw->fp, initial_pos, SEEK_SET);

  int counter = 0;

  while(ftell(fw->fp) != final_pos){
    Register reg = read_register(fw->fp);
    if(!is_removed(reg)) {
      print_register(reg);
      counter++;
    }
    free_register(&reg);
  }
  return counter;
}

//Imprime todos os registros dentro do arquivo associado ao File Walker que respeitem as condições de um dado filtro
int fw_print_all_filter(FileWalker fw, Filter filter) {
  long int initial_pos = ftell(fw->fp);
  fseek(fw->fp, 0, SEEK_END);
  long int final_pos = ftell(fw->fp);
  fseek(fw->fp, initial_pos, SEEK_SET);

  int counter = 0;

  while(ftell(fw->fp) != final_pos){
    Register reg = read_register(fw->fp);
    if(!is_removed(reg) && check_register(reg, filter)){
      print_register(reg);
      counter+=1;
      // Se apenas um registro for permitido para esse filtro, a busca para
      if (filter_unique(filter)){
        free_register(&reg);
        break;
      }
    }
    free_register(&reg);
  }

  return counter;
}

// Atualiza o header do arquivo com os dados do header do FileWalker
void fw_refresh_header(FileWalker fw) {
  int initial_position = ftell(fw->fp);
  fseek(fw->fp, 0, SEEK_SET);
  write_header(fw->fp, fw->header);
  fseek(fw->fp, initial_position, SEEK_SET);

}

//Insere um único registro no arquivo associado ao File Walker
void fw_insert(FileWalker fw, Register reg) {
  write_register(fw->fp, reg);
}
