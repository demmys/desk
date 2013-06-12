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
    classfile->classfile_storage = storage;
    classfile ->constant_pool_count = 0;
    classfile->constant_pool = NULL;
    classfile->this_class_index = 0;
    classfile->super_class_index = 0;
    classfile->source_file = NULL;
    static_current_classfile = classfile;
    return classfile;
}
