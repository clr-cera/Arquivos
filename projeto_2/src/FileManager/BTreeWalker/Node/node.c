#include "stdio.h"
#include "stdlib.h"
#include "node.h"


typedef struct node_* Node;

typedef struct node_ {
  int alturaNo;
  int nroChaves;

  Index chaves[ORDER];
  Node descendetes[ORDER];
}nodeObj;



Node create_node(int altura) {
  Node node = (Node) malloc(sizeof(nodeObj));
  node->alturaNo = altura;
  node->nroChaves = 0;

  for(int i = 0; i < ORDER; i++) {
    node->chaves[i] = NULL;
    node->descendetes[i] = NULL;
  }

  return node;
}

void insert_index_in_node(Node node, Index index) {
  node->chaves[node->nroChaves] = index;
  node->nroChaves+=1;
  sort_index_vector(node->chaves, node->nroChaves);
}

void erase_node(Node* pnode) {
  free(*pnode);
  *pnode = NULL;
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


