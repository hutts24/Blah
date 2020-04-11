/* blah_font_texture.h - Extension of blah_font data structure to represent a
	//font style rendered using	textured polygons.  Textured fonts are stored as 2D texture objects */

#ifndef _BLAH_FONT_TEXTURE

#define _BLAH_FONT_TEXTURE

#include "blah_types.h"
#include "blah_font.h"
#include "blah_matrix.h"
#include "blah_texture.h"
	
	
/* Symbol Definitions */	
	

/* Type Definitions */

/* Structure Definitions */

typedef struct Blah_Font_Texture {
	Blah_Font fontBase;
	Blah_Texture *fontTexture; //Pointer to the texture to be used to render font
	Blah_Texture_Map *charMaps[256]; //Texture coordinate mappings for each character from 0 to 255
} Blah_Font_Texture;

/* Font Function Prototypes */

#ifdef __cplusplus
	extern "C" {
#endif //__cplusplus

void Blah_Font_Texture_destroy(Blah_Font_Texture *font);
	//Frees any allocated memory occupied by the texture font structure and destroys it

void Blah_Font_Texture_disable(Blah_Font_Texture *font);
	//This function deallocates all internal memory allocated for the texture font.
	//Specifically, the character mappings and texture are deallocated.
	//This function expects that all unused texture mappings for each character are marked as NULL.
	
blah_bool Blah_Font_Texture_init(Blah_Font_Texture *font, char *fontName, Blah_Image *source,
 unsigned int charMap[256], int charWidth, int charHeight);
	//Initialisesd a new textured font structure from source image using index char map
	//and given width and height of each character.  Width and height of source
	//image must be a discreet multiple of character width and height. Index char
	//map begins with first character at position 1.  0 is ignored.
	//This function implictly adds to the internal font tree.
	//Returns TRUE on success, or FALSE on error.

Blah_Font_Texture *Blah_Font_Texture_new(char *fontName, Blah_Image *source, unsigned int charMap[256], int charWidth, int charHeigth);
	//Creates a new textured font structure from source image using index char map
	//and given width and height of each character.  Width and height of source
	//image must be a discreet multiple of character width and height. Index char
	//map begins with first character at position 1.  0 is ignored.
	//Implictly adds to the internal font tree.  Returns NULL on error.

void Blah_Font_Texture_printChar2d(Blah_Font_Texture *font, char singleChar, int x, int y);
	//Prints a single text character using the given font at supplied screen
	//coordinates, in 2D mode.  Text is rendered in 3D mode to appear as 2D.

void Blah_Font_Texture_printString2d(Blah_Font *font, char *text, int x, int y);
	//Prints a text string using the given font at supplied screen coordinates,
	//in 2D mode.  Text is not rendered in 3D mode.

void Blah_Font_Texture_printChar3d(Blah_Font *font, char singleChar, Blah_Matrix *matrix);
	//Prints a single text character using the given font with orientation and position
	//denoted by 'matrix' in 3D space.

void Blah_Font_Texture_printString3d(Blah_Font *font, char *text, float x, float y, float z);
	//Prints a text string using the given font with orientation and position
	//denoted by 'matrix' in 3D space.

#ifdef __cplusplus
	}
#endif //__cplusplus

#endif
