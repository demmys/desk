#ifndef CODE_H_INCLUDED
#define CODE_H_INCLUDED
#include "classfile_base.h"

typedef struct{
    u1 byte;
    char *mnemonic;
    u2 max_stack;
    u2 max_locals;
} OpcodeInfo;

typedef enum{
} Opcode;

#endif // CODE_H_INCLUDED
