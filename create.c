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
    ConstantInfo *ci;

    compiler = get_current_compiler();
    if(compiler->main_statement){
        compile_error(statement->line_number, "function main is already defined");
    } else{
        compiler->main_statement = statement;
        
        ci = add_constant_info(CONSTANT_Utf8);
        ci->u.utf8_info.length = 4;
        ci->u.utf8_info.value = "main";

        ci = add_constant_info(CONSTANT_Utf8);
        ci->u.utf8_info.length = 22;
        ci->u.utf8_info.value = "([Ljava/lang/String;)V";
    }
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
    return 0;
}

void add_class(char *class_name){
    ConstantInfo *ci;
    int index;

    if(!(index = add_utf8(class_name)))
        index = get_current_compiler()->constant_pool_count;
    ci = add_constant_info(CONSTANT_Class);
    ci->u.cp_index = index;
}

void add_name_and_type(char *name, char *type){
    ConstantInfo *ci;
    int name_index, type_index;

    if(!(name_index = add_utf8(name)))
        name_index = get_current_compiler()->constant_pool_count;
    if(!(type_index = add_utf8(type)))
        type_index = name_index + 1;
    ci = add_constant_info(CONSTANT_NameAndType);
    ci->u.name_and_type_info.name_index = name_index;
    ci->u.name_and_type_info.descriptor_index = type_index;
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
