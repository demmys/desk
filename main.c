#include "desk.h"

int main(int argc, char *argv[]){
    extern int yyparse(void);
    extern File *yyin;
    Compiler *compiler;
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

    compiler = create_compiler();
    if(yyparse()){
        fprintf(stderr, "Error!\n");
        exit(1);
    }
    //fix_tree(compiler);
    //generate(compiler);
    dispose_compiler(compiler);

    return 0;
}
