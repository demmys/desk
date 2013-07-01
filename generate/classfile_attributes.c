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

    /* add attributes */
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
