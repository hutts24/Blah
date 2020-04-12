/* blah_iff.h
	Defines common functions for files that comply to the IFF
	(interchangeable file format), 	*/


#ifndef _BLAH_IFF

#define _BLAH_IFF

#include "blah_types.h"
#include <stdio.h>

/* Definitions */

#define BLAH_IFF_CHUNK_HEADER_LENGTH 8	//Number of bytes in a chunk header (type + length)
#define BLAH_IFF_SUBCHUNK_HEADER_LENGTH 6	//Number of bytes in a subchunk header (type + length)

/* Data Structures */

typedef struct Blah_IFF_Chunk {
	blah_unsigned32 idTag;			//sequence of four bytes, identifies chunk type
	blah_unsigned32 dataLength;	//length of the data in the chunk (in bytes)
	blah_unsigned32 chunkLength;	//Total length of chunk (including header tag and pad byte if present)
	FILE *filePointer;				//file pointer holds chunk data, after the header
	bool padBytePresent;		//Signifies if a pad byte of data is required to make even data length
	blah_unsigned32 currentOffset; //current read position within data (offset into chunk data)
} Blah_IFF_Chunk;

typedef struct Blah_IFF_Subchunk {
	blah_unsigned32 idTag;		//sequence of four bytes, identifies subchunk type
	blah_unsigned16 dataLength;	//length of the data in the subchunk (in bytes)
	blah_unsigned32 subchunkLength;	//Total length of subchunk (including header tag and pad byte if present)
	Blah_IFF_Chunk *parentChunk;	//Parent chunk that holds chunk data
	bool padBytePresent;		//Signifies if a pad byte of data is required to make even data length
	blah_unsigned32 currentOffset; //current read position (offset into subchunk data)
} Blah_IFF_Subchunk;

/* Function Prototypes */

#ifdef __cplusplus
	extern "C" {
#endif //__cplusplus

void Blah_IFF_Chunk_destroy(Blah_IFF_Chunk *chunk);
	//Destroys chunk structure pointed to by chunk

bool Blah_IFF_Chunk_get(Blah_IFF_Chunk *chunk, FILE *file);
	//Discards current settings of given chunk object and connects it to the specified
	//file pointer

bool Blah_IFF_Chunk_init(Blah_IFF_Chunk *chunk, FILE *file);
	//Initialises an IFF chunk structure from given file_pointer
	//All it really does is call Blah_IFF_Chunk_get()
	//Returns TRUE on success on FALSE on error.

Blah_IFF_Chunk *Blah_IFF_Chunk_new(FILE *file);
	//Creates a IFF chunk structure from given file_pointer
	//Returns NULL on error

bool Blah_IFF_Chunk_readFloat32(Blah_IFF_Chunk *chunk, blah_float32 *dest);
	//Reads a 32bit floating point value from IFF chunk into 'dest'
	//Returns true on success, false on error

bool Blah_IFF_Chunk_readUnsigned8(Blah_IFF_Chunk *chunk, blah_unsigned8 *dest);
	//Reads a 8bit unsigned integer value from IFF Chunk into 'dest'
	//Returns true on success, false on error

bool Blah_IFF_Chunk_readInt16(Blah_IFF_Chunk *chunk, blah_int16 *dest);
	//Reads a 16bit signed integer value from IFF Chunk into 'dest'
	//Returns true on success, false on error

bool Blah_IFF_Chunk_readUnsigned16(Blah_IFF_Chunk *chunk,
	blah_unsigned16 *dest);
	//Reads a 16bit unsigned integer value from IFF Chunk into 'dest'

bool Blah_IFF_Chunk_readUnsigned32(Blah_IFF_Chunk *chunk,
	blah_unsigned32 *dest);
	//Reads a 32bit unsigned integer value from IFF chunk into 'dest'
	//Returns true on success, false on error

size_t Blah_IFF_Chunk_read(Blah_IFF_Chunk *chunk, void *dest, size_t numBytes);
	//Reads num_bytes of information from chunk to dest
	//Like fread, returns 0 on error or EOF.  Either way that's the end.

char *Blah_IFF_Chunk_readString(Blah_IFF_Chunk *chunk);
	//Reads a null terminated character string from chunk
	//Returns pointer to allocated string on success, NULL on error

int Blah_IFF_Chunk_seek(Blah_IFF_Chunk *chunk, long offset);
	//Seeks into chunk file 'offset' number of points from current position
	//Currently no error checking

/* Subchunk Prototypes */

Blah_IFF_Subchunk *Blah_IFF_Subchunk_new(Blah_IFF_Chunk *chunk);
	//Creates a IFF subchunk structure from given chunk pointer
	//Returns NULL on error

void Blah_IFF_Subchunk_destroy(Blah_IFF_Subchunk *subchunk);
	//Destroys chunk structure pointed to by subchunk

bool Blah_IFF_Subchunk_get(Blah_IFF_Subchunk *subchunk, Blah_IFF_Chunk *chunk);
	//Discards current settings of given subchunk object and connects it to the specified
	//chunk pointer

bool Blah_IFF_Subchunk_init(Blah_IFF_Subchunk *subchunk, Blah_IFF_Chunk *chunk);
	//Intialises a subchunk structure, given parent chunk structure.
	//All it does is call Blah_IFF_Subchunk_get()

bool Blah_IFF_Subchunk_readUnsigned8(Blah_IFF_Subchunk *subchunk, blah_unsigned8 *dest);
	//Reads a 8bit unsigned integer value from IFF subchunk into 'dest'
	//Returns true on success, false on error

bool Blah_IFF_Subchunk_readInt16(Blah_IFF_Subchunk *subchunk, blah_int16 *dest);
	//Reads a 16bit signed integer value from IFF subchunk into 'dest'
	//Returns true on success, false on error

bool Blah_IFF_Subchunk_readUnsigned16(Blah_IFF_Subchunk *subchunk,
	blah_unsigned16 *dest);
	//Reads a 16bit unsigned integer value from IFF subchunk into 'dest'

bool Blah_IFF_Subchunk_readUnsigned32(Blah_IFF_Subchunk *subchunk,
	blah_unsigned32 *dest);
	//Reads a 32bit unsigned integer value from IFF subchunk into 'dest'
	//Returns true on success, false on error

bool Blah_IFF_Subchunk_readFloat32(Blah_IFF_Subchunk *subchunk, blah_float32 *dest);
	//Reads a 32bit floating point value from IFF subchunk into 'dest'
	//Returns true on success, false on error

size_t Blah_IFF_Subchunk_read(Blah_IFF_Subchunk *subchunk, void *dest, size_t numBytes);
	//Reads num_bytes of information from subchunk to dest
	//Like fread, returns 0 on error or EOF.  Either way that's the end.

char *Blah_IFF_Subchunk_readString(Blah_IFF_Subchunk *subchunk);
	//Reads a null terminated character string from subchunk
	//Returns pointer to allocated string on success, NULL on error

int Blah_IFF_Subchunk_seek(Blah_IFF_Subchunk *subchunk, long offset);
	//Seeks into chunk data 'offset' number of points from current position
	//Currently no error checking

#ifdef __cplusplus
	}
#endif //__cplusplus

#endif
