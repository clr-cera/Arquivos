#include "FileWalker/file_walker.h"
#include "IndexWalker/index_walker.h"
#include "BTreeWalker/btree_walker.h"
#include "file_manager.h"
#include <stdlib.h>
#include <stdio.h>

// Esse arquivo é responsável pela implementação do File Manager,
// estrutura a qual é responsável pela manipulação de arquivos e lógica geral em que pedidos
// de alterações internas ocorrem ao File Walker e Index Walker

int fm_create_file_walker(FileManager fm, string file_name, OpenType open_type);
void fm_close_file_walker(FileManager fm);
void fm_write_register_collection(FileManager fm, string file_name, RegisterCollection regcol);

int fm_create_index_walker(FileManager fm, string file_name, OpenType open_type);
void fm_close_index_walker(FileManager fm);

typedef struct file_manager_obj_ {
  FileWalker curr_fw;
  IndexWalker curr_iw;
  BTreeWalker curr_bw;
}file_manager_obj;

typedef file_manager_obj* FileManager;

// Retorna um File Manager sem registro de arquivos
FileManager create_file_manager(void) {
  FileManager fm = (FileManager) malloc(sizeof(file_manager_obj));
  fm->curr_fw = NULL; 
  fm->curr_iw = NULL;
  fm->curr_bw = NULL;
  return fm;
}

// Apaga o file manager da memória primária
// Assumimos que qualquer walker já tenho sido apagado da memória
// neste ponto, uma vez que após uma operação não é permitido que qualquer walker
// ainda exista na memória
void erase_file_manager(FileManager* fmp) {
  free(*fmp);
  *fmp = NULL;
}

// Cria um Index Walker e o associa ao File Manager
// Utiliza a enum OpenType para selecionar o modo de abertura do arquivo
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

//Fecha o index walker associado a um dado File Manager
void fm_close_index_walker(FileManager fm) {
  close_index_walker(&fm->curr_iw, true);
}

// Cria um BTree Walker e o associa ao File Manager
// Utiliza a enum OpenType para selecionar o modo de abertura do arquivo
int fm_create_b_walker(FileManager fm, string file_name, OpenType open_type) {
  string file_path = concat_string(DATA_PATH, file_name);

  if(open_type == WRITE) {
    fm->curr_bw = create_b_walker(file_path, "wb+");
  }
  if(open_type == READ) {
    fm->curr_bw = create_b_walker(file_path, "rb");
  }
  if(open_type == READWRITE) {
    fm->curr_bw = create_b_walker(file_path, "rb+");
  }

  if (fm->curr_bw == NULL) return -1;
  else return 1;
}

