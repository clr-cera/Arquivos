#include "../../../Lib/lib.h"
#include "../../../Funcoes_fornecidas/funcoes_fornecidas.h"
#include "../Register/register.h"

#include <stdbool.h>

typedef struct filter_* Filter;

Filter new_filter();
void erase_filter(Filter* filterp);
void print_filter(Filter filter);

Filter read_filter();
bool check_register(Register reg, Filter filter);

