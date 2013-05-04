%{
#include <stdio.h>

int yywrap(void){
    return 1;
}
%}
%start L_COMMENT R_COMMENT
%%
<INITIAL>"+" puts("ADD");
<INITIAL>"-" puts("SUB");
<INITIAL>"*" puts("MUL");
<INITIAL>"/" puts("DIV");
<INITIAL>"%" puts("MOD");
<INITIAL>"(" puts("LP");
<INITIAL>")" puts("RP");
<INITIAL>"=" puts("BOUND");
<INITIAL>";" puts("SEMICOLON");
<INITIAL>"::" puts("DEF");
<INITIAL>"->" puts("IMPL");
<INITIAL>"main" puts("MAIN");

<INITIAL>"Int" puts("INT_TYPE");
<INITIAL>"Float" puts("FLOAT_TYPE");
<INITIAL>"Char" puts("CHAR_TYPE");

<INITIAL>[a-z][A-Za-z_0-9]* {
    printf("IDENTIFIER:%s\n", yytext);
}
<INITIAL>[A-Z][A-Za-z_0-9]* {
    printf("TYPE_IDENTIFIER:%s\n", yytext);
}
<INITIAL>[1-9][0-9]* {
    int t;
    sscanf(yytext, "%d", &t);
    printf("INT_LITERAL:%d\n", t);
}
<INITIAL>"0" {
    printf("INT_LITERAL:%d\n", 0);
}
<INITIAL>[0-9]+\.[0-9]* {
    double t;
    sscanf(yytext, "%lf", &t);
    printf("DOUBLE_LITERAL:%lf\n", t);
}
<INITIAL>'.' {
    char t;
    t = yytext[1];
    printf("CHAR_LITERAL:%c\n", t);
}

<INITIAL>[ \t] ;
<INITIAL>\n puts("LF");

<INITIAL>"/*" BEGIN R_COMMENT;
<R_COMMENT>. ;
<R_COMMENT>\n puts("LF");
<R_COMMENT>"*/" BEGIN INITIAL;

<INITIAL>"//" BEGIN L_COMMENT;
<L_COMMENT>. ;
<L_COMMENT>\n {
    puts("LF");
    BEGIN INITIAL;
}

<INITIAL>. {
    fprintf(stderr, "lexical analyze error around %s\n", yytext);
    exit(1);
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

    // run lex
    yylex();

    fclose(fp);
    return 0;
}
