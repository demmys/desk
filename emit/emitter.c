#include "emitter.h"

static void swap16(u2 *value){
    u2 val = *value;
    *value = (val << 8) | ((val >> 8) & 0xff);
}

static void swap32(u4 *value){
    u4 val = *value;
    *value = (val << 24) | ((val << 8) & 0xff0000) | ((val >> 8) & 0xff00) | ((val >> 24) & 0xff);
}

static void emit_prefixes(ClassFile *cf, FILE *fp){
    /* magic number */
    swap32(&(cf->magic));
    /* viersion information */
    swap16(&(cf->minor_version));
    swap16(&(cf->major_version));

    fwrite(cf, sizeof(u1), 8, fp);
}

static void emit_constant_pool(ConstantInfo *ci, u2 *count, FILE *fp){
    u2 len;

    /* counstant pool length */
    (*count)++;
    swap16(count);
    fwrite(count, sizeof(u1), 2, fp);

    /* constant pool */
    while(ci){
        u1 tag = ci->tag;
        fwrite(&tag, sizeof(u1), 1, fp);
        switch(ci->tag){
            case CONSTANT_Class:
                swap16(&(ci->u.cp_index));
                fwrite(&(ci->u.cp_index), sizeof(u1), 2, fp);
                break;
            case CONSTANT_Fieldref:
            case CONSTANT_Methodref:
            case CONSTANT_InterfaceMethodref:
                swap16(&(ci->u.reference_info.class_index));
                swap16(&(ci->u.reference_info.name_and_type_index));
                fwrite(&(ci->u.reference_info), sizeof(u1), 4, fp);
                break;
            case CONSTANT_NameAndType:
                swap16(&(ci->u.name_and_type_info.name_index));
                swap16(&(ci->u.name_and_type_info.descriptor_index));
                fwrite(&(ci->u.name_and_type_info), sizeof(u1), 4, fp);
                break;
            case CONSTANT_Utf8:
                len = ci->u.utf8_info.length;
                swap16(&(ci->u.utf8_info.length));
                fwrite(&(ci->u.utf8_info.length), sizeof(u1), 2, fp);
                fwrite(ci->u.utf8_info.value, sizeof(u1), len, fp);
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

static void emit_middles(ClassFile *cf, FILE *fp){
    /* access flags */
    swap16(&(cf->access_flags));
    /* this class information */
    swap16(&(cf->this_class_index));
    /* super class information */
    swap16(&(cf->super_class_index));

    fwrite(&(cf->access_flags), sizeof(u1), 6, fp);
}

static void emit_unused_definitions(u2 *count, FILE *fp){
    swap16(count);
    fwrite(count, sizeof(u1), 2, fp);
}

static void emit_methods(Definition *mi, u2 *count, FILE *fp){
    // TODO test
    *count = 0;
    swap16(count);
    fwrite(count, sizeof(u1), 2, fp);
    
    // TODO emit methods
}

static void emit_attributes(AttributeInfo *ai, u2 *count, FILE *fp){
    /* attributes length */
    swap16(count);
    fwrite(count, sizeof(u1), 2, fp);

    /* attributes */
    // TODO emit all attributes
    swap16(&(ai->attribute_name_index));
    swap32(&(ai->attribute_length));
    swap16(&(ai->u.cp_index));
    fwrite(&(ai->attribute_name_index), sizeof(u1), 2, fp);
    fwrite(&(ai->attribute_length), sizeof(u1), 4, fp);
    fwrite(&(ai->u.cp_index), sizeof(u1), 2, fp);
}

void emit(ClassFile *cf){
    FILE *fp;

    if(!(fp = fopen(cf->emit_file, "wb"))){
        fprintf(stderr, "system error!\n%d: class file \"%s\" could not create\n", 0, cf->emit_file);
        exit(1);
    }

    emit_prefixes(cf, fp);
    emit_constant_pool(cf->constant_pool, &(cf->constant_pool_count), fp);
    emit_middles(cf, fp);
    emit_unused_definitions(&(cf->interfaces_count), fp);
    emit_unused_definitions(&(cf->fields_count), fp);
    emit_methods(cf->methods, &(cf->methods_count), fp);
    emit_attributes(cf->source_file, &(cf->attributes_count), fp);

    fclose(fp);
    printf("generated %s\n", cf->emit_file);
}
