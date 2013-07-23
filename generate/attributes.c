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

static void generate_expression_code(CodeAttribute *ca, Expression *ex);

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

static Code *add_code(CodeAttribute *ca, Opcode op, ...){
    OpcodeInfo *oi;
    Code *operator, *operand;
    va_list args;
    int i;

    /* add opcode */
    oi = &(opcode_table[op]);
    if(ca->code){
        operator = classfile_storage_malloc(sizeof(Code));
        operator->next = NULL;
        operator->prev = ca->code->prev;
        operator->prev->next = operator;
        ca->code->prev = operator;
    } else{
        ca->code = classfile_storage_malloc(sizeof(Code));
        ca->code->next = NULL;
        ca->code->prev = ca->code;
        operator = ca->code;
    }
    operator->tag = CODE_OPERATOR;
    operator->u.opcode = oi;
    ca->max_stack += oi->max_stack;
    ca->max_locals += oi->max_locals;
    ca->code_length++;

    /* add operand */
    va_start(args, op);
    for(i = 0; i < oi->operand_count; i++){
        operand = classfile_storage_malloc(sizeof(Code));
        operand->next = NULL;
        operand->prev = ca->code->prev;
        operand->prev->next = operand;
        ca->code->prev = operand;

        switch(oi->operands_type[i]){
            case 'b':
                operand->tag = CODE_OPERAND_BYTE;
                operand->u.operand_byte = va_arg(args, int);
                ca->code_length++;
                break;
            case 's':
                operand->tag = CODE_OPERAND_SHORT;
                operand->u.operand_short = va_arg(args, int);
                ca->code_length += 2;
        }
    }
    va_end(args);

    return operator;
}

/*
 * constructor code generate method
 */
static void generate_constructor_code(CodeAttribute *ca){
    add_code(ca, ALOAD_0);
    add_code(ca, INVOKESPECIAL, add_constant_method_info("java/lang/Object", "<init>", "()V"));
    add_code(ca, RETURN);
}

/*
 * load int to stack code generate method
 */
static void generate_int_expression(CodeAttribute *ca, int value){
    switch(value){
        case 0:
            add_code(ca, ICONST_0);
            return;
            break;
        case 1:
            add_code(ca, ICONST_1);
            return;
            break;
        case 2:
            add_code(ca, ICONST_2);
            return;
            break;
        case 3:
            add_code(ca, ICONST_3);
            return;
            break;
        case 4:
            add_code(ca, ICONST_4);
            return;
            break;
        case 5:
            add_code(ca, ICONST_5);
            return;
            break;
    }
    if(-128 <= value && value < 128)
        add_code(ca, BIPUSH, value);
    else if(-32768 <= value && value < 32768)
        add_code(ca, SIPUSH, value);
    else{
        // TODO ldc
    }
}

/*
 * operate binary operator code generate method
 */
static void generate_binary_expression(CodeAttribute *ca, BinaryExpression be){
    generate_expression_code(ca, be.left);
    generate_expression_code(ca, be.right);
    switch(be.kind){
        case ADD_OPERATOR:
            add_code(ca, IADD);
            break;
        case SUB_OPERATOR:
            add_code(ca, ISUB);
            break;
        case MUL_OPERATOR:
            add_code(ca, IMUL);
            break;
        case DIV_OPERATOR:
            add_code(ca, IDIV);
            break;
        case MOD_OPERATOR:
            add_code(ca, IREM);
    }
}

/*
 * valiable code generate method
 */
static void generate_identifier_expression(CodeAttribute *ca, Expression *ex){
    if(!(ca->parameter_name) || strcmp(ca->parameter_name, ex->u.identifier))
        compile_error(ERROR_NOT_DEFINED_LOCAL_VALIABLE, ex->line_number, ex->u.identifier);
    add_code(ca, ILOAD_0);
}

/*
 * reverse number sign code generate method
 */
static void generate_minus_expression(CodeAttribute *ca, Expression *me){
    generate_expression_code(ca, me);
    add_code(ca, INEG);
}

/*
 * call function code generate method
 */
