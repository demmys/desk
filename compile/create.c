#include "create.h"

/*
 * memory allocate methods
 */
Expression* alloc_expression(ExpressionKind kind){
    Expression *exp;
 
    exp = compiler_storage_malloc(sizeof(Expression));
    exp->kind = kind;
    exp->line_number = get_current_compiler()->current_line_number;
    return exp;
}

static Statement* alloc_statement(StatementType type){
    Statement *st;

    st = compiler_storage_malloc(sizeof(Statement));
    st->type = type;
    st->line_number = get_current_compiler()->current_line_number;
    return st;
}

/*
 * define methods
 */
void constructor_define(){
    Compiler *compiler;

    compiler = get_current_compiler();
    compiler->constructor_statement = alloc_statement(CONSTRUCTOR_STATEMENT);
    compiler->constructor_statement->u.constructor = 0;
}

void main_define(char *parameter, Statement *statement){
    Compiler *compiler;

    compiler = get_current_compiler();
    if(compiler->main_statement){
        compile_error(ERROR_MAIN_ALREADY_DEFINED, statement->line_number);
    }
    else{
        statement->type = MAIN_STATEMENT;
        compiler->main_statement = statement;
    }
}

void function_define(char *identifier, char *parameter, Statement *statement){
    printf("function defined: %s, %s\n", identifier, parameter);
    /*
    Compiler *compiler;

    compiler = get_current_compiler();
    if(compiler->main_statement){
        compile_error(ERROR_MAIN_ALREADY_DEFINED, statement->line_number);
    }
    else{
        statement->type = MAIN_STATEMENT;
        compiler->main_statement = statement;
    }
    */
}

void function_pattern_define(char *identifier, Expression *pattern, Statement *statement){
    printf("function pattern defined: %s, %d\n", identifier, pattern->u.int_value);
}

/*
 * create methods
 */
Statement *create_expression_statement(Expression *expression){
    Statement *st;

    st = alloc_statement(EXPRESSION_STATEMENT);
    st->u.expression = expression;
    return st;
}

Expression *create_binary_expression(OperatorKind operator, Expression *left, Expression *right){
    Expression *exp;

    exp = alloc_expression(BINARY_EXPRESSION);
    exp->u.binary_expression.kind = operator;
    exp->u.binary_expression.left = left;
    exp->u.binary_expression.right = right;
    return exp;
}

Expression *create_minus_expression(Expression *operand){
    Expression *exp;

    exp = alloc_expression(MINUS_EXPRESSION);
    exp->u.minus_expression = operand;
    return exp;
}

Expression *create_call_expression(char *identifier, Expression *parameter){
    printf("created call expression: %s\n", identifier);
    return NULL;
}

Expression *create_identifier_expression(char *identifier){
    printf("created identifier expression: %s\n", identifier);
    return NULL;
}
