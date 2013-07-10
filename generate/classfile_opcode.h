#ifndef CODE_H_INCLUDED
#define CODE_H_INCLUDED
#include "classfile_base.h"

/*
 * struct
 */
typedef struct{
    u1 byte;
    char *mnemonic;
    char *operand;
    u2 max_stack;
    u2 max_locals;
} OpcodeInfo;

typedef enum{
    ICONST_M1 = 1,
    ICONST_0,
    ICONST_1,
    ICONST_2,
    ICONST_3,
    ICONST_4,
    ICONST_5
} Opcode;

/*
 * extern
 */
extern OpcodeInfo opcode_table[];

#endif // CODE_H_INCLUDED
