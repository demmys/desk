#include "desk.h"

/*
 * Using static valiable compiler methods
 */
static Compiler *static_current_compiler;

Compiler *get_current_compiler(){
    return static_current_compiler;
}

void set_current_compiler(Compiler *compiler){
    static_current_compiler = compiler;
}

/*
 * Methods work with compiler
 */
void compile_error(int line_number, char *message){
    fprintf(stderr, "compile error!\n%d: %s\n", line_number, message);
    exit(1);
}

void *compiler_storage_malloc(size_t size){
    void *p;
    Compiler *compiler;

    compiler = get_current_compiler();
    p = storage_malloc(compiler->compile_storage, size);

    return p;
}

ConstantInfo *get_constant_info(int index){
    ConstantInfo *ci;
    Compiler *compiler;
    int i;

    i = index;
    compiler = get_current_compiler();
    for(ci = compiler->constant_pool, i--; i > 0; i--){
        ci = ci->next;
        if(!ci){
            fprintf(stderr, "system error!\n%d: orderd constant pool index is too large %d.\n", compiler->current_line_number, index);
            exit(1);
        }
    }
    return ci;
}

/*
 * Methods for controle compiler itself
 */
void init_compiler(){
    Compiler *compiler;

    compiler = get_current_compiler();
    compiler->constant_pool_count = 0;
    compiler->constant_pool = NULL;
    //TODO set constructor in specific method(set constructor method method)
    add_utf8("Code");
    add_utf8("LineNumberTable");
    add_methodref("java/lang/Object", "<init>", "()V");
    compiler->main_statement = NULL;
    compiler->current_line_number = 1;
}

Compiler *create_compiler(){
    Storage *storage;
    Compiler *compiler;

    storage = open_storage(0);
    compiler = storage_malloc(storage, sizeof(Compiler));
    compiler->compile_storage = storage;
    set_current_compiler(compiler);
    init_compiler();
    return compiler;
}

void dispose_compiler(Compiler *compiler){
    dispose_storage(compiler->compile_storage);
}
