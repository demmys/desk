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
%token ADD SUB MUL DIV MOD LP RP BOUND SEMICOLON DEF IMPL MAIN
%token <identifier> IDENTIFIER TYPE_IDENTIFIER
%token <expression> INT_LITERAL FLOAT_LITERAL CHAR_LITERAL
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
    : main_definition;
    /* | function_definition; */

main_definition
    : MAIN BOUND statement {
        main_define($3);
    };

/*
function_definition
    : IDENTIFIER parameters BOUND statement {
        function_define($1, $2, $3);
    };

parameter_list
    : IDENTIFIER {
        $$ = create_parameter($1);
    }
    | parameter_list IDENTIFIER {
        $$ = chain_parameter($1, $2);
    };
*/

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
        $$ = create_binary_expression(ADD_EXPRESSION, $1, $3);
    }
    | add_expression SUB mul_expression {
        $$ = create_binary_expression(SUB_EXPRESSION, $1, $3);
    };

mul_expression
    : unary_expression
    | mul_expression MUL unary_expression {
        $$ = create_binary_expression(MUL_EXPRESSION, $1, $3);
    }
    | mul_expression DIV unary_expression {
        $$ = create_binary_expression(DIV_EXPRESSION, $1, $3);
    }
    | mul_expression MOD unary_expression {
        $$ = create_binary_expression(MOD_EXPRESSION, $1, $3);
    };

unary_expression
    : call_expression
    | SUB unary_expression{
        $$ = create_minus_expression($2);
    };

call_expression
    : primary_expression;

primary_expression
    : LP expression RP {
        $$ = $2;
    }
    | INT_LITERAL
    | FLOAT_LITERAL
    | CHAR_LITERAL;
%%
