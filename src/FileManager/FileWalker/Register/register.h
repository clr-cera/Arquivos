// register contém a struct do registro e funções relacionadas
#include "../../../Lib/lib.h"

typedef struct register_* Register;

typedef struct register_collection_ register_collection;

Register create_empty_register(void);
register_collection csv_to_register_vector(string file_path);
