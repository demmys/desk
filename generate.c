#include "generate.h"

static int format_name(char *file_name){
    int classc = 0;
    char *i;

    // Skip path and get file name only.
    for(i = file_name; *i; i++){
        if(*i == '/')
            file_name = i + 1;
    }
    // Check initial of file name(must be capital letter).
    if(*file_name < 'A' || 'Z' < *file_name){
        fprintf(stderr, "unsupported file name.\nfile name must begin with a capital letter.\n");
        exit(1);
    }
    // Check and remove file extension.
    for(i = file_name; *i; i++){
        if(*i == '.'){
            if(strcmp(i, INPUT_EXTENSION)){
                fprintf(stderr, "unsupported file extension.\nfile extension must be \"%s\".\n", INPUT_EXTENSION);
                exit(1);
            }
            return classc;
        }
        classc++;
    }
    fprintf(stderr, "unsupported file extension.\nfile extension must be \"%s\".\n", INPUT_EXTENSION);
    exit(1);
}

static void init_class_file(char *source_file, char *super_class){
    ClassFile *classfile;
    char *class_name, *emit_name;
    int class_name_length, i;

    classfile = get_current_classfile();
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

    classfile->this_class_index = add_class(class_name);
    classfile->super_class_index = add_class(super_class);
    classfile->source_file = set_source_file_attribute(source_file);
    classfile->emit_file = emit_name;
}

ClassFile *generate(Compiler *compiler){
    ClassFile *classfile;

    classfile = create_class_file();
    init_class_file(compiler->source_file, "java/lang/Object");
    //TODO set constructor in specific method(set constructor method method)
    add_utf8("Code");
    add_utf8("LineNumberTable");
    add_methodref("java/lang/Object", "<init>", "()V");
    add_utf8("main");
    add_utf8("([Ljava/lang/String;)V");

    //TODO test
    test_constant_pool();

    return classfile;
}

ConstantInfo *get_constant_info(u2 index){
    ConstantInfo *ci;
    ClassFile *classfile;
    int i;

    i = index;
    classfile = get_current_classfile();
    for(ci = classfile->constant_pool, i--; i > 0; i--){
        ci = ci->next;
        if(!ci){
            fprintf(stderr, "system error!\n%d: orderd constant pool index is too large %d.\n", 0, index);
            exit(1);
        }
    }
    return ci;
}

/*
 * memory allocate methods
 */
static ConstantInfo *alloc_constant_info(ConstantInfoTag tag){
    ConstantInfo *ci;

    ci = classfile_storage_malloc(sizeof(ConstantInfo));
    ci->tag = tag;
    ci->next = NULL;
    return ci;
}
static AttributeInfo *alloc_attribute_info(){
    AttributeInfo *ai;
    ai = classfile_storage_malloc(sizeof(AttributeInfo));
    return ai;
}

/*
 * define methods
 */

static ConstantInfo *add_constant_info(ConstantInfoTag tag){
    ClassFile *classfile;
    ConstantInfo *ci;
    ConstantInfo *ci_tail;

    classfile = get_current_classfile();
    ci = alloc_constant_info(tag);
    if(classfile->constant_pool){
        ci_tail = classfile->constant_pool;
        for(; ci_tail->next; ci_tail = ci_tail->next);
        ci_tail->next = ci;
    } else{
        classfile->constant_pool = ci;
    }
    classfile->constant_pool_count++;
    return ci;
}

u2 add_utf8(char *value){
    ConstantInfo *ci;
    ClassFile *classfile;
    int i;

    classfile = get_current_classfile();
    for(ci = classfile->constant_pool, i = 1; ci; ci = ci->next, i++){
        if(ci->tag == CONSTANT_Utf8 && !strcmp(ci->u.utf8_info.value, value))
            return i;
    }
    ci = add_constant_info(CONSTANT_Utf8);
    ci->u.utf8_info.length = strlen(value);
    ci->u.utf8_info.value = value;
    return get_current_classfile()->constant_pool_count;
}

u2 add_class(char *class_name){
    ClassFile *classfile;
    ConstantInfo *ci;
    int utf8_index, i;

    classfile = get_current_classfile();
    if((utf8_index = add_utf8(class_name)) != classfile->constant_pool_count){
        for(ci = classfile->constant_pool, i = 1; ci; ci = ci->next, i++){
            if(ci->tag == CONSTANT_Class && ci->u.cp_index == utf8_index)
                return i;
        }
    }
    ci = add_constant_info(CONSTANT_Class);
    ci->u.cp_index = utf8_index;
    return classfile->constant_pool_count;
}

u2 add_name_and_type(char *name, char *type){
    ConstantInfo *ci;
    int index;

    ci = add_constant_info(CONSTANT_NameAndType);
    index = get_current_classfile()->constant_pool_count;
    ci->u.name_and_type_info.name_index = add_utf8(name);
    ci->u.name_and_type_info.descriptor_index = add_utf8(type);
    return index;
}

u2 add_methodref(char *class, char *name, char *type){
    ConstantInfo *ci;
    int index;

    ci = add_constant_info(CONSTANT_Methodref);
    index = get_current_classfile()->constant_pool_count;
    ci->u.reference_info.class_index = add_class(class);
    ci->u.reference_info.name_and_type_index = add_name_and_type(name, type);
    return index;
}

AttributeInfo *set_source_file_attribute(char *source_file){
    AttributeInfo *ai;

    ai = alloc_attribute_info();
    ai->attribute_name_index = add_utf8("SourceFile");
    ai->attribute_length = 2;
    ai->u.cp_index = add_utf8(source_file);
    return ai;
}







// TODO test
void test_constant_pool(){
    ClassFile *classfile = get_current_classfile();
    ConstantInfo *ci;
    int i, tag, temp;

    for(i = classfile->constant_pool_count; i > 0; i--){
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
