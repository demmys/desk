#ifndef DESK_H_INCLUDED
#define DESK_H_INCLUDED
#include <string.h>
#include "compiler.h"

/*
 * create.c function prototype
 */
Expression* alloc_expression(ExpressionKind kind);
void main_define(Statement *statement);
Statement *create_expression_statement(Expression *expression);
Expression *create_binary_expression(ExpressionKind operator, Expression *left, Expression *right);
Expression *create_minus_expression(Expression *operand);

#endif // DESK_H_INCLUDED
