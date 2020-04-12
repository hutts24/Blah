/* blah_font_raster.c - Defines routines for use with raster fonts.  Fonts are stored
	as 2D raster information and have a depth of 1 pixel for 3D purposes */

#include <malloc.h>
#include <string.h>
#include <stdio.h>

#include "blah_font_raster.h"
#include "blah_debug.h"
#include "blah_macros.h"
#include "blah_tree.h"
#include "blah_draw.h"

/* Element Function Definitions */


void Blah_Font_Raster_destroy(Blah_Font_Raster *font)
{	//Frees any allocated memory occupied by the font structure and destroys it
	Blah_Font_Raster_disable(font); //Disable font structure first before deallocating it
	free(font);
}

void Blah_Font_Raster_disable(Blah_Font_Raster *font)
{	//Frees internally allocated memory for the raster data of the raster font structure,
	//leaving basic raster font structure available for reuse.
	free(font->rasterData);
}

bool Blah_Font_Raster_init(Blah_Font_Raster *rasterFont, char *fontName, Blah_Image *source, unsigned int charMap[256], int charWidth, int charHeight)
{	//Given a pointer to a simple unitialised raster font structure, this function initialises all important
	//variables within the structure, allocates font data and performs all other tasks necessary to make
	//the font usable.  Function returns true on success, or false on error.
	Blah_Debug_Log fontLog = { .filePointer = NULL};
	bool success = false; //This argument will return the success of the oepration.  Default to false until succeeded.

	Blah_Debug_Log_init(&fontLog, "blah_font_new");
	Blah_Debug_Log_message(&fontLog, "creating font from image");

	void *rasterPointer = rasterFont->rasterData = malloc(source->width * source->height * (source->pixelDepth>>3));;

	if (rasterPointer) //Ensure that allocation of raster data buffer succeeded before continuing
	{
	    //Call common basic init routine for all font structures
		Blah_Font_init(BLAH_FONT_RASTER, &rasterFont->fontBase, fontName, source, charWidth, charHeight);

	    //determine how many characters are represented in the source image
		const unsigned int charsWide = source->width / charWidth;
		const unsigned int charsHigh = source->height / charHeight;
		const unsigned int charsNum = charsHigh * charsWide;
		const unsigned int charSize = charWidth * charHeight * (source->pixelDepth >> 3);

		//copy characters from source image one block at a time
		for (unsigned int charRow = 0; charRow < charsHigh; charRow++) {
			for (unsigned int charCol = 0; charCol < charsWide; charCol++) {
				Blah_Image_copyRasterData(source, rasterPointer, charCol * charWidth, charRow * charHeight, charWidth, charHeight, 0);
				rasterPointer += charSize;
			}
		}

		// Setup the character mapping
		for (unsigned int charCount = 0; charCount < 256; charCount++) {
			unsigned int mapIndex = charMap[charCount];
			Blah_Debug_Log_message(&fontLog, "ascii '%c' mapped to char %d", charCount, mapIndex);
			if (!mapIndex || mapIndex > charsNum) { //if map index is 0 or beyond actual number of characters
				rasterFont->charIndices[charCount] = NULL; //set raster pointer to NULL
			} else {
				rasterFont->charIndices[charCount] = rasterFont->rasterData + (mapIndex - 1) * charSize; //set raster pointer accordingly
			}
		}

		success = true;
	}
	else
	{
		Blah_Debug_Log_message(&fontLog, "Failed to allocate buffer for raster font data");
	}

	Blah_Debug_Log_disable(&fontLog);

	return success;
}

Blah_Font_Raster *Blah_Font_Raster_new(char *fontName, Blah_Image *source, unsigned int charMap[256], int charWidth, int charHeight)
{	//Creates a new font structure from source image using index char map
	//and given width and height of each character.  Width and height of source
	//image must be a discreet multiple of character width and height. Index char
	//map begins with first character at position 1.  0 is ignored.
	//Returns NULL on error.
	Blah_Font_Raster *newFont;

	//allocate extended structure and assign basic font properties
	newFont = (Blah_Font_Raster*)malloc(sizeof(Blah_Font_Raster));

	if (newFont) //If memory allocation OK
	{
		if (!Blah_Font_Raster_init(newFont, fontName, source, charMap, charWidth, charHeight))
		{	//If initialisation of raster font fails, free the base structure and return NULL pointer
			free(newFont); //Bail out!
			newFont = NULL;
		}
	}

	return newFont;
}

void Blah_Font_Raster_printChar2d(Blah_Font_Raster *font, char singleChar, int x, int y)
{	//Prints a single text character using the given font at supplied screen
	//coordinates, in 2D mode.  Text is not rendered in 3D mode.
	void *rasterPointer = font->charIndices[(unsigned char)singleChar];
	//fprintf(stderr,"begin font_print_char\n");
	if (rasterPointer)
		blah_draw_pixels2d(rasterPointer, font->fontBase.pixelFormat, font->fontBase.width, font->fontBase.height, x, y);
	//fprintf(stderr,"end font_print_char\n");
}

void Blah_Font_Raster_printString2d(Blah_Font_Raster *font, char *text, int x, int y)
{	//Prints a text string using the given font at supplied screen coordinates,
	//in 2D mode.  Text is not rendered in 3D mode.
	char *charPointer = text;
	char tempChar = *charPointer;  //get first character in string
	int xPos = x;

	//fprintf(stderr,"begin font_print_string\n");

	while (tempChar) {  //loop until NULL char encountered
		//fprintf(stderr,"printing character '%c'\n",temp_char);
		if (tempChar == '\n') { //if current character is a new line,
			xPos = x; y -= font->fontBase.height;
		} else {
			Blah_Font_printChar2d((Blah_Font*)font, tempChar, xPos, y);
			xPos+=font->fontBase.width; //advance to position for next character
		}
		charPointer++;
		tempChar = *charPointer; //get next character in string
	}
	//fprintf(stderr,"end font_print_string\n");
}

void Blah_Font_Raster_printChar3d(Blah_Font_Raster *font, char singleChar, float x, float y, float z)
{
    // TODO
    // Prints a single text character using the given font at supplied world
	// coordinates, in 3D space.
}

void Blah_Font_Raster_printString3d(Blah_Font_Raster *font, char *text, float x, float y, float z)
{
    // TODO
    // Prints a text string using the given font at supplied world coordinates,
	// in 3D space.
}
