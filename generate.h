#ifndef GENERATE_H_INCLUDED
#define GENERATE_H_INCLUDED
#include "create.h"
#include "classfile.h"

#define INPUT_EXTENSION ".desk"
#define INPUT_EXTENSION_LENGTH 5
#define OUTPUT_EXTENSION ".class"
#define OUTPUT_EXTENSION_LENGTH 6

/*
 * generate.c function prototype
 */
ClassFile *generate(Compiler *compiler);
//TODO test
void test_constant_pool();

#endif // GENERATE_H_INCLUDED
