/* blah_image_targa.c
	Defines functions for using targa image files
	All images are stored in raw pixel data	raster format
	top left through to bottom right */

#include <malloc.h>
#include <string.h>

#include "blah_image_targa.h"
#include "blah_error.h"

#define BLAH_IMAGE_TARGA_HEADER_LENGTH 18

/* Extern function declarations */
extern bool Blah_Image_init(Blah_Image *image, const char *name, unsigned char pixelDepth, unsigned int width, unsigned int height, blah_pixel_format pixFormat);

/* Private Function Prototypes */


/* Static Function Definitions */

/* static void Blah_Image_Targa_print_info(Blah_Image_Targa_Header *header)
{	//Prints info to the screen about targa, extracted from header information
	fprintf(stderr,"targa id field length:%d\n",header->idFieldLength);
	fprintf(stderr,"targa colour map type:%d\n",header->colourMapType);
	fprintf(stderr,"targa image type:%d\n",header->imageTypeCode);
	fprintf(stderr,"targa image map origin:%d\n",header->colourMapOrigin);
	fprintf(stderr,"targa image map count:%d\n",header->colourMapCount);
	fprintf(stderr,"targa image entry size:%d\n",header->colourMapEntrySize);
	fprintf(stderr,"targa image origin x:%d\n",header->imageOriginX);
	fprintf(stderr,"targa image origin y:%d\n",header->imageOriginY);
	fprintf(stderr,"targa image width:%d\n",header->width);
	fprintf(stderr,"targa image height:%d\n",header->height);
	fprintf(stderr,"targa image pixel size:%d\n",header->pixelSize);
	fprintf(stderr,"targa image descriptor:%d\n",header->imageDescriptor);
} */

// Subfunction to deal with uncompressed colour mapped targas
// Load targa image from file into raw pixel data in memory
static bool Blah_Image_Targa_loadMapped(Blah_Image* newImage, FILE *fileStream, const Blah_Image_Targa_Header* header, const char* imageName)
{
	unsigned int numPixels = header->width * header->height; //Total number of pixels in image
	unsigned char mapEntryByteSize = header->colourMapEntrySize >> 3;
	unsigned char pixelByteSize = header->pixelSize >> 3;
	unsigned long colourMapSize = mapEntryByteSize * header->colourMapCount;

	// Initialise the image structure dimensions etc and with allocated raster data buffer
	Blah_Image_init(newImage, imageName, header->colourMapEntrySize, header->width,
		header->height,	mapEntryByteSize == 3 ? BLAH_PIXEL_FORMAT_BGR : BLAH_PIXEL_FORMAT_BGRA);

	uint8_t colourMap[colourMapSize]; // Allocate Temporary pointer to store colour map data
	if (fread(colourMap, colourMapSize, 1, fileStream) < 1) { // Read colour map from file into buffer
        blah_error_raise(errno, "Failed to read colour map from targa image '%s'", imageName);
        return false;
	}

	uint8_t indexBuffer[numPixels * pixelByteSize]; // Allocate temporary buffer to store pixel indices
	if (fread(indexBuffer, pixelByteSize, numPixels, fileStream) < numPixels) { // Read pixel map indexes into buffer
        blah_error_raise(errno, "Failed to read pixel indices from targa image '%s'", imageName);
        return false;
	}
	// Construct raw image from colour map indices

	void* tempIndexPointer = indexBuffer; 	//Navigates pixel index buffer
	void* tempRasterPointer = newImage->pixelData; // Navigates constructed raster buffer
	//temp_raster pointer will be used to navigate raster buffer
	for (unsigned int pixelCounter = 0 ; pixelCounter < numPixels ; pixelCounter++) {
		unsigned int tempIndex = 0;
		//Get next palette index from pixel index buffer and store in temp_index
		memcpy(&tempIndex, tempIndexPointer, pixelByteSize);
		//fprintf(stderr,"Pixel index:%d\n",temp_index);
		tempIndexPointer += pixelByteSize;  //Advance pointer to next index
		memcpy(tempRasterPointer, colourMap + ((tempIndex + header->colourMapOrigin) * mapEntryByteSize), mapEntryByteSize);
		//Copy colour map entry data for pixel into raster data
		tempRasterPointer += mapEntryByteSize; //Advance raster data pointer
	}

	return true;	//Return complete raw image data
}

