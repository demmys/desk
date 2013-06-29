#include "classfile.h"

static ClassFile *static_current_classfile;

ClassFile *get_current_classfile(){
    return static_current_classfile;
}

ClassFile *create_class_file(){
    ClassFile *classfile;

    classfile = malloc(sizeof(ClassFile));
    classfile->magic = 0xcafebabe;
    classfile->minor_version = 0;
    classfile->major_version = 50;
    classfile ->constant_pool_count = 0;
    classfile->constant_pool = NULL;
    classfile->access_flags = ACC_SUPER | ACC_PUBLIC;
    classfile->this_class_index = 0;
    classfile->super_class_index = 0;
    classfile->interfaces_count = 0;
    classfile->fields_count= 0;
    classfile->methods_count = 0;
    classfile->methods = NULL;
    classfile->attributes_count = 1;
    classfile->source_file = NULL;
    classfile->emit_file = NULL;
    static_current_classfile = classfile;
    return classfile;
}
