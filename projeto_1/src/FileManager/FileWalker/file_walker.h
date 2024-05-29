// file_walker é responsável por todas operações internas a um arquivo
#include "../../Lib/lib.h"
#include "Register/register.h"
#include "Filter/filter.h"
#include "../IndexWalker/Index/index.h"
#include <stdbool.h>

typedef struct file_walker_* FileWalker;

FileWalker create_file_walker(string file_path, string mode);
void close_file_walker(FileWalker* fwp, bool is_con);

void fw_insert_all(FileWalker fw, RegisterCollection regcol);
void fw_insert(FileWalker fw, Register reg);

int fw_print_all(FileWalker fw);
int fw_print_all_filter(FileWalker fw, Filter filter);
Index* data_to_index_vector(FileWalker fw);

void update_register(Register reg, FileWalker fw);
int fw_delete_all_filter(FileWalker fw, Filter filter);
int fw_delete_with_offset(FileWalker fw, Filter filter, long int offset);
int fw_get_reg_number(FileWalker fw);
void add_removed_list(FileWalker fw, Register reg);

int fw_insert_into(FileWalker fw, Register reg);
