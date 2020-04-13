/* blah_console.c
    Defines routines for standard input and output interacting with the user console.
*/

#include <stdio.h>
#include <stdarg.h>

#include "blah_console.h"
#include "blah_file.h"

// Write formatted output to standard output.  Works like printf.  Does not flush stream.
void blah_console_message(const char* messageFormat, ...)
{
    va_list varArgs;
    va_start(varArgs, messageFormat);
    blah_file_writeString(stdout, messageFormat, varArgs);
    va_end(varArgs);
}

// Write formatted output to standard error.  Works like printf.  Does not flush stream.
void blah_console_error(const char* messageFormat, ...)
{
    va_list varArgs;
    va_start(varArgs, messageFormat);
    blah_file_writeString(stderr, messageFormat, varArgs);
    va_end(varArgs);
}

// Flush output to both console messages and errors
void blah_console_flush()
{
    fflush(stdout);
    fflush(stderr);
}