//Fecha o b walker associado a um dado File Manager
void fm_close_b_walker(FileManager fm) {
  close_b_walker(&fm->curr_bw, true);
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

// Cria um File Walker e o associa ao File Manager
// Utiliza a enum OpenType para selecionar o modo de abertura do arquivo
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

// Essa função retorna o número de registros do arquivo informado
int fm_get_reg_number(FileManager fm, string data_file_name) {
  bool should_close = false;

  // Caso o File Walker já exista assumimos que o ponteiro fp está na posição do primeiro registro
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

// Essa função gera um vetor de índices a partir do arquivo de dados informado
// É utilizada para gerar o vetor que será ordenado antes da geração do arquivo de índices
Index* fm_get_index_vector(FileManager fm,string data_file_name){
  bool has_to_close = false;
  // Caso o File Walker já exista assumimos que o ponteiro fp está na posição do primeiro registro
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

// Essa função insere o vetor de índices informado no arquivo informado
int fm_insert_all_index(FileManager fm, string index_file_name, Index* vector, int size) {
  bool has_to_close = false;
  
  // Caso o arquivo já exista assumimos que o ponteiro fp está na posição do primeiro índice
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

// Essa função busca em um vetor de índices o offset relacionado ao id informado,
// utiliza da busca binária para isso
// Retorna -1 caso não ache
long int fm_get_offset_by_id(Index* vector, int size, int id) {
  int start = 0;
  int end = size;

  while(end > start) {
    int pivot = (end+start)/2;

    if(get_index_id(vector[pivot]) == id) {
      return get_index_offset(vector[pivot]);
    }

    else if(get_index_id(vector[pivot]) > id) {
      end = pivot;
    }
    else {
      start = pivot+1;
    }
  }
  return -1;
}

// Essa função gera um arquivo de índices a partir do arquivo de dados informado
// Ela retorna NULL se não houver o arquivo de dados, retorna um endereço lixo caso
// should_erase seja true, e retorna o vetor de índices gerado em qualquer outro caso
Index* fm_create_index_table(string data_file_name, string index_file_name, FileManager fm, bool should_erase) {
  Index* vector = fm_get_index_vector(fm, data_file_name); 
  if (vector == NULL) {
    return vector;
  }

  int size = fm_get_reg_number(fm, data_file_name);
  vector = sort_index_vector(vector, size);
  fm_insert_all_index(fm, index_file_name, vector, size);

  if (should_erase) {
    erase_index_vector(&vector, size);
    return (Index*) 1;
  }

  return vector;
}

// Essa função gera um arquivo de árvore B a partir do arquivo de dados informado
int fm_create_b_tree(string data_file_name, string b_file_name, FileManager fm) {
  int returnal;
  bool should_close_fw = false;
  bool should_close_bw = false;

  if(fm->curr_fw == NULL){
    returnal = fm_create_file_walker(fm, data_file_name, READ);
    if (returnal == -1){
      return returnal;
    }
    should_close_fw = true;
  }

  if(fm->curr_bw == NULL) {
    returnal = fm_create_b_walker(fm, b_file_name, WRITE);
    if (returnal == -1){
      if(should_close_fw) {
        fm_close_file_walker(fm);
      }
      return returnal;
    }
    should_close_bw = true;
  }

  Index index;

  while(fw_index_tok(&index, fm->curr_fw) != NULL) {
    bw_insert(fm->curr_bw, index);
  };

  if(should_close_fw) {
    fm_close_file_walker(fm);
  }
  if(should_close_bw) {
    fm_close_b_walker(fm);
  }

  return 1;
}

// Essa função remove todos os registros do arquivo informado que passarem 
// por pelo menos um filtro do vetor de filtros
int fm_delete_all_filter(FileManager fm, string file_name, string index_name, Filter* filterv, int times) {
  int returnal;
  returnal = fm_create_file_walker(fm, file_name, READWRITE);

  Index* vector = NULL;
  int size = 0;

  if(returnal == -1) {
    return returnal;
  }
  
  // Caso algum filtro seja único (contenha informação sobre id), a tabela de índices é 
  // gerada e armazenada também em memória primária
  for (int i = 0; i < times; i++) {
    if (filter_unique(filterv[i])) {
      //Cria o índice para busca
      vector = fm_create_index_table(file_name, index_name, fm, false);
      size = fm_get_reg_number(fm, file_name);
      break;
    }
  }

  for (int i = 0; i < times; i++) {
    Filter filter = filterv[i];
    // Caso o filtro seja único a tabela de índices é utilizada para conseguir alcançar 
    // o registro em menor complexidade de tempo, uma vez que é capaz de realizar busca
    // binária
    if(filter_unique(filter)) {
      long int offset = fm_get_offset_by_id(vector, size, filter_get_id(filter));
      // Caso o offset seja -1 significa que o id informado não pertence à nenhum registro
      // do arquivo de dados, então não é necessário o deslocamento do ponteiro fp
      if (offset != -1)
        fw_delete_with_offset(fm->curr_fw, filter, offset);
    }

    else {
      returnal = fw_delete_all_filter(fm->curr_fw, filter);
    }
  }

  if(vector != NULL)
    erase_index_vector(&vector, size);

  //Após a remoção, cria o índice novamente, para que ele esteja atualizado sem os valores removidos pelas operações anteriores
  fm_create_index_table(file_name, index_name, fm, true);

  fm_close_file_walker(fm);
  return returnal;
}

// Essa função insere todos os registros do vetor de registros no arquivo de dados
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

//Imprime todos os registros no arquivo informado que respeitem as condições de um dado filtro
int fm_print_id_B(FileManager fm, string file_name, string index_file_name, int* index_vector, int times) {
  int returnal;
  returnal = fm_create_file_walker(fm, file_name, READ);
  if (returnal == -1){
    return returnal;
  }

  returnal = fm_create_b_walker(fm, index_file_name, READ);
  if (returnal == -1){
    fm_close_file_walker(fm);
    return returnal;
  }

  for(int i = 0; i < times; i++) {
    printf("BUSCA %d\n\n", i+1);

    returnal = 0;

    long int offset = bw_search_offset(fm->curr_bw, index_vector[i]);

    if(offset != -1) {
      returnal = fw_print_with_offset(fm->curr_fw, NULL, offset);
    }

    if (returnal == 0) {
      printf("Registro inexistente.\n\n");
    }
  }

  fm_close_file_walker(fm);
  fm_close_b_walker(fm);
  return returnal;
}

int fm_print_all_filter_B(FileManager fm, string file_name, string B_name, Filter* filterv, int times) {
  int returnal;
  returnal = fm_create_file_walker(fm, file_name, READ);

  if(returnal == -1) {
    return returnal;
  }
  
  returnal = fm_create_b_walker(fm, B_name, READ);
  if (returnal == -1){
    fm_close_file_walker(fm);
    return returnal;
  }

  for (int i = 0; i < times; i++) {
    printf("Busca %d\n\n", i+1);

    Filter filter = filterv[i];

    returnal = -1;
    if(filter_unique(filter)) {
      long int offset = bw_search_offset(fm->curr_bw, filter_get_id(filter)); 
      // Caso o offset seja -1 significa que o id informado não pertence à nenhum registro
      // do arquivo de dados, então não é necessária a busca
      if (offset != -1){
        returnal = fw_print_with_offset(fm->curr_fw, filter, offset);
      }
    }

    else {
      returnal = fw_print_all_filter(fm->curr_fw, filter);
    }

    if (returnal == 0) {
      printf("Registro inexistente.\n\n");
    }
  }

  fm_close_file_walker(fm);
  fm_close_b_walker(fm);
  
  return 1;
}

// Essa função insere todos os registros do vetor de registros no arquivo de dados
int fm_insert_into_B(FileManager fm, string file_name, string b_name, Register* regv, int times){
  int returnal;
  returnal = fm_create_file_walker(fm, file_name, READWRITE);
  if(returnal == -1) {
    return returnal;
  }

  returnal = fm_create_b_walker(fm, b_name, READWRITE);
  if (returnal == -1) {
    fm_close_file_walker(fm);
    return returnal;
  }
  
  for(int i = 0; i < times; i++) {
    Register reg = regv[i];
    int id = get_id(reg);
    long int byteoffset = fw_insert_into(fm->curr_fw, reg);
    bw_insert(fm->curr_bw, create_index(id,  byteoffset));
  }

  fm_close_file_walker(fm);
  fm_close_b_walker(fm);
  return returnal;
}

