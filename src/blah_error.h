/* blah_error.h
    Error routines
*/

#ifndef _BLAH_ERROR

#define _BLAH_ERROR

#include <errno.h>
#include <stdnoreturn.h>

#define BLAH_ERROR_BASE 10000
#define BLAH_ERROR_MAX_DESC_LENGTH 200

noreturn void blah_error_raise(int errorCode, const char* messageFormat, ...);


#endif // _BLAH_ERROR
