/* blah_console.h
    Defines routines for standard input and output interacting with the user console.
*/

#ifndef _BLAH_CONSOLE

#define _BLAH_CONSOLE

// Write formatted output to standard output.  Works like printf.  Does not flush stream.
void blah_console_message(const char* messageFormat, ...);

// Write formatted output to standard error.  Works like printf.  Does not flush stream.
void blah_console_error(const char* messageFormat, ...);


#endif
