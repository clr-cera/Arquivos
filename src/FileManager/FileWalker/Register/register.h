// register contém a struct do registro e funções relacionadas
#include "../../../Lib/lib.h"
#include <stdio.h>

typedef struct register_* Register;

typedef struct register_collection_ register_collection;

register_collection csv_to_register_vector(string file_path);

Register csv_line_to_register(FILE* fp);
