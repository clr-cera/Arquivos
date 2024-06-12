#include "Index/index.h"
#include "IndexHeader/index_header.h"
#include "../../Lib/lib.h"
#include "stdio.h"

typedef struct index_walker_* IndexWalker;

IndexWalker create_index_walker(string file_path, string mode);
void close_index_walker(IndexWalker* iwp);

void iw_insert_all_index(IndexWalker iw, Index* vector, int size);
long int search_offset(IndexWalker iw, int id);


