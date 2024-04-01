// file_walker é responsável por todas operações internas a um arquivo
#include "../../Lib/lib.h"
#include "Register/register.h"
#include <stdbool.h>

typedef struct file_walker_* FileWalker;

FileWalker create_file_walker(string file_path, bool new_header);

void fw_insert_all(FileWalker fw, RegisterCollection regcol);

