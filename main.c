#include "desk.h"
#include "y.tab.h"

#define INPUT_EXTENSION ".desk"
#define INPUT_EXTENSION_LEN 5
#define OUTPUT_EXTENSION ".class"
#define OUTPUT_EXTENSION_LEN 6

//TODO test
void test_constant_pool();
int format_name(char *file_name);

int main(int argc, char *argv[]){
    extern int yyparse(void);
    extern FILE *yyin;
    Compiler *compiler;
    FILE *fp;
    char *class_name;
    int class_name_length, i;

    // open file
    if(argc < 2){
        fprintf(stderr, "no input files\n");
        exit(1);
    }
    if(!(fp = fopen(argv[1], "r"))){
        fprintf(stderr, "%s: no such file or directory\n", argv[1]);
        exit(1);
    }
    yyin = fp;

    class_name_length = format_name(argv[1]);
    class_name = malloc(sizeof(char *) * (class_name_length + 1));
    for(i = 0; i < class_name_length; i++){
        class_name[i] = argv[1][i];
    }
    class_name[i] = '\0';

    compiler = create_compiler();
    compile_info_define(class_name, "java/lang/Object", argv[1]);
    if(yyparse()){
        compile_error(get_current_compiler() -> current_line_number, "parse error");
    }
    //generate(class_name_length, argv[1], compiler);
    test_constant_pool();
    dispose_compiler(compiler);

    return 0;
}

int yyerror(char const *str){
    return 0;
}

int format_name(char *file_name){
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







// TODO test
void test_constant_pool(){
    Compiler *compiler = get_current_compiler();
    ConstantInfo *ci;
    int i, tag, temp;

    for(i = compiler->constant_pool_count; i > 0; i--){
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
