#include "emitter.h"

static FILE *fp;

static void emit_class_file(void *p, size_t size){
    fwrite(p, sizeof(u1), size, fp);
}

static void swap16(u2 *value){
    u2 val = *value;
    *value = (val << 8) | ((val >> 8) & 0xff);
}

static void swap32(u4 *value){
    u4 val = *value;
    *value = (val << 24) | ((val << 8) & 0xff0000) | ((val >> 8) & 0xff00) | ((val >> 24) & 0xff);
}

static void emit_prefixes(ClassFile *cf){
    /* magic number */
    swap32(&(cf->magic));
    /* viersion information */
    swap16(&(cf->minor_version));
    swap16(&(cf->major_version));

    emit_class_file(cf, 8);
}

static void emit_constant_pool(ConstantInfo *ci, u2 *count){
    u2 len;

    /* counstant pool length */
    (*count)++;
    swap16(count);
    emit_class_file(count, 2);

    /* constant pool */
    while(ci){
        emit_class_file(&(ci->tag), 1);
        switch(ci->tag){
            case CONSTANT_Class:
                swap16(&(ci->u.cp_index));
                emit_class_file(&(ci->u.cp_index), 2);
                break;
            case CONSTANT_Fieldref:
            case CONSTANT_Methodref:
            case CONSTANT_InterfaceMethodref:
                swap16(&(ci->u.reference_info.class_index));
                swap16(&(ci->u.reference_info.name_and_type_index));
                emit_class_file(&(ci->u.reference_info), 4)
                break;
            case CONSTANT_NameAndType:
                swap16(&(ci->u.name_and_type_info.name_index));
                swap16(&(ci->u.name_and_type_info.descriptor_index));
                emit_class_file(&(ci->u.name_and_type_info), 4);
                break;
            case CONSTANT_Utf8:
                len = ci->u.utf8_info.length;
                swap16(&(ci->u.utf8_info.length));
                emit_class_file(&(ci->u.utf8_info.length), 2);
                emit_class_file(&(ci->u.utf8_info.value), len);
                break;
            case CONSTANT_String:
            case CONSTANT_Integer:
            case CONSTANT_Float:
            case CONSTANT_Long:
            case CONSTANT_Double:
            default:
                break;
        }
        ci = ci->next;
    }
}

static void emit_middles(ClassFile *cf){
    /* access flags */
    swap16(&(cf->access_flags));
    /* this class information */
    swap16(&(cf->this_class_index));
    /* super class information */
    swap16(&(cf->super_class_index));

    emit_class_file(&(cf->access_flags), 6);
}

static void emit_definitions(Definition *mi, u2 *count){
    // TODO test
    *count = 0;
    swap16(count);
    emit_class_file(count, 2);
    
    // TODO emit methods
}

static void emit_code_attribute(CodeAttribute *ca){
    swap16(&(ca->max_stack));
    swap16(&(ca->max_locals));
    swap32(&(ca->code_length));
    swap16(&(ca->exception_table_length));
    emit_class_file(&(ca->exception_table_length), 2);
    emit_attributes(ca->attributes, &(ca->attributes_count));
}

static void emit_attributes(AttributeInfo *ai, u2 *count){
    u2 len;

    /* attributes length */
    swap16(count);
    emit_class_file(count, 2);

    /* attributes */
    while(ai){
        swap16(&(ai->attribute_name_index));
        swap32(&(ai->attribute_length));
        emit_class_file(&(ai->attribute_name_index), 6);
        switch(ai->tag){
            case ATTRIBUTE_Code:
                emit_code_attribute(ai->u.code_attribute);
                break;
            case CONSTANT_ConstantValue:
            case ATTRIBUTE_SourceFile:
                swap16(&(ai->u.cp_index));
                emit_class_file(&(ai->u.cp_index), 2);
                break;
            default:
                break;
        }
        ai = ai->next;
    }
}

void emit(ClassFile *cf){
    if(!(fp = fopen(cf->emit_file, "wb"))){
        fprintf(stderr, "system error!\n%d: class file \"%s\" could not create\n", 0, cf->emit_file);
        exit(1);
    }

    emit_prefixes(cf);
    emit_constant_pool(cf->constant_pool, &(cf->constant_pool_count));
    emit_middles(cf);
    emit_definitions(cf->interfaces, &(cf->interfaces_count));
    emit_definitions(cf->fields, &(cf->fields_count));
    emit_definitions(cf->methods, &(cf->methods_count));
    emit_attributes(cf->source_file, &(cf->attributes_count));

    fclose(fp);
    printf("generated %s\n", cf->emit_file);
}