static void generate_call_expression(CodeAttribute *ca, CallExpression *ce){
    if(ce->parameter_expression){
        generate_expression_code(ca, ce->parameter_expression);
        add_code(ca, INVOKESTATIC, add_constant_method_info_with_class(ca->this_class_index, ce->identifier, "(I)I"));
    } else{
        add_code(ca, INVOKESTATIC, add_constant_method_info_with_class(ca->this_class_index, ce->identifier, "()I"));
    }
}

/*
 * expression code generate method
 */
static void generate_expression_code(CodeAttribute *ca, Expression *ex){
    switch(ex->kind){
        case INT_EXPRESSION:
            generate_int_expression(ca, ex->u.int_value);
            break;
        case IDENTIFIER_EXPRESSION:
            generate_identifier_expression(ca, ex);
            break;
        case BINARY_EXPRESSION:
            generate_binary_expression(ca, ex->u.binary_expression);
            break;
        case MINUS_EXPRESSION:
            generate_minus_expression(ca, ex->u.minus_expression);
            break;
        case CALL_EXPRESSION:
            generate_call_expression(ca, &(ex->u.call_expression));
    }
}

/*
 * main prefix code generate method
 */
static void generate_main_prefix_code(CodeAttribute *ca){
    add_code(ca, ALOAD_0);
    add_code(ca, ICONST_0);
    add_code(ca, AALOAD);
    add_code(ca, INVOKESTATIC, add_constant_method_info("java/lang/Integer", "parseInt", "(Ljava/lang/String;)I"));
    add_code(ca, ISTORE_0);
}

static void generate_function_pattern_code(CodeAttribute *ca, FunctionPattern *fp){
    Code *jump_operator;
    u2 start_pc;

    add_code(ca, ILOAD_0);
    if(fp->pattern){
        generate_int_expression(ca, fp->pattern);
        start_pc = ca->code_length;
        jump_operator = add_code(ca, IF_ICOMPNE, 0);
    }
    else{
        start_pc = ca->code_length;
        jump_operator = add_code(ca, IFNE, 0);
    }
    generate_expression_code(ca, fp->statement->expression);
    add_code(ca, IRETURN);
    jump_operator->next->u.operand_short = ca->code_length - start_pc;
}

static u4 create_attribute_code(CodeAttribute *ca, Statement *statement, FunctionPattern *pattern_list){
    ca->max_stack = 0;
    ca->max_locals = 1;
    ca->code_length = 0;
    ca->exception_table_length = 0;
    ca->exception_table = NULL;

    /* generate prefix code */
    if(statement->type == MAIN_STATEMENT){
        add_code(ca, GETSTATIC, add_constant_field_info("java/lang/System", "out", "Ljava/io/PrintStream;"));
        if(ca->parameter_name)
            generate_main_prefix_code(ca);
    }

    /* generate function pattern */
    while(pattern_list){
        generate_function_pattern_code(ca, pattern_list);
        pattern_list = pattern_list->next;
    }

    /* generate function */
    switch(statement->type){
        case CONSTRUCTOR_STATEMENT:
            generate_constructor_code(ca);
            break;
        case MAIN_STATEMENT:
            generate_expression_code(ca, statement->expression);
            add_code(ca, INVOKEVIRTUAL, add_constant_method_info("java/io/PrintStream", "println", "(I)V"));
            add_code(ca, RETURN);
            break;
        case FUNCTION_STATEMENT:
            generate_expression_code(ca, statement->expression);
            add_code(ca, IRETURN);
            break;
        default:
            break;
    }

    return 12 + ca->code_length;
}

void add_attribute_code(AttributeInfo **ai_list, u2 *list_length, FunctionDefinition *fd, u2 this_class_index){
    AttributeInfo *ai;
    ai = add_attribute_info(ai_list, list_length, ATTRIBUTE_Code);
    ai->u.code_attribute.this_class_index = this_class_index;
    ai->u.code_attribute.parameter_name = fd->parameter_name;
    ai->attribute_length = create_attribute_code(&(ai->u.code_attribute), fd->statement, fd->pattern_list);
}
