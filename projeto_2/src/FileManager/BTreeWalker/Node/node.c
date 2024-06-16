#include "stdio.h"
#include "stdlib.h"
#include "node.h"


typedef struct node_* Node;

typedef struct node_ {
  int alturaNo;
  int nroChaves;

  Index chaves[ORDER];
  int descendetes[ORDER];
}nodeObj;



Node create_node(int altura) {
  Node node = (Node) malloc(sizeof(nodeObj));
  node->alturaNo = altura;
  node->nroChaves = 0;

  for(int i = 0; i < ORDER; i++) {
    node->chaves[i] = create_index(-1, -1);
    node->descendetes[i] = -1;
  }

  return node;
}

void erase_node(Node* pnode) {
  for(int i = 0; i < ORDER; i++) {
    erase_index(&(*pnode)->chaves[i]);
  }

  free(*pnode);
  *pnode = NULL;
}

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

Node read_node(FILE* fp) {
  Node node = create_node(0);

  fread(&(node->alturaNo), 1, sizeof(int), fp);
  fread(&(node->nroChaves), 1, sizeof(int), fp);

  for(int i = 0; i < ORDER-1; i++) {
    node->chaves[i] = read_index(fp);
  }

  for(int i = 0; i < ORDER; i++) {
    fread(&(node->descendetes[i]), 1, sizeof(int), fp);
  }

  return node;
}

//TODO need to sort both vectors based on index
void insert_index_in_node(Node node, Index index, int right_child) {
  node->chaves[node->nroChaves] = index;
  node->descendetes[node->nroChaves+1] = right_child;
  node->nroChaves+=1;
}

int get_nroChaves(Node node){
  return node->nroChaves;
}

void debug_node(Node node) {
  printf("Altura: %d\n", node->alturaNo);
  printf("NroChaves: %d\n", node->nroChaves);
  
  for(int i = 0; i < ORDER; i++) {
    print_index(node->chaves[i]);
  }
}

SearchAnswer search_offset_or_rrn(Node node, int key) {
  SearchAnswer answer;
  
  for(int i = 0; i < node->nroChaves; i++) {
    if(get_index_id(node->chaves[i]) == key) {
      answer.is_offset = true;
      answer.value = get_index_offset(node->chaves[i]);
      return answer;
    }

    if(get_index_id(node->chaves[i]) > key) {
      answer.is_offset = false;
      answer.value = node->descendetes[i];
      return answer;
    }
  }

  answer.is_offset = false;
  answer.value = node->descendetes[node->nroChaves];
  return answer;
}

bool node_is_full(Node node) {
  return node->nroChaves == ORDER-1;
}

//TODO implement split
SplitReturnal node_split(Index index, int right_child, Node node);
