#include "classfile.h"

static ClassFile *static_current_classfile;

ClassFile *get_current_classfile(){
    return static_current_classfile;
}

ClassFile *create_class_file(){
    ClassFile *cf;

    cf = malloc(sizeof(ClassFile));
    cf->magic = 0xcafebabe;
    cf->minor_version = 0;
    cf->major_version = 50;
    cf ->constant_pool_count = 0;
    cf->constant_pool = NULL;
    cf->access_flags = ACC_SUPER | ACC_PUBLIC;
    cf->this_class_index = 0;
    cf->super_class_index = 0;
    cf->interfaces_count = 0;
    cf->fields_count= 0;
    cf->methods_count = 0;
    cf->methods = NULL;
    cf->attributes_count = 1;
    cf->source_file = NULL;
    cf->emit_file = NULL;
    static_current_classfile = cf;
    return cf;
}

void dispose_classfile(ClassFile *cf){
    dispose_constant_pool(cf->constant_pool);
    // dispose_definition(cf->methods);
    // dispose_attributes(cf->source_file);
}
