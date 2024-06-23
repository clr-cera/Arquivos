#include "stdio.h"
#include "stdbool.h"

//BTreeHeader é a struct do header da árvore B e aqui estão definidas as operações relacionadas a essa struct

typedef struct btree_header_* BTreeHeader;

BTreeHeader new_b_header();
BTreeHeader get_b_header(FILE* fp);
void erase_b_header(BTreeHeader* pb_header);

bool is_inconsistent_b_header(BTreeHeader b_header);
void write_b_header(FILE* fp, BTreeHeader b_header);

void b_header_set_status_incon(BTreeHeader b_header);
void b_header_set_status_con(BTreeHeader b_header);

void b_header_increase_chaves(BTreeHeader b_header, int number);
void b_header_increase_prox(BTreeHeader b_header);
void b_header_set_raiz(BTreeHeader b_header, int raiz);

int b_header_get_raiz(BTreeHeader b_header);
int b_header_get_prox(BTreeHeader b_header);

