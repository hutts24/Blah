/* blah_image.h
	Header file for image.c
	Defines image structures and functions for manipulating images
	All images are stored in raw pixel data	raster format
	bottom left through to top right */

#ifndef _BLAH_IMAGE

#define _BLAH_IMAGE

#include <stdio.h>

#include "blah_types.h"

/* Defines */

#define BLAH_IMAGE_NAME_LENGTH 50


/* Type Definitions */

enum Blah_Pixel_Format {BLAH_PIXEL_FORMAT_INDEX, BLAH_PIXEL_FORMAT_RGB,
	BLAH_PIXEL_FORMAT_BGR, BLAH_PIXEL_FORMAT_RGBA, BLAH_PIXEL_FORMAT_BGRA};
typedef enum Blah_Pixel_Format blah_pixel_format;

/* Data Structures */

typedef struct Blah_Image {// Represents a raw data image in raster pattern
	char name[BLAH_IMAGE_NAME_LENGTH+1];
	unsigned char pixelDepth;// Colour depth of pixels in bits per pixel
	unsigned int width; // width of image
	unsigned int height; // height of image (size is width * height * pixel depth
	blah_pixel_format pixelFormat; // Format the pixel data is stored in
	void *pixelData; // pointer to buffer of raster data
} Blah_Image;

/* Function Prototypes */

#ifdef __cplusplus
	extern "C" {
#endif //__cplusplus

void Blah_Image_copyImage(Blah_Image* dest, unsigned int destX, unsigned int destY,
    const Blah_Image* source, unsigned int srcLeft, unsigned int srcRight,
    unsigned int srcBottom, unsigned int srcTop);
	// Copies the region defined by src_left,src_right,src_bottom,src_top from
	// image 'source' to position dest_x,dest_y in image 'dest'

void Blah_Image_copyRasterData(const Blah_Image* source, void *destination, unsigned int left,
 unsigned int bottom, unsigned int width, unsigned int height, unsigned int rowSkip);
	// Copies image raster data from a region (width * height) within the source
	// image, origin defined by left,bottom to destination pointer.  Destination
	// pointer will skip an additional 'row_skip' bytes after each row is copied.

void Blah_Image_destroy(Blah_Image *image);
	// Destroys an image structure and removes from tree

void blah_image_destroyAll();
	// Garbage collection function to deallocate all images still in memory

void Blah_Image_disable(Blah_Image *image);
	// Remove image from internal image list, and release raster data buffer

// Attempts to find an existing image in the image tree given the name of the image.
// Image names are used as keys in the tree structure.  Returns a pointer to the
// image with given name if successful match is found, else NULL.
Blah_Image* blah_image_find(const char* name);

// Makes a new image by copying data from existing image.  Supplied coordinate
// values define area which is copied.
Blah_Image* Blah_Image_fromImage(const Blah_Image* source, const char* name, int left, int right, int bottom, int top);


// MADE PRIVATE.  Obsolete since the load functions.  bool
// Blah_Image_init(Blah_Image* image, const char* name, unsigned char pixelDepth, unsigned int width, unsigned int height, blah_pixel_format pixFormat);
// Initialises given image structure with given name, pixel depth, width, height and pixel format
// Returns pointer to new image structure with allocated raster buffer within.
// Function returns true if there were no errrors encountered


// Creates a new Image structure from a file given by 'filename'.  Memory is allocated etc
Blah_Image* Blah_Image_fromFile(const char* filename);


Blah_Image *Blah_Image_new(const char *name, unsigned char pixelDepth, unsigned int width, unsigned int height, blah_pixel_format pixFormat);
	// Construct a new image with given name, pixel depth, width, height and pixelf format
	// Adds new image structure to tree
	// Returns pointer to new image structure with allocated raster buffer within or NULL pointer if an error occurred


#ifdef __cplusplus
	}
#endif //__cplusplus

#endif
