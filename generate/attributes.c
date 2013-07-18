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

static void add_code(CodeAttribute *ca, Opcode op, ...){
    OpcodeInfo *oi;
    Code *c;
    va_list args;
    int i;

    /* add opcode */
    oi = &(opcode_table[op]);
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
    c->tag = CODE_OPCODE;
    c->u.opcode = oi;
    ca->max_stack += oi->max_stack;
    ca->max_locals += oi->max_locals;
    ca->code_length++;

    /* add operand */
    va_start(args, op);
    for(i = 0; i < oi->operand_count; i++){
        c = classfile_storage_malloc(sizeof(Code));
        c->next = NULL;
        c->prev = ca->code->prev;
        c->prev->next = c;
        ca->code->prev = c;

        switch(oi->operands_type[i]){
            case 'b':
                c->tag = CODE_OPERAND_BYTE;
                c->u.operand_byte = va_arg(args, int);
                printf("b: %d\n", c->u.operand_byte);
                ca->code_length++;
                break;
            case 's':
                c->tag = CODE_OPERAND_SHORT;
                c->u.operand_short = va_arg(args, int);
                printf("l: %d\n", c->u.operand_short);
                ca->code_length += 2;
        }
    }
    va_end(args);
}

static void generate_constructor_code(CodeAttribute *ca){
    add_code(ca, ALOAD_0);
    add_code(ca, INVOKESPECIAL, add_constant_method_info("java/lang/Object", "<init>", "()V"));
    add_code(ca, RETURN);
}

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
        add_code(ca, BIPUSH);
    else if(-32768 <= value && value < 32768)
        add_code(ca, SIPUSH);
    else{
        // TODO ldc
    }
}

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

static void generate_minus_expression(CodeAttribute *ca, Expression *me){
    generate_expression_code(ca, me);
    add_code(ca, INEG);
}

static void generate_expression_code(CodeAttribute *ca, Expression *ex){
    switch(ex->kind){
        case INT_EXPRESSION:
            generate_int_expression(ca, ex->u.int_value);
            break;
        case BINARY_EXPRESSION:
            generate_binary_expression(ca, ex->u.binary_expression);
            break;
        case MINUS_EXPRESSION:
            generate_minus_expression(ca, ex->u.minus_expression);
    }
}

static void generate_main_code(CodeAttribute *ca, Expression *ex){
    add_code(ca, GETSTATIC, add_constant_field_info("java/lang/System", "out", "Ljava/io/PrintStream;"));
    generate_expression_code(ca, ex);
    add_code(ca, INVOKEVIRTUAL, add_constant_method_info("java/io/PrintStream", "println", "(I)V"));
    add_code(ca, RETURN);
}

static u4 create_attribute_code(CodeAttribute *ca, Statement *st){
    ca->max_stack = 0;
    ca->max_locals = 1;
    ca->code_length = 0;
    ca->exception_table_length = 0;
    ca->exception_table = NULL;

    switch(st->type){
        case CONSTRUCTOR_STATEMENT:
            generate_constructor_code(ca);
            break;
        case MAIN_STATEMENT:
            generate_main_code(ca, st->u.expression);
            break;
        case EXPRESSION_STATEMENT:
            generate_expression_code(ca, st->u.expression);
            break;
        default:
            break;
    }

    return 12 + ca->code_length;
}

void add_attribute_code(AttributeInfo **ai_list, u2 *list_length, Statement *st){
    AttributeInfo *ai;

    ai = add_attribute_info(ai_list, list_length, ATTRIBUTE_Code);
    ai->attribute_length = create_attribute_code(&(ai->u.code_attribute), st);
}
