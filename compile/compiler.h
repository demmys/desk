#ifndef COMPILE_H_INCLUDED
#define COMPILE_H_INCLUDED
#include "storage.h"
#include "error.h"

/*
 * enumerated type
 */

typedef enum{
    ADD_OPERATOR,
    SUB_OPERATOR,
    MUL_OPERATOR,
    DIV_OPERATOR,
    MOD_OPERATOR
} OperatorKind;

typedef enum{
    INT_EXPRESSION,
    IDENTIFIER_EXPRESSION,
    BINARY_EXPRESSION,
    MINUS_EXPRESSION,
    CALL_EXPRESSION
} ExpressionKind;

typedef enum{
    NONE_TYPE_STATEMENT,
    MAIN_STATEMENT,
    CONSTRUCTOR_STATEMENT,
    FUNCTION_STATEMENT,
    FUNCTION_PATTERN_STATEMENT
} StatementType;

/*
 * struct declaration
 */
typedef struct Expression_tag Expression;
typedef struct{
    char *identifier;
    Expression *parameter_expression;
} CallExpression;

typedef struct{
    OperatorKind kind;
    Expression  *left;
    Expression  *right;
} BinaryExpression;

struct Expression_tag{
    ExpressionKind kind;
    int line_number;
    union{
        int int_value;
        char *identifier;
        BinaryExpression binary_expression;
        Expression *minus_expression;
        CallExpression call_expression;
    } u;
};

typedef struct{
    StatementType type;
    int line_number;
    Expression *expression;
} Statement;

typedef struct FunctionPattern_tag FunctionPattern;
struct FunctionPattern_tag{
    int pattern;
    Statement *statement;
    FunctionPattern *prev;
    FunctionPattern *next;
};

typedef struct FunctionDefinition_tag FunctionDefinition;
struct FunctionDefinition_tag{
    char *name;
    char *descriptor;
    char *parameter_name;
    Statement *statement;
    FunctionPattern *pattern_list;
    FunctionDefinition *prev;
    FunctionDefinition *next;
};

typedef struct{
    // TODO count not in use
    int function_count;
    FunctionDefinition *function_list;
    int current_line_number;
    char *source_file;
    Storage *compile_storage;
}Compiler;


/*
 * compiler.c function prototype
 */
Compiler *get_current_compiler();
void *compiler_storage_malloc(size_t size);
Compiler *create_compiler(char *source_file);
void dispose_compiler(Compiler *compiler);

#endif // COMPILE_H_INCLUDED
