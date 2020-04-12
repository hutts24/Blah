/* blah_types.h
	Defines some useful simple types, enums etc */

#ifndef _BLAH_TYPES

#define _BLAH_TYPES

#include <stdbool.h>

typedef enum Blah_Boolean {BLAH_FALSE = false, BLAH_TRUE = true} blah_bool;

//typedef void (*blah_function)();

/* typedef void (*blah_function_1arg)(void*);
typedef void (*blah_function_2arg)(void*,void*);
typedef blah_bool (*blah_function_bool_2arg)(void*,void*); */


typedef float blah_float32;
typedef int blah_int32;
typedef unsigned int blah_unsigned32;
typedef short blah_int16;
typedef unsigned short blah_unsigned16;
typedef unsigned char blah_unsigned8;
typedef char blah_int8;

typedef void **blah_pointerstring;

#endif
