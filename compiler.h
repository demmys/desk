#ifndef COMPILE_H_INCLUDED
#define COMPILE_H_INCLUDED
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

/*
 * struct declaration
 */
typedef struct Expression_tag Expression;
typedef struct Statement_tag Statement;

typedef struct{
    Storage *compile_storage;
    //int function_count;
    Statement *main_statement; // FunctionList *function_list;
    int current_line_number;
    //InputMode input_mode;
    //Encoding source_encoding;
    char *source_file;
}Compiler;

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
        Expression *minus_expression;
    } u;
};

struct Statement_tag{
    StatementType type;
    int line_number;
    union{
        Expression *expression_s;
    } u;
};


/*
 * compiler.c function prototype
 */
Compiler *get_current_compiler();
void compile_error(int line_number, char *message);
void *compiler_storage_malloc(size_t size);
Compiler *create_compiler(char *source_file);
void dispose_compiler(Compiler *compiler);

#endif // COMPILE_H_INCLUDED
