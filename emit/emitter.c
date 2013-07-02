#include "emitter.h"

void swap16(u2 *value){
    u2 val = *value;
    *value = (val << 8) | ((val >> 8) & 0xff);
}

void swap32(u4 *value){
    u4 val = *value;
    *value = (val << 24) | ((val << 8) & 0xff0000) | ((val >> 8) & 0xff00) | ((val >> 24) & 0xff);
}

void emit_prefixes(FILE *fp){
    ClassFile *cf;

    cf = get_current_classfile();
    swap32(&(cf->magic));
    swap16(&(cf->minor_version));
    swap16(&(cf->major_version));
    cf->constant_pool_count++;
    swap16(&(cf->constant_pool_count));

    fwrite(cf, sizeof(u1), 10, fp);
}

void emit_constant_pool(FILE *fp){
    ClassFile *cf;
    ConstantInfo *ci;
    u2 len;

    cf = get_current_classfile();
    for(ci = cf->constant_pool; ci; ci = ci->next){
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
    }
}

void emit_middles(FILE *fp){
    ClassFile *cf;

    cf = get_current_classfile();
    swap16(&(cf->access_flags));
    swap16(&(cf->this_class_index));
    swap16(&(cf->super_class_index));
    swap16(&(cf->interfaces_count));
    swap16(&(cf->fields_count));
    swap16(&(cf->methods_count));

    fwrite(&(cf->access_flags), sizeof(u1), 12, fp);
}

void emit_attributes(FILE *fp){
    ClassFile *cf;

    cf = get_current_classfile();
    swap16(&(cf->attributes_count));
    swap16(&(cf->source_file->attribute_name_index));
    swap32(&(cf->source_file->attribute_length));
    swap16(&(cf->source_file->u.cp_index));

    fwrite(&(cf->attributes_count), sizeof(u1), 2, fp);
    fwrite(&(cf->source_file->attribute_name_index), sizeof(u1), 2, fp);
    fwrite(&(cf->source_file->attribute_length), sizeof(u1), 4, fp);
    fwrite(&(cf->source_file->u.cp_index), sizeof(u1), 2, fp);
}

void emit(){
    FILE *fp;
    ClassFile *cf;

    cf = get_current_classfile();
    if(!(fp = fopen(cf->emit_file, "wb"))){
        fprintf(stderr, "system error!\n%d: class file \"%s\" could not create\n", 0, cf->emit_file);
        exit(1);
    }

    emit_prefixes(fp);
    emit_constant_pool(fp);
    emit_middles(fp);
    emit_attributes(fp);

    fclose(fp);
    printf("generated %s\n", cf->emit_file);
}
