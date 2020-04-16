/* blah_console.c
    Defines routines for standard input and output interacting with the user console.
*/

#include <stdio.h>
#include <stdarg.h>

#include "blah_console.h"
#include "blah_file.h"
#include "blah_message.h"
#include "blah_error.h"

/* External Declarations of functions for internal use */
extern void blah_message_writeToFileVA(FILE* file, const char* messageFormat, va_list varArgs);
extern void blah_message_writeErrorToFileVA(FILE* file, int errorCode, const char* messageFormat, va_list varArgs);

/* Private Functions */

void blah_console_errorVA(blah_error errorCode, const char* messageFormat, va_list varArgs)
{
    blah_message_writeErrorToFileVA(stderr, errorCode, messageFormat, varArgs);
}

/* Public Functions */

// No new line appended
void blah_console_writeString(const char* formatString, ...)
{
    va_list varArgs;
    va_start(varArgs, formatString);
    vfprintf(stdout, formatString, varArgs);
    va_end(varArgs);
}

// Write formatted output to standard output.  Works like printf.
// A new line character is appended at the end of output and error output is flushed.
void blah_console_message(const char* messageFormat, ...)
{
    va_list varArgs;
    va_start(varArgs, messageFormat);
    blah_message_writeToFileVA(stdout, messageFormat, varArgs);
    va_end(varArgs);
}

// Write formatted output to standard error.  Works like printf.  Does not flush stream.
void blah_console_error(blah_error errorCode, const char* messageFormat, ...)
{
    va_list varArgs;
    va_start(varArgs, messageFormat);
    blah_console_errorVA(errorCode, messageFormat, varArgs);
    va_end(varArgs);
}

// Flushes standard output only.  There is no need to flush the error output because the error functions do that automatically
void blah_console_flush()
{
    fflush(stdout);
}
