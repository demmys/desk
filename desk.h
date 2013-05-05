#include <stdio.h>

/*
 * Compiler
 */
typedef struct {
    MEM_Storage compile_storage;
    Statement *main_statement; // FunctionList *function_list;
    int function_count;
    int current_line_number;
    //InputMode input_mode;
    //Encoding source_encoding;
} Compiler;

/*
 * Expression
 */
typedef struct Expression_tag{
    BasicType type; // TypeSpecifier *type;
    ExpressionKind kind;
    //int line_number;
    union{
        int int_value;
        double float_value;
        char char_value;
        BinaryExpression binary_expression;
        struct Expression_tag *minus_expression;
    } u;
} Expression;

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

typedef enum{
    INT_TYPE,
    FLOAT_TYPE,
    CHAR_TYPE
} BasicType;

// ExpressionKind
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

// BinaryExpression
typedef struct {
    Expression  *left;
    Expression  *right;
} BinaryExpression;

/*
 * Statement
 */
typedef struct{
    StatementType type;
    //int line_number;
    union{
        Expression *expression_s;
    } u;
} Statement;

typedef enum{
    EXPRESSION_STATEMENT = 1,
    STATEMENT_TYPE_COUNT_PLUS_1
} StatementType;

/*
 * function prototype
 */
// TODO
void main_define(Statement *statement){
    Compiler *compiler;

    compiler = get_current_compiler();
    if(compiler -> main_statement){
        compile_error();
    } else{
        compiler -> main_statement = statement;
    }
}

Statement *create_expression_statement(Expression *expression){
    Statement *st;

    st = alloc_statement(EXPRESSION_STATEMENT);
    st -> u.expression_s = expression;

    return st;
}

Expression *create_binary_expression(ExpressionKind operator, Expression *left, Expression *right){
    Expression *exp;
    exp = alloc_expression(operator);
    exp -> u.binary_expression.left = left;
    exp -> u.binary_expression.right = right;
    return exp;
}

Expression *create_minus_expression(Expression *operand){
    Expression *exp;
    exp = alloc_expression(MINUS_EXPRESSION);
    exp -> u.minus_expression = operand;
    return exp;
}

/*
 * storage allocable
 */
Expression* alloc_expression(ExpressionKind kind){
    Expression *exp;
    exp = storage_malloc(sizeof(Expression));
    exp -> type = NULL;
    exp -> kind = kind;
    //exp -> line_number = get_current_compiler() -> current_line_number;
    return exp;
}

static Statement* alloc_statement(StatementType type){
    Statement *st;
    st = storage_malloc(sizeof(Statement));
    st -> type = type;
    //st -> line_number = get_current_compiler() -> current_line_number;
    return st;
}
