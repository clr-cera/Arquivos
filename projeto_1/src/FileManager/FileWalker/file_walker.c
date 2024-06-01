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
FileWalker create_file_walker(string file_path, string mode) {
  FileWalker fw = (FileWalker) malloc(sizeof(file_walker_obj));
  fw->fp = fopen(file_path, mode);
  if (fw->fp == NULL) {
    free(fw);
    return NULL;
  }

  
  fw->file_path = file_path;

  if (strcmp(mode, "wb") == 0) {
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

  fseek(fw->fp, initial_pos, SEEK_SET);
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
  long int initial_pos = ftell(fw->fp);
  int size = get_reg_number(fw->header);

  Index* vector = (Index*) malloc(size * sizeof(Index));
  for(int i = 0; i < size; i++) {
    vector[i] = NULL;
  } 

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

  fseek(fw->fp, initial_pos, SEEK_SET);
  return vector;
}

//Lê um registro sem tirar o ponteiro do byte offset, útil para atualizar informações de um registro
Register read_in_place(FileWalker fw){
  Register reg = read_register(fw->fp);
  fseek(fw->fp, get_read_at(reg), SEEK_SET);
  return reg;
}

//Deleta todos os registros que correspondem ao filtro, realiza uma busca
int fw_delete_all_filter(FileWalker fw, Filter filter) {
  long int initial_pos = ftell(fw->fp);
  fseek(fw->fp, 0, SEEK_END);
  long int final_pos = ftell(fw->fp);
  fseek(fw->fp, initial_pos, SEEK_SET);

  int counter = 0;

  while(ftell(fw->fp) < final_pos){
    Register reg = read_register(fw->fp);

    if(!is_removed(reg) && check_register(reg, filter)){
      //debug_register(reg); //DEBUG
      fseek(fw->fp, get_read_at(reg), SEEK_SET);
      add_removed_list(fw, reg);
      counter+=1;
      //printf("REMOVED!\n"); //DEBUG
      //debug_removed_list(fw);
    }
    else {
      free_register(&reg);
    }
  }


  fseek(fw->fp, initial_pos, SEEK_SET);
  return counter;
}

//Remove um registro localizado exatamente no offset recebido pela função, não realiza busca
int fw_delete_with_offset(FileWalker fw, Filter filter, long int offset) {
  long int initial_pos = ftell(fw->fp);

  fseek(fw->fp, offset, SEEK_SET); 
  Register reg = read_register(fw->fp);
  
  int counter = 0;

  if(!is_removed(reg)) {
    fseek(fw->fp, get_read_at(reg), SEEK_SET);
    add_removed_list(fw, reg);
    counter=1;
  }

  fseek(fw->fp, initial_pos, SEEK_SET);
  return counter;
}

void add_removed_list(FileWalker fw, Register reg){
  //Configura o registro como logicamente removido
  set_removed(reg);

  //Atualiza as propriedades do header
  header_decrease_register_number(fw->header, 1);
  header_increase_removed_number(fw->header, 1);

  long int topo = header_get_topo(fw->header);
  
  //Lista vazia, logo o removido deve ser inserido no topo
  if(topo == -1){
    //Configura a ligação da lista
    header_set_topo(fw->header, get_read_at(reg));

    //Escreve as mudanças no disco
    overwrite_register(fw->fp, reg, reg);
    fw_refresh_header(fw);

    //Libera RAM
    free_register(&reg);
    return;
  }

  //Ponteiro se move para o primeiro elemento da lista encadeada para iniciar o percurso
  fseek(fw->fp, topo, SEEK_SET);
  Register current = read_in_place(fw);

  //Caso o primeiro registro da lista de removidos seja maior que o novo registro, o topo deve também ser atualizado
  if (get_register_tamanho(current) > get_register_tamanho(reg)){//Aqui um >= seria mais eficiente, pois realizaria menos leituras no disco, mas só fica identico aos casos 5 e 6 com um >
    //Configura a ligação da lista
    set_prox(reg, topo);
    header_set_topo(fw->header, get_read_at(reg));
    
    //Escreve as modificações no disco
    overwrite_register(fw->fp, reg, reg);
    fw_refresh_header(fw);

    //Libera RAM
    free_register(&current);
    free_register(&reg);
    return;
  }

  //Inicia o percurso da lista de removidos
  Register prev = current;
  current = NULL;
  while(get_prox(prev) != -1){

    //Lê o próximo item da lista
    fseek(fw->fp, get_prox(prev), SEEK_SET);
    current = read_in_place(fw);

    //Se encontrar o local para inserir o novo, para
    if(get_register_tamanho(current) > get_register_tamanho(reg))//Aqui um >= seria mais eficiente, pois realizaria menos leituras no disco, mas só fica identico aos casos 5 e 6 com um >
      break;

    //Avança para o próximo da lista
    free_register(&prev);
    prev = current;
    current = NULL;
  }

  //Configura a ligação da lista
  set_prox(reg, get_prox(prev));
  set_prox(prev, get_read_at(reg));

  //Escreve as mudanças na memória
  overwrite_register(fw->fp, prev, prev);
  overwrite_register(fw->fp, reg, reg);
  fw_refresh_header(fw);

  //Libera RAM
  free_register(&prev);
  if (current != NULL) {
    free_register(&current);
  }
  free_register(&reg);
}

//Insere um registro no arquivo binário, conforme as especificações da funcionalidade 6
int fw_insert_into(FileWalker fw, Register reg){
  long int initial_pos = ftell(fw->fp);

  long int topo = header_get_topo(fw->header);
  header_increse_register_number(fw->header, 1);

  //Se o topo é -1, então não há espaços livres no arquivo para inserir o registro. Logo, deve ser inserado no final
  if(topo == -1){
    fseek(fw->fp, 0, SEEK_END);
    write_register(fw->fp, reg);

    //Atualiza o Header para acomodar um novo registro no fim do arquivo
    header_increse_offset_number(fw->header, get_register_tamanho(reg));

    free_register(&reg);
    fseek(fw->fp, initial_pos, SEEK_SET);
    fw_refresh_header(fw);
    return 0;
  }

  // Se o a lista de removidos não for vazia, significa que algum espaço vazio vai ser ocupado, então já se retira do número de registros removidos
  header_decrease_removed_number(fw->header, 1);

  fseek(fw->fp, topo, SEEK_SET);
  Register current = read_in_place(fw);

  //O caso do primeiro da lista de removidos ser o best-fit recebe um tratamento isolado, pois é o único caso onde o topo deve ser modificado
  if (get_register_tamanho(current) >= get_register_tamanho(reg)){
    header_set_topo(fw->header, get_prox(current));
    overwrite_register(fw->fp, reg, current);
    free_register(&reg);
    free_register(&current);

    fseek(fw->fp, initial_pos, SEEK_SET);
    fw_refresh_header(fw);
    return 0;
  }

  //Percorre a lista procurando por espaço
  Register prev = current;
  current = NULL;
  while(get_prox(prev) != -1){
    fseek(fw->fp, get_prox(prev), SEEK_SET);
    current = read_in_place(fw);

    if(get_register_tamanho(current) >= get_register_tamanho(reg)){
      break;
    }

    free_register(&prev);
    prev = current;
    current = NULL;
  }

  //Há espaço na lista
  if(current != NULL && get_register_tamanho(current) >= get_register_tamanho(reg)){
    set_prox(prev, get_prox(current));
    fseek(fw->fp, get_read_at(prev), SEEK_SET);
    write_register(fw->fp, prev);
    overwrite_register(fw->fp, reg, current);
    free_register(&prev);
    free_register(&current);
    free_register(&reg);

    fseek(fw->fp, initial_pos, SEEK_SET);
    fw_refresh_header(fw);
    return 0;
  }

  //Não há espaço na lista, logo insere no fim
  fseek(fw->fp, 0, SEEK_END);
  write_register(fw->fp, reg);

  //Atualiza o Header para acomodar um novo registro no fim do arquivo
  header_increse_offset_number(fw->header, get_register_tamanho(reg));
  header_increse_register_number(fw->header, 1);

  free_register(&reg);
  if(current != NULL) {
    free_register(&current);
  }
  free_register(&prev);

  fseek(fw->fp, initial_pos, SEEK_SET);
  fw_refresh_header(fw);
  return 0;
}

void debug_removed_list(FileWalker fw){

  if(header_get_topo(fw->header) == -1){
    printf("Nem tem lista ainda\n");
    return;
  }

  fseek(fw->fp, header_get_topo(fw->header), SEEK_SET);
  Register current = read_register(fw->fp);
  debug_register(current);

  while (get_prox(current) != -1){
    fseek(fw->fp, get_prox(current), SEEK_SET);
    free_register(&current);
    current = read_register(fw->fp);
    debug_register(current);
  }

  printf("Fim da lista\n");


}
