#ifndef ERROR_H_INCLUDED
#define ERROR_H_INCLUDED
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum{
    ERROR_NO_INPUT_FILES = 1,
    ERROR_NO_SUCH_FILE,
    ERROR_UNSUPPORTED_FILE_NAME,
    ERROR_UNSUPPORTED_FILE_EXTENSION,
    ERROR_PARSE,
    ERROR_FUNCTION_ALREADY_DEFINED,
    ERROR_FUNCTION_NOT_DEFINED,
    ERROR_FUNCTION_PATTERN_ALREADY_DEFINED,
    ERROR_NOT_DEFINED_LOCAL_VALIABLE,
    ERROR_FUNCTION_WITH_ARG_NOT_DEFINED
} CompileErrorType;

typedef enum{
    ERROR_INVALID_CP_INDEX = 1,
    ERROR_UNKNOWN_CONSTANT_TAG
} SystemErrorType;

void system_error(SystemErrorType type, ...);
void compile_error(CompileErrorType type, ...);

#endif // ERROR_H_INCLUDED
