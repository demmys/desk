%{
#include <stdio.h>
#include "y.tab.h"

int yywrap(){
    return 1;
}
%}
%%
<INITIAL>"+" return ADD;
<INITIAL>"-" return SUB;
<INITIAL>"*" return MUL;
<INITIAL>"/" return DIV;
<INITIAL>"%" return MOD;
<INITIAL>"(" return LP;
<INITIAL>")" return RP;
<INITIAL>"=" return BOUND;
<INITIAL>";" return SEMICOLON;
<INITIAL>"::" return DEF;
<INITIAL>"->" return IMPL;
<INITIAL>"main" return MAIN;

<INITIAL>"Int" {
    yylval.identifier = create_type_identifier(yytext);
    return TYPE_IDENTIFIER;
}
<INITIAL>"Float" {
    yylval.identifier = create_type_identifier(yytext);
    return TYPE_IDENTIFIER;
}
<INITIAL>"Char" {
    yylval.identifier = create_type_identifier(yytext);
    return TYPE_IDENTIFIER;
}

<INITIAL>[a-z][A-Za-z_0-9]* {
    yylval.identifier = create_identifier(yytext);
    return IDENTIFIER;
}
<INITIAL>[A-Z][A-Za-z_0-9]* {
    yylval.identifier = create_type_identifier(yytext);
    return TYPE_IDENTIFIER;
}
<INITIAL>[1-9][0-9]* {
    Expression *expression = alloc_expression(INT_EXPRESSION);
    sscanf(yytext, "%d", &expression -> u.int_value);
    yyval.expression = expression;
    return INT_LITERAL;
}
<INITIAL>"0" {
    Expression *expression = alloc_expression(INT_EXPRESSION);
    expression -> u.int_value = 0;
    yyval.expression = expression;
    return INT_LITERAL;
}
<INITIAL>[0-9]+\.[0-9]* {
    Expression *expression = alloc_expression(FLOAT_EXPRESSION);
    sscanf(yytext, "%lf", &expression -> u.float_value);
    yyval.expression = expression;
    return FLOAT_LITERAL;
}
<INITIAL>'.' {
    Expression *expression = alloc_expression(CHAR_EXPRESSION);
    expression -> u.char_value = yytext[1];
    yyval.expression = expression;
    return CHAR_LITERAL;
}

<INITIAL>[ \t] ;
<INITIAL>\n puts("LF");

<INITIAL>"/*" BEGIN R_COMMENT;
<R_COMMENT>. ;
<R_COMMENT>\n /*increment_line_number()*/;
<R_COMMENT>"*/" BEGIN INITIAL;

<INITIAL>"//" BEGIN L_COMMENT;
<L_COMMENT>. ;
<L_COMMENT>\n {
    //increment_line_number();
    BEGIN INITIAL;
}
%%
int main(int argc, char *argv[]){
    FILE *fp;

    // open file
    if(argc < 2){
        fprintf(stderr, "dpfc: no input files\n");
        exit(1);
    }
    if(!(fp = fopen(argv[1], "r"))){
        fprintf(stderr, "dpfc: %s: no such file or directory\n", argv[1]);
        exit(1);
    }
    yyin = fp;

    yylex();
}
