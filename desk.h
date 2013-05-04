#include <stdio.h>

/*
 * Expression
 */
typedef struct Expression_tag{
    TypeSpecifier *type;
    ExpressionKind kind;
    int line_number;
    union{
        int int_value;
        double float_value;
        char char_value;
        BinaryExpression binary_expression;
        struct Expression_tag *minus_expression;
    } u;
} Expression;

// TypeSpecifier
typedef struct{
    BasicType basic_type;
    TypeDerive *derive;
} TypeSpecifier;

typedef enum{
    INT_TYPE,
    FLOAT_TYPE,
    CHAR_TYPE
} BasicType;

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
    int line_number;
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

