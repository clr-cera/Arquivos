#include "../../IndexWalker/Index/index.h"
#include "stdbool.h"

#define ORDER 4
typedef struct node_* Node;

typedef struct search_answer_ {
  bool is_offset;
  long int value;
}SearchAnswer;

typedef struct split_returnal_ {
  Index promoted;
  int right_child;
  Node new_node;
}SplitReturnal;

Node create_node(int altura);
void erase_node(Node* pnode);

void debug_node(Node node);

void write_node(FILE* fp, Node node);
Node read_node(FILE* fp);

SearchAnswer search_offset_or_rrn(Node node, int key);

bool node_is_full(Node node);

int node_get_altura(Node node);
void set_left_child(Node node, int child);

void insert_index_in_node(Node node, Index index, int right_child);
SplitReturnal node_split(Index index, int right_child, Node node);

