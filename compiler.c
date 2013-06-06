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
void init_compiler(Compiler *compiler){
    ConstantInfo *ci;

    // initialize constant_pool
    compiler->constant_pool_count = 0;
    compiler->constant_pool = NULL;
    add_utf8("SourceFile");
    add_utf8("Sample.java");
    add_class("Sample");
    add_class("java/lang/Object");
    add_utf8("Code");
    add_utf8("LineNumberTable");
    add_name_and_type("<init>", "()V");
    ci = add_constant_info(CONSTANT_Methodref);
    ci->u.reference_info.class_index = 6;
    ci->u.reference_info.name_and_type_index = 11;

    // initialize others
    compiler->main_statement = NULL;
    compiler->current_line_number = 1;

    // TODO test
    int i, tag, temp;
    for(i = compiler->constant_pool_count; i > 0; i--){
        printf("index: %d\n", i);
        ci = get_constant_info(i);
        tag = ci->tag;
        switch(tag){
            case CONSTANT_Utf8:
                printf("\tUTF-8: %d, %s\n", ci->u.utf8_info.length, ci->u.utf8_info.value);
                break;
            case CONSTANT_Class:
                temp = ci->u.cp_index;
                printf("\tClass: %d // %s\n", temp, get_constant_info(temp)->u.utf8_info.value);
                break;
            case CONSTANT_Methodref:
                temp = ci->u.reference_info.class_index;
                printf("\tMethod: %d // ", temp);
                temp = get_constant_info(temp)->u.cp_index;
                printf("%s\n", get_constant_info(temp)->u.utf8_info.value);
                ci = get_constant_info(ci->u.reference_info.name_and_type_index);
                temp = ci->u.name_and_type_info.name_index;
                printf("\t\t %d // %s\n", temp, get_constant_info(temp)->u.utf8_info.value);
                temp = ci->u.name_and_type_info.descriptor_index;
                printf("\t\t, %d // %s\n", temp, get_constant_info(temp)->u.utf8_info.value);
                break;
            case CONSTANT_NameAndType:
                temp = ci->u.name_and_type_info.name_index;
                printf("\tNameAndType: %d // %s\n", temp, get_constant_info(temp)->u.utf8_info.value);
                temp = ci->u.name_and_type_info.descriptor_index;
                printf("\t\t, %d // %s\n", temp, get_constant_info(temp)->u.utf8_info.value);
                break;
            default:
                printf("\ttag: %d\n", tag);
        }
    }
}

Compiler *create_compiler(){
    Storage *storage;
    Compiler *compiler;

    storage = open_storage(0);
    compiler = storage_malloc(storage, sizeof(Compiler));
    compiler->compile_storage = storage;
    set_current_compiler(compiler);
    init_compiler(compiler);
    return compiler;
}

void dispose_compiler(Compiler *compiler){
    dispose_storage(compiler->compile_storage);
}
