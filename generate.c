#include "generate.h"

void generate(char *class_name, Compiler *compiler){


void generate(char *file_name, Compiler *compiler){
    FILE *fp;
    int i, len;
    ClassFile *cf;
    char *output_name, *extension = ".class";

    len = strlen(file_name);
    output_name = storage_malloc(compiler->compile_storage, len + 7);
    for(i = 0; i < len; i++){
        if(file_name[i] == '.'){
            break;
        }
        output_name[i] = file_name[i];
    }
    while(*extension){
        output_name[i] = *extension;
        extension++;
        i++;
    }

    if(!(fp = fopen(output_name, "wb")))
        compile_error(0, "could not create class file.");
    cf = create_class_file(get_current_compiler(), file_name);
    write_class_file(cf, fp);

    fclose(fp);
    printf("generated %s\n", output_name);
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

ClassFile *create_class_file(Compiler *compiler, char *file_name){
    ClassFile *cf;
    cf = storage_malloc(compiler->compile_storage, sizeof(ClassFile));

    cf->magic = 0xcafebabe;
    cf->minor_version = 0;
    cf->major_version = 50;

    cf->constant_pool_count = 13;
    cf->constant_pool = create_constant_pool(compiler->compile_storage - 1, cf->constant_pool_count);

    cf->access_flags = ACC_PUBLIC | ACC_SUPER;
    cf->this_class = 2;
    cf->super_class = 3;

    cf->interfaces_count = 0;

    cf->fields_count = 0;

    cf->methods_count = 1;
    cf->methods = create_methods(compiler->compile_storage, cf->methods_count);

    AttributeTag tags[1] = {SOURCE_FILE_ATTRIBUTE};
    cf->attributes_count = 1;
    cf->attributes = create_attributes(compiler->compile_storage, cf->attributes_count, tags);

    return cf;
}

//本当はコンパイルしながら動的に作成
ConstantPool *create_constant_pool(Storage *storage, char* file_name, int count, ConstantPoolTag *tags){
    int i;
    ConstantPool *cp = storage_malloc(storage, sizeof(ConstantPool) * count);
    for(i = 0; i < count; i++){
        cp[i].tag = tags[i];
        switch(tags[i]){
            case CONSTANT_Fieldref:
            case CONSTANT_Methodref:
            case CONSTANT_InterfaceMethodref:
                cp[i].u.reference_info = storage_malloc(storage, sizeof(ReferenceInfo));
                //本当は領域確保だけ
                cp[i].u.reference_info->class_index = 3;
                cp[i].u.reference_info->name_and_type_index = 10;
                break;
            case CONSTANT_Long:
            case CONSTANT_Double:
                cp[i].u.long_bytes = storage_malloc(storage, sizeof(LongBytes));
                break;
            case CONSTANT_NameAndType:
                cp[i].u.name_and_type_info = storage_malloc(storage, sizeof(NameAndTypeInfo));
                cp[i].u.name_and_type_info->name_index = 4;
                cp[i].u.name_and_type_info->descriptor_index = 5;
                break;
            case CONSTANT_Utf8:
                cp[i].u.utf8_info = storage_malloc(storage, sizeof(Utf8Info));
                //本当は領域確保だけ
                switch(i){
                    case 3:
                        cp[i].u.utf8_info->length = 6;
                        cp[i].u.utf8_info->bytes = "<init>";
                        break;
                    case 4:
                        cp[i].u.utf8_info->length = 3;
                        cp[i].u.utf8_info->bytes = "()V";
                        break;
                    case 5:
                        cp[i].u.utf8_info->length = 4;
                        cp[i].u.utf8_info->bytes = "Code";
                        break;
                    case 6:
                        cp[i].u.utf8_info->length = 15;
                        cp[i].u.utf8_info->bytes = "LineNumberTable";
                        break;
                    case 7:
                        cp[i].u.utf8_info->length = 10;
                        cp[i].u.utf8_info->bytes = "SourceFile";
                        break;
                    case 8:
                        cp[i].u.utf8_info->length = strlen(file_name);
                        cp[i].u.utf8_info->bytes = file_name;
                        break;
                    case 10:
                        cp[i].u.utf8_info->length = 6;
                        cp[i].u.utf8_info->bytes = "<init>";
                        break;
                    case 11:
                        cp[i].u.utf8_info->length = 16;
                        cp[i].u.utf8_info->bytes = "java/lang/Object";
                        break;
                }
        }
        //本当はここまではやらない
        switch(i){
            case 1:
                cp[i].u.cp_index = 11;
                break;
            case 2:
                cp[i].u.cp_index = 12;
        }
    }
    return cp;
}

//本当は関数の名前などの情報を持った構造体を引数にとる
Definition *create_methods(Storage *storage, int count){
    Definition *methods = storage_malloc(storage, sizeof(Definition) * count);
    int i;
    for(i = 0; i < count; i++){
        methods[i].access_flags = ACC_PUBLIC;
        //本当はconstant poolにAscizで名前と型をを格納して、そのindexを登録する
        methods[i].name_index = 4;
        methods[i].descriptor_index = 5;
        //本当は構造体の情報から必要なattributeを適切に設定
        AttributeTag tags[1] = {CODE_ATTRIBUTE};
        methods[i].attributes_count = 1;
        methods[i].attributes = create_attributes(storage, methods[i].attributes_count, tags);
        //本当はcreate_code_attributeとかで実装
        CodeAttribute *ca = &methods[i].attributes[0].u.code_attribute;
        ca->max_stack = 1;
        ca->max_locals = 1;
        ca->code_length = 5;
        ca->code = storage_malloc(storage, 5);
        u1 code[5] = {0x2a, 0xb7, 0x00, 0x01, 0xb1};
        ca->code = code;
        ca->exception_table_length = 0;
        AttributeTag c_tags[1] = {LINE_NUMBER_TABLE_ATTRIBUTE};
        ca->attributes_count = 1;
        ca->attributes = create_attributes(storage, ca->attributes_count, c_tags);
    }
    return methods;
}

Attribute *create_attributes(Storage *storage, int count, AttributeTag *tags){
    int i;
    Attribute *attributes = storage_malloc(storage, sizeof(Attribute) * count);
    for(i = 0; i < count; i++){
        switch(tags[i]){
            case CONSTANT_VALUE_ATTRIBUTE:
                break;
            case CODE_ATTRIBUTE:
                //本当はconstant poolにAscizで"Code"を格納して、そのindexを登録する
                attributes[i].attribute_name_index = 6;
                break;
            case EXCEPTIONS_ATTRIBUTE:
                break;
            case INNER_CLASSES_ATTRIBUTE:
                break;
            case SYNTHETIC_ATTRIBUTE:
                break;
            case SOURCE_FILE_ATTRIBUTE:
                //本当はconstant poolにAscizで"SourceFile"を格納して、そのindexを登録する
                attributes[i].attribute_name_index = 8;
                //本当は作った後にcpに確保してindexを登録
                attributes[i].u.cp_index = 9;
                break;
            case LINE_NUMBER_TABLE_ATTRIBUTE:
                //本当はconstant poolにAscizで"LineNumberTable"を格納して、そのindexを登録する
                attributes[i].attribute_name_index = 7;
                break;
            case LOCAL_VARIABLE_TABLE_ATTRIBUTE:
                break;
            case DEPRECATED_ATTRIBUTE:
                break;
        }
    }
    return attributes;
}
