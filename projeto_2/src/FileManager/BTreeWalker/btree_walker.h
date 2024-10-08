#include "../../Lib/lib.h"
#include "../IndexWalker/Index/index.h"

//BTreeWalker é a struct para navegar dentro de uma árvore e aqui estão definidas as operações relacionadas a essa struct

typedef struct btree_walker_* BTreeWalker;

BTreeWalker create_b_walker(string file_path, string mode);
void close_b_walker(BTreeWalker* bwp, bool is_con);

long int bw_search_offset(BTreeWalker bw, int id);
int bw_insert(BTreeWalker bw, Index index);

