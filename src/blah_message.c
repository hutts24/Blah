/* blah_message.c
    Routines for displaying messages to the user
*/

#include <string.h>
#include <stdarg.h>

#include "blah_message.h"
#include "blah_time.h"
#include "blah_error.h"
#include "blah_macros.h"

#define ERROR_CODE_MAX_LENGTH 20 // Large enough to display unsigned 64bit integers

// Internal function only.  Write current timestamp to file in local time zone with a trailing space.
// Does not append new line character or flush the stream
static void blah_message_writeTimeStampToFile(FILE* file)
{
    // Get local time in UTC
    blah_time currentUTC;
    blah_time_getCurrentUTC(&currentUTC);
    char timeString[50];
    Blah_Time_toLocalTimeString(&currentUTC, timeString, blah_countof(timeString)); // Convert to local time and format as time only
    // Print the time string, followed by formatted message
    fprintf(file, "%s ", timeString);
}

// Internal function accepts variable arguments directly.
// Write message preceded by a timestamp and followed by a new line character and flush the stream.
// All messages going to a FILE should use this function.
void blah_message_writeToFileVA(FILE* file, const char* messageFormat, va_list varArgs)
{
    // Print the time string, followed by formatted message
    blah_message_writeTimeStampToFile(file);
    vfprintf(file, messageFormat, varArgs);
    fputc('\n', file); // Add new line
    fflush(file);
}

// Internal function that accepts va_list directly like vprintf for internal use.
// Write formatted error message to file stream with new line character appended and flush the stream
// All errors going to a FILE should use this function.
void blah_message_writeErrorToFileVA(FILE* file, int errorCode, const char* messageFormat, va_list varArgs)
{
    // Write timestamp first
    blah_message_writeTimeStampToFile(file);
    // Write the error code to the file stream
    char errorCodeString[ERROR_CODE_MAX_LENGTH];
    if (errorCode == 0 || snprintf(errorCodeString, blah_countof(errorCodeString), "%d", errorCode) >= ERROR_CODE_MAX_LENGTH) {
        strcpy(errorCodeString, "Not specified"); // error code is too large to display (greater than 64 bits)
        errorCode = 0; // Ignore the error code supplied
    }
    fprintf(file, "Error Code: %s", errorCodeString);
    // Now if the error code is valid, add its description to the output
    if (errorCode != 0) {
        char errorDescription[200];
        strerror_s(errorDescription, sizeof(errorDescription), errorCode);
        fprintf(file, " - %s", errorDescription);
    }
    // Now print the formatted message supplied with the error code
    fputs(".  ", file);
    vfprintf(file, messageFormat, varArgs); // Adds new line automatically
    fputc('\n', file);
    fflush(file);
}

// Write formatted message to given FILE stream.
// A new line character is appened to the end of the output and flush the stream
void blah_message_writeToFile(FILE* file, const char* messageFormat, ...)
{
    va_list varArgs;
    va_start(varArgs, messageFormat);
    blah_message_writeToFileVA(file, messageFormat, varArgs);
    va_end(varArgs);
}


// Write a detailed error message to given FILE stream and flush to output (errors are important).
// A new line character is appened to the end of the output
void blah_message_writeErrorToFile(FILE* file, int errorCode, const char* messageFormat, ...)
{
    va_list varArgs;
    va_start(varArgs, messageFormat);
    blah_message_writeErrorToFileVA(file, errorCode, messageFormat, varArgs);
    va_end(varArgs);
    fflush(file);
}

