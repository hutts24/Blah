/* blah_image_targa.c
	Defines functions for using targa image files
	All images are stored in raw pixel data	raster format
	top left through to bottom right */

#include <malloc.h>
#include <string.h>

#include "blah_image_targa.h"

/* Private Function Prototypes */

static void* Blah_Image_Targa_loadMapped(FILE *fileStream,  Blah_Image_Targa_Header *header,  char *imageName);
	//Subfunction to deal with uncompressed colour mapped targas

static void* Blah_Image_Targa_loadRLEMapped(FILE *fileStream,  Blah_Image_Targa_Header *header,  char *imageName);
	//Subfunction to deal with run length encoded colour mapped targas

static void* Blah_Image_Targa_loadRGB(FILE *fileStream,  Blah_Image_Targa_Header *header,  char *imageName);
	//Subfunction to deal with raw RGB targas

static void* Blah_Image_Targa_loadRLERGB(FILE *fileStream,  Blah_Image_Targa_Header *header,  char *imageName);
	//Subfunction to deal with run length encoded RGB targas

/* Function Declarations */

void Blah_Image_Targa_print_info(Blah_Image_Targa_Header *header)
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
}

Blah_Image *Blah_Image_Targa_fromFile(const char *filename, FILE *fileStream)
{
    // TODO
    // Creates a new Image structure.  Memory is allocated etc
	Blah_Image *newImage = NULL; //Pointer for new Image structure


	return newImage; //Return pointer whether it be null or valid image
}

/* bool Blah_Image_Targa_load(Blah_Image* image, const char* filename, FILE* const fileStream) {
	Blah_Image_Targa_Header header;		//Stores targa information
	char tempHeader[18];	//Temporary storage to read header from file

	if (fread(tempHeader, 18, 1, fileStream) != 1)
		fprintf(stderr,"image_load_targa: Could not load image header\n");
	else { //Construct targa header structure
		header.idFieldLength = tempHeader[0];
		header.colourMapType = tempHeader[1];
		header.imageTypeCode = tempHeader[2];
		header.colourMapOrigin = *(short*)(tempHeader+3);
		header.colourMapCount = *(short*)(tempHeader+5);
		header.colourMapEntrySize = tempHeader[7];
		header.imageOriginX = *(short*)(tempHeader+8);
		header.imageOriginY = *(short*)(tempHeader+10);
		header.width = *(short*)(tempHeader+12);
		header.height = *(short*)(tempHeader+14);
		header.pixelSize = tempHeader[16];
		header.imageDescriptor = tempHeader[17];

		//Skip Image identification data to colour map
		if (fseek(fileStream, header.idFieldLength, SEEK_CUR)==-1) //if error
			fprintf(stderr, "Couldn't seek past identification field");
		else {
			switch (header.imageTypeCode) {
				case BLAH_IMAGE_TARGA_MAPPED :
					Blah_Image_Targa_loadMapped(image, fileStream, &header, filename);
					break;
				case BLAH_IMAGE_TARGA_RLE_MAPPED :
					Blah_Image_Targa_loadRLEMapped(image, fileStream, &header, filename);
					break;
				case BLAH_IMAGE_TARGA_RGB :
					Blah_Image_Targa_loadRGB(image, fileStream, &header, filename);
					break;
				case BLAH_IMAGE_TARGA_RLE_RGB :
					Blah_Image_Targa_loadRLERGB(image, fileStream, &header, filename);
					break;
				default:
					fprintf(stderr,"Unsupported file type\n");
					break;
			}
		}
	}
	return true;
} */

