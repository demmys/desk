#include "classfile_attributes.h"

char *attribute_name[] = {
    {"dummy"},
    {"ConstantValue"},
    {"Code"},
    {"Exceptions"},
    {"InnerClasses"},
    {"Synthetic"},
    {"SourceFile"},
    {"LineNumberTable"},
    {"LocalVariableTable"},
    {"Deprecated"}
};

// TODO implement calc attribute length function

AttributeInfo *add_attribute_info(AttributeInfo *ais, int *count, AttributeTag tag, ...){
    AttributeInfo *ai;
    CodeAttribute *ca;
    ExceptionsAttribute *ea;
    InnerClassesAttribute *ica;
    LineNumberTableAttribute *lnta;
    LocalVariableTableAttribute *lvta;
    va_list args;

    /* realloc attributes */
    ais = realloc(ais, sizeof(AttributeInfo) * (*count + 1));
    ai = &(ats[(*count)++]);

    /* set value to new attribute info */
    ai->tag = tag;
    ai->attribute_name_index = add_constant_info(CONSTANT_Utf8, attribute_name[tag]);
    ai->attribute_length = 0;

    /* initialize new attribute info core value */
    va_start(args, tag);
    switch(tag){
        case ATTRIBUTE_ConstantValue:
        case ATTRIBUTE_SourceFile:
            ai->u.cp_index = add_constant_info(CONSTANT_Utf8, va_arg(arg, char *));
            ai->attribute_length = 2;
            break;
        case ATTRIBUTE_Code:
            ca = &(ai->u.code_attribute);
            // TODO create_code_attribute(CodeAttribute *ca, Statement *sm)
            ca.max_stack = 0;
            ca.max_locals = 0;
            ca.code_length = 0;
            ca.code = NULL;
            ca.exception_table_length = 0;
            ca.exception_table = NULL;
            ca.attributes_count = 0;
            ca.attributes = NULL;
            // TODO add_attribute_info(ca->attributes, ca->attributes_count, ATTRIBUTE_LineNumberTable, LineNumberTableAttribute *lnta);
            break;
        case ATTRIBUTE_Exceptions:
            ea = &(ai->u.excepsions_attribute);
            ea->start_pc = va_arg(args, u2);
            ea->end_pc = va_arg(args, u2);
            ea->handler_pc = va_arg(args, u2);
            ea->catch_type = va_arg(args, u2);
            break;
        case ATTRIBUTE_InnerClasses:
            ica = &(ai->u.inner_classes_attribute);
            ica->number_of_classes = 0;
            ica->classes = NULL;
            break;
        case ATTRIBUTE_Synthetic:
        case ATTRIBUTE_Deprecated:
            ai->u.dummy = 0;
            break;
        case ATTRIBUTE_LineNumberTable:
            lnta = &(ai->u.line_number_table);
            lnta->line_number_table_length = 0;
            lnta->line_number_table = NULL;
            break;
        case ATTRIBUTE_LocalVariableTable:
            lvta = &(ai->u.local_variable_table);
            lvta->local_variable_table_length = 0;
            lvta->local_variable_table = NULL;
    }
    va_end(args);
    return ai;
}
