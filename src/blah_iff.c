/* blah_iff.c
	Defines common functions on files conforming to IFF,
	(Interchangeable File Format) */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "blah_file.h"
#include "blah_iff.h"
#include "blah_types.h"
#include "blah_util.h"

/* Public Function Declarations */

void Blah_IFF_Chunk_destroy(Blah_IFF_Chunk *chunk)
{	//Destroys chunk structure pointed to by chunk
	free(chunk);
}

blah_bool Blah_IFF_Chunk_get(Blah_IFF_Chunk *chunk, FILE *file)
{	//Discards current settings of given chunk object and connects it to the specified
	//file pointer.  Returns 0/false on error, 1/true on success

	blah_unsigned32 chunkId, dataLength;

	fread(&chunkId, 4, 1, file); //Read 4 byte chunk_id tag into variable
	blah_file_readUnsigned32(file, &dataLength); //Get chunk length

	chunk->idTag = chunkId;
	chunk->dataLength = dataLength;
	chunk->padBytePresent = dataLength & 1 ? BLAH_TRUE : BLAH_FALSE;
	//If data length is uneven, we need a pad byte
	chunk->chunkLength = BLAH_IFF_CHUNK_HEADER_LENGTH + dataLength +
		(chunk->padBytePresent ? 1 : 0); //Assign total chunk length

	chunk->filePointer = file;
	chunk->currentOffset = 0;

	return BLAH_TRUE;
}

blah_bool Blah_IFF_Chunk_init(Blah_IFF_Chunk *chunk, FILE *file)
{	//Initialises an IFF chunk structure from given file_pointer
	//All it really does is call Blah_IFF_Chunk_get()
	//Returns TRUE on success on FALSE on error.
	return Blah_IFF_Chunk_get(chunk, file);
}

Blah_IFF_Chunk *Blah_IFF_Chunk_new(FILE *file)
{	//Creates a IFF chunk structure from given file_pointer
	//Returns NULL on error
	Blah_IFF_Chunk *newChunk = (Blah_IFF_Chunk*)malloc(sizeof(Blah_IFF_Chunk));

	if (!Blah_IFF_Chunk_init(newChunk, file)) //If intialisation failed
	{
		free(newChunk); //Free the allocated memory and return NULL pointer
		newChunk = NULL;
	}

	return newChunk;
}

blah_bool Blah_IFF_Chunk_readInt16(Blah_IFF_Chunk *chunk, blah_int16 *dest)
{	//Reads a 16bit signed integer value from IFF Chunk into 'dest'
	//Returns true on success, false on error
	chunk->currentOffset+=2; //Increment offset by 2 bytes (16bits)
	return blah_file_readInt16(chunk->filePointer, dest);
}

bool Blah_IFF_Chunk_readUnsigned8(Blah_IFF_Chunk *chunk, blah_unsigned8 *dest)
{	//Reads a 8bit unsigned integer value from IFF Chunk into 'dest'
	//Returns true on success, false on error
	chunk->currentOffset++; //Increment offset by 2 bytes (16bits)
	*dest = fgetc(chunk->filePointer);
	return *dest != EOF;
}

blah_bool Blah_IFF_Chunk_readUnsigned16(Blah_IFF_Chunk *chunk, blah_unsigned16 *dest)
{	//Reads a 16bit unsigned integer value from IFF Chunk into 'dest'
	//Returns true on success, false on error
	chunk->currentOffset+=2; //Increment offset by 2 bytes (16bits)
	return blah_file_readUnsigned16(chunk->filePointer, dest);
}

blah_bool Blah_IFF_Chunk_readUnsigned32(Blah_IFF_Chunk *chunk, blah_unsigned32 *dest)
{	//Reads a 32bit unsigned integer value from IFF chunk into 'dest'
	//Returns true on success, false on error
	chunk->currentOffset+=4; //Increment offset by 4 bytes (32bits)
	return blah_file_readUnsigned32(chunk->filePointer, dest);
}

size_t Blah_IFF_Chunk_read(Blah_IFF_Chunk *chunk, void *dest, size_t numBytes)
{	//Reads num_bytes of information from chunk to dest
	//Like fread, returns 0 on error or EOF.  Either way that's the end.
	chunk->currentOffset+=numBytes; //Increment offset by num_bytes
	return fread(dest, numBytes, 1, chunk->filePointer);
}

blah_bool Blah_IFF_Chunk_readFloat32(Blah_IFF_Chunk *chunk, blah_float32 *dest)
{	//Reads a 32bit floating point value from IFF chunk into 'dest'
	//Returns true on success, false on error
	chunk->currentOffset+=4; //Increment offset by 4 bytes (32bits)
	return blah_file_readFloat32(chunk->filePointer, dest);
}

char *Blah_IFF_Chunk_readString(Blah_IFF_Chunk *chunk)
{	//Reads a null terminated character string from chunk
	//Returns pointer to allocated string on success, NULL on error
	char *returnString = blah_file_readString(chunk->filePointer);
	blah_unsigned32 dataLength;
	if (returnString) {
		dataLength = strlen(returnString) + 1; //string length +1 for null char
		if (dataLength & 1) {
			dataLength++;
			fseek(chunk->filePointer,1, SEEK_CUR);
		}
		chunk->currentOffset+=dataLength;
	} //If successful, update chunk pointer
	return returnString;
}

int Blah_IFF_Chunk_seek(Blah_IFF_Chunk *chunk, long offset)
{	//Seeks into chunk file 'offset' number of points from current position
	//Currently no error checking
	chunk->currentOffset+=offset;
	return fseek(chunk->filePointer, offset, SEEK_CUR);
}

/* Sub Chunk Functions */

