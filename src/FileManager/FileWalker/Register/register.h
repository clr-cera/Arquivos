// register contém a struct do registro e funções relacionadas
#include <stdio.h>
#include "../../../Lib/lib.h"
#include <stdbool.h>

typedef struct register_* Register;

typedef struct register_collection_* RegisterCollection;

void debug_register(Register reg);
void debug_register_collection(RegisterCollection regcol);

void print_register(Register reg);

void free_register(Register* regp); 
void free_register_collection(RegisterCollection* regcolp);

int size_register_collection(RegisterCollection regcol);
long int get_register_tamanho(Register reg);

RegisterCollection csv_to_register_vector(string file_path);
Register csv_line_to_register(FILE* fp);

void write_register(FILE* fp, Register reg);
// Retorna o tamanho de toda a coleção de registros
long int write_register_collection(FILE* fp, RegisterCollection regcol);

Register read_register(FILE* fp);

bool is_removed(Register reg);

int get_id(Register reg);
int get_idade(Register reg);
string get_nome_jogador(Register reg);
string get_nacionalidade(Register reg);
string get_nome_clube(Register reg);

