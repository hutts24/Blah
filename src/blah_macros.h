#ifndef _BLAH_MACROS

#define _BLAH_MACROS

#include "blah_types.h"

// Use this macro to determine the number of elements in a static array
// DO NOT USE WITH POINTER VALUES!
#define blah_countof(array) ((sizeof(array)/sizeof(0[array])) / ((size_t)(!(sizeof(array) % sizeof(0[array])))))

#endif
