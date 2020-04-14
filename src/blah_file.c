/* blah_file.c
	Defines common functions on files, using standard FILE* */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "blah_file.h"
#include "blah_types.h"
#include "blah_util.h"

/* Private Function Prototypes */

FILE *blah_file_open(const char *filename, const char* mode)
{	// Simplifies opening files across different platforms.  Calls fopen()
	// Parameters have same purpose as in fopen()
	char osFilename[200];
    blah_util_strncpy(osFilename, filename, sizeof(osFilename));
	blah_util_stringReplaceChar(osFilename, '\\', '/');
	// change backslashes to forward slashes
	return fopen(osFilename, mode);
}

bool blah_file_readX86(FILE* fileStream, void* dest, int byteLength);
	// Reads a binary number of size 'byteLength' bytes into 'dest'
	// and reverses it for x86 compatible registers.  Returns true on success, false error.

/* Private Function Definitions */

bool blah_file_readX86(FILE* fileStream, void* dest, int byteLength)
{	// Reads a binary number of size 'byteLength' bytes into 'dest'
	// and reverses it for x86 compatible registers.  Returns true on success, false error.
	if (fread(dest, byteLength, 1, fileStream)) { //Read binary value from file
		blah_util_byteSwap(dest, byteLength);  //Put into Intel compatible format
		return true;
	} else {
		return false;
	}
}

/* Public Function Declarations */

bool blah_file_readFloat32(FILE *file, blah_float32 *dest)
{	// Reads a 32bit floating point value from binary file_pointer into 'dest'
	// Returns true on succes, false on error
	return blah_file_readX86(file, dest, 4);
}

bool blah_file_readInt16(FILE *file, blah_int16 *dest)
{	// Reads a 16bit signed integer value from binary file_pointer into 'dest'
	// Returns true on succes, false on error
	return blah_file_readX86(file, dest, 2);
}

bool blah_file_readInt32(FILE *file, blah_int32 *dest)
{	// Reads a 32bit signed integer value from binary file_pointer into 'dest'
	// Returns true on succes, false on error
	return blah_file_readX86(file, dest, 4);
}

char* blah_file_readString(FILE *file)
{
    // Reads a null terminated string from the give file pointer.
	// Returns pointer to allocated string on success, null on error.
	// Caller must free the returned string.
	unsigned long length = 0;
	char *newString;
	bool nullFound = false;

	while (!(feof(file) || nullFound)) { // While a null char hasn't been found and not eof
		length++;
		if (fgetc(file) == '\0') { nullFound = true; } // If next character from file stream is not null
	}

	if (nullFound) { //If we have found a null termed string before eof
		fseek(file, -length, SEEK_CUR);		//Rewind file pointer to original position
		newString = (char*)malloc(length); //Allocate memory buffer for new string
		fread(newString, length, 1, file);	//Read whole string including null char into buffer
		return newString;
	} else {
		return NULL; //If failure, return NULL pointer
	}
}

bool blah_file_readUnsigned16(FILE *file, blah_unsigned16 *dest)
{	// Reads a 16bit unsigned integer value from binary file_pointer into 'dest'
	// Returns true on succes, false on error
	return blah_file_readX86(file, dest, 2);
}

bool blah_file_readUnsigned32(FILE *file, blah_unsigned32 *dest)
{	// Reads a 32bit unsigned integer value from binary file_pointer into 'dest'
	// Returns true on succes, false on error
	return blah_file_readX86(file, dest, 4);
}
