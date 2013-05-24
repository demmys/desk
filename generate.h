#ifndef GENERATE_H_INCLUDED
#define GENERATE_H_INCLUDED
#include <string.h>
#include "desk.h"

typedef unsigned char u1;
typedef unsigned short u2;
typedef unsigned int u4;

/*
 * constants
 */
// access flags
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

// constant pool tags
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
} ConstantPoolTag;

// attribute tags
typedef enum{
    CONSTANT_VALUE_ATTRIBUTE, // fields
    CODE_ATTRIBUTE, // mehods
    EXCEPTIONS_ATTRIBUTE, // methods
    INNER_CLASSES_ATTRIBUTE, // classfile
    SYNTHETIC_ATTRIBUTE, // classfile, fields, methods
    SOURCE_FILE_ATTRIBUTE, //classfile
    LINE_NUMBER_TABLE_ATTRIBUTE, // codeAttribute
    LOCAL_VARIABLE_TABLE_ATTRIBUTE, // codeAttribute
    DEPRECATED_ATTRIBUTE //classfile, fields, methods
} AttributeTag;

/*
 * structs
 */
//constant pool
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
    u1 *bytes;
} Utf8Info;

typedef struct{
    ConstantPoolTag tag;
    union{
        u2 cp_index;
        ReferenceInfo reference_info;
        u4 bytes;
        LongBytes long_bytes;
        NameAndTypeInfo name_and_type_info;
        Utf8Info utf8_info;
    } u;
} ConstantPoolInfo;

// attribute
typedef struct Attribute_tag Attribute;

/*
typedef struct{
    u2 start_pc;
    u2 length;
    u2 name_index;
    u2 descriptor_index;
    u2 index;
} LocalVariable;

typedef struct{
    u2 local_variable_table_length;
    LocalVariable *local_variable_table;
} LocalVariableTabelAttribute;
*/

typedef struct{
    u2 start_pc;
    u2 line_number;
} LineNumber;

typedef struct{
    u2 line_number_table_length;
    LineNumber *line_number_table; //[line_number_table_length]
} LineNumberTableAttribute;

typedef struct{
    u2 start_pc;
    u2 end_pc;
    u2 handler_pc;
    u2 catch_type;
} Exception;

typedef struct{
    u2 max_stack;
    u2 max_locals;
    u4 code_length;
    u1 *code;
    u2 exception_table_length;
    Exception *exception_table; //[exception_table_length]
    u2 attributes_count;
    Attribute *attributes; //[attributes_count]
} CodeAttribute;

struct Attribute_tag{
    u2 attribute_name_index;
    u4 attribute_length;
    union{
        u2 cp_index; // SourceFile
        CodeAttribute code_attribute; // Code
        LineNumberTableAttribute line_number_table_attribute; // LineNumberTable
    } u;
};

// method, field
typedef struct{
    u2 access_flags;
    u2 name_index;
    u2 descriptor_index;
    u2 attributes_count;
    Attribute *attributes;
} Definition;

// class file
typedef struct{
    u4 magic;
    u2 minor_version;
    u2 major_version;
    u2 constant_pool_count;
    ConstantPoolInfo *constant_pool;//[constant_pool_count - 1]
    u2 access_flags;
    u2 this_class;
    u2 super_class;
    u2 interfaces_count;
    u2 *interfaces;//[interfaces_count]
    u2 fields_count;
    Definition *fields;//[fields_count]
    u2 methods_count;
    Definition *methods;//[methods_count]
    u2 attributes_count;
    Attribute *attributes;//[attributes_count]
} ClassFile;

/*
 * function prototype
 */
void generate(char *file_name, Compiler *compiler);
ClassFile *create_class_file(Compiler *compiler);
void write_class_file(ClassFile *cf, FILE *fp);
Definition *create_methods(Storage *storage, int count);
Attribute *create_attributes(Storage *storage, int count, AttributeTag *tags);

#endif // GENERATE_H_INCLUDED
