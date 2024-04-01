#include "file_walker.h"
#include "Register/register.h"
#include "Header/header.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct file_walker_ {
  string file_path;
  Header header;

}file_walker_obj;
typedef file_walker_obj* FileWalker;


FileWalker create_file_walker(string file_path) {
  FileWalker fw = (FileWalker) malloc(sizeof(file_walker_obj));
  
  fw->file_path = file_path;
  fw->header = NULL;

  return fw;
}
