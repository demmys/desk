#include <string.h>
#include "desk.h"

//static ClassFile *static_current_class_file;

void generate(char *file_name, Compiler *compiler){
    FILE *fp;
    int i;
    //unsigned char buf[4] = {0xca, 0xfe,  0xba, 0xbe};
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

    //fwrite(buf, sizeof(unsigned char), 4, fp);
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
    swap32(&(cf -> magic));
    swap16(&(cf -> minor_version));
    swap16(&(cf -> major_version));
    swap16(&(cf -> constant_pool_count));

    fwrite(cf, sizeof(unsigned char), 10, fp);
}

ClassFile *make_class_file(Compiler *compiler){
    ClassFile *cf;

    cf = storage_malloc(compiler -> compile_storage, sizeof(ClassFile));

    cf -> magic = 0xcafebabe;
    cf -> minor_version = 0x0000;
    cf -> major_version = 0x0032;
    cf -> constant_pool_count = 0x000d;
    cf -> constant_pool = storage_malloc(compiler -> compile_storage, sizeof(ConstantPoolInfo) * cf -> constant_pool_count);

    return cf;
}
