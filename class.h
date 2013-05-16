
typedef unsigned char u1;
typedef unsigned short u2;
typedef unsigned long u4;

typedef enum{
    ACC_PUBLIC = 0x0001,
    ACC_FINAL = 0x0010,
    ACC_SUPER = 0x0020,
    ACC_INTERFACE = 0x0200,
    ACC_ABSTRACT = 0x0400
} AccessFlags; // u2

typedef enum{
    CONSTANT_Class = 7,
    CONSTANT_Fieldref = 9,
    CONSTANT_Methodref = 10,
    CONSTANT_InterfaceMethodref = 11,
    CONSTANT_String = 8,
    CONSTANT_Integer = 3,
    CONSTANT_Float = 4,
    CONSTANT_Long = 5,
    CONSTANT_Double = 6,
    CONSTANT_NameAndType = 12,
    CONSTANT_Utf8 = 1
} ConstantPoolTag;

typdef struct{
    u2 class_index;
    u2 name_and_type_index;
} ReferenceInfo;

typdef struct{
    u4 high_bytes;
    u4 low_bytes;
} LongBytes;

typedef struct{
    u2 name_index;
    u2 descriptor_index;
} NameAndTypeInfo;

typedef struct{
    u2 length;
    u1 *bytes;
} Utf8Info;

typdef struct{
    ConstantPoolTag tag;
    union u{
        u2 cp_index;
        RefarenceInfo reference_info;
        b4 bytes;
        LongBytes long_bytes;
        NameAndTypeInfo name_and_type_info;
        Utf8Info utf8_info;
    };
} ConstantPoolInfo;

typedef struct{
    u4 magic;
    u2 minor_version;
    u2 major_version;
    u2 constant_pool_count;
    ConstantPoolInfo *constant_pool;//[constant_pool_count - 1]
    AccessFlags access_flags;
    u2 this_class;
    u2 super_class;
    u2 interfaces_count;
    u2 *interfaces;//[interfaces_count]
    u2 fields_count;
    field_info *fields;//[fields_count]
    u2 methods_count;
    method_info *methods;//[methods_count]
    u2 attributes_count;
    attribute_info *attributes://[attributes_count]
} ClassFile;
