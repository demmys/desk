#ifndef DESK_H_INCLUDED
#define DESK_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "storage.h"

/*
 * enumerated type
 */
typedef enum{
    INT_TYPE,
    FLOAT_TYPE,
    CHAR_TYPE
} BasicType;

typedef enum{
    INT_EXPRESSION = 1,
    FLOAT_EXPRESSION,
    CHAR_EXPRESSION,
    ADD_EXPRESSION,
    SUB_EXPRESSION,
    MUL_EXPRESSION,
    DIV_EXPRESSION,
    MOD_EXPRESSION,
    MINUS_EXPRESSION,
    EXPRESSION_KIND_COUNT_PLUS_1
} ExpressionKind;

typedef enum{
    EXPRESSION_STATEMENT = 1,
    STATEMENT_TYPE_COUNT_PLUS_1
} StatementType;

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

/*
 * struct declaration
 */
typedef unsigned char u1;
typedef unsigned short u2;
typedef unsigned int u4;
typedef struct Compiler_tag Compiler;
typedef struct ConstantInfo_tag ConstantInfo;
typedef struct Expression_tag Expression;
typedef struct Statement_tag Statement;
typedef struct AttributeInfo_tag AttributeInfo;

struct Compiler_tag{
    Storage *compile_storage;
    int constant_pool_count;
    ConstantInfo *constant_pool;
    int this_class_index;
    int super_class_index;
    AttributeInfo *source_file;
    //int function_count;
    Statement *main_statement; // FunctionList *function_list;
    int current_line_number;
    //InputMode input_mode;
    //Encoding source_encoding;
};

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
};

// expressions
typedef struct {
    Expression  *left;
    Expression  *right;
} BinaryExpression;

struct Expression_tag{
    BasicType type; // TypeSpecifier *type;
    ExpressionKind kind;
    int line_number;
    union{
        int int_value;
        double float_value;
        char char_value;
        BinaryExpression binary_expression;
        struct Expression_tag *minus_expression;
    } u;
};

struct Statement_tag{
    StatementType type;
    int line_number;
    union{
        Expression *expression_s;
    } u;
};

// TypeSpecifier
/*
typedef struct{
    BasicType basic_type;
    TypeDerive *derive;
} TypeSpecifier;

typedef struct TypeDerive_tag{
    DeriveTag tag;
    union{
        FunctionDerive function_d;
    } u;
    struct TypeDerive_tag *next;
} TypeDerive;

typedef enum{
    FUNCTION_DERIVE
} DeriveTag;

typedef struct{
    ParameterList *parameter_list;
} FunctionDerive;

typedef struct ParameterList_tag{
    char *name;
    TypeSpecifier *type;
    int line_number;
    struct ParameterList_tag *next;
} ParameterList;
*/

/*
 * create.c function prototype
 */
void main_define(Statement *statement);
void compile_info_define(char *this_class, char *super_class, char *source_file);
ConstantInfo *add_constant_info(ConstantInfoTag tag);
AttributeInfo *alloc_attribute_info();
int add_utf8(char *value);
int add_class(char *class_name);
int add_name_and_type(char *name, char *type);
int add_methodref(char *class, char *name, char *type);
AttributeInfo *set_source_file_attribute(char *source_file);
Statement *create_expression_statement(Expression *expression);
Expression *create_binary_expression(ExpressionKind operator, Expression *left, Expression *right);
Expression *create_minus_expression(Expression *operand);
Expression* alloc_expression(ExpressionKind kind);

/*
 * compiler.c function prototype
 */
Compiler *get_current_compiler();
void set_current_compiler(Compiler *compiler);
void compile_error(int line_number, char *message);
void *compiler_storage_malloc(size_t size);
ConstantInfo *get_constant_info(int index);
Compiler *create_compiler();
void dispose_compiler(Compiler *compiler);

#endif // DESK_H_INCLUDED
