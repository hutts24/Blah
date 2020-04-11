/* blah_texture.h
	Header file for blah_texture.c
	Defines structure for textures and functions to manipulate textures */

#ifndef _BLAH_TEXTURE

#define _BLAH_TEXTURE

#include "blah_image.h"
#include "blah_point.h"

/* Defines */

#define BLAH_TEXTURE_NAME_LENGTH 50

/* Type Definitions */

typedef unsigned long blah_texture_handle;


/* Data Structures */

typedef struct Blah_Texture {
	char name[BLAH_TEXTURE_NAME_LENGTH+1];
	unsigned int width;			//Width of texture, in pixels
	unsigned int height;		//Height of texture, in pixels
	blah_texture_handle handle;		//Handle to texture, e.g. GLuint tex name for OpenGL
	blah_pixel_format pixelFormat;	//Format texture pixels are stored in
	unsigned char pixelDepth;	//Colour depth of pixels in bits per pixel e.g. 32
	// unsigned char mipMapLevel;
} Blah_Texture;

typedef struct Blah_Texture_Map {
	Blah_Point *mapping; //Pointer to an allocated array of texture coordinates
								//NULL means auto mapping.  Need one per vertex in sequence
	const Blah_Texture *texture; //pointer to texture used in mapping
} Blah_Texture_Map;

/* Public Function Prototypes */

#ifdef __cplusplus
	extern "C" {
#endif //__cplusplus

Blah_Texture *Blah_Texture_fromImage(Blah_Image *sourceImage);
	//Creates a new texture from a source image, adds to texture_tree and returns pointer to new texture object.
	//If an error occurs, this function returns a NULL pointer.

void Blah_Texture_destroy(Blah_Texture *texture);
	//Destroys a texture and removes the texture from the tree

void Blah_Texture_disable(Blah_Texture *texture);
	//free texture resources and remove texture from the tree

Blah_Texture *blah_texture_find(char *name);
	//Attempts to find a texture with given name in the texture tree.  Texture names
	//are used as keys in the binary tree.  Returns pointer to texture if successful
	//match is found, else NULL.

Blah_Texture_init(Blah_Texture *texture, const char* name, unsigned int width, unsigned int height, blah_texture_handle handle,
 blah_pixel_format pixelFormat, unsigned char pixelDepth /*, unsigned char mipMapLevel */);
    // Initialise texture object.

Blah_Texture *Blah_Texture_new(const char* name, unsigned int width, unsigned int height, blah_texture_handle handle,
 blah_pixel_format pixelFormat, unsigned char pixelDepth /*, unsigned char mipMapLevel */);
    // Create new texture object, add to internal list of textures and return pointer

// Texture Map functions

blah_bool Blah_Texture_Map_init(Blah_Texture_Map *map, Blah_Texture *texture, Blah_Point *mapping[]);
	//Allocate copy of mapping coordinates and assign to given texture

Blah_Texture_Map *Blah_Texture_Map_new(Blah_Texture *texture, Blah_Point *mapping[]);
	//Constructs a new texture map object with a pointer to given texture,
	//and a newly allocated array of texture coordinates (points)

void Blah_Texture_Map_destroy(Blah_Texture_Map *map);
	//Destroys the given texture map structure.  Frees dynamic mapping coordinates
	//array and frees basic structure.

void blah_texture_destroyAll();
	//Destroys and deallocates all textures still in memory

#ifdef __cplusplus
	}
#endif //__cplusplus

#endif
