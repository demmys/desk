#ifndef CLASSFILE_ATTRIBUTES_H_INCLUDED
#define CLASSFILE_ATTRIBUTES_H_INCLUDED
#include "classfile_base.h" 

/*
 * enumerated type
 */
typedef enum{
    ATTRIBUTE_ConstantValue = 1, //FieldInfo
    ATTRIBUTE_Code, //MethodInfo
    ATTRIBUTE_Exceptions, //MethodInfo
    ATTRIBUTE_InnerClasses, //ClassFile
    ATTRIBUTE_Synthetic, //ClassFile, FieldInfo, MethodInfo
    ATTRIBUTE_SourceFile, //ClassFile
    ATTRIBUTE_LineNumberTable, //CodeAttribute
    ATTRIBUTE_LocalVariableTable, //Code
    ATTRIBUTE_Deprecated //ClassFile, FieldInfo, MethodInfo
} AttributeTag;

/*
 * struct declaration
 */
typedef struct AttributeInfo_tag AttributeInfo;

typedef struct LocalVariable_tag LocalVariable;
struct LocalVariable_tag{
    u2 start_pc;
    u2 length;
    u2 name_index;
    u2 descriptor_index;
    u2 index;
    LocalVariable *prev;
    LocalVariable *next;
};
typedef struct{
    u2 local_variable_table_length;
    LocalVariable *local_variable_table;
} LocalVariableTableAttribute;

typedef struct LineNumber_tag LineNumber;
struct LineNumber_tag{
    u2 start_pc;
    u2 line_number;
    LineNumber *prev;
    LineNumber *next;
};
typedef struct{
    u2 line_number_table_length;
    LineNumber *line_number_table;
} LineNumberTableAttribute;

typedef struct InnerClass_tag InnerClass;
struct InnerClass_tag{
    u2 inner_class_info_index;
    u2 outer_class_info_index;
    u2 inner_name_index;
    u2 inner_class_access_flags;
};
typedef struct{
    u2 number_of_classes;
    InnerClass *classes;
} InnerClassesAttribute;

typedef struct ExceptionIndex_tag ExceptionIndex;
struct ExceptionIndex_tag{
    u2 exception_index;
    ExceptionIndex *prev;
    ExceptionIndex *next;
};
typedef struct{
    u2 number_of_exceptions;
    ExceptionIndex *exception_index_table;
} ExceptionsAttribute;

typedef struct Code_tag Code;
struct Code_tag{
    u1 code;
    Code *prev;
    Code *next;
};
typedef struct Exception_tag Exception;
struct Exception_tag{
    u2 start_pc;
    u2 end_pc;
    u2 handler_pc;
    u2 catch_type;
    Exception *prev;
    Exception *next;
};
typedef struct{
    u2 max_stack;
    u2 max_locals;
    u4 code_length;
    Code *code;
    u2 exception_table_length;
    Exception *exception_table;
    u2 attributes_count;
    AttributeInfo *attributes;
} CodeAttribute;

struct AttributeInfo_tag{
    AttributeTag tag;
    u2 attribute_name_index;
    u4 attribute_length;
    union{
        u2 cp_index; // ConstantValue, SourceFile
        CodeAttribute code_attribute; // Code
        ExceptionsAttribute exceptions_attribute;
        InnerClassesAttribute inner_classes_attribute;
        u1 dummy; // Deprecated, Synthetic
        LineNumberTableAttribute line_number_table_attribute;
        LocalVariableTableAttribute local_variable_table_attribute;
    } u;
    AttributeInfo *prev;
    AttributeInfo *next;
};

/*
 * classfile.c function prototype
 */
AttributeInfo *add_attribute_info(AttributeInfo *ais, u2 *count, AttributeTag tag, ...);

#endif // CLASSFILE_ATTRIBUTES_H_INCLUDED
