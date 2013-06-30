#include "compiler.h"

static Compiler *static_current_compiler;

Compiler *get_current_compiler(){
    return static_current_compiler;
}

void *compiler_storage_malloc(size_t size){
    void *p;
    p = storage_malloc(static_current_compiler->compile_storage, size);
    return p;
}

Compiler *create_compiler(char *source_file){
    Storage *storage;
    Compiler *compiler;

    storage = open_storage(0);
    compiler = storage_malloc(storage, sizeof(Compiler));
    compiler->compile_storage = storage;
    compiler->source_file = source_file;
    compiler->main_statement = NULL;
    compiler->current_line_number = 1;
    static_current_compiler = compiler;
    return compiler;
}

void dispose_compiler(Compiler *compiler){
    dispose_storage(compiler->compile_storage);
}
