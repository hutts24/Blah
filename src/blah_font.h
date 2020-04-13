/* blah_font.h - Base data structure to represent a font style.  Fonts are stored
	as 2D raster information and have a depth of 1 pixel for 3D purposes */

#ifndef _BLAH_FONT

#define _BLAH_FONT

#include "blah_colour.h"
#include "blah_image.h"
#include "blah_types.h"
#include "blah_tree.h"
#include "blah_matrix.h"
//#include "blah_scene.h"

/* Symbol Definitions */

#define BLAH_FONT_NUM_CHARS 256
#define BLAH_FONT_NAME_LENGTH	20  //number of characters allowed for name property

/* Type Definitions */

typedef enum Blah_Font_Type {
	BLAH_FONT_TEXTURE, //Full colour texture mapped font
	BLAH_FONT_RASTER, //Full colour raster image representation
	BLAH_FONT_BITMAP	//Mono colour bit-per-pixel storage format
} blah_font_type;

/* Structure Definitions */

typedef struct Blah_Font {
	blah_font_type type; //denotes the format of the font data (image/bitmap)
	char name[BLAH_FONT_NAME_LENGTH+1];//name of font
	blah_pixel_format pixelFormat; //Format the pixel data is stored in
	unsigned char pixelDepth; //Colour depth of pixels in bits per pixel
	int height;			//height of font in pixels
	int width;			//width of font in pixels
} Blah_Font;

/* External variable declarations - hack */

extern struct Blah_Scene *blah_draw_current_scene;

/* Font Function Prototypes */

#ifdef __cplusplus
	extern "C" {
#endif //__cplusplus

Blah_Font *Blah_Font_load(const char* filename);
	//creates new font structure from a font datafile and returns pointer
	//Return NULL if error

Blah_Font *Blah_Font_new(blah_font_type type, const char* fontName, const Blah_Image* source, unsigned int charMap[BLAH_FONT_NUM_CHARS], int charWidth, int charHeigth);
	// Creates a new font structure from source image using index char map
	// and given width and height of each character.  Width and height of source
	// image must be a discreet multiple of character width and height. Index char
	// map begins with first character at position 1.  0 is ignored.
	// Implictly adds to the internal font tree.  Returns NULL on error.

void Blah_Font_init(Blah_Font *font, blah_font_type type, const char* fontName, const Blah_Image *source, int charWidth, int charHeigth);
	// Initialises base font information structure.

void Blah_Font_printChar2d(const Blah_Font* font, char singleChar, int x, int y);
	// Prints a single text character using the given font at supplied screen
	// coordinates, in 2D mode.  Text is not rendered in 3D mode.

void Blah_Font_printString2d(const Blah_Font* font, const char* text, int x, int y);
	// Prints a text string using the given font at supplied screen coordinates,
	// in 2D mode.  Text is not rendered in 3D mode.

void Blah_Font_printChar3d(const Blah_Font* font, char singleChar, const Blah_Matrix* matrix);
	// Prints a single text character using the given font at with orientation
	// specified by 'matrix' in 3D space.

void Blah_Font_printString3d(const Blah_Font* font, const char* text, const Blah_Matrix* matrix);
	// Prints a text string using the given font at with orientation
	// specified by 'matrix' in 3D space.

void Blah_Font_destroy(Blah_Font *font);
	// Frees any allocated memory occupied by the font structure and destroys it
	// Removes font from internal font tree

void blah_font_destroyAll();
	//Destroys all remaining font structures in the internal font tree

#ifdef __cplusplus
	}
#endif //__cplusplus

#endif
