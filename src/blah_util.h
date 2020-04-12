/* blah_util.h
	Defines some useful functions */


#ifndef _BLAH_UTIL

#define _BLAH_UTIL

#include "blah_types.h"

/* Function Prototypes */

#ifdef __cplusplus
	extern "C" {
#endif //__cplusplus

int blah_util_randRangeInt(int min, int max);
	//returns a random integer in the range of min..max

void blah_util_byteSwap(void *byteArray, int numBytes);
	//Swaps an array num_bytes in memory, at address byte_array

int blah_util_byteSwapInt(int swapMe);
	//Returns the given integer with byte order reversed

unsigned long blah_util_byteSwapUnsigned(unsigned long swapMe);
	//Returns the given integer with byte order reversed

blah_unsigned16 blah_util_byteSwapUnsigned16(blah_unsigned16 swapMe);
	//Returns the given 16bit unsigned integer with byte order reversed

bool blah_util_stringReplaceChar(char *string, char replaceMe, char with);
	//Replaces all occurences in string of 'replace_me' with 'with'

unsigned int blah_util_ceilPowerOf2(unsigned int num);
	//Returns an integer which is a power of 2 and equal or greater than given
	//integer 'num'

char *blah_util_strncpy(char *to, const char *from, size_t count);
	//Behaves like standard C strncpy, but always appends an extra NULL char in addition
	//to the size_t count bytes.  Therefore, if the source contains atleast (count) bytes,
	//then the destination array must have a capacity of (count) + 1 bytes, and the last byte
	//of the destination array will be a NULL.


#ifdef __cplusplus
	}
#endif //__cplusplus

#endif
