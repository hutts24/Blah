/* blah_image.c
	Defines functions for manipulating images
	All images are stored in raw pixel data	raster format
	top left through to bottom right */

#include <malloc.h>
#include <string.h>

#include "blah_image.h"
#include "blah_image_targa.h"
#include "blah_tree.h"
#include "blah_util.h"
#include "blah_file.h"
#include "blah_types.h"
#include "blah_error.h"

/* Private locals */
Blah_Tree imageTree = {"", NULL, NULL, 0}; //Binary tree of all images

/* Private Function Prototypes */

/* Function Declarations */

void Blah_Image_copyImage(Blah_Image *dest, unsigned int destX, unsigned int destY,
    const Blah_Image *source, unsigned int srcLeft, unsigned int srcRight, unsigned int srcBottom, unsigned int srcTop)
{	//Copies the region defined by src_left,src_right,src_bottom,src_top from
	//image 'source' to position dest_x,dest_y in image 'dest'
	const int copyWidth = srcRight - srcLeft + 1;
	const int copyHeight = srcTop - srcBottom + 1;
	Blah_Image_copyRasterData(source, NULL, srcLeft, srcBottom, copyWidth, copyHeight, dest->width - source->width);
}

void Blah_Image_copyRasterData(const Blah_Image *source, void *destination, unsigned int left, unsigned int bottom,
 unsigned int width, unsigned int height, unsigned int rowSkip)
{	// Copies image raster data from a region (width * height) within the source
	// image, origin defined by left,bottom to destination pointer.  Destination
	// pointer will skip an additional 'row_skip' bytes after each row is copied.
	const int pixelBytes = source->pixelDepth >> 3; // bytes per pixel in raster data
	const int sourceRowBytes = source->width * pixelBytes; // bytes in a row of the dest
	const int destRowBytes = width * pixelBytes; // bytes in a row of the source

	void* sourcePtr = source->pixelData + (sourceRowBytes * bottom) + (left * pixelBytes);
	// set pointer to beginning of copy region

	for (unsigned int rowCount = 0; rowCount < height; rowCount++) {
		memcpy(destination, sourcePtr, destRowBytes);
		destination += destRowBytes + rowSkip;
		sourcePtr += sourceRowBytes;
	}
}

void Blah_Image_destroy(Blah_Image *image)
{	//Destroys an image structure
	Blah_Image_disable(image);
	free(image);
}


void blah_image_destroyAll()
{	//Garbage collection function to deallocate all images still in memory
	Blah_Tree_destroyElements(&imageTree);
}


void Blah_Image_disable(Blah_Image *image)
{
	Blah_Tree_removeElement(&imageTree, image->name);
	free(image->pixelData); //Free buffer full of pixel data
}

Blah_Image* blah_image_find(const char* name)
{
    // Attempts to find an existing image in the image tree given the name of the image.
	// Image names are used as keys in the tree structure.  Returns a pointer to the
	// image with given name if successful match is found, else NULL.
	Blah_Tree_Element* imageElement = Blah_Tree_findElement(&imageTree, name);
	return imageElement != NULL ? (Blah_Image*)imageElement->data : NULL;
}

Blah_Image* Blah_Image_fromImage(const Blah_Image *source, const char *name, int left, int right, int bottom, int top)
{	// Makes a new image by copying data from existing image.  Supplied coordinate
	// values define area which is copied.
	const int copyWidth = right - left + 1;
	const int copyHeight = top - bottom + 1;
	Blah_Image* copyImage = Blah_Image_new(name, source->pixelDepth, copyWidth,	copyHeight, source->pixelFormat);  // Create new empty image for copy
	Blah_Image_copyRasterData(source, copyImage->pixelData, left, bottom, copyWidth, copyHeight, 0);
	return copyImage;
}

// Initialisaes given image structure with given name, pixel depth, width, height and pixel format
// Allocates a new raster buffer within.  Raster content is undefined.
// Function returns true if there were no errrors encountered
bool Blah_Image_init(Blah_Image *image, const char *name, unsigned char pixelDepth, unsigned int width, unsigned int height, blah_pixel_format pixFormat) {
	bool result = false;
	void *pixelData = (void*)malloc(width * height * (pixelDepth >> 3)); //Attempt to allocate memory for pixel data

	if (pixelData !=NULL) { //Continue only if memory allocation succeeded
		blah_util_strncpy(image->name, name, BLAH_IMAGE_NAME_LENGTH);
		image->pixelDepth = pixelDepth;
		image->width = width;
		image->height = height;
		image->pixelFormat = pixFormat;
		image->pixelData = pixelData;
		result = true;
	}

	return result;
}

// Creates a new Image structure from file given by 'filename'.
Blah_Image* Blah_Image_fromFile(const char* filename) {
	FILE* fileStream = blah_file_open(filename, "rb");
	if (fileStream == NULL) {
	    // If failed to open file, exit with error
	    blah_error_raise(errno, "Blah_Image_fromFile() failed to open filename '%s'", filename);
	    return NULL;
    }
	Blah_Image* const newImage = Blah_Image_Targa_fromFile(filename, fileStream);
	fclose(fileStream);
	return newImage;
}

// Construct a new image with given name, pixel depth, width, height and pixel format
// Adds new image structure to tree.  Raster content is undefined.
// Returns pointer to new image structure with allocated raster buffer within or NULL pointer if an error occurred
Blah_Image* Blah_Image_new(const char* name, unsigned char pixelDepth, unsigned int width, unsigned int height, blah_pixel_format pixFormat)
{
	Blah_Image* newImage = (Blah_Image*)malloc(sizeof(Blah_Image));
	if (newImage != NULL) //Continue only if image structure was allocated successfully
	{
		// If initialisation of image structure failed, bail out and free allocated memory
		if (!Blah_Image_init(newImage, name, pixelDepth, width, height, pixFormat)) {
			free(newImage);
			newImage = NULL; //Set new image pointer to return NULL to indicate failure
		} else {
		    Blah_Tree_insertElement(&imageTree, newImage->name, newImage);
		}
	}
	return newImage;
}
