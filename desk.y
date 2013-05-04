%{
#include <stdio.h>
#define YYDEBUG 1
%}
%union{
    char *identifier;
    Expression *expression;
}
%token ADD SUB MUL DIV MOD LP RP BOUND SEMICOLON DEF IMPL MAIN
%token <identifier> IDENTIFIER
%token <identifier> TYPE_IDENTIFIER
%token <expression> INT_LITERAL
%token <expression> FLOAT_LITERAL
%token <expression> CHAR_LITERAL
%%
/*
 * unit
 */
unit
    : definition
    | unit definition;
/*
 * definition
 */
definition
    : main_definition
    | function_definition;

main_definition
    : MAIN BOUND statement;

function_definition
    : IDENTIFIER parameters BOUND statement;

parameters
    : IDENTIFIER
    | parameters IDENTIFIER;

/*
 * type definition
 */
/*
type_definition
    : IDENTIFIER DEF type SEMICOLON;

type
    : TYPE_IDENTIFIER
    | LP type RP
    | type IMPL type;
*/

/*
 * body statement(right-hand side)
 */
statement
    : expression SEMICOLON;

/*
 * expression
 */
expresson
    : add_expression;

add_expression
    : mul_expression
    | add_expression ADD mul_expression
    | add_expression SUB mul_expression;

mul_expression
    : unary_expression
    | mul_expression MUL unary_expression
    | mul_expression DIV unary_expression
    | mul_expression MOD unary_expression;

unary_expression
    : term_expression
    | SUB unary_expression;

term_expression
    : primary_expression

primary_expression
    : LP expression RP
    | IDENTIFIER
    | INT_LITERAL
    | FLOAT_LITERAL
    | CHAR_LITERAL;
%%
