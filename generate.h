#ifndef GENERATE_H_INCLUDED
#define GENERATE_H_INCLUDED
#include <string.h>
#include "desk.h"
#include "class.h"

void generate(char *file_name, Compiler *compiler);
ClassFile *make_class_file(Compiler *compiler);
void write_class_file(ClassFile *cf, FILE *fp);

#endif // GENERATE_H_INCLUDED
