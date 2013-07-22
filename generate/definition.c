#include "classfile.h"

Definition *add_definition(Definition **dn_list, u2 *list_length, char *name, char *type){
    Definition *dn;

    /* add difinition */
    if(*dn_list){
        dn = classfile_storage_malloc(sizeof(Definition));
        dn->next = NULL;
        dn->prev = (*dn_list)->prev;
        dn->prev->next = dn;
        (*dn_list)->prev = dn;
    } else{
        *dn_list = classfile_storage_malloc(sizeof(Definition));
        (*dn_list)->next = NULL;
        (*dn_list)->prev = *dn_list;
        dn = *dn_list;
    }
    (*list_length)++;

    /* set value to new definition */
    dn->access_flags = ACC_PUBLIC | ACC_STATIC;
    dn->name_index = add_constant_utf8_info(name);
    // TODO 型を表現する文字列の配列を作っておき、typeで与えられた型を表すJVMの文字列を自動生成する
    dn->descriptor_index = add_constant_utf8_info(type);
    dn->attributes_count = 0;
    dn->attributes = NULL;
    return dn;
}
