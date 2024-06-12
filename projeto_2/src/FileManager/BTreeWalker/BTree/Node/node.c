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

void erase_node(Node* pnode) {
  free(*pnode);
  *pnode = NULL;
}


