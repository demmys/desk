#include "classfile_constant_pool.h.h"

u2 search_constant_info(ConstantInfoTag tag, ...){
    ClassFile *cf;
    ConstantInfo ci;
    u2 i;

    cf = get_current_classfile();
    va_start(args, tag);
    for(i = 0; i < cf->constant_pool_count; i++){
        ci = cf->constant_pool[i];
        if(ci.tag == tag){
            switch(tag){
                case CONSTANT_Utf8:
                    if(ci.u.utf8_info.length == ci.u.length)
                        if(!strcmp(ci.u.utf8_info.value, va_arg(args, char *)))
                            return ++i;
                case CONSTANT_Class:
                case CONSTANT_String:
                    if(ci.u.cp_index == va_arg(arg, u2))
                        return ++i;
                case CONSTANT_Integer:
                case CONSTANT_Float:
                    if(ci.u.bytes == va_arg(args, u4))
                        return ++i;
                case CONSTANT_Long:
                case CONSTANT_Double:
                    if(ci.u.long_bytes.low_bytes == va_arg(args, u4))
                        if(ci.u.long_bytes.high_bytes == va_arg(args, u4))
                            return ++i;
                case CONSTANT_Fieldref:
                case CONSTANT_Methodref:
                case CONSTANT_InterfaceMethodref:
                    if(ci.u.reference_info.class_index == va_arg(args, u2))
                        if(ci.u.reference_info.name_and_type_index == va_arg(args, u2))
                            return ++i;
                case CONSTANT_NameAndType:
                    if(ci->u.name_and_type_info.name_index == va_arg(args, u2))
                        if(ci.u.name_and_type_index.descriptor_index == va_arg(args, u2))
                                return ++i;
                default:
                    fprintf(stderr, "system error!\n%d: unknown constan tag number %d.\n", 0, tag);
                    exit(1);
            }
        }
    }
    va_end(args);
    return 0;
}

u2 add_constant_info(ConstantInfoTag tag, ...){
    ClassFile *cf;
    ConstantInfo *ci;
    va_list args;
    u2 index;

    cf = get_current_classfile();
    va_start(args, tag);
    /* if adding constant exists, return the index */
    index = search_constant_info(tag, args);
    if(index > 0)
        return index;

    /* realloc constant pool */
    cf->constant_pool = realloc(cf->constant_pool, sizeof(ConstantInfo) * (cf->constant_pool_count + 1));
    ci = &(cf->constant_pool[(cf->constant_pool_count)++]);
    ci->tag = tag;

    /* set value to new constant info */
    switch(tag){
        case CONSTANT_Utf8:
            ci->u.utf8_info.value = va_arg(args, char *);
            ci->u.utf8_info.length = strlen(ci->u.utf8_info.value);
            break;
        case CONSTANT_Class:
        case CONSTANT_String:
            ci->u.cp_index = va_arg(arg, u2);
            break;
        case CONSTANT_Integer:
        case CONSTANT_Float:
            ci->u.bytes = va_arg(args, u4);
            break;
        case CONSTANT_Long:
        case CONSTANT_Double:
            ci->u.long_bytes.high_bytes = va_arg(arg, u4);
            ci->u.long_bytes.low_bytes = va_arg(arg, u4);
            break;
        case CONSTANT_Fieldref:
        case CONSTANT_Methodref:
        case CONSTANT_InterfaceMethodref:
            ci->u.reference_info.class_index = va_arg(arg, u2);
            ci->u.reference_info.name_and_type_index = va_arg(arg, u2);
            break;
        case CONSTANT_NameAndType:
            ci->u.name_and_type_info.name_index = va_arg(arg, u2);
            ci->u.name_and_type_info.descriptor_index = va_arg(arg, u2);
    }
    va_end(args);
    return cf->constant_pool_count;
}

void dispose_constant_pool(ConstantInfo *ci){
    free(ci);
}



/*
 * search methods
 */
