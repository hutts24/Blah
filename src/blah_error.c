/* blah_error.c
    Defines routines for error handling.
*/

// #include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#include "blah_error.h"

/* External functions used internally */
extern void blah_console_errorVA(int errorCode, const char* messageFormat, va_list varArgs);

// This function should be called when an error is detected and it will respond to the error with appropriate action.
noreturn void blah_error_raise(int errorCode, const char* messageFormat, ...)
{
    // Display the error on the console
    va_list varArgs;
    va_start(varArgs, messageFormat);
    blah_console_errorVA(errorCode, messageFormat, varArgs);
    va_end(varArgs);
    // Exit optimistically with error code
    exit(errorCode);
}