static void *Blah_Image_Targa_loadMapped(FILE *fileStream, Blah_Image_Targa_Header *header, char *imageName)
{	//Subfunction to deal with uncompressed colour mapped targas
	Blah_Image *newImage;

	unsigned int numPixels = header->width * header->height; //Total number of pixels in image
	unsigned char mapEntryByteSize = header->colourMapEntrySize >> 3;
	unsigned char pixelByteSize = header->pixelSize >> 3;
	unsigned long colourMapSize = mapEntryByteSize * header->colourMapCount;

	void *colourMap;		//Temporary storage for colour map
	void *indexBuffer;		//Temporary pointer to store pixel indices
	void *tempRasterPointer;  //Navigates constructed raster buffer
	void *tempIndexPointer;	//Navigates pixel index buffer

	unsigned int tempIndex;
	unsigned int pixelCounter;	//temporary loop control value

	//Create new image structure
	newImage = Blah_Image_new(imageName, header->colourMapEntrySize, header->width,
		header->height,	mapEntryByteSize==3 ? BLAH_PIXEL_FORMAT_BGR : BLAH_PIXEL_FORMAT_BGRA);
	//Allocate temp storage for colour map
	colourMap = (void*)malloc(colourMapSize); //Temporary pointer to store colour map data
	//Read colour map from file into buffer
	fread(colourMap, colourMapSize, 1, fileStream);
	//Allocate index buffer
	indexBuffer = (void*)malloc(numPixels * pixelByteSize);
	//Read pixel map indexes into buffer
	fread(indexBuffer, pixelByteSize, numPixels, fileStream);
	//Construct raw image from colour map indices

	tempIndexPointer = indexBuffer;
	tempRasterPointer = newImage->pixelData;
	//temp_raster pointer will be used to navigate raster buffer
	for (pixelCounter = 0 ; pixelCounter < numPixels ; pixelCounter++) {
		tempIndex = 0;
		//Get next palette index from pixel index buffer and store in temp_index
		memcpy(&tempIndex, tempIndexPointer, pixelByteSize);
		//fprintf(stderr,"Pixel index:%d\n",temp_index);
		tempIndexPointer+=pixelByteSize;  //Advance pointer to next index
		memcpy(tempRasterPointer, colourMap + ((tempIndex + header->colourMapOrigin) * mapEntryByteSize), mapEntryByteSize);
		//Copy colour map entry data for pixel into raster data
		tempRasterPointer+=mapEntryByteSize; //Advance raster data pointer
	}

	free(colourMap);	//temporary colour map no longer needed
	free(indexBuffer);	//free index buffer

	return newImage;	//Return complete raw image data
}

static void *Blah_Image_Targa_loadRLEMapped(FILE *fileStream, Blah_Image_Targa_Header *header, char *imageName)
{	//Subfunction to deal with run length encoded colour mapped targas
	Blah_Image *newImage;

	unsigned int numPixels = header->width * header->height; //Total number of pixels in image
	unsigned char mapEntryByteSize = header->colourMapEntrySize >> 3;
	unsigned char pixelByteSize = header->pixelSize >> 3;
	unsigned long colourMapSize = mapEntryByteSize * header->colourMapCount;

	void *colourMap;		//Temporary storage for colour map
	void *indexBuffer;		//Temporary pointer to store pixel indices
	void *tempRasterPointer;  //Navigates constructed raster buffer
	void *tempIndexPointer;	//Navigates pixel index buffer
	void *tempColourMapPointer;

	unsigned int tempIndex;
	long remainingPixels;	//counts down constructed pixels till complete (0) left
	unsigned char pixelPacket;
	unsigned char runLength;

	//Create new image structure
	newImage = Blah_Image_new(imageName, header->colourMapEntrySize, header->width,
		header->height,	mapEntryByteSize==3 ? BLAH_PIXEL_FORMAT_BGR : BLAH_PIXEL_FORMAT_BGRA);

	//Allocate temp storage for colour map
	colourMap = (void*)malloc(colourMapSize); //Temporary pointer to store colour map data
	//Read colour map from file into buffer
	fread(colourMap, colourMapSize, 1, fileStream);
	//Allocate index buffer for largest possible run of indices
	indexBuffer = (void*)malloc(128 * pixelByteSize);
	//Construct raw image from colour map indices

	tempRasterPointer = newImage->pixelData;
	//temp_raster pointer will be used to navigate raster buffer
	remainingPixels = numPixels;
	while (remainingPixels > 0) {
		fread(&pixelPacket, 1, 1, fileStream); //get next packet byte from file
		runLength = (pixelPacket & 127) +1 ; //Get run length from 7 other bits
		remainingPixels -= runLength; //update total remaining pixels
		if (pixelPacket & 128) { //test if high bit (7) is set.  If so, run follows

			//Process run
			tempIndex = 0;
			fread(&tempIndex, pixelByteSize, 1, fileStream);
			//Read repeated pixel index from file following run length packet
			tempColourMapPointer = colourMap + ((tempIndex + header->colourMapOrigin) * mapEntryByteSize);
			while (runLength > 0) {
				memcpy(tempRasterPointer, tempColourMapPointer, mapEntryByteSize);
				//Copy colour map entry data for pixel into raster data
				tempRasterPointer+=mapEntryByteSize;
				runLength--;
			}
		} else { //Read run of raw indices
			fread(indexBuffer, pixelByteSize, runLength, fileStream);
			tempIndexPointer = indexBuffer; //Set temp index pointer to start of buffer
			while (runLength > 0) {
				tempIndex = 0;
				//Get next palette index from pixel index buffer and store in temp_index
				memcpy(&tempIndex, tempIndexPointer, pixelByteSize);
				tempIndexPointer+=pixelByteSize;  //Advance pointer to next index
				memcpy(tempRasterPointer, colourMap + ((tempIndex + header->colourMapOrigin) * mapEntryByteSize), mapEntryByteSize);
				//Copy colour map entry data for pixel into raster data
				tempRasterPointer+=mapEntryByteSize; //Advance raster data pointer
				runLength--;
			}
		}
	}

	free(colourMap);	//temporary colour map no longer needed
	free(indexBuffer);	//free index buffer

	return newImage;	//Return complete raw image data
}