// Subfunction to deal with run length encoded colour mapped targas
static bool Blah_Image_Targa_loadRLEMapped(Blah_Image* newImage, FILE *fileStream, const Blah_Image_Targa_Header* header, const char* imageName)
{
	const size_t numPixels = header->width * header->height; // Total number of pixels in image
	const uint8_t mapEntryByteSize = header->colourMapEntrySize >> 3;
	const uint8_t pixelByteSize = header->pixelSize >> 3;
	const unsigned long colourMapSize = mapEntryByteSize * header->colourMapCount;

	// Initiase new image structure with dimensions and allocate pixel data buffer
	Blah_Image_init(newImage, imageName, header->colourMapEntrySize, header->width,
		header->height,	mapEntryByteSize == 3 ? BLAH_PIXEL_FORMAT_BGR : BLAH_PIXEL_FORMAT_BGRA);

    uint8_t colourMap[colourMapSize]; // Allocate temp storage for colour map
	if (fread(colourMap, colourMapSize, 1, fileStream) < 1) { // Read colour map from file into buffer
        blah_error_raise(errno, "Failed dot read colour map from targa image '%s'", imageName);
        return false;
	}

   	// Construct raw image from colour map indices
	uint8_t indexBuffer[128 * pixelByteSize]; // Allocate index buffer for largest possible run of indices
	void* tempRasterPointer = newImage->pixelData; // temp_raster pointer will be used to navigate raster buffer
	long remainingPixels = numPixels; // counts down constructed pixels till complete (0) left
	while (remainingPixels > 0) {
		uint8_t pixelPacket;
		fread(&pixelPacket, 1, 1, fileStream); //get next packet byte from file
		uint8_t runLength = (pixelPacket & 127) +1 ; //Get run length from 7 other bits
		remainingPixels -= runLength; //update total remaining pixels
		if (pixelPacket & 128) { //test if high bit (7) is set.  If so, run follows
            // Process run
			unsigned int tempIndex = 0;
			fread(&tempIndex, pixelByteSize, 1, fileStream);
			// Read repeated pixel index from file following run length packet
			void* tempColourMapPointer = colourMap + ((tempIndex + header->colourMapOrigin) * mapEntryByteSize);
			while (runLength > 0) {
				memcpy(tempRasterPointer, tempColourMapPointer, mapEntryByteSize);
				//Copy colour map entry data for pixel into raster data
				tempRasterPointer += mapEntryByteSize;
				runLength--;
			}
		} else { // Read run of raw indices
			fread(indexBuffer, pixelByteSize, runLength, fileStream);
			void* tempIndexPointer = indexBuffer; //Set temp index pointer to start of buffer
			while (runLength > 0) {
				unsigned int tempIndex = 0;
				// Get next palette index from pixel index buffer and store in temp_index
				memcpy(&tempIndex, tempIndexPointer, pixelByteSize);
				tempIndexPointer+=pixelByteSize;  //Advance pointer to next index
				memcpy(tempRasterPointer, colourMap + ((tempIndex + header->colourMapOrigin) * mapEntryByteSize), mapEntryByteSize);
				// Copy colour map entry data for pixel into raster data
				tempRasterPointer+=mapEntryByteSize; // Advance raster data pointer
				runLength--;
			}
		}
	}

	return true;	//Return complete raw image data
}

// Subfunction to deal with uncompressed raw RGB targas
static bool Blah_Image_Targa_loadRGB(Blah_Image *newImage, FILE *fileStream, const Blah_Image_Targa_Header* header, const char* imageName)
{
	const size_t numPixels = header->width * header->height; //Total number of pixels in image
	const uint8_t mapEntryByteSize = header->colourMapType ? header->colourMapEntrySize >> 3 : 0;
	const uint8_t pixelByteSize = header->pixelSize >> 3;
	const unsigned long colourMapSize = mapEntryByteSize * header->colourMapCount;

	// Initiase new image structure with dimensions and allocate pixel data buffer
	Blah_Image_init(newImage, imageName, header->pixelSize, header->width,
		header->height, pixelByteSize==3 ? BLAH_PIXEL_FORMAT_BGR : BLAH_PIXEL_FORMAT_BGRA);

	if (fseek(fileStream, colourMapSize, SEEK_CUR) != 0) { // Skip colour map in file if there is one defined
        blah_error_raise(errno, "Failed to seek past colour map in targa image '%s'", imageName);
        return false;
	}

	if (fread(newImage->pixelData, pixelByteSize, numPixels, fileStream) < numPixels) { // Construct raw image directly from file
        blah_error_raise(errno, "Failed to read RGB pixel data from targa image '%s'", imageName);
    }

	return true;	//Return complete raw image data
}

