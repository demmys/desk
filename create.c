#include "desk.h"

void main_define(Statement *statement){
    Compiler *compiler;

    compiler = get_current_compiler();
    if(compiler -> main_statement){
        compile_error(statement -> line_number, "function main is already defined");
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

Expression* alloc_expression(ExpressionKind kind){
    Expression *exp;

    exp = compiler_storage_malloc(sizeof(Expression));
    exp -> type = 0;
    exp -> kind = kind;
    exp -> line_number = get_current_compiler() -> current_line_number;
    return exp;
}

Statement* alloc_statement(StatementType type){
    Statement *st;

    st = compiler_storage_malloc(sizeof(Statement));
    st -> type = type;
    st -> line_number = get_current_compiler() -> current_line_number;
    
    return st;
}
