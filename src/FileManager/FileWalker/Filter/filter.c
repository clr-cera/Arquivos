#include "filter.h"
#include <string.h>

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

Filter new_filter() {
  Filter filter = (Filter) malloc(sizeof(filterObj));
  
  filter->id = -1;
  filter->idade = -1;
  filter->nomeJogador = NULL;
  filter->nacionalidade = NULL;
  filter->nomeClube = NULL;

  return filter;
}

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

Filter read_filter() {
  Filter filter = new_filter();
  string type = NULL;
  char test;

  while(true){
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

    scanf("%c", &test);
    if (test == '\n') {
      return filter;
    }
  }
}

bool check_register(Register reg, Filter filter) {
  bool a = true;
  if (filter->id != -1) a = get_id(reg) == filter->id;
  
  bool b = true;
  if (filter->idade != 1) b = get_idade(reg) == filter->idade;

  bool c = true;
  if (filter->nomeJogador != NULL) c = filter_string(get_nome_jogador(reg), filter->nomeJogador);

  bool d = true;
  if (filter->nacionalidade != NULL) c = filter_string(get_nacionalidade(reg), filter->nacionalidade);
  
  bool e = true;
  if (filter->nomeClube != NULL) c = filter_string(get_nome_clube(reg), filter->nomeClube);


  return a&&b&&c&&d&&e;
}

// TODO make filter_string
bool filter_string(string reg_string, string filter_string) {
  if (reg_string == NULL)
    return false;

  return strcmp(reg_string, filter_string) == 0;
}
