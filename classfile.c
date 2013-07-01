#include "classfile.h"

static ClassFile *static_current_classfile;

ClassFile *get_current_classfile(){
    return static_current_classfile;
}

void *classfile_storage_malloc(size_t size){
    void *p;
    p = storage_malloc(static_current_classfile->classfile_storage, size);
    return p;
}

ClassFile *create_class_file(){
    Storage *storage;
    ClassFile *cf;

    storage = open_storage(0);
    cf = storage_malloc(storage, sizeof(ClassFile));
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
    cf->attributes_count = 0;
    cf->source_file = NULL;
    cf->emit_file = NULL;
    cf->classfile_storage = storage;
    static_current_classfile = cf;
    return cf;
}

void dispose_classfile(ClassFile *cf){
    dispose_storage(cf->classfile_storage);
}
