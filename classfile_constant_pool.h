#ifndef CLASSFILE_CONSTANT_POOL_H_INCLUDED
#define CLASSFILE_CONSTANT_POOL_H_INCLUDED
#include "classfile.h"

/*
 * enumerated type
 */
typedef enum{
    CONSTANT_Utf8 = 1,
    CONSTANT_Integer = 3,
    CONSTANT_Float = 4,
    CONSTANT_Long = 5,
    CONSTANT_Double = 6,
    CONSTANT_Class = 7,
    CONSTANT_String = 8,
    CONSTANT_Fieldref = 9,
    CONSTANT_Methodref = 10,
    CONSTANT_InterfaceMethodref = 11,
    CONSTANT_NameAndType = 12
} ConstantInfoTag;

/*
 * struct declaration
 */
typedef struct{
    u2 class_index;
    u2 name_and_type_index;
} ReferenceInfo;
typedef struct{
    u4 high_bytes;
    u4 low_bytes;
} LongBytes;
typedef struct{
    u2 name_index;
    u2 descriptor_index;
} NameAndTypeInfo;
typedef struct{
    u2 length;
    char *value;
} Utf8Info;
typedef struct{
    ConstantInfoTag tag;
    union{
        u2 cp_index;
        ReferenceInfo reference_info;
        u4 bytes;
        LongBytes long_bytes;
        NameAndTypeInfo name_and_type_info;
        Utf8Info utf8_info;
    } u;
} ConstantInfo;

/*
 * classfile_constant_pool.c function prototype
 */
u2 search_constant_info(ConstantInfoTag tag, ...);
u2 add_constant_info(ConstantInfoTag tag, ...);

#endif // CLASSFILE_CONSTANT_POOL_H_INCLUDED
