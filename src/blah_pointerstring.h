/* blah_pointerstring.h
	Defines operations on pointer strings.
	A pointer string is an allocated array of arbitray data pointers, with a terminating
	NULL pointer as the last element of the array.  Resembles a character string.
	Pointer strings must de deallocated from memory with a call to free();
*/

#ifndef _BLAH_POINTERSTRING

#define _BLAH_POINTERSTRING

#include "blah_types.h"

#ifdef __cplusplus
	extern "C" {
#endif //__cplusplus

int blah_pointerstring_length(blah_pointerstring pstring);
	//Returns the length (number of pointers) contained in the pointer string.
	
#ifdef __cplusplus
	}
#endif //__cplusplus	

#endif
