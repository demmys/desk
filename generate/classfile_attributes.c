#include "classfile.h"

char *attribute_name[] = {
    "dummy",
    "ConstantValue",
    "Code",
    "Exceptions",
    "InnerClasses",
    "Synthetic",
    "SourceFile",
    "LineNumberTable",
    "LocalVariableTable",
    "Deprecated"
};

// TODO implement calc attribute length function

static AttributeInfo *add_attribute_info(AttributeInfo **ai_list, u2 *list_length, AttributeTag tag){
    AttributeInfo *ai;

    /* add attribute */
    if(*ai_list){
        ai = classfile_storage_malloc(sizeof(AttributeInfo));
        ai->next = NULL;
        ai->prev = (*ai_list)->prev;
        ai->prev->next = ai;
        (*ai_list)->prev = ai;
    } else{
        *ai_list = classfile_storage_malloc(sizeof(AttributeInfo));
        (*ai_list)->next = NULL;
        (*ai_list)->prev = *ai_list;
        ai = *ai_list;
    }
    (*list_length)++;

    /* set value to new attribute info */
    ai->tag = tag;
    ai->attribute_name_index = add_constant_utf8_info(attribute_name[tag]);
    ai->attribute_length = 0;
    return ai;
}

void add_attribute_source_file_info(AttributeInfo **ai_list, u2 *list_length, char *source_file){
    AttributeInfo *ai;

    ai = add_attribute_info(ai_list, list_length, ATTRIBUTE_SourceFile);
    ai->attribute_length = 2;
    ai->u.cp_index = add_constant_utf8_info(source_file);
}

static void add_(CodeAttribute *ca, Opcode op, ...){
    OpcodeInfo *oi;
    Code *c;
    va_list args;
    int i;

    /* add code */
    oi = &(opcode_table[op]);
    va_start(args, op);
    for(i = 0; i <= oi->operand_count; i++){
        if(ca->code){
            c = classfile_storage_malloc(sizeof(Code));
            c->next = NULL;
            c->prev = ca->code->prev;
            c->prev->next = c;
            ca->code->prev = c;
        } else{
            ca->code = classfile_storage_malloc(sizeof(Code));
            ca->code->next = NULL;
            ca->code->prev = ca->code;
            c = ca->code;
        }
        if(i){
            c->u.operand = va_arg(args, int);
        } else{
            c->u.opcode = oi;
        }
    }
    va_end(args);

    /* set value to new code */
    ca->code_length++;
    ca->code_length += c->opcode->operand_count;
}

static void generate_constructor_code(CodeAttribute *ca){
    u2 method_index;

    add_code(ca, ALOAD_0);
    method_index = add_constant_method_info("java/lang/Object", "<init>", "()V");
    add_code(ca, INVOKESPECIAL, );
    add_code(ca, RETURN);
}

static u4 create_attribute_code(CodeAttribute *ca, Statement *st){
    u4 length;

    length = 12;
    ca->max_stack = 0;
    ca->max_locals = 1;
    ca->code_length = 0;
    ca->exception_table_length = 0;
    ca->exception_table = NULL;

    switch(st->type){
        case CONSTRUCTOR_STATEMENT:
            generate_constructor_code(ca);
            break;
        case EXPRESSION_STATEMENT:
            break;
    }

    return length;
}

void add_attribute_code(AttributeInfo **ai_list, u2 *list_length, Statement *st){
    AttributeInfo *ai;

    ai = add_attribute_info(ai_list, list_length, ATTRIBUTE_Code);
    ai->attribute_length = create_attribute_code(&(ai->u.code_attribute), st);
}
