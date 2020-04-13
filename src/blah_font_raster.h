/* blah_font_raster.h - Extension of font data structure to represent a raster font.
	Font images are stored as 2D raster information and have a depth of 1 pixel for
	3D purposes */

#ifndef _BLAH_FONT_RASTER

#define _BLAH_FONT_RASTER

#include "blah_font.h"
#include "blah_colour.h"
#include "blah_image.h"
#include "blah_types.h"


/* Symbol Definitions */

/* Structure Definitions */

typedef struct Blah_Font_Raster {
	Blah_Font fontBase;
	void *rasterData;	//2D raster font data
	void *charIndices[256]; //addresses winthin font data for each character from 0 to 255
} Blah_Font_Raster;

/* Font Function Prototypes */

#ifdef __cplusplus
	extern "C" {
#endif //__cplusplus

void Blah_Font_Raster_destroy(Blah_Font_Raster *font);
	//Frees any allocated memory occupied by the font structure and destroys it
	//Removes font from internal font tree

void Blah_Font_Raster_disable(Blah_Font_Raster *font);
	//Frees internally allocated memory for the raster data of the raster font structure,
	//leaving basic raster font structure available for reuse.

bool Blah_Font_Raster_init(Blah_Font_Raster *rasterFont, const char* fontName, const Blah_Image* source, unsigned int charMap[256], int charWidth, int charHeight);
	//Given a pointer to a simple unitialised raster font structure, this function initialises all important
	//variables within the structure, allocates font data and performs all other tasks necessary to make
	//the font usable.  Function returns true on success, or false on error.

Blah_Font_Raster *Blah_Font_Raster_new(const char* fontName, const Blah_Image* source, unsigned int charMap[256], int charWidth, int charHeigth);
	//Creates a new raster font structure from source image using index char map
	//and given width and height of each character.  Width and height of source
	//image must be a discreet multiple of character width and height. Index char
	//map begins with first character at position 1.  0 is ignored.
	//Returns NULL on error.

void Blah_Font_Raster_printChar2d(Blah_Font_Raster *font, char singleChar, int x, int y);
	//Prints a single text character using the given raster font at supplied screen
	//coordinates, in 2D mode.  Text is rendered infront of 3D scene.

void Blah_Font_Raster_printString2d(Blah_Font_Raster *font, char *text, int x, int y);
	//Prints a text string using the given raster font at supplied screen coordinates,
	//in 2D mode.  Text is not rendered in 3D mode.

void Blah_Font_Raster_printChar3d(Blah_Font_Raster *font, char singleChar, float x, float y, float z);
	//Prints a single text character using the given font at supplied world
	//coordinates, in 3D space.

void Blah_Font_Raster_printString3d(Blah_Font_Raster *font, char *text, float x, float y, float z);
	//Prints a text string using the given font at supplied world coordinates,
	//in 3D space.


#ifdef __cplusplus
	}
#endif //__cplusplus

#endif
