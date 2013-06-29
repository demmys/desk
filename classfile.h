#ifndef CLASSFILE_H_INCLUDED
#define CLASSFILE_H_INCLUDED
#include "classfile_constant_pool.h"
#include "classfile_attributes.h"

/*
 * enumerated type
 */
typedef enum{
    ACC_PUBLIC = 0x0001,
    ACC_PRIVATE = 0x0002,
    ACC_PROTECTED = 0x0004,
    ACC_STATIC = 0x0008,
    ACC_FINAL = 0x0010,
    ACC_SUPER = 0x0020,
    ACC_VOLATILE = 0x0040,
    ACC_TRANSIENT = 0x0080,
    ACC_INTERFACE = 0x0200,
    ACC_ABSTRACT = 0x0400
} AccessFlag; // u2

/*
 * struct declaration
 */
typedef struct{
    u2 access_flags;
    u2 name_index;
    u2 descriptor_index;
    u2 attributes_count;
    AttributeInfo attributes[];
} Definition;

typedef struct {
    u4 magic;
    u2 minor_version;
    u2 major_version;
    u2 constant_pool_count;
    ConstantInfo constant_pool[];
    u2 access_flags;
    u2 this_class_index;
    u2 super_class_index;
    u2 interfaces_count;
    Definition interfaces[];
    u2 fields_count;
    Definition fields[];
    u2 methods_count;
    Definition methods[];
    u2 attributes_count;
    AttributeInfo source_file[];
    char *emit_file;
} ClassFile;

/*
 * classfile.c function prototype
 */
ClassFile *get_current_classfile();
ClassFile *create_class_file();
void dispose_classfile(ClassFile *cf);

#endif // CLASSFILE_H_INCLUDED
