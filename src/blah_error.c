/* blah_error.c
    Defines routines for error handling.
*/

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdarg.h>

#include "blah_time.h"

// Write a detailed error message to given FILE stream and flush to output (errors are important).
void blah_error_writeToFile(FILE* file, int errorCode, const char* messageFormat, ...)
{
    // Get local time in UTC
    Blah_Time currentUTC;
    blah_time_getCurrentUTC(&currentUTC);
    char timeString[50];
    Blah_Time_toLocalTimeString(currentUTC, timeString, sizeof(timeString) / sizeof(char)); // Convert to local time and format as time only
    // Print first part with time, error code and description
    char errorDescription[200];
    strerror_s(errorDescription, sizeof(errorDescription), errorCode);
    fprintf(file, "%s Error Code: %d, %s.  ", timeString, errorCode, errorDescription);
    // Print second part which is provided message and variable arguments
    va_list varArgs;
    va_start(varArgs, messageFormat);
    fprintf(file, messageFormat, varArgs);
    va_end(varArgs);

    fflush(file);
}
