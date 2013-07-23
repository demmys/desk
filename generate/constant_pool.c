#include "classfile.h"

/*
 * search method
 */
static u2 vsearch_constant_info(ConstantInfoTag tag, va_list args){
    ClassFile *cf;
    ConstantInfo *ci;
    u2 i;

    u2 arg1, arg2;
    char *arg3;
    u4 arg4, arg5;

    switch(tag){
        case CONSTANT_Utf8:
            arg1 = va_arg(args, int);
            arg3 = va_arg(args, char *);
            break;
        case CONSTANT_Class:
        case CONSTANT_String:
            arg1 = va_arg(args, int);
            break;
        case CONSTANT_Integer:
        case CONSTANT_Float:
            arg4 = va_arg(args, u4);
            break;
        case CONSTANT_Long:
        case CONSTANT_Double:
            arg4 = va_arg(args, u4);
            arg5 = va_arg(args, u4);
            break;
        case CONSTANT_Fieldref:
        case CONSTANT_Methodref:
        case CONSTANT_InterfaceMethodref:
            arg1 = va_arg(args, int);
            arg2 = va_arg(args, int);
            break;
        case CONSTANT_NameAndType:
            arg1 = va_arg(args, int);
            arg2 = va_arg(args, int);
            break;
        default:
            system_error(ERROR_UNKNOWN_CONSTANT_TAG, tag);
    }

    cf = get_current_classfile();
    for(i = 1, ci = cf->constant_pool; ci; ci = ci->next, i++){
        if(ci->tag == tag){
            switch(tag){
                case CONSTANT_Utf8:
                    if(!strcmp(ci->u.utf8_info.value, arg3))
                        return i;
                    break;
                case CONSTANT_Class:
                case CONSTANT_String:
                    if(ci->u.cp_index == arg1)
                        return i;
                    break;
                case CONSTANT_Integer:
                case CONSTANT_Float:
                    if(ci->u.bytes == arg4)
                        return i;
                    break;
                case CONSTANT_Long:
                case CONSTANT_Double:
                    if(ci->u.long_bytes.low_bytes == arg4)
                        if(ci->u.long_bytes.high_bytes == arg5)
                            return i;
                    break;
                case CONSTANT_Fieldref:
                case CONSTANT_Methodref:
                case CONSTANT_InterfaceMethodref:
                    if(ci->u.reference_info.class_index == arg1)
                        if(ci->u.reference_info.name_and_type_index == arg2)
                            return i;
                    break;
                case CONSTANT_NameAndType:
                    if(ci->u.name_and_type_info.name_index == arg1)
                        if(ci->u.name_and_type_info.descriptor_index == arg2)
                            return i;
            }
        }
    }
    return 0;
}

/*
   u2 search_constant_info(ConstantInfoTag tag, ...){
   va_list args;
   u2 index;

   va_start(args, tag);
   index = vsearch_constant_info(tag, args);
   va_end(args);
   return index;
   }
   */


/*
 * add method
 */
static u2 add_constant_info(ConstantInfoTag tag, ...){
    ClassFile *cf;
    ConstantInfo *ci;
    va_list args;
    u2 index;

    cf = get_current_classfile();

    /* if adding constant exists, return the index */
    va_start(args, tag);
    index = vsearch_constant_info(tag, args);
    if(index > 0)
        return index;
    va_end(args);

    /* add constant pool */
    if(cf->constant_pool){
        ci = classfile_storage_malloc(sizeof(ConstantInfo));
        ci->next = NULL;
        ci->prev = cf->constant_pool->prev;
        ci->prev->next = ci;
        cf->constant_pool->prev = ci;
    } else{
        cf->constant_pool = classfile_storage_malloc(sizeof(ConstantInfo));
        cf->constant_pool->next = NULL;
        cf->constant_pool->prev = cf->constant_pool;
        ci = cf->constant_pool;
    }
    (cf->constant_pool_count)++;

    /* set value to new constant info */
    ci->tag = tag;
    va_start(args, tag);
    switch(tag){
        case CONSTANT_Utf8:
            ci->u.utf8_info.length = va_arg(args, int);
            ci->u.utf8_info.value = va_arg(args, char *);
            break;
        case CONSTANT_Class:
        case CONSTANT_String:
            ci->u.cp_index = va_arg(args, int);
            break;
        case CONSTANT_Integer:
        case CONSTANT_Float:
            ci->u.bytes = va_arg(args, u4);
            break;
        case CONSTANT_Long:
        case CONSTANT_Double:
            ci->u.long_bytes.high_bytes = va_arg(args, u4);
            ci->u.long_bytes.low_bytes = va_arg(args, u4);
            break;
        case CONSTANT_Fieldref:
        case CONSTANT_Methodref:
        case CONSTANT_InterfaceMethodref:
            ci->u.reference_info.class_index = va_arg(args, int);;
            ci->u.reference_info.name_and_type_index = va_arg(args, int);
            break;
        case CONSTANT_NameAndType:
            ci->u.name_and_type_info.name_index = va_arg(args, int);
            ci->u.name_and_type_info.descriptor_index = va_arg(args, int);
    }
    va_end(args);
    return cf->constant_pool_count;
}

u2 add_constant_utf8_info(char *value){
    return add_constant_info(CONSTANT_Utf8, strlen(value), value);
}

u2 add_constant_class_info(char *name){
    int index;
    index = add_constant_utf8_info(name);
    return add_constant_info(CONSTANT_Class, index);
}

u2 add_constant_name_and_type_info(char *name, char *type){
    int nindex, dindex;
    nindex = add_constant_utf8_info(name);
    dindex = add_constant_utf8_info(type);
    return add_constant_info(CONSTANT_NameAndType, nindex, dindex);
}

static u2 add_constant_reference_info(ConstantInfoTag tag, char *class, char *name, char *type){
    int cindex, ntindex;
    cindex = add_constant_class_info(class);
    ntindex = add_constant_name_and_type_info(name, type);
    return add_constant_info(tag, cindex, ntindex);
}

static u2 add_constant_reference_info_with_class(ConstantInfoTag tag, u2 class_index, char *name, char *type){
    int ntindex;
    ntindex = add_constant_name_and_type_info(name, type);
    return add_constant_info(tag, class_index, ntindex);
}

u2 add_constant_method_info(char *class, char *name, char *type){
    return add_constant_reference_info(CONSTANT_Methodref, class, name, type);
}

u2 add_constant_method_info_with_class(u2 class_index, char *name, char *type){
    return add_constant_reference_info_with_class(CONSTANT_Methodref, class_index, name, type);
}

u2 add_constant_field_info(char *class, char *name, char *type){
    return add_constant_reference_info(CONSTANT_Fieldref, class, name, type);
}

u2 add_constant_interface_method_info(char *class, char *name, char *type){
    return add_constant_reference_info(CONSTANT_InterfaceMethodref, class, name, type);
}


/*
 * get method
 */
ConstantInfo *get_constant_info(int index){
    ClassFile *cf;
    ConstantInfo *ci;
    int i;

    cf = get_current_classfile();
    if(index < (cf->constant_pool_count / 2)){
        for(i = 1, ci = cf->constant_pool; ci; ci = ci->next, i++){
            if(i == index)
                return ci;
        }
    } else{
        for(i = cf->constant_pool_count - index, ci = cf->constant_pool->prev; ci->prev->next; ci = ci->prev, i--){
            if(i == 0)
                return ci;
        }
    }
    system_error(ERROR_INVALID_CP_INDEX, index);
    return NULL;
}
