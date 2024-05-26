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

    if(is_inconsistent(fw->header)) {
      fw->current_register = NULL;
      close_file_walker(&fw, false);
      return NULL;
    }

    header_set_status_incon(fw->header);
    fw_refresh_header(fw);
  }

  fw->current_register = NULL;

  return fw;
}

//Encerra um file walker, liberando memória e fechando o arquivo com fclose
void close_file_walker(FileWalker* fwp, bool is_con) {
  FileWalker fw = *fwp;
  
  free(fw->file_path);
  if (is_con) {
    header_set_status_con(fw->header);
    fw_refresh_header(fw);
  }


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
      debug_register(reg);
      counter++;
    }
    free_register(&reg);
  }
  return counter;
}

int fw_get_reg_number(FileWalker fw) {
  return get_reg_number(fw->header);
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

Index* data_to_index_vector(FileWalker fw) {
  int size = get_reg_number(fw->header);

  Index* vector = (Index*) malloc(size * sizeof(Index));

  for(int i = 0; i < size; i++) {
    long int position = ftell(fw->fp);
    Register reg = read_register(fw->fp);
    
    if(is_removed(reg)) {
      i--;
    }
    else {
      vector[i] = create_index(get_id(reg), position);
    } 

    free_register(&reg); 
  }

  return vector;
}

//Lê um registro sem tirar o ponteiro do byte offset, útil para atualizar informações de um registro
Register read_in_place(FileWalker fw){
  long int return_pos = ftell(fw->fp);
  Register reg = read_register(fw->fp);
  fseek(fw->fp, return_pos, SEEK_SET);
  return reg;
}

//Deleta todos os registros que correspondem ao filtro
int fw_delete_all_filter(FileWalker fw, Filter filter, IndexWalker iw) {
  long int initial_pos = ftell(fw->fp);
  fseek(fw->fp, 0, SEEK_END);
  long int final_pos = ftell(fw->fp);
  fseek(fw->fp, initial_pos, SEEK_SET);

  int counter = 0;

  if (filter_unique(filter)){
    long int offset = search_offset(iw, filter_get_id(filter));
    fseek(fw->fp, offset, SEEK_SET);
    Register reg = read_in_place(fw);
    add_removed_list(fw, reg);
    
  }

  while(ftell(fw->fp) != final_pos){
    long int return_pos = ftell(fw->fp);
    Register reg = read_register(fw->fp);

    if(!is_removed(reg) && check_register(reg, filter)){
      fseek(fw->fp, return_pos, SEEK_SET);
      add_removed_list(fw, reg);
      counter+=1;
    }
  }
  fseek(fw->fp, initial_pos, SEEK_SET);
  return counter;
}

void add_removed_list(FileWalker fw, Register reg){
  //Configura o registro como logicamente removido
  set_removed(reg);

  long int topo = header_get_topo(fw->header);
  if(topo == -1){
    header_set_topo(fw->header, ftell(fw->fp));
    return;
  }
  
  //Salva a posição do ponteiro do filewalker para retornar no fim da função
  long int return_pos = ftell(fw->fp);

  //Ponteiro se move para o primeiro elemento da lista encadeada para iniciar o percurso
  fseek(fw->fp, topo, SEEK_SET);
  Register current = read_in_place(fw);

  //Caso o primeiro registro da lista de removidos seja maior que o novo registro, o topo deve também ser atualizado
  if (get_register_tamanho(current) >= get_register_tamanho(reg)){
    set_prox(reg, topo);
    header_set_topo(fw->header, ftell(fw->fp));
    free_register(&current);
  } else {
    Register next;
    while(get_prox(current) != -1){
      
      fseek(fw->fp, get_prox(current), SEEK_SET);
      free_register(&current);
      current = read_in_place(fw);


      if(get_prox(current) == -1){
        break;
      } 

      fseek(fw->fp, get_prox(current), SEEK_SET);
      free_register(&next);
      next = read_in_place(fw);

      if(get_register_tamanho(next) >= get_register_tamanho(reg))
        break;
    }
    set_prox(reg, get_prox(current));
    set_prox(current, return_pos);
    write_register(fw->fp, current);
    free_register(&current);
    free_register(&next);
  }

  //Retorna a posição do ponteiro do filewaker para posição salva anteriormente
  fseek(fw->fp, return_pos, SEEK_SET);
  write_register(fw->fp,reg);
  free_register(&reg);
}