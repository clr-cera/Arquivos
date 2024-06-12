#include "stdio.h"
#include "stdbool.h"

typedef struct index_header_* IndexHeader; 

IndexHeader new_index_header();
IndexHeader get_index_header(FILE* fp);

void erase_index_header(IndexHeader* index_headerp);
bool is_inconsistent_index_header(IndexHeader index_header);
void write_index_header(FILE* fp, IndexHeader index_header);
void index_header_set_status_incon(IndexHeader index_header);
void index_header_set_status_con(IndexHeader index_header);

