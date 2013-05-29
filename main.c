#include "desk.h"
#include "generate.h"
#include "y.tab.h"

#define INPUT_EXTENSION ".desk"
#define INPUT_EXTENSION_LEN 5
#define OUTPUT_EXTENSION ".class"
#define OUTPUT_EXTENSION_LEN 6

int main(int argc, char *argv[]){
    extern int yyparse(void);
    extern FILE *yyin;
    Compiler *compiler;
    FILE *fp;
    int classc = 0;
    char *i;

    // open file
    if(argc < 2){
        fprintf(stderr, "no input files\n");
        exit(1);
    }
    if(!(fp = fopen(argv[1], "r"))){
        fprintf(stderr, "%s: no such file or directory\n", argv[1]);
        exit(1);
    }
    yyin = fp;
    for(i = argv[1]; *i; i++){
        if(*i == '/')
            argv[1] = i + 1;
    }
    if(*argv[1] < 'A' || 'Z' < *argv[1]){
        fprintf(stderr, "unsupported file name.\nfile name must begin with a capital letter.\n");
        exit(1);
    }
    for(i = argv[1]; *i; i++){
        if(*i == '.'){
            if(strcmp(i, INPUT_EXTENSION)){
                fprintf(stderr, "unsupported file extension.\nfile extension must be \"%s\".\n", INPUT_EXTENSION);
                exit(1);
            }
            *i = '\0';
            break;
        } else if(!*i){
            fprintf(stderr, "unsupported file extension.\nfile extension must be \"%s\".\n", INPUT_EXTENSION);
            exit(1);
        }
        classc++;
    }

    compiler = create_compiler();
    if(yyparse()){
        compile_error(get_current_compiler() -> current_line_number, "parse error");
    }
    //generate(argv[1], compiler);
    dispose_compiler(compiler);

    return 0;
}

int yyerror(char const *str){
    return 0;
}