// Subfunction to deal with run length encoded RGB targas
static bool Blah_Image_Targa_loadRLERGB(Blah_Image* newImage, FILE *fileStream, const Blah_Image_Targa_Header* header, const char* imageName)
{
	const size_t numPixels = header->width * header->height; // Total number of pixels in image
	const uint8_t mapEntryByteSize = header->colourMapType ? header->colourMapEntrySize >> 3 : 0;
	const uint8_t pixelByteSize = header->pixelSize >> 3;
	const unsigned long colourMapSize = mapEntryByteSize * header->colourMapCount;

	// Initiase new image structure with dimensions and allocate pixel data buffer
	Blah_Image_init(newImage, imageName, header->pixelSize, header->width,
		header->height, pixelByteSize==3 ? BLAH_PIXEL_FORMAT_BGR : BLAH_PIXEL_FORMAT_BGRA);

	if (fseek(fileStream, colourMapSize, SEEK_CUR) != 0) { // Skip colour map in file if there is one defined
	    blah_error_raise(errno, "Failed to seek past colour map in targa image '%s'", imageName);
        return false;
	}

	//Construct raw image directly from file
	void* tempRasterPointer = newImage->pixelData; // temp_raster pointer will be used to navigate raster buffer
	long remainingPixels = numPixels; // counts down constructed pixels till complete (0) left
	while (remainingPixels > 0) {
        uint8_t pixelPacket;
		fread(&pixelPacket, 1, 1, fileStream); //get next packet byte from file
		uint8_t runLength = (pixelPacket & 127) +1 ; //Get run length from 7 other bits
		remainingPixels -= runLength; //update total remaining pixels
		if (pixelPacket & 128) { //test if high bit (7) is set.  If so, run follows
			//Process repeated run
			unsigned int tempPixel = 0;
			fread(&tempPixel, pixelByteSize, 1, fileStream);
			//Read repeated pixel index from file following run length packet
			while (runLength > 0) {
				memcpy(tempRasterPointer, &tempPixel, pixelByteSize);
				//Copy colour map entry data for pixel into raster data
				tempRasterPointer+=pixelByteSize;
				runLength--;
			}
		} else { //Read run of raw pixels
			fread(tempRasterPointer, pixelByteSize, runLength, fileStream);
			tempRasterPointer+=(runLength * pixelByteSize);
			//Advance raster data pointer
		}
	}

	return true;	//Return complete raw image data
}

// Read 18 bytes packed into the file and unpack into a useful structure
static bool Blah_Image_Targa_Header_load(Blah_Image_Targa_Header* dest, FILE* fileStream)
{
    // Read header from file into temporary storage
    char headerBytes[BLAH_IMAGE_TARGA_HEADER_LENGTH];
    if (fread(headerBytes, BLAH_IMAGE_TARGA_HEADER_LENGTH, 1, fileStream) != 1) {
		blah_error_raise(errno, "Could not load header for targa image");
		return false;
	}
    // Unpack header bytes into useful structure
    dest->idFieldLength = headerBytes[0];
    dest->colourMapType = headerBytes[1];
    dest->imageTypeCode = headerBytes[2];
    dest->colourMapOrigin = *(uint16_t*)(headerBytes+3);
    dest->colourMapCount = *(uint16_t*)(headerBytes+5);
    dest->colourMapEntrySize = headerBytes[7];
    dest->imageOriginX = *(uint16_t*)(headerBytes+8);
    dest->imageOriginY = *(uint16_t*)(headerBytes+10);
    dest->width = *(uint16_t*)(headerBytes+12);
    dest->height = *(uint16_t*)(headerBytes+14);
    dest->pixelSize = headerBytes[16];
    dest->imageDescriptor = headerBytes[17];
    return true;
}

static bool Blah_Image_Targa_load(Blah_Image* image, const char* imageName, FILE* fileStream) {
	Blah_Image_Targa_Header header;
	Blah_Image_Targa_Header_load(&header, fileStream);

	// Skip Image identification data to colour map
    if (fseek(fileStream, header.idFieldLength, SEEK_CUR) == -1) {
        blah_error_raise(errno, "Couldn't seek past identification field in targa image: %s", imageName);
        return false;
    }

    switch (header.imageTypeCode) {
        case BLAH_IMAGE_TARGA_MAPPED :
            Blah_Image_Targa_loadMapped(image, fileStream, &header, imageName);
            break;
        case BLAH_IMAGE_TARGA_RLE_MAPPED :
            Blah_Image_Targa_loadRLEMapped(image, fileStream, &header, imageName);
            break;
        case BLAH_IMAGE_TARGA_RGB :
            Blah_Image_Targa_loadRGB(image, fileStream, &header, imageName);
            break;
        case BLAH_IMAGE_TARGA_RLE_RGB :
            Blah_Image_Targa_loadRLERGB(image, fileStream, &header, imageName);
            break;
        default:
            fprintf(stderr,"Unsupported file type\n");
            break;
    }
	return true;
}

// Creates a new Image structure from targa file stream.  Memory is allocated etc
Blah_Image* Blah_Image_Targa_fromFile(const char *fileName, FILE *fileStream)
{
	Blah_Image *newImage = malloc(sizeof(Blah_Image)); //Pointer for new Image structure
    if (newImage != NULL) {
        Blah_Image_Targa_load(newImage, fileName, fileStream);
    }

	return newImage; //Return pointer whether it be null or valid image
}
