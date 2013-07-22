#include "error.h"

char *compile_error_format[] = {
    "dummy",
    "no input files.\n",
    "%s: no such file or directory.\n",
    "unsupported file name.\nfile name must begin with a capital letter.\n",
    "unsupported file extension.\nfile extension must be \".desk\".\n",
    "%d: parse error.\n",
    "%d: function %s with parameter %s is already defined.\n",
    "%d: function %s not defined. cannot set pattern %d.\n",
    "%d: function %s with pattern %d is already defined.\n"
};

char *system_error_format[] = {
    "dummy",
    "invalid constant pool index %d.\n",
    "unknown constant tag number %d.\n"
};

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
    vfprintf(stderr, system_error_format[type], args);
    exit(1);
    va_end(args);
}
