#include "Index/index.h"
#include "IndexHeader/index_header.h"
#include "../../Lib/lib.h"
#include "stdio.h"

typedef struct index_walker_* IndexWalker;

IndexWalker create_index_walker(string file_path, bool create_header);
void close_index_walker(IndexWalker* iwp);

