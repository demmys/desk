#ifndef DESK_H_INCLUDED
#define DESK_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include "storage.h"
#include "class.h"

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

/*
 * struct declaration
 */
typedef struct Compiler_tag Compiler;
typedef struct Expression_tag Expression;
//typedef struct BinaryExpression_tag BinaryExpression;
typedef struct Statement_tag Statement;

struct Compiler_tag{
    Storage *compile_storage;
    Statement *main_statement; // FunctionList *function_list;
    //int function_count;
    int current_line_number;
    //InputMode input_mode;
    //Encoding source_encoding;
};

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
Statement *create_expression_statement(Expression *expression);
Expression *create_binary_expression(ExpressionKind operator, Expression *left, Expression *right);
Expression *create_minus_expression(Expression *operand);
Expression* alloc_expression(ExpressionKind kind);
Statement* alloc_statement(StatementType type);

/*
 * compiler.c function prototype
 */
Compiler *get_current_compiler();
void set_current_compiler(Compiler *compiler);
void *compiler_storage_malloc(size_t size);
Compiler *create_compiler();
void dispose_compiler(Compiler *compiler);
void compile_error(int line_number, char *message);

/*
 * generate.c function prototype
 */
void generate(char *file_name, Compiler *compiler);
ClassFile *make_class_file(Compiler *compiler);
void write_class_file(ClassFile *cf, FILE *fp);

#endif // DESK_H_INCLUDED
