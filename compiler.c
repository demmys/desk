#include "desk.h"

static Compiler *static_current_compiler;

Compiler *get_current_compiler(){
    return static_current_compiler;
}

void set_current_compiler(Compiler *compiler){
    static_current_compiler = compiler;
}

void *compiler_storage_malloc(size_t size){
    void *p;
    Compiler *compiler;

    compiler = get_current_compiler();
    p = storage_malloc(compiler -> compile_storage, size);

    return p;
}

Compiler *create_compiler(){
    Storage storage;
    Compiler *compiler;
    
    storage = open_storage(0);
    compiler = storage_malloc(storage, sizeof(struct Compiler));
    compiler -> compile_storage = storage;
    compiler -> main_statement = NULL;
    compiler -> current_line_number = 1;
    set_current_compiler(compiler);
    return compiler;
}

void dispose_compiler(Compiler *compiler){
    free(compiler -> main_statement);
    dispose_storage(compiler -> compile_storage);
}
