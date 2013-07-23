#include "create.h"

/*
 * memory allocate methods
 */
Expression* alloc_expression(ExpressionKind kind){
    Expression *exp;
 
    exp = compiler_storage_malloc(sizeof(Expression));
    exp->kind = kind;
    exp->line_number = get_current_compiler()->current_line_number;
    return exp;
}

static Statement* alloc_statement(){
    Statement *st;

    st = compiler_storage_malloc(sizeof(Statement));
    st->type = NONE_TYPE_STATEMENT;
    st->line_number = get_current_compiler()->current_line_number;
    st->expression = NULL;
    return st;
}

/*
 * define methods
 */
void constructor_define(){
    Compiler *compiler;
    Statement *statement;

    compiler = get_current_compiler();
    statement = alloc_statement();
    function_define("<init>", "()V", NULL, statement);
    statement->type = CONSTRUCTOR_STATEMENT;
}

static FunctionDefinition *search_function(char *identifier){
    Compiler *compiler;
    FunctionDefinition *fd;

    compiler = get_current_compiler();
    for(fd = compiler->function_list; fd; fd = fd->next){
        if(!strcmp(fd->name, identifier)){
            return fd;
        }
    }
    return NULL;
}

static int search_function_pattern(FunctionDefinition *fd, int pattern){
    FunctionPattern *fp;

    for(fp = fd->pattern_list; fp; fp = fp->next){
        if(fp->pattern == pattern)
            return 1;
    }
    return 0;
}

void function_define(char *identifier, char *descriptor, char *parameter, Statement *statement){
    Compiler *compiler;
    FunctionDefinition *fd;

    if(search_function(identifier))
        compile_error(ERROR_FUNCTION_ALREADY_DEFINED, statement->line_number, identifier, parameter);

    compiler = get_current_compiler();
    if(compiler->function_list){
        fd = compiler_storage_malloc(sizeof(FunctionDefinition));
        fd->next = NULL;
        fd->prev = compiler->function_list->prev;
        fd->prev->next = fd;
        compiler->function_list->prev = fd;
    }
    else{
        compiler->function_list = compiler_storage_malloc(sizeof(FunctionDefinition));
        compiler->function_list->next = NULL;
        compiler->function_list->prev = compiler->function_list;
        fd = compiler->function_list;
    }
    (compiler->function_count)++;

    fd->name = identifier;
    fd->descriptor = descriptor;
    fd->parameter_name = parameter;
    statement->type = FUNCTION_STATEMENT;
    fd->statement = statement;
    fd->pattern_list = NULL;
}

void function_pattern_define(char *identifier, Expression *pattern, Statement *statement){
    FunctionDefinition *fd;
    FunctionPattern *fp;
    int pattern_value;

    pattern_value = pattern->u.int_value;
    fd = search_function(identifier);
    if(!fd)
        compile_error(ERROR_FUNCTION_NOT_DEFINED, statement->line_number, identifier, pattern_value);
    if(search_function_pattern(fd, pattern_value))
        compile_error(ERROR_FUNCTION_PATTERN_ALREADY_DEFINED, statement->line_number, identifier, pattern_value);

    if(fd->pattern_list){
        fp = compiler_storage_malloc(sizeof(FunctionPattern));
        fp->next = NULL;
        fp->prev = fd->pattern_list->prev;
        fp->prev->next = fp;
        fd->pattern_list->prev = fp;
    }
    else{
        fd->pattern_list = compiler_storage_malloc(sizeof(FunctionPattern));
        fd->pattern_list->next = NULL;
        fd->pattern_list->prev = fd->pattern_list;
        fp = fd->pattern_list;
    }

    fp->pattern = pattern_value;
    statement->type = FUNCTION_PATTERN_STATEMENT;
    fp->statement = statement;
}

void main_define(char *parameter, Statement *statement){
    function_define("main", "([Ljava/lang/String;)V", parameter, statement);
    statement->type = MAIN_STATEMENT;
}

void main_pattern_define(Expression *pattern, Statement *statement){
    function_pattern_define("main", pattern, statement);
    statement->type = FUNCTION_PATTERN_STATEMENT;
}


/*
 * create methods
 */
Statement *create_expression_statement(Expression *expression){
    Statement *st;

    st = alloc_statement();
    st->expression = expression;
    return st;
}

Expression *create_binary_expression(OperatorKind operator, Expression *left, Expression *right){
    Expression *exp;

    exp = alloc_expression(BINARY_EXPRESSION);
    exp->u.binary_expression.kind = operator;
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

Expression *create_call_expression(char *identifier, Expression *parameter){
    Expression *exp;

    exp = alloc_expression(CALL_EXPRESSION);
    exp->u.call_expression.identifier = identifier;
    exp->u.call_expression.parameter_expression = parameter;
    return exp;
}

Expression *create_identifier_expression(char *identifier){
    Expression *exp;

    exp = alloc_expression(IDENTIFIER_EXPRESSION);
    exp->u.identifier = identifier;
    return exp;
}
