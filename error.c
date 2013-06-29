#include "error.h"

char *compile_error_format[] = {
    {"dummy"},
    {"no input files\n"},
    {"%s: no such file or directory\n"},
    {"unsupported file name.\nfile name must begin with a capital letter.\n"},
    {"unsupported file extension.\nfile extension must be \".desk\".\n"},
    {"%d: parse error\n"},
    {"%d: function main is already defined\n"}
};

char *system_error_format[] = {
    {"dummy"},
    {"orderd constant pool index %d is too large.\n"}
}

void compile_error(CompileErrorType type, ...){
    va_list args;

    va_start(args, type);
    vfprintf(stderr, compile_error_format[type], args);
    exit(1);
    va_end(args);
}

void system_error(SystemErrorType type, ...){
    va_list args;

    va_start(args, type);
    vfprintf(stderr, sysmte_error_format[type], args);
    exit(1);
    va_end(args);
}
