/* blah_image_targa.h
	Header file for blah_image_targa.c
	Defines image structures and functions for manipulating images
	All images are stored in raw pixel data	raster format
	top left through to bottom right */

#ifndef _BLAH_IMAGE_TARGA

#define _BLAH_IMAGE_TARGA

#include <stdio.h>
#include "blah_image.h"

/* Defines */

//Targa Types
#define BLAH_IMAGE_TARGA_MAPPED		1	//Uncompressed, color-mapped images.
#define BLAH_IMAGE_TARGA_RGB			2	//Uncompressed, RGB images.
#define BLAH_IMAGE_TARGA_RLE_MAPPED	9	//Runlength encoded color-mapped images.
#define BLAH_IMAGE_TARGA_RLE_RGB		10	//Runlength encoded RGB images.

/* Data Structures */

typedef struct Blah_Image_Targa_Header { //Represents a raw data image in raster patter
	unsigned char idFieldLength;  //offset 0 - number of chars in indentification data
	unsigned char colourMapType;	//offset 1 - notes the format of the pixel data
	unsigned char imageTypeCode;  //offset 2 - Image format code (rgb, run encoded)
	unsigned short colourMapOrigin;			//offset 3 - index of first colour map entry
	unsigned short colourMapCount;			//offset 5 - number of colour map entries
	unsigned char colourMapEntrySize; //offset 7 - number of bits in each map entry
	unsigned short imageOriginX;				//offset 8 - x coord of lower left corner of image
	unsigned short imageOriginY;				//offset 10 - y coord of lower left corner of image
	unsigned short width;						//offset 12 - width in pixels
	unsigned short height;						//offset 14 - height in pixels
	unsigned char pixelSize;		//offset 16 - size of pixels or indices (in bits)
	unsigned char imageDescriptor;	//offset 17 - Should be zero?
} Blah_Image_Targa_Header;

/* Function Prototypes */

#ifdef __cplusplus
	extern "C" {
#endif //__cplusplus

Blah_Image *Blah_Image_Targa_fromFile(const char *filename, FILE *fileStream);
	//Creates a new Image structure from file 'filename'.  Memory is allocated etc

void Blah_Image_Targa_printInfo(Blah_Image_Targa_Header *header);
	//Prints info to the screen about targa, extracted from header information

#ifdef __cplusplus
	}
#endif //__cplusplus

#endif
