/* blah_error.h
    Error routines
*/

#ifndef _BLAH_ERROR

#define _BLAH_ERROR

#include <errno.h>
#include <stdnoreturn.h>

#define BLAH_ERROR_BASE 10000
#define BLAH_ERROR_MAX_DESC_LENGTH 200

typedef int blah_error;

// This is the real function prototype, but it should not be called directly.
// Instead, it should be called by the macro below 'blah_error_raise' which shadows it.
noreturn extern void _blah_error_raise(int errorCode, const char* messageFormat, ...);
#define blah_error_raise(errCode, msgFmt, ...) (_blah_error_raise)(errCode, msgFmt ".  File: %s, Line: %d.", ## __VA_ARGS__, __FILE__, __LINE__)
#define _blah_error_raise(...) error_do_not_call_this_function_directly

#endif // _BLAH_ERROR
