#ifndef GENERATE_H_INCLUDED
#define GENERATE_H_INCLUDED
#include "create.h"
#include "classfile.h"

#define INPUT_EXTENSION ".desk"
#define INPUT_EXTENSION_LEN 5
#define OUTPUT_EXTENSION ".class"
#define OUTPUT_EXTENSION_LEN 6

/*
 * generate.c function prototype
 */
ClassFile *generate(Compiler *compiler);
ConstantInfo *get_constant_info(int index);
int add_utf8(char *value);
int add_class(char *class_name);
int add_name_and_type(char *name, char *type);
int add_methodref(char *class, char *name, char *type);
AttributeInfo *set_source_file_attribute(char *source_file);
//TODO test
void test_constant_pool();

#endif // GENERATE_H_INCLUDED
