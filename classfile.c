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
    ClassFile *classfile;

    storage = open_storage(0);
    classfile = storage_malloc(storage, sizeof(ClassFile));
    classfile->magic = 0xcafebabe;
    classfile->minor_version = 0;
    classfile->major_version = 50;
    classfile ->constant_pool_count = 0;
    classfile->constant_pool = NULL;
    classfile->access_flags = 0x21;
    classfile->this_class_index = 0;
    classfile->super_class_index = 0;
    classfile->interfaces_count = 0;
    classfile->fields_count= 0;
    classfile->methods_count = 0;
    classfile->methods = NULL;
    classfile->attributes_count = 1;
    classfile->source_file = NULL;
    classfile->emit_file = NULL;
    classfile->classfile_storage = storage;
    static_current_classfile = classfile;
    return classfile;
}
