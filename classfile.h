#ifndef CLASSFILE_H_INCLUDED
#define CLASSFILE_H_INCLUDED
#include "storage.h"

typedef unsigned char u1;
typedef unsigned short u2;
typedef unsigned int u4;

/*
 * enumerated type
 */
typedef enum{
    CONSTANT_Class = 7,
    CONSTANT_Fieldref = 9,
    CONSTANT_Methodref = 10,
    CONSTANT_InterfaceMethodref = 11,
    CONSTANT_String = 8,
    CONSTANT_Integer = 3,
    CONSTANT_Float = 4,
    CONSTANT_Long = 5,
    CONSTANT_Double = 6,
    CONSTANT_NameAndType = 12,
    CONSTANT_Utf8 = 1
} ConstantInfoTag;

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
typedef struct ClassFile_tag ClassFile;
typedef struct ConstantInfo_tag ConstantInfo;
typedef struct AttributeInfo_tag AttributeInfo;

// Constant pool
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
struct ConstantInfo_tag{
    ConstantInfoTag tag;
    union{
        u2 cp_index;
        ReferenceInfo reference_info;
        u4 bytes;
        LongBytes long_bytes;
        NameAndTypeInfo name_and_type_info;
        Utf8Info utf8_info;
    } u;
    ConstantInfo *next;
};

// attributes
typedef struct{
    u2 start_pc;
    u2 line_number;
} LineNumber;
typedef struct{
    u2 line_number_table_length;
    LineNumber *line_number_table; //[line_number_table_length]
} LineNumberTableAttribute;
/*
typedef struct{
    u2 start_pc;
    u2 end_pc;
    u2 handler_pc;
    u2 catch_type;
} Exception;
*/
typedef struct{
    u2 max_stack;
    u2 max_locals;
    u4 code_length;
    u1 *code;
    u2 exception_table_length;
    //Exception *exception_table; //[exception_table_length]
    u2 attributes_count;
    AttributeInfo *attributes; //[attributes_count]
} CodeAttribute;
struct AttributeInfo_tag{
    u2 attribute_name_index;
    u4 attribute_length;
    union{
        u2 cp_index; // SourceFile
        CodeAttribute code_attribute; // Code
        LineNumberTableAttribute line_number_table_attribute; // LineNumberTable
    } u;
    AttributeInfo *next;
};

typedef struct{
    u2 access_flags;
    u2 name_index;
    u2 descriptor_index;
    u2 attributes_count;
    AttributeInfo *attributes;
} Definition;

struct ClassFile_tag{
    u4 magic;
    u2 minor_version;
    u2 major_version;
    u2 constant_pool_count;
    ConstantInfo *constant_pool;
    u2 access_flags;
    u2 this_class_index;
    u2 super_class_index;
    u2 interfaces_count;
    // Definition *interfaces;
    u2 fields_count;
    // Definition *fields;
    u2 methods_count;
    Definition *methods;
    u2 attributes_count;
    AttributeInfo *source_file;
    char *emit_file;
    Storage *classfile_storage;
};

/*
 * classfile.c function prototype
 */
ClassFile *get_current_classfile();
void *classfile_storage_malloc(size_t size);
ClassFile *create_class_file();

#endif // CLASSFILE_H_INCLUDED
