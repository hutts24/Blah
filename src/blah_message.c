/* blah_message.c
    Routines for displaying messages to the user
*/

#include <string.h>
#include <stdarg.h>

#include "blah_message.h"
#include "blah_time.h"
#include "blah_error.h"

// Internal function only.  Write current timestamp to file in local time zone with a trailing space.
// Does not append new line character or flush the stream
static void blah_message_writeTimeStampToFile(FILE* file)
{
    // Get local time in UTC
    blah_time currentUTC;
    blah_time_getCurrentUTC(&currentUTC);
    char timeString[50];
    Blah_Time_toLocalTimeString(&currentUTC, timeString, sizeof(timeString) / sizeof(char)); // Convert to local time and format as time only
    // Print the time string, followed by formatted message
    fprintf(file, "%s ", timeString);
}

// Internal function accepts variable arguments directly.
// Write message preceded by a timestamp and followed by a new line character.
// All messages going to a FILE should use this function.  File stream is not flushed.
void blah_message_writeToFileVA(FILE* file, const char* messageFormat, va_list varArgs)
{
    // Print the time string, followed by formatted message
    blah_message_writeTimeStampToFile(file);
    vfprintf(file, messageFormat, varArgs);
    fputc('\n', file); // Add new line
}

// Internal function that accepts va_list directly like vprintf for internal use.
// Write formatted error message to file stream with new line character appended and flush stream
// All errors going to a FILE should use this function.
void blah_message_writeErrorToFileVA(FILE* file, int errorCode, const char* messageFormat, va_list varArgs)
{
    // If a non zero error code has been supplied, print first line with time, error code and description
    if (errorCode != 0) {
        char errorDescription[BLAH_ERROR_MAX_DESC_LENGTH + 1];
        strerror_s(errorDescription, sizeof(errorDescription), errorCode);
        blah_message_writeToFile(file, "Error Code: %d, %s", errorCode, errorDescription); // Adds new line automatically
    }
    // Print second line using formatted message and variable arguments
    blah_message_writeTimeStampToFile(file);
    fputs("Error Info: ", file);
    vfprintf(file, messageFormat, varArgs); // Adds new line automatically
    fputc('\n', file);
    fflush(file);
}

// Write formatted message to given FILE stream.
// A new line character is appened to the end of the output.
// File stream is not flushed.
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

