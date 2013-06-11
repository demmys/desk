#include "desk.h"

/*
 * memory allocate methods
 */
ConstantInfo *alloc_constant_info(ConstantInfoTag tag){
    ConstantInfo *ci;

    ci = compiler_storage_malloc(sizeof(ConstantInfo));
    ci->tag = tag;
    ci->next = NULL;
    return ci;
}
AttributeInfo *alloc_attribute_info(){
    AttributeInfo *ai;
    ai = compiler_storage_malloc(sizeof(AttributeInfo));
    return ai;
}
Expression* alloc_expression(ExpressionKind kind){
    Expression *exp;
 
    exp = compiler_storage_malloc(sizeof(Expression));
    exp->type = 0;
    exp->kind = kind;
    exp->line_number = get_current_compiler()->current_line_number;
    return exp;
}
Statement* alloc_statement(StatementType type){
    Statement *st;

    st = compiler_storage_malloc(sizeof(Statement));
    st->type = type;
    st->line_number = get_current_compiler()->current_line_number;
    return st;
}

/*
 * define methods
 */
void main_define(Statement *statement){
    Compiler *compiler;

    compiler = get_current_compiler();
    if(compiler->main_statement){
        compile_error(statement->line_number, "function main is already defined");
    } else{
        compiler->main_statement = statement;
        add_utf8("main");
        add_utf8("([Ljava/lang/String;)V");
    }
}

void compile_info_define(char *this_class, char *super_class, char *source_file){
    Compiler *compiler;
    compiler = get_current_compiler();
    compiler->this_class_index = add_class(this_class);
    compiler->super_class_index = add_class(super_class);
    compiler->source_file = set_source_file_attribute(source_file);
}

ConstantInfo *add_constant_info(ConstantInfoTag tag){
    Compiler *compiler;
    ConstantInfo *ci;
    ConstantInfo *ci_tail;

    compiler = get_current_compiler();
    ci = alloc_constant_info(tag);
    if(compiler->constant_pool){
        ci_tail = compiler->constant_pool;
        for(; ci_tail->next; ci_tail = ci_tail->next);
        ci_tail->next = ci;
    } else{
        compiler->constant_pool = ci;
    }
    compiler->constant_pool_count++;
    return ci;
}

int add_utf8(char *value){
    ConstantInfo *ci;
    Compiler *compiler;
    int i;

    compiler = get_current_compiler();
    for(ci = compiler->constant_pool, i = 1; ci; ci = ci->next, i++){
        if(ci->tag == CONSTANT_Utf8 && !strcmp(ci->u.utf8_info.value, value))
            return i;
    }
    ci = add_constant_info(CONSTANT_Utf8);
    ci->u.utf8_info.length = strlen(value);
    ci->u.utf8_info.value = value;
    return get_current_compiler()->constant_pool_count;
}

int add_class(char *class_name){
    Compiler *compiler;
    ConstantInfo *ci;
    int utf8_index, i;

    compiler = get_current_compiler();
    if((utf8_index = add_utf8(class_name)) != compiler->constant_pool_count){
        for(ci = compiler->constant_pool, i = 1; ci; ci = ci->next, i++){
            if(ci->tag == CONSTANT_Class && ci->u.cp_index == utf8_index)
                return i;
        }
    }
    ci = add_constant_info(CONSTANT_Class);
    ci->u.cp_index = utf8_index;
    return compiler->constant_pool_count;
}

int add_name_and_type(char *name, char *type){
    ConstantInfo *ci;
    int index;

    ci = add_constant_info(CONSTANT_NameAndType);
    index = get_current_compiler()->constant_pool_count;
    ci->u.name_and_type_info.name_index = add_utf8(name);
    ci->u.name_and_type_info.descriptor_index = add_utf8(type);
    return index;
}

int add_methodref(char *class, char *name, char *type){
    ConstantInfo *ci;
    int index;

    ci = add_constant_info(CONSTANT_Methodref);
    index = get_current_compiler()->constant_pool_count;
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

/*
 * create methods
 */
Statement *create_expression_statement(Expression *expression){
    Statement *st;

    st = alloc_statement(EXPRESSION_STATEMENT);
    st->u.expression_s = expression;
    return st;
}
Expression *create_binary_expression(ExpressionKind operator, Expression *left, Expression *right){
    Expression *exp;

    exp = alloc_expression(operator);
    exp->u.binary_expression.left = left;
    exp->u.binary_expression.right = right;
    return exp;
}
Expression *create_minus_expression(Expression *operand){
    Expression *exp;

    exp = alloc_expression(MINUS_EXPRESSION);
    exp->u.minus_expression = operand;
    return exp;
}
