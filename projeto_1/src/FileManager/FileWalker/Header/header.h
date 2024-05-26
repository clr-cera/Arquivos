// header contém a struct do header e funções relacionadas
#include <stdio.h>
#include <stdbool.h>

typedef struct header_* Header;

Header new_header(void);
Header get_header(FILE* fp);

void header_increse_register_number(Header header, int number);
void header_increse_offset_number(Header header, long int number); 

void header_set_status_incon(Header header);
void header_set_status_con(Header header);

int get_reg_number(Header header);

void write_header(FILE* fp, Header header);
void erase_header(Header* headerp);

bool is_inconsistent(Header header);

int header_get_topo(Header h);

void header_set_topo(Header h, int t);