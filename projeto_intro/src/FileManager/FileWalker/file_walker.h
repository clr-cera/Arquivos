// file_walker é responsável por todas operações internas a um arquivo
#include "../../Lib/lib.h"
#include "Register/register.h"
#include "Filter/filter.h"
#include <stdbool.h>

typedef struct file_walker_* FileWalker;

FileWalker create_file_walker(string file_path, bool new_header);
void close_file_walker(FileWalker* fwp);

void fw_insert_all(FileWalker fw, RegisterCollection regcol);
void fw_insert(FileWalker fw, Register reg);

int fw_print_all(FileWalker fw);
int fw_print_all_filter(FileWalker fw, Filter filter);

