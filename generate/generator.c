#include "generator.h"

//TODO test
void test_constant_pool();

static int format_name(char *file_name){
    int classc = 0;
    char *i;

    /* Skip path and get file name only. */
    for(i = file_name; *i; i++){
        if(*i == '/')
            file_name = i + 1;
    }
    /* Check initial of file name(must be capital letter). */
    if(*file_name < 'A' || 'Z' < *file_name)
        compile_error(ERROR_UNSUPPORTED_FILE_NAME);
    /* Check and remove file extension. */
    for(i = file_name; *i; i++){
        if(*i == '.'){
            if(strcmp(i, INPUT_EXTENSION))
                compile_error(ERROR_UNSUPPORTED_FILE_EXTENSION);
            return classc;
        }
        classc++;
    }
    compile_error(ERROR_UNSUPPORTED_FILE_EXTENSION);
    return -1;
}

static void generate_class_information(char *source_file, char *super_class){
    ClassFile *cf;
    char *class_name, *emit_name;
    int class_name_length, i;

    cf = get_current_classfile();
    class_name_length = format_name(source_file);
    class_name = classfile_storage_malloc(sizeof(char *) * (class_name_length + 1));
    emit_name = classfile_storage_malloc(sizeof(char *) * (class_name_length + OUTPUT_EXTENSION_LENGTH + 1));
    for(i = 0; i < class_name_length; i++){
        class_name[i] = source_file[i];
        emit_name[i] = source_file[i];
    }
    for(i = 0; i < OUTPUT_EXTENSION_LENGTH; i++){
        emit_name[class_name_length + i] = OUTPUT_EXTENSION[i];
    }
    class_name[class_name_length] = '\0';
    emit_name[class_name_length + i] = '\0';

    cf->this_class_index = add_constant_class_info(class_name);
    cf->super_class_index = add_constant_class_info(super_class);
    add_attribute_source_file_info(&(cf->source_file), &(cf->attributes_count), source_file);
    cf->emit_file = emit_name;
}

// TODO
static void generate_method(char *name, char *type, Statement *st){
    ClassFile *cf;
    Definition *dn;

    cf = get_current_classfile();
    dn = add_definition(&(cf->methods), &(cf->methods_count), name, type);
    add_attribute_code(&(dn->attributes), &(dn->attributes_count), st);
}

ClassFile *generate(Compiler *compiler){
    ClassFile *cf;

    cf = create_class_file();
    generate_class_information(compiler->source_file, "java/lang/Object");
    generate_method("<init>", "()V", compiler->constructor_statement);
    generate_method("main", "([Ljava/lang/String:)V", compiler->main_statement);
    //add_constant_utf8_info(attribute_name[ATTRIBUTE_LineNumberTable]);

    //TODO test
    //test_constant_pool();

    return cf;
}




// TODO test
void test_constant_pool(){
    ClassFile *cf = get_current_classfile();
    ConstantInfo *ci, *ci_temp;
    int temp, i;

    for(ci = cf->constant_pool, i = 0; ci; ci = ci->next, i++){
        printf("index: %d\n", i + 1);
        switch(ci->tag){
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
                ci_temp = get_constant_info(ci->u.reference_info.name_and_type_index);
                temp = ci_temp->u.name_and_type_info.name_index;
                printf("\t\t %d // %s\n", temp, get_constant_info(temp)->u.utf8_info.value);
                temp = ci_temp->u.name_and_type_info.descriptor_index;
                printf("\t\t, %d // %s\n", temp, get_constant_info(temp)->u.utf8_info.value);
                break;
            case CONSTANT_NameAndType:
                temp = ci->u.name_and_type_info.name_index;
                printf("\tNameAndType: %d // %s\n", temp, get_constant_info(temp)->u.utf8_info.value);
                temp = ci->u.name_and_type_info.descriptor_index;
                printf("\t\t, %d // %s\n", temp, get_constant_info(temp)->u.utf8_info.value);
                break;
            default:
                printf("\ttag: %d\n", ci->tag);
        }
    }
}
