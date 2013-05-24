#include "generate.h"

//static ClassFile *static_current_class_file;

void generate(char *file_name, Compiler *compiler){
    FILE *fp;
    int i;
    ClassFile *cf;
    
    for(i = strlen(file_name) - 1; 0 <= i; i--){
        if(file_name[i] == '.'){
            file_name[i] = '\0';
            break;
        }
    }
    file_name = strcat(file_name, ".class");

    if(!(fp = fopen(file_name, "wb")))
        compile_error(0, "could not create class file.");

    cf = make_class_file(get_current_compiler());
    write_class_file(cf, fp);

    fclose(fp);
    printf("generated %s\n", file_name);
}

void swap16(u2 *value){
    u2 val = *value;
    *value = (val << 8) | ((val >> 8) & 0xff);
}

void swap32(u4 *value){
    u4 val = *value;
    *value = (val << 24) | ((val << 8) & 0xff0000) | ((val >> 8) & 0xff00) | ((val >> 24) & 0xff);
}

void write_class_file(ClassFile *cf, FILE *fp){
    swap32(&(cf->magic));
    swap16(&(cf->minor_version));
    swap16(&(cf->major_version));
    swap16(&(cf->constant_pool_count));

    fwrite(cf, sizeof(unsigned char), 10, fp);
}

ClassFile *make_class_file(Compiler *compiler){
    ClassFile *cf;
    cf = storage_malloc(compiler->compile_storage, sizeof(ClassFile));

    cf->magic = 0xcafebabe;
    cf->minor_version = 0;
    cf->major_version = 50;
    cf->constant_pool_count = 13;
    //constant pool
    cf->constant_pool = storage_malloc(compiler->compile_storage, sizeof(ConstantPoolInfo) * cf->constant_pool_count);
    cf->access_flags = ACC_PUBLIC | ACC_SUPER;
    cf->this_class = 2;
    cf->super_class = 3;
    cf->interfaces_count = 0;
    cf->fields_count = 0;
    cf->methods_count = 1;
    //mehods

    return cf;
}

/*
void set_attribute(ClassFile *cf, ){
    cf->attribute_count = 1;
    cf->attributes = storage_malloc(compiler->compile_storage, sizeof(AttributeInfo) * cf->attribute_count);
    cf->attributes[0].attribute_name_index = 8;
    cf->attributes[0].attribute_length = 2;
    cf->attributes[0].u.cp_index
}
*/
