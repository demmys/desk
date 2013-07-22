#ifndef DESK_H_INCLUDED
#define DESK_H_INCLUDED
#include <string.h>
#include "compiler.h"

/*
 * create.c function prototype
 */
Expression* alloc_expression(ExpressionKind kind);
void constructor_define();
void function_define(char *identifier, char *parameter, Statement *statement);
void function_pattern_define(char *identifier, Expression *pattern, Statement *statement);
void main_define(char *parameter, Statement *statement);
void main_pattern_define(Expression *pattern, Statement *statement);
Statement *create_expression_statement(Expression *expression);
Expression *create_binary_expression(OperatorKind operator, Expression *left, Expression *right);
Expression *create_minus_expression(Expression *operand);
Expression *create_call_expression(char *identifier, Expression *parameter);
Expression *create_identifier_expression(char *identifier);

#endif // DESK_H_INCLUDED
