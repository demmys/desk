#ifndef CLASSFILE_DEFINITION_H_INCLUDED
#define CLASSFILE_DEFINITION_H_INCLUDED
#include "attributes.h"

/*
 * struct declaration
 */
typedef struct Definition_tag Definition;
struct Definition_tag{
    u2 access_flags;
    u2 name_index;
    u2 descriptor_index;
    u2 attributes_count;
    AttributeInfo *attributes;
    Definition *prev;
    Definition *next;
};

/*
 * methods prototype of classfile_definition.c.
 */
Definition *add_definition(Definition **dn_list, u2 *list_length, char *name, char *type);

#endif // CLASSFILE_DEFINITION_H_INCLUDED
