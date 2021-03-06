#include "emitter.h"
#include "y.tab.h"

int main(int argc, char *argv[]){
    extern int yyparse(void);
    extern FILE *yyin;
    Compiler *compiler;
    ClassFile *classfile;
    FILE *fp;

    /* open file */
    if(argc < 2)
        compile_error(ERROR_NO_INPUT_FILES);
    if(!(fp = fopen(argv[1], "r")))
        compile_error(ERROR_NO_SUCH_FILE, argv[1]);
    yyin = fp;

    /* compile */
    compiler = create_compiler(argv[1]);
    constructor_define();
    if(yyparse()){
        compile_error(ERROR_PARSE, get_current_compiler()->current_line_number);
    }
    /* generate */
    classfile = generate(compiler);
    /* emit */
    emit(classfile);
    dispose_compiler(compiler);
    dispose_classfile(classfile);
    return 0;
}

int yyerror(char const *str){
    return 0;
}
