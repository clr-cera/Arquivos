#include <stdlib.h>

typedef char* string;
// lib é responsável por conter qualquer conteúdo necessário mas não diretamente relacionado à matéria
string read_string(void);
string concat_string(string s1, string s2);
int string_to_int(char* str, int start);
int dist_to_char(char* str, int start, char target);
char* string_slicer(char* str, int start, int size);
int count_lines(FILE* fp);

