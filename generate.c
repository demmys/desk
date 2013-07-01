#include "generate.h"

//TODO test
void test_constant_pool();

extern char *attribute_name[];

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

static void init_class_file(char *source_file, char *super_class){
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

ClassFile *generate(Compiler *compiler){
    ClassFile *cf;

    cf = create_class_file();
    init_class_file(compiler->source_file, "java/lang/Object");
    //TODO set constructor in specific method(set constructor method method)
    add_constant_utf8_info(attribute_name[ATTRIBUTE_Code]);
    add_constant_utf8_info(attribute_name[ATTRIBUTE_LineNumberTable]);
    add_constant_method_info("java/lang/Object", "<init>", "()V");
    add_constant_utf8_info("main");
    add_constant_utf8_info("([Ljava/lang/String;)V");

    //TODO test
    //test_constant_pool();

    return cf;
}

/*
ConstantInfo *get_constant_info(u2 index){
    ConstantInfo *ci;
    ClassFile *cf;
    int i;

    i = index;
    cf = get_current_classfile();
    for(ci = cf->constant_pool, i--; i > 0; i--){
        ci = ci->next;
        if(!ci)
            system_error(ERROR_CP_INDEX_TOO_LARGE, index);
    return ci;
}
*/

/*
 * memory allocate methods
 */
/*
static ConstantInfo *alloc_constant_info(ConstantInfoTag tag){
    ConstantInfo *ci;

    ci = classfile_storage_malloc(sizeof(ConstantInfo));
    ci->tag = tag;
    ci->next = NULL;
    return ci;
}

static AttributeInfo *alloc_attribute_info(char *attribute_name){
    AttributeInfo *ai;

    ai = classfile_storage_malloc(sizeof(AttributeInfo));
    ai->attribute_name_index = add_utf8(attribute_name);
    ai->attribute_length = 0;
    ai->next = NULL;
    return ai;
}

static CodeAttribute *alloc_code_attribute(){
    CodeAttribute *ca;

    ca = classfile_storage_malloc(sizeof(CodeAttribute));
    ca->max_stack = 0;
    ca->max_locals = 0;
    ca->code_length = 0;
    ca->exception_table_length = 0;
    return ca;
}

static LineNumberTableAttribute *alloc_line_number_table_attribute(){
    LineNumberTableAttribute *lnta;

    lnta = classfile_storage_malloc(sizeof(LineNumberTableAttribute));
    lnta->line_number_table_length = 0;
    lnta->line_number_table = NULL;
    return lnta;
}

static Definition *alloc_definition(){
    Definition *def;

    def = classfile_storage_malloc(sizeof(Definition));
    def->access_flags = ACC_PUBLIC;
    return def;
}
*/

/*
 * define methods
 */
/*
static ConstantInfo *add_constant_info(ConstantInfoTag tag){
    ClassFile *cf;
    ConstantInfo *ci, *ci_tail;

    cf = get_current_classfile();
    ci = alloc_constant_info(tag);
    if(cf->constant_pool){
        for(ci_tail = cf->constant_pool; ci_tail->next; ci_tail = ci_tail->next);
        ci_tail->next = ci;
    } else{
        cf->constant_pool = ci;
    }
    cf->constant_pool_count++;
    return ci;
}

u2 add_utf8(char *value){
    ConstantInfo *ci;
    ClassFile *cf;
    int i;

    cf = get_current_classfile();
    for(ci = cf->constant_pool, i = 1; ci; ci = ci->next, i++){
        if(ci->tag == CONSTANT_Utf8 && !strcmp(ci->u.utf8_info.value, value))
            return i;
    }
    ci = add_constant_info(CONSTANT_Utf8);
    ci->u.utf8_info.length = strlen(value);
    ci->u.utf8_info.value = value;
    return get_current_classfile()->constant_pool_count;
}

u2 add_class(char *class_name){
    ClassFile *cf;
    ConstantInfo *ci;
    int utf8_index, i;

    cf = get_current_classfile();
    if((utf8_index = add_utf8(class_name)) != cf->constant_pool_count){
        for(ci = cf->constant_pool, i = 1; ci; ci = ci->next, i++){
            if(ci->tag == CONSTANT_Class && ci->u.cp_index == utf8_index)
                return i;
        }
    }
    ci = add_constant_info(CONSTANT_Class);
    ci->u.cp_index = utf8_index;
    return cf->constant_pool_count;
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

void set_source_file_attribute(char *source_file){
    ClassFile *cf;
    AttributeInfo *ai;

    cf = get_current_classfile();
    ai->attribute_length = 2;
}
*/

/*
AttributeInfo *add_attribute_info(AttributeInfo **attributes_pointer, u2 *attributes_count_pointer, char *attribute_name){
    AttributeInfo *ai, *ai_tail;

    ai = alloc_attribute_info(attribute_name);
    if(*attributes_pointer){
        for(ai_tail = *attributes_pointer; ai_tail->next; ai_tail = ai_tail->next);
        ai_tail->next = ai;
    } else{
        *attributes_pointer = ai;
    }
    (*attributes_count_pointer)++;
    return ai;
}

void add_line_number_table_attribute(CodeAttribute *ca, Statement *statement){
    AttributeInfo *ai;
    LineNumberTableAttribute *lnta;

    lnta = alloc_line_number_table_attribute();
    // TODO set lnta->line_number_table
    // TODO set lnta->line_number_table_length

    ai = add_attribute_info(&(ca->attributes), &(ca->attributes_count), "LineNumberTable");
    ai->attribute_length += lnta->line_number_table_length * 2 + 2;
    ai->u.line_number_table_attribute = *lnta;
}

void add_code_attribute(Definition *method, Statement *statement){
    AttributeInfo *ai;
    CodeAttribute *ca;

    ca = alloc_code_attribute();
    // code.c & code.h
    // TODO set ca->code
    // TODO set ca->colde_length
    // TODO set ca->max_locals
    // TODO set ca->max_stack
    // TODO code.cでLineNumberTable作っちゃう。
    add_line_number_table_attribute(ca, statement);

    ai = add_attribute_info(&(method->attributes), &(method->attributes_count), "Code");
    ai->u.code_attribute = *ca;
    // TODO add child attribute length (maybe in child attribute maker function)
    ai->attribute_length += 12 + ca->code_length;
}

Definition *set_constructor(){
    Definition *def;
    Statement *statement;

    def = alloc_definition();
    def->name_index = add_utf8("<init>");
    def->descriptor_index = add_utf8("()V");
    add_code_attribute(def, statement);
    return def;
}

*/






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
