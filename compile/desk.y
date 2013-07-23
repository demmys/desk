%{
#include "create.h"
#define YYDEBUG 1

extern int yylex(void);
extern int yyerror(char const *str);
%}
%union{
    char *identifier;
    Expression *expression;
    Statement *statement;
}
/*
 * terminal symbol
 */
%token ADD SUB MUL DIV MOD LP RP BOUND SEMICOLON MAIN
%token <identifier> IDENTIFIER
%token <expression> INT_LITERAL
/*
 * non-terminal symbol
 */
%type <expression> expression add_expression mul_expression
unary_expression call_expression primary_expression
%type <statement> statement
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

/*
 * main or main pattern definition
 */
main_definition
    : MAIN LP IDENTIFIER RP BOUND statement {
        main_define($3, $6);
    }
    | MAIN LP INT_LITERAL RP BOUND statement {
        main_pattern_define($3, $6);
    }
    | MAIN LP RP BOUND statement{
        main_define(NULL, $5);
    }
    | MAIN BOUND statement{
        main_define(NULL, $3);
    };

/*
 * function or function pattern definition
 */
function_definition
    : IDENTIFIER LP IDENTIFIER RP BOUND statement {
        function_define($1, "(I)I", $3, $6);
    }
    | IDENTIFIER LP INT_LITERAL RP BOUND statement {
        function_pattern_define($1, $3, $6);
    }
    | IDENTIFIER LP RP BOUND statement{
        function_define($1, "()I", NULL, $5);
    }
    | IDENTIFIER BOUND statement{
        function_define($1, "()I", NULL, $3);
    };

/*
parameter_list
    : IDENTIFIER {
        $$ = create_parameter($1);
    }
    | parameter_list IDENTIFIER {
        $$ = chain_parameter($1, $2);
    };
*/

/*
 * statement(right-hand side)
 */
statement
    : expression SEMICOLON{
        $$ = create_expression_statement($1);
    };

/*
 * expression
 */
expression
    : add_expression;

add_expression
    : mul_expression
    | add_expression ADD mul_expression {
        $$ = create_binary_expression(ADD_OPERATOR, $1, $3);
    }
    | add_expression SUB mul_expression {
        $$ = create_binary_expression(SUB_OPERATOR, $1, $3);
    };

mul_expression
    : unary_expression
    | mul_expression MUL unary_expression {
        $$ = create_binary_expression(MUL_OPERATOR, $1, $3);
    }
    | mul_expression DIV unary_expression {
        $$ = create_binary_expression(DIV_OPERATOR, $1, $3);
    }
    | mul_expression MOD unary_expression {
        $$ = create_binary_expression(MOD_OPERATOR, $1, $3);
    };

unary_expression
    : call_expression
    | SUB unary_expression{
        $$ = create_minus_expression($2);
    };

call_expression
    : primary_expression
    | IDENTIFIER LP RP {
        $$ = create_call_expression($1, NULL);
    }
    | IDENTIFIER LP expression RP{
        $$ = create_call_expression($1, $3);
    };

primary_expression
    : LP expression RP {
        $$ = $2;
    }
    | INT_LITERAL
    | IDENTIFIER{
        $$ = create_identifier_expression($1);
    };
%%
