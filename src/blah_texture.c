/* blah_texture.c
	Defines functions for manipulating textures */

#include <malloc.h>
#include <string.h>

#include "blah_texture.h"
#include "blah_texture_gl.h"
#include "blah_tree.h"
#include "blah_util.h"
#include "blah_types.h"

/* Private local variables */

static Blah_Tree textureTree = {"", NULL, NULL, 0};	//Tree of all constructed textures in memory, key is file name


/* Function Declarations */

Blah_Texture *Blah_Texture_fromImage(Blah_Image *sourceImage) {
	//Creates a new texture from a source image, adds to texture_tree and retunrs pointer to new texture object.
	//If an error occurs, this function returns a NULL pointer.

	/* Blah_Texture *newTexture = (Blah_Texture*)malloc(sizeof(Blah_Texture));

	if (newTexture) { //Check that memory allocation succeeded
		//Copy statistics from source image
		newTexture->pixelFormat = sourceImage->pixelFormat;
		newTexture->pixelDepth = sourceImage->pixelDepth;
		newTexture->width = sourceImage->width;
		newTexture->height = sourceImage->height;
		blah_util_strncpy(newTexture->name, sourceImage->name, BLAH_TEXTURE_NAME_LENGTH);
		//Call API specific function to create a texture and return handle to it
		newTexture->handle = Blah_Texture_gl_new(sourceImage);

		if (!Blah_Tree_insertElement(&textureTree, newTexture->name, newTexture)) { //If an error occurred inserting into tree,
			Blah_Texture_destroy(newTexture); //Destroy texture that was just created because something is obviously wrong
			newTexture = NULL; //Set the pointer to return NULL value
		}
	}

	return newTexture; */

	const blah_texture_handle handle = Blah_Texture_gl_new(sourceImage);
	return Blah_Texture_new(sourceImage->name, sourceImage->width, sourceImage->height, handle, sourceImage->pixelFormat, sourceImage->pixelDepth);
}


void Blah_Texture_destroy(Blah_Texture *texture) {
	Blah_Texture_disable(texture); //free resources
	free(texture);	//Destroy engine texture structure
}

void Blah_Texture_disable(Blah_Texture *texture) {
	Blah_Tree_removeElement(&textureTree, texture->name); //Remove texture by name
	Blah_Texture_gl_destroy(texture->handle);
}

Blah_Texture *blah_texture_find(const char *name) {
	//Attempts to find a texture with given name in the texture tree.  Texture names
	//are used as keys in the binary tree.  Returns pointer to texture if successful
	//match is found, else NULL.
	Blah_Tree_Element *textureElement;

	textureElement = Blah_Tree_findElement(&textureTree, name);
	if (textureElement)
		return (Blah_Texture*)textureElement->data;
	else
		return NULL;
}

Blah_Texture_init(Blah_Texture *texture, const char* name, unsigned int width, unsigned int height, blah_texture_handle handle,
 blah_pixel_format pixelFormat, unsigned char pixelDepth /*, unsigned char mipMapLevel */) {
    // Initialise texture object.
    texture->pixelFormat = sourceImage->pixelFormat;
    texture->pixelDepth = sourceImage->pixelDepth;
    texture->width = sourceImage->width;
    texture->height = sourceImage->height;
    blah_util_strncpy(texture->name, name, BLAH_TEXTURE_NAME_LENGTH);
    //Call API specific function to create a texture and return handle to it
    texture->handle = handle;
}

Blah_Texture *Blah_Texture_new(const char* name, unsigned int width, unsigned int height, blah_texture_handle handle,
 blah_pixel_format pixelFormat, unsigned char pixelDepth /*, unsigned char mipMapLevel */) {
    // Create new texture object, add to internal list of textures and return pointer
    Blah_Texture *newTexture = (Blah_Texture*)malloc(sizeof(Blah_Texture));
    if (newTexture != NULL) { // Ensure memory allocation successful before initialising
        Blah_Texture_init(newTexture, name, width, height, handle, pixelFormat, pixelDepth);
        // Add the new texture to the internal cache
        if (!Blah_Tree_insertElement(&textureTree, newTexture->name, newTexture)) { //If an error occurred inserting into tree,
            Blah_Texture_destroy(newTexture); //Destroy texture that was just created because something is obviously wrong
            newTexture = NULL; //Set the pointer to return NULL value
        }
    }
    return newTexture;
 }

//  Blah_Texture_Map functions

Blah_Texture_Map *Blah_Texture_Map_new(Blah_Texture *texture, Blah_Point *mapping[]) {
	//Constructs a new texture map object with a pointer to given texture,
	//and a newly allocated array of texture coordinates (points)
	Blah_Texture_Map *newMap = (Blah_Texture_Map*)malloc(sizeof(Blah_Texture_Map));
	if (newMap != NULL)
		if (!Blah_Texture_Map_init(newMap, texture, mapping)) {
			//if init failed, then free memory and return NULL
			free(newMap);
			newMap = NULL;
		}

	return newMap;
}

blah_bool Blah_Texture_Map_init(Blah_Texture_Map *map, Blah_Texture *texture, Blah_Point *mapping[]) {
	int coordCount = 0, coordIndex;

	while (mapping[coordCount])
		coordCount++; //sum the number of coordinate supplied

	map->mapping = (Blah_Point*)malloc(sizeof(Blah_Point)*coordCount);
	if (map->mapping != NULL) {
		//allocate new array of texture coordinates (one for each vertex) and copy values
		for (coordIndex = 0;coordIndex < coordCount; coordIndex++)
			memcpy(&map->mapping[coordIndex], mapping[coordIndex], sizeof(Blah_Point));
		map->texture = texture; //assign texture pointer
		return BLAH_TRUE;
	} else
		return BLAH_FALSE;
}

void Blah_Texture_Map_destroy(Blah_Texture_Map *map) {
	//Destroys the given texture map structure.  Frees dynamic mapping coordinates
	//array and frees basic structure.
	free(map->mapping);
	free(map);
}

void blah_texture_destroyAll() {
	//Destroys and deallocates all textures still in memory
	Blah_Tree_destroyElements(&textureTree);
}
