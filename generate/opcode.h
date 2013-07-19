#ifndef CODE_H_INCLUDED
#define CODE_H_INCLUDED
#include "base.h"

/*
 * struct
 */
typedef struct{
    u1 byte;
    char *mnemonic;
    u1 operand_count;
    char *operands_type;
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
    ICONST_5,
    BIPUSH,
    SIPUSH,
    LDC,
    LDC_W,
    ILOAD,
    ILOAD_0,
    ILOAD_1,
    ILOAD_2,
    ILOAD_3,
    ALOAD_0,
    ISTORE,
    ISTORE_0,
    ISTORE_1,
    ISTORE_2,
    ISTORE_3,
    POP,
    POP2,
    DUP,
    DUP_X1,
    DUP_X2,
    DUP2,
    DUP2_X1,
    DUP2_X2,
    SWAP,
    IADD,
    ISUB,
    IMUL,
    IDIV,
    IREM,
    INEG,
    IINC,
    IFEQ,
    IFNE,
    IFLT,
    IFGE,
    IFGT,
    IFLE,
    IF_ICOMPEQ,
    IF_ICOMPNE,
    IF_ICOMPLT,
    IF_ICOMPGE,
    IF_ICOMPGT,
    IF_ICOMPLE,
    GOTO,
    IRETURN,
    RETURN,
    GETSTATIC,
    PUTSTATIC,
    INVOKEVIRTUAL,
    INVOKESPECIAL,
    INVOKESTATIC
} Opcode;

/*
 * extern
 */
extern OpcodeInfo opcode_table[];

#endif // CODE_H_INCLUDED
