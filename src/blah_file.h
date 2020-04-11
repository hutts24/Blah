/* blah_file.h 
	Defines common functions on files, using standard FILE*
	These functions work on the assumption that the processor is an Intel
	x86 compatible and that the binary values are ordered with least significant
	last.  Intel values are stored in reverse order in memory.	*/
	
	
#ifndef _BLAH_FILE

#define _BLAH_FILE

#include "blah_types.h"
#include <stdio.h>

/* Function Prototypes */

#ifdef __cplusplus
	extern "C" {
#endif //__cplusplus

FILE *blah_file_open(char *name, char *mode);
	//Simplifies opening files across different platforms.  Calls fopen()
	//Parameters have same purpose as in fopen()

blah_bool blah_file_readFloat32(FILE *file, blah_float32 *dest);
	//Reads a 32bit floating point value from binary file pointer into 'dest'
	//Returns true on success, false on error

blah_bool blah_file_readInt16(FILE *file, blah_int16 *dest);
	//Reads a 16bit signed integer value from binary file pointer into 'dest'
	//Returns true on success, false on error
	
blah_bool blah_file_readInt32(FILE *file, blah_int32 *dest);
	//Reads a 32bit signed integer value from binary file pointer into 'dest'
	//Returns true on success, false on error
	
blah_bool blah_file_readUnsigned16(FILE *file, blah_unsigned16 *dest);
	//Reads a 16bit unsigned integer value from binary file pointer into 'dest'
	//Returns true on success, false on error
	
blah_bool blah_file_readUnsigned32(FILE *file, blah_unsigned32 *dest);
	//Reads a 32bit unsigned integer value from binary file pointer into 'dest'
	//Returns true on success, false on error

char *blah_file_readString(FILE *file);
	//Reads a null terminated string from the give file pointer.
	//Returns pointer to allocated string on success, null on error.


#ifdef __cplusplus
	}
#endif //__cplusplus

#endif