/*
u2 search_constant_utf8(char *value){
    ClassFile *cf;
    ConstantInfo ci;
    u2 i;

    cf = get_current_classfile();
    for(i = 0; i < cf->constant_pool_count; i++){
        ci = cf->constant_pool[i];
        if(ci.tag == CONSTANT_Utf8)
            if(!strcmp(ci.u.utf8_info.value, value))
                return ++i;
    }
    return 0;
}

static u2 search_constant_class_and_string(ConstantInfoTag tag, u2 cp_index){
    ClassFile *cf;
    ConstantInfo ci;
    u2 i;

    cf = get_current_classfile();
    for(i = 0; i < cf->constant_pool_count; i++){
        ci = cf->constant_pool[i];
        if(ci.tag == tag)
            if(ci.u.cp_index == cp_index)
                return ++i;
    }
    return 0;
}

u2 search_constant_class(u2 cp_index){
    return search_constant_class_and_string(CONSTANT_Class, cp_index);
}

u2 search_constant_string(u2 cp_index){
    return search_constant_class_and_string(CONSTANT_String, cp_index);
}

static u2 search_constant_integer_and_float(ConstantInfoTag tag, u4 bytes){
    ClassFile *cf;
    ConstantInfo ci;
    u2 i;

    cf = get_current_classfile();
    for(i = 0; i < cf->constant_pool_count; i++){
        ci = cf->constant_pool[i];
        if(ci.tag == tag)
            if(ci.u.bytes == bytes)
                return ++i;
    }
    return 0;
}

u2 search_constant_integer(u4 bytes){
    return search_constant_integer_and_float(CONSTANT_Integer, bytes);
}

u2 search_constant_float(u4 bytes){
    return search_constant_integer_and_float(CONSTANT_Float, bytes);
}

static u2 search_constant_long_and_double(ConstantInfoTag tag, u4 high_bytes, u4 low_bytes){
    ClassFile *cf;
    ConstantInfo ci;
    u2 i;

    cf = get_current_classfile();
    for(i = 0; i < cf->constant_pool_count; i++){
        ci = cf->constant_pool[i];
        if(ci.tag == tag)
            if(ci.u.long_bytes.high_bytes == high_bytes)
                if(ci.u.long_bytes.low_bytes == low_bytes)
                    return ++i;
    }
    return 0;
}

u2 search_constant_long(u4 high_bytes, u4 low_bytes){
    return search_constant_long_and_double(CONSTANT_Long, high_bytes, low_bytes);
}

u2 search_constant_double(u4 high_bytes, u4 low_bytes){
    return search_constant_long_and_double(CONSTANT_Double, high_bytes, low_bytes);
}

u2 search_constant_fieldref_methodref_and_interface_methodref(ConstantInfoTag tag, u2 class_index, u2 name_and_type_index){
    ClassFile *cf;
    ConstantInfo ci;
    u2 i;

    cf = get_current_classfile();
    for(i = 0; i < cf->constant_pool_count; i++){
        ci = cf->constant_pool[i];
        if(ci.tag == tag)
            if(ci.u.reference_info.class_index == class_index)
                if(ci.u.reference_info.name_and_type_index == name_and_type_index)
                return ++i;
    }
    return 0;
}

u2 search_constant_fieldref(u2 class_index, u2 name_and_type_index){
    return search_constant_fieldref_methodref_and_interface_methodref(CONSTANT_Fieldref, class_index, name_and_type_index);
}

u2 search_constant_methodref(u2 class_index, u2 name_and_type_index){
    return search_constant_fieldref_methodref_and_interface_methodref(CONSTANT_Methodref, class_index, name_and_type_index);
}

u2 search_constant_interface_methodref(u2 class_index, u2 name_and_type_index){
    return search_constant_fieldref_methodref_and_interface_methodref(CONSTANT_InterfaceMethodref, class_index, name_and_type_index);
}
*/
