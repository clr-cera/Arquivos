#include "stdio.h"
#include "stdlib.h"
#include "node.h"


typedef struct node_* Node;
//Struct do nó da árvore B
typedef struct node_ {
  int alturaNo;
  int nroChaves;

  Index chaves[ORDER];
  int descendetes[ORDER+1];
}nodeObj;


//Cria um nó
Node create_node(int altura) {
  Node node = (Node) malloc(sizeof(nodeObj));
  node->alturaNo = altura;
  node->nroChaves = 0;

  for(int i = 0; i < ORDER; i++) {
    node->chaves[i] = create_index(-1, -1);
    node->descendetes[i] = -1;
  }
  node->descendetes[ORDER] = -1; // Colocar o último descendente

  return node;
}

//Apaga um nó da RAM
void erase_node(Node* pnode) {
  for(int i = 0; i < ORDER; i++) {
    erase_index(&(*pnode)->chaves[i]);
  }

  free(*pnode);
  *pnode = NULL;
}

//Escreve um nó no disco
void write_node(FILE* fp, Node node) {
  fwrite(&(node->alturaNo), 1, sizeof(int), fp);
  fwrite(&(node->nroChaves), 1, sizeof(int), fp);

  for(int i = 0; i < ORDER-1; i++) {
    write_index(fp, node->chaves[i]);
  }

  for(int i = 0; i < ORDER; i++) {
    fwrite(&(node->descendetes[i]), 1, sizeof(int), fp);
  }
}

//Lê um nó do disco
Node read_node(FILE* fp) {
  Node node = create_node(0);

  fread(&(node->alturaNo), 1, sizeof(int), fp);
  fread(&(node->nroChaves), 1, sizeof(int), fp);

  for(int i = 0; i < ORDER-1; i++) {
    erase_index(&node->chaves[i]);
    node->chaves[i] = read_index(fp);
  }

  for(int i = 0; i < ORDER; i++) {
    fread(&(node->descendetes[i]), 1, sizeof(int), fp);
  }

  return node;
}

// Esta função ordena o node assumindo que só há um indíce desordenado no fim do vetor
void sort_keys(Node node) {
  for(int i = node->nroChaves; i > 0; i--) {
    if(get_index_id(node->chaves[i-1]) > get_index_id(node->chaves[i])) {
      Index tmp_index = node->chaves[i];
      node->chaves[i] = node->chaves[i-1];
      node->chaves[i-1] = tmp_index;

      int tmp_desc = node->descendetes[i+1];
      node->descendetes[i+1] = node->descendetes[i];
      node->descendetes[i] = tmp_desc; 
    }
    else {
      break;
    }
  }
}

//Insere um novo índice em um nó já existente
void insert_index_in_node(Node node, Index index, int right_child) {
  if(node->chaves[node->nroChaves] != NULL){
    erase_index(&node->chaves[node->nroChaves]);
  }

  node->chaves[node->nroChaves] = index;
  node->descendetes[node->nroChaves+1] = right_child;
  sort_keys(node);
  
  //debug_node(node); // DEBUG
  node->nroChaves+=1;
  //debug_node(node); // DEBUG
}

int get_nroChaves(Node node){
  return node->nroChaves;
}

//Imprime um nó para fins de transparência durante debug do código
void debug_node(Node node) {
  printf("Altura: %d\n", node->alturaNo);
  printf("NroChaves: %d\n", node->nroChaves);
  
  printf("Child: %d\n", node->descendetes[0]);
  for(int i = 0; i < ORDER; i++) {
    print_index(node->chaves[i]);
    printf("Child: %d\n", node->descendetes[i]);
  }
}

//Realiza a busca de uma chave entre as chaves de um nó
SearchAnswer search_offset_or_rrn(Node node, int key) {
  SearchAnswer answer;
  
  for(int i = 0; i < node->nroChaves; i++) {
    //Caso a chave seja encontrada retorna o byte offset
    if(get_index_id(node->chaves[i]) == key) {
      answer.is_offset = true;
      answer.value = get_index_offset(node->chaves[i]);
      return answer;
    }

    //Caso a chave não seja encontrada retorna o RRN do descendente que ela deve pertencer
    if(get_index_id(node->chaves[i]) > key) {
      answer.is_offset = false;
      answer.value = node->descendetes[i];
      return answer;
    }
  }

  //Caso a chave não seja encontrada retorna o RRN do descendente que ela deve pertencer
  answer.is_offset = false;
  answer.value = node->descendetes[node->nroChaves];
  return answer;
}

//Retorna se um nó está cheio ou não
bool node_is_full(Node node) {
  return node->nroChaves >= ORDER-1;
}

//Retorna a algura de um nó
int node_get_altura(Node node) {
  return node->alturaNo;
}

//Define o filho mais a esquerda de um nó
void set_left_child(Node node, int child) {
  node->descendetes[0] = child;
}

//Realiza o split
SplitReturnal node_split(Index index, int right_child, Node node) {
  insert_index_in_node(node, index, right_child);

  // Configurar novo nó
  Node new_node = create_node(node->alturaNo);
  set_left_child(new_node, node->descendetes[1+((ORDER-1)/2)]);

  // Guardar o promovido e remover de node
  Index promoted = node->chaves[(ORDER-1)/2];
  int prom_r_child = -1; // Não é possível saber ainda, uma vez que isso vai ser configurado no código de inserção
  node->chaves[(ORDER-1)/2] = create_index(-1, -1);
  node->descendetes[1+((ORDER-1)/2)] = -1;
  node->nroChaves--;
  

  // Inserir chaves e filhos à direita do promovido em new_node e removê-los de node
  int max = node->nroChaves;
  for(int i = 1+((ORDER-1)/2); i <= max; i++) {
    insert_index_in_node(new_node, node->chaves[i], node->descendetes[i+1]);
    
    node->chaves[i] = create_index(-1, -1);
    node->descendetes[i+1] = -1;
    node->nroChaves--;
  }

  SplitReturnal split_returnal;
  split_returnal.new_node = new_node;
  split_returnal.promoted = promoted;
  split_returnal.right_child = prom_r_child;

  return split_returnal;
}
