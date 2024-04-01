// header contém a struct do header e funções relacionadas
#include <stdio.h>
typedef struct header_* Header;

Header new_header(void);
Header get_header(FILE* fp);
void write_header(FILE* fp, Header header);
void erase_header(Header* headerp);
