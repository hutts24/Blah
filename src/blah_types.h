/* blah_types.h
	Defines some useful simple types, enums etc */

#ifndef _BLAH_TYPES

#define _BLAH_TYPES

#include <stdbool.h>
#include <stdint.h>

/* typedef void (*blah_function_1arg)(void*);
typedef void (*blah_function_2arg)(void*,void*);
typedef bool (*blah_function_bool_2arg)(void*,void*); */

// TODO - C99 Fixed width types have made these obsolete.  Remove them!
typedef float blah_float32;
typedef int32_t blah_int32;
typedef uint32_t blah_unsigned32;
typedef int16_t blah_int16;
typedef uint16_t blah_unsigned16;
typedef uint8_t blah_unsigned8;
typedef int8_t blah_int8;

typedef void **blah_pointerstring;

#endif