static void *Blah_Image_Targa_loadRGB(FILE *fileStream, Blah_Image_Targa_Header *header, char *imageName)
{	//Subfunction to deal with uncompressed raw RGB targas
	Blah_Image *newImage;

	unsigned int numPixels = header->width * header->height; //Total number of pixels in image
	unsigned char mapEntryByteSize = header->colourMapType ? header->colourMapEntrySize >> 3 : 0;
	unsigned char pixelByteSize = header->pixelSize >> 3;
	unsigned long colourMapSize = mapEntryByteSize * header->colourMapCount;

	//Create the new image structure
	newImage = Blah_Image_new(imageName, header->pixelSize, header->width,
		header->height, pixelByteSize==3 ? BLAH_PIXEL_FORMAT_BGR : BLAH_PIXEL_FORMAT_BGRA);

	//Skip colour map in file if there is one defined
	fseek(fileStream, colourMapSize, SEEK_CUR);

	//Construct raw image directly from file
	fread(newImage->pixelData, pixelByteSize, numPixels, fileStream);

	return newImage;	//Return complete raw image data
}

static void *Blah_Image_Targa_loadRLERGB(FILE *fileStream, Blah_Image_Targa_Header *header, char *imageName)
{	//Subfunction to deal with run length encoded RGB targas
	Blah_Image *newImage;

	unsigned int numPixels = header->width * header->height; //Total number of pixels in image
	unsigned char mapEntryByteSize = header->colourMapType ? header->colourMapEntrySize >> 3 : 0;
	unsigned char pixelByteSize = header->pixelSize >> 3;
	unsigned long colourMapSize = mapEntryByteSize * header->colourMapCount;

	void *tempRasterPointer;  //Navigates constructed raster buffer

	unsigned int tempPixel;
	long remainingPixels;	//counts down constructed pixels till complete (0) left
	unsigned char pixelPacket;
	unsigned char runLength;

	//Create the new image structure
	newImage = Blah_Image_new(imageName, header->pixelSize, header->width,
		header->height, pixelByteSize==3 ? BLAH_PIXEL_FORMAT_BGR : BLAH_PIXEL_FORMAT_BGRA);

	//Skip colour map in file if there is one defined
	fseek(fileStream, colourMapSize, SEEK_CUR);

	//Construct raw image directly from file
	tempRasterPointer = newImage->pixelData;

	//temp_raster pointer will be used to navigate raster buffer
	remainingPixels = numPixels;
	while (remainingPixels > 0) {
		fread(&pixelPacket, 1, 1, fileStream); //get next packet byte from file
		runLength = (pixelPacket & 127) +1 ; //Get run length from 7 other bits
		remainingPixels -= runLength; //update total remaining pixels
		if (pixelPacket & 128) { //test if high bit (7) is set.  If so, run follows
			//Process repeated run
			tempPixel = 0;
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

	return newImage;	//Return complete raw image data
}
