#ifndef COMPILE_H_INCLUDED
#define COMPILE_H_INCLUDED
#include "storage.h"
#include "error.h"

/*
 * enumerated type
 */
/*
typedef enum{
    INT_TYPE,
    FLOAT_TYPE,
    CHAR_TYPE
} BasicType;
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
    /*FLOAT_EXPRESSION,
    CHAR_EXPRESSION,*/
    BINARY_EXPRESSION,
    MINUS_EXPRESSION
} ExpressionKind;

typedef enum{
    MAIN_STATEMENT,
    CONSTRUCTOR_STATEMENT,
    EXPRESSION_STATEMENT,
    IF_STATEMENT
} StatementType;

/*
 * struct declaration
 */
typedef struct Expression_tag Expression;
typedef struct Statement_tag Statement;

typedef struct{
    //int function_count;
    Statement *main_statement; // FunctionList *function_list;
    Statement *constructor_statement;
    int current_line_number;
    //InputMode input_mode;
    //Encoding source_encoding;
    char *source_file;
    Storage *compile_storage;
}Compiler;

typedef struct {
    OperatorKind kind;
    Expression  *left;
    Expression  *right;
} BinaryExpression;
struct Expression_tag{
    //BasicType type; // TypeSpecifier *type;
    ExpressionKind kind;
    int line_number;
    union{
        int int_value;
        /*double float_value;
        char char_value;*/
        BinaryExpression binary_expression;
        Expression *minus_expression;
    } u;
};

struct Statement_tag{
    StatementType type;
    int line_number;
    union{
        int constructor;
        Expression *expression;
    } u;
};


/*
 * compiler.c function prototype
 */
Compiler *get_current_compiler();
void *compiler_storage_malloc(size_t size);
Compiler *create_compiler(char *source_file);
void dispose_compiler(Compiler *compiler);

#endif // COMPILE_H_INCLUDED