void Blah_IFF_Subchunk_destroy(Blah_IFF_Subchunk *subchunk)
{	//Destroys chunk structure pointed to by subchunk
	free(subchunk);
}

blah_bool Blah_IFF_Subchunk_get(Blah_IFF_Subchunk *subchunk, Blah_IFF_Chunk *chunk)
{	//Discards current settings of given subchunk object and connects it to the specified
	//file pointer
	blah_unsigned32 subchunkId;
	blah_unsigned16 dataLength;

	Blah_IFF_Chunk_read(chunk, &subchunkId, sizeof(blah_unsigned32));
	//Read 4 byte chunk_id tag into variable
	Blah_IFF_Chunk_readUnsigned16(chunk, &dataLength); //Get chunk length

	subchunk->idTag = subchunkId;
	subchunk->dataLength = dataLength;
	subchunk->padBytePresent = dataLength & 1 ? BLAH_TRUE : BLAH_FALSE;
	//If data length is uneven, we need a pad byte
	subchunk->subchunkLength = BLAH_IFF_SUBCHUNK_HEADER_LENGTH + dataLength +
		(subchunk->padBytePresent ? 1 : 0); //Assign total chunk length

	subchunk->parentChunk = chunk;
	subchunk->currentOffset = 0;

	return BLAH_TRUE;
}

blah_bool Blah_IFF_Subchunk_init(Blah_IFF_Subchunk *subchunk, Blah_IFF_Chunk *chunk)
{	//Intialises a subchunk structure, given parent chunk structure.
	//All it does is call Blah_IFF_Subchunk_get()
	return Blah_IFF_Subchunk_get(subchunk, chunk);
}

Blah_IFF_Subchunk *Blah_IFF_Subchunk_new(Blah_IFF_Chunk *chunk)
{	//Creates a IFF subchunk structure from given chunk pointer
	//Returns NULL on error

	Blah_IFF_Subchunk *newSubchunk = (Blah_IFF_Subchunk*)malloc(sizeof(Blah_IFF_Subchunk));

	if (!Blah_IFF_Subchunk_init(newSubchunk, chunk)) //If failed to initialise subchunk
	{
		Blah_IFF_Subchunk_destroy(newSubchunk); //Free allocated memory
		newSubchunk = NULL; //Return NULL pointer
	}

	return newSubchunk;
}

blah_bool Blah_IFF_Subchunk_readUnsigned8(Blah_IFF_Subchunk *subchunk, blah_unsigned8 *dest)
{	//Reads a 8bit unsigned integer value from IFF subchunk into 'dest'
	//Returns true on success, false on error
	subchunk->currentOffset++; //Increment offset by 2 bytes (16bits)
	return Blah_IFF_Chunk_readUnsigned8(subchunk->parentChunk, dest);
}

blah_bool Blah_IFF_Subchunk_readInt16(Blah_IFF_Subchunk *subchunk, blah_int16 *dest)
{	//Reads a 16bit signed integer value from IFF subchunk into 'dest'
	//Returns true on success, false on error
	subchunk->currentOffset+=2; //Increment offset by 2 bytes (16bits)
	return Blah_IFF_Chunk_readInt16(subchunk->parentChunk, dest);
}

blah_bool Blah_IFF_Subchunk_readUnsigned16(Blah_IFF_Subchunk *subchunk, blah_unsigned16 *dest)
{	//Reads a 16bit unsigned integer value from IFF subchunk into 'dest'
	subchunk->currentOffset+=2; //Increment offset by 2 bytes (16bits)
	return Blah_IFF_Chunk_readUnsigned16(subchunk->parentChunk, dest);
}

blah_bool Blah_IFF_Subchunk_readUnsigned32(Blah_IFF_Subchunk *subchunk, blah_unsigned32 *dest)
{	//Reads a 32bit unsigned integer value from IFF subchunk into 'dest'
	//Returns true on success, false on error
	subchunk->currentOffset+=4; //Increment offset by 4 bytes (32bits)
	return Blah_IFF_Chunk_readUnsigned32(subchunk->parentChunk, dest);
}

blah_bool Blah_IFF_Subchunk_readFloat32(Blah_IFF_Subchunk *subchunk, blah_float32 *dest)
{	//Reads a 32bit floating point value from IFF subchunk into 'dest'
	//Returns true on success, false on error
	subchunk->currentOffset+=4; //Increment offset by 4 bytes (32bits)
	return Blah_IFF_Chunk_readFloat32(subchunk->parentChunk, dest);
}

size_t Blah_IFF_SubchunkRead(Blah_IFF_Subchunk *subchunk, void *dest, size_t numBytes)
{	//Reads num_bytes of information from subchunk to dest
	//Like fread, returns 0 on error or EOF.  Either way that's the end.
	subchunk->currentOffset+=numBytes; //Increment offset by num_bytes
	return Blah_IFF_Chunk_read(subchunk->parentChunk, dest, numBytes);
}

char *Blah_IFF_Subchunk_readString(Blah_IFF_Subchunk *subchunk)
{	//Reads a null terminated character string from subchunk
	//Returns pointer to allocated string on success, NULL on error
	char *returnString = Blah_IFF_Chunk_readString(subchunk->parentChunk);
	blah_unsigned32 dataLength;
	if (returnString) {
		dataLength = strlen(returnString) +1; //Add 1 for null char
		if (dataLength & 1)
			dataLength++;
		subchunk->currentOffset+=dataLength;
	}
	//If successful, update chunk pointer
	return returnString;
}

int Blah_IFF_Subchunk_seek(Blah_IFF_Subchunk *subchunk, long offset)
{	//Seeks into chunk data 'offset' number of points from current position
	//Currently no error checking
	subchunk->currentOffset+=offset;
	return Blah_IFF_Chunk_seek(subchunk->parentChunk, offset);
}
