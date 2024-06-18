#include "filter.h"
#include <string.h>

//Struct filter serve para comparar com registros e verificar se os registros respeitam as condições do filtro
typedef struct filter_{
  int id;
  int idade;
  string nomeJogador;
  string nacionalidade;
  string nomeClube;
}filterObj;
typedef filterObj* Filter;

bool filter_string(string reg_string, string filter_string);
string read_filter_type();

//Cria um filtro vazio
Filter new_filter() {
  Filter filter = (Filter) malloc(sizeof(filterObj));
  
  filter->id = -1;
  filter->idade = -1;
  filter->nomeJogador = NULL;
  filter->nacionalidade = NULL;
  filter->nomeClube = NULL;

  return filter;
}

//Apaga um filtro, liberando memória
void erase_filter(Filter* filterp) {
  Filter filter = *filterp;
  if (filter->nomeJogador != NULL)
    free(filter->nomeJogador);

  if (filter->nacionalidade != NULL)
    free(filter->nacionalidade);
  
  if (filter->nomeClube != NULL)
    free(filter->nomeClube);

  free(filter);
  *filterp = NULL;
}

//Imprime os campos de um dado filtro (utilizado apenas para fins de testes)
void print_filter(Filter filter) {
  printf("id: %d\n", filter->id);
  printf("idade: %d\n", filter->idade);
  if (filter->nomeJogador != NULL)
    printf("Nome do Jogador: %s\n", filter->nomeJogador);
  if (filter->nacionalidade != NULL)
    printf("Nacionalidade: %s\n", filter->nacionalidade);
  if (filter->nomeClube != NULL)
    printf("Nome do Clube: %s\n", filter->nomeClube);
}

//Lê m campos do teclado e os aplica como condições no filtro
Filter read_filter(int m) {
  Filter filter = new_filter();
  string type = NULL;

  for (int i = 0; i < m; i++){
    type = (string) malloc(50*sizeof(char));
    scanf(" %s", type);

    if(strcmp("id",type) == 0) {
      scanf("%d", &filter->id);
    }
    else if(strcmp("idade", type) == 0) {
      scanf("%d", &filter->idade);
    }
    else if(strcmp("nomeJogador", type) == 0) {
      filter->nomeJogador = (string) malloc(50 * sizeof(char));
      scan_quote_string(filter->nomeJogador);
    }
    else if(strcmp("nacionalidade", type) == 0) {
      filter->nacionalidade = (string) malloc(50 * sizeof(char));
      scan_quote_string(filter->nacionalidade);
    }
    else if(strcmp("nomeClube", type) == 0) {
      filter->nomeClube = (string) malloc(50 * sizeof(char));
      scan_quote_string(filter->nomeClube);
    }
    free(type);
  }
  return filter;
}

//Compara um registro com um filtro e retorna se o registro passa no filtro ou não
bool check_register(Register reg, Filter filter) {
  if (filter == NULL) {
    return true;
  }

  bool a = true;
  if (filter->id != -1) a = get_id(reg) == filter->id;
  
  bool b = true;
  if (filter->idade != -1) b = get_idade(reg) == filter->idade;

  bool c = true;
  if (filter->nomeJogador != NULL) c = filter_string(get_nome_jogador(reg), filter->nomeJogador);

  bool d = true;
  if (filter->nacionalidade != NULL) d = filter_string(get_nacionalidade(reg), filter->nacionalidade);
  
  bool e = true;
  if (filter->nomeClube != NULL) e = filter_string(get_nome_clube(reg), filter->nomeClube);


  return a&&b&&c&&d&&e;
}

//Compara duas strings, uma do registro e outra do filtro
bool filter_string(string reg_string, string filter_string) {
  if (reg_string == NULL)
    return false;

  return strcmp(reg_string, filter_string) == 0;
}

// Retorna true se o filtro apenas permitir passar um registro
bool filter_unique(Filter filter) {
  return filter->id != -1;
}

// Retorna o id do filtro
// É usado para buscar o offset em caso de filtro único
int filter_get_id(Filter filter){
  return filter->id;
}
