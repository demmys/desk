#include "desk.h"
#include "y.tab.h"

#define INPUT_EXTENSION ".desk"
#define INPUT_EXTENSION_LEN 5
#define OUTPUT_EXTENSION ".class"
#define OUTPUT_EXTENSION_LEN 6

int format_name(char *file_name){
    int classc = 0;
    char *i;

    // Skip path and get file name only.
    for(i = file_name; *i; i++){
        if(*i == '/')
            file_name = i + 1;
    }
    // Check initial of file name(must be capital letter).
    if(*file_name < 'A' || 'Z' < *file_name){
        fprintf(stderr, "unsupported file name.\nfile name must begin with a capital letter.\n");
        exit(1);
    }
    // Check and remove file extension.
    for(i = file_name; *i; i++){
        if(*i == '.'){
            if(strcmp(i, INPUT_EXTENSION)){
                fprintf(stderr, "unsupported file extension.\nfile extension must be \"%s\".\n", INPUT_EXTENSION);
                exit(1);
            }
            *i = '\0';
            return classc;
        }
        classc++;
    }
    fprintf(stderr, "unsupported file extension.\nfile extension must be \"%s\".\n", INPUT_EXTENSION);
    exit(1);
}

int main(int argc, char *argv[]){
    extern int yyparse(void);
    extern FILE *yyin;
    Compiler *compiler;
    FILE *fp;
    int classc;

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

    classc = format_name(argv[1]);

    compiler = create_compiler();
    if(yyparse()){
        compile_error(get_current_compiler() -> current_line_number, "parse error");
    }
    //generate(classc, argv[1], compiler);
    dispose_compiler(compiler);

    return 0;
}

int yyerror(char const *str){
    return 0;
}
