/* blah_font.c - Defines routines for use with fonts.  Fonts are stored
	as 2D raster information and have a depth of 1 pixel for 3D purposes */

#include <malloc.h>
#include <string.h>
#include <stdio.h>

#include "blah_image.h"
#include "blah_draw.h"
#include "blah_font_raster.h"
#include "blah_font_texture.h"
#include "blah_font.h"
#include "blah_types.h"
#include "blah_debug.h"
#include "blah_macros.h"
#include "blah_tree.h"
#include "blah_util.h"

/* Internal Global Variables */

Blah_Tree blah_font_tree = {"font tree", NULL, (blah_tree_element_dest_func*)Blah_Font_destroy, 0};
	//A tree holding all fonts in memory

/* Element Function Definitions */

// Frees any allocated memory occupied by the font structure and destroys it
// Removes font from internal font tree
void Blah_Font_destroy(Blah_Font *font)
{
	Blah_Tree_removeElement(&blah_font_tree, font->name);

	switch (font->type) {
		case BLAH_FONT_RASTER :
			Blah_Font_Raster_destroy((Blah_Font_Raster*)font);
			break;
		case BLAH_FONT_TEXTURE :
			Blah_Font_Texture_destroy((Blah_Font_Texture*)font);
			break;
		default :
		    free(font);
		    break;
	}
}

void blah_font_destroyAll()
{	//Destroys all remaining font structures in the internal font tree
	Blah_Tree_destroyElements(&blah_font_tree);
}

void Blah_Font_init(Blah_Font* font, blah_font_type type, const char* fontName, const Blah_Image* source, int charWidth, int charHeight)
{	//Initialises base font information structure and assigns basic font properties
	font->type = type; //Assign font type e.g. raster, texture
	blah_util_strncpy(font->name, fontName, BLAH_FONT_NAME_LENGTH);
	font->width = charWidth;
	font->height = charHeight;
	font->pixelDepth = source->pixelDepth;
	font->pixelFormat = source->pixelFormat;
}

Blah_Font* Blah_Font_load(const char* filename)
{	// creates new font structure from a font datafile and returns pointer
	// Return NULL if error
    // TODO - complete me
    (void)filename;
	return NULL;
}

// Creates a new font structure from source image using index char map
// and given width and height of each character.  Width and height of source
// image must be a discreet multiple of character width and height. Index char
// map begins with first character at position 1.  0 is ignored.
// Implictly adds to the internal font tree.  Returns NULL on error.
Blah_Font* Blah_Font_new(blah_font_type type, const char *fontName, const Blah_Image* source, unsigned int charMap[BLAH_FONT_NUM_CHARS], int charWidth, int charHeight)
{
	Blah_Font *newFont = NULL;

	if (!(source->width % charWidth) && !(source->height % charHeight)) {
		//continue if source width and height divisible by char width and height
		switch (type) {
			case BLAH_FONT_RASTER :
				newFont = (Blah_Font*)Blah_Font_Raster_new(fontName, source, charMap, charWidth, charHeight);
				break;
			case BLAH_FONT_TEXTURE :
				newFont = (Blah_Font*)Blah_Font_Texture_new(fontName, source, charMap, charWidth, charHeight);
				break;
			default :
			    newFont = NULL;
			    break;
		}

		Blah_Tree_insertElement(&blah_font_tree, fontName, newFont);
	}
	return newFont;
}

void Blah_Font_printChar2d(const Blah_Font* font, char singleChar, int x, int y) {
	//Prints a single text character using the given font at supplied screen
	//coordinates, in 2D mode.  Text is not rendered in 3D mode.
	switch (font->type) {
		case BLAH_FONT_RASTER :
			Blah_Font_Raster_printChar2d((Blah_Font_Raster*)font, singleChar, x, y);
			break;
		case BLAH_FONT_TEXTURE :
			Blah_Font_Texture_printChar2d((Blah_Font_Texture*)font, singleChar,	x, y);
			break;
		default : break;
	}
}

void Blah_Font_printString2d(const Blah_Font* font, const char* text, int x, int y)
{	//Prints a text string using the given font at supplied screen coordinates,
	//in 2D mode.  Text is not rendered in 3D mode.
	const char *charPointer = text;
	char tempChar = *charPointer;  // get first character in string
	int xPos = x;

	while (tempChar) {  //loop until NULL char encountered
		if (tempChar == '\n') { //if current character is a new line,
			xPos = x;
			y -= font->height;
		} else {
			Blah_Font_printChar2d(font, tempChar, xPos, y);
			xPos += font->width; //advance to position for next character
		}
		charPointer++;
		tempChar = *charPointer; //get next character in string
	}
}

void Blah_Font_printChar3d(const Blah_Font* font, char singleChar, const Blah_Matrix* matrix)
{
    // TODO
    // Prints a single text character using the given font at with orientation
	// specified by 'matrix' in 3D space.
}

void Blah_Font_printString3d(const Blah_Font* font, const char* text, const Blah_Matrix* matrix)
{
    // TODO
    // Prints a text string using the given font at with orientation
	// specified by 'matrix' in 3D space.
}


