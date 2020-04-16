#include <math.h>

#include <stdlib.h>
#include <string.h>

#include "blah_util.h"
#include "blah_types.h"

void blah_util_byteSwap(void *byteArray, int numBytes) {
	//Swaps an array num_bytes in memory, at address byte_array
	int numSwap = numBytes >> 1; //Integer divide byte num by 2
	int swapCount;
	unsigned char *left, *right, tempByte;

	left = (unsigned char*)byteArray;
	right = left + numBytes - 1;

	for (swapCount = 0; swapCount < numSwap; swapCount++) {
		tempByte = *left;
		*left = *right;
		*right = tempByte;
		left++; right--;
	}

		/*(unsigned char*)byte_array)[swap_count] = (unsigned char*)byte_array)[num_bytes-swap_count];
		(unsigned char*)byte_array) */
}

int blah_util_byteSwapInt(int swapMe) {
	//Returns the given integer with byte order reversed
	int tempInt = swapMe;

	blah_util_byteSwap(&tempInt, sizeof(int));
	return tempInt;
}

unsigned long blah_util_byteSwapUnsigned(unsigned long swapMe) {
	//Returns the given unsigned long with byte order reversed
	unsigned long tempLong = swapMe;

	blah_util_byteSwap(&tempLong, sizeof(unsigned long));
	return tempLong;
}

blah_unsigned16 blah_util_byteSwapUnsigned16(blah_unsigned16 swapMe) {
	//Returns the given unsigned long with byte order reversed
	blah_unsigned16 temp = swapMe;

	blah_util_byteSwap(&temp, 2);
	return temp;
}

unsigned int blah_util_ceilPowerOf2(unsigned int num) {
	//Returns an integer which is a power of 2 and equal or greater than given
	//integer 'num'
	unsigned int powOf2 = 0;
	while (powOf2<num) {
		if (!powOf2) {
			powOf2 = 1;  //if pow of 2 is zero, advance it to one
		} else {
			powOf2*=2;  //else multiply by 2 for the next power of 2
		}
	}
	return powOf2;
}

int blah_util_randRangeInt(int min, int max) { //returns a random integer in the range of min..max
	int range, result;

	range = abs(max - min);
	result = ((float)(range+1)*rand()/(RAND_MAX+1.0))+min;
	return result;
}


bool blah_util_stringReplaceChar(char *string, char replaceMe, char with) {
	// Replaces all occurences in string of 'replace_me' with 'with'
	char *pos = strchr(string, replaceMe);
	bool success = pos != NULL;

	while (pos != NULL) { // while a character has been found
		*pos = with; // Place new char in place of old one
		pos = strchr(pos + 1, replaceMe);  //look for char again, starting at next position in memory
	}

	return success;
}

char* blah_util_strncpy(char *to, const char *from, size_t count)
{	// Behaves like standard C strncpy, but always appends an extra NULL char in addition
	// to the size_t count bytes.  Therefore, if the source contains atleast (count) bytes,
	// then the destination array must have a capacity of (count) + 1 bytes, and the last byte
	// of the destination array will be a NULL.
	char *returnPointer = strncpy(to, from, count); //Copy name string
	to[count] = '\0'; //Append null character to end of name string

	return returnPointer;
}

