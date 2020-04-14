/* blah_message.h
      Routines for displaying messages to the user
*/

#ifndef _BLAH_MESSAGE

#define _BLAH_MESSAGE

#include <stdio.h>

// Write formatted message to given FILE stream.
// A new line character is appened to the end of the output.
// File stream is not flushed.
void blah_message_writeToFile(FILE* file, const char* messageFormat, ...);

// Write a detailed error message to given FILE stream and flush to output (errors are important).
// A new line character is appened to the end of the output
void blah_message_writeErrorToFile(FILE* file, int errorCode, const char* messageFormat, ...);

#endif // _BLAH_MESSAGE

