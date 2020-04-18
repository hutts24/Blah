/* blah_font_texture.c - Extension of blah_font data structure to represent a
	//font style rendered using	textured polygons.  Textured fonts are stored as 2D texture objects */

#include <malloc.h>
#include <string.h>

#include "blah_font_texture.h"
#include "blah_debug.h"
#include "blah_material.h"
#include "blah_draw.h"
#include "blah_macros.h"

/* Texture Font Functions */

void Blah_Font_Texture_destroy(Blah_Font_Texture *font)
{	// Frees any allocated memory occupied by the texture font structure and destroys it
	Blah_Font_Texture_disable(font); // Deallocate internal dynamically allocated resources
	free(font); // Free memory representing texture font
}

void Blah_Font_Texture_disable(Blah_Font_Texture *font)
{	// This function deallocates all internal memory allocated for the texture font.
	// Specifically, the character mappings and texture are deallocated.
	// This function expects that all unused texture mappings for each character are marked as NULL.
	for (int mapCount = 0; mapCount < BLAH_FONT_NUM_CHARS; mapCount++) {
		if (font->charMaps[mapCount]) {	Blah_Texture_Map_destroy(font->charMaps[mapCount]); }
	}

	Blah_Texture_destroy(font->fontTexture);  //Destroy the texture which this font uses.
}

bool Blah_Font_Texture_init(Blah_Font_Texture *font, const char *fontName, const Blah_Image *source,
 const unsigned int charMap[BLAH_FONT_NUM_CHARS], int charWidth, int charHeight)
{	// Initialisesd a new textured font structure from source image using index char map
	// and given width and height of each character.  Width and height of source
	// image must be a discreet multiple of character width and height. Index char
	// map begins with first character at position 1.  0 is ignored.
	// This function implictly adds to the internal font tree.
	// Returns TRUE on success, or FALSE on error.
	bool result = false, error = false;

	// Construct temporary log just for this font
	Blah_Debug_Log fontLog = { .filePointer = NULL };
	Blah_Debug_Log_init(&fontLog, "blah_font_texture_new");
	Blah_Debug_Log_message(&fontLog, "creating texture font from image");

	//determine how many characters are represented in the source image
	const unsigned int charsWide = source->width / charWidth;
	const unsigned int charsHigh = source->height / charHeight;
	const unsigned int charsNum = charsHigh * charsWide;

	Blah_Font_init(&font->fontBase, BLAH_FONT_TEXTURE, fontName, source, charWidth, charHeight);  // assign basic font properties
	Blah_Texture* const fontTexture = Blah_Texture_fromImage(source); // Create new texture from source image
	if (fontTexture) // Check that texture creation succeeded, then create character mappings
	{
		font->fontTexture = fontTexture;
		memset(font->charMaps, 0, sizeof(font->charMaps)); // Zero out array of texture map pointers

		Blah_Point coords[4];
        const Blah_Point* tempMapping[] = {&coords[0], &coords[1], &coords[2], &coords[3], NULL};

		unsigned int charCount = 0;
		while (!error && charCount < blah_countof(font->charMaps)) //Loop through all characters in font whilst an error has not been encountered
		{
			const unsigned int mapIndex = charMap[charCount];
			Blah_Debug_Log_message(&fontLog, "ascii '%c' mapped to char %d", charCount, mapIndex);
			if (mapIndex > 0 && mapIndex <= charsNum) //if map index is greater than 0 and within actual number of characters
			{	//Create mapping for current character
				const unsigned int charRow = (mapIndex-1) / charsWide; //get row (beginning 0)
				const unsigned int charCol = (mapIndex-1) % charsWide; //get column (beginning 0)
				const float coordLeft = (float)((charCol * charWidth) + 1) / source->width;
				const float coordRight = (float)((charCol+1) * charWidth) / source->width;
				const float coordBottom = (float)((charRow * charHeight) + 1) / source->height;
				const float coordTop = (float)((charRow+1) * charHeight) / source->height; //get texture coordinates
				Blah_Debug_Log_message(&fontLog, "col: %d, row: %d", charCol, charRow);
				//create mapping array
				coords[0].x = coords[3].x = coordLeft;
				coords[1].x = coords[2].x = coordRight;
				coords[0].y = coords[1].y = coordTop;
				coords[2].y = coords[3].y = coordBottom;
				coords[0].z = coords[1].z = coords[2].z = coords[3].z = 0;
				Blah_Texture_Map *newMap = Blah_Texture_Map_new(font->fontTexture, tempMapping); // Create new texture mapping for font character
				if (newMap) {
                    font->charMaps[charCount] = newMap; // If mapping was created ok, assign the mapping for current character
				} else {
					error = true; //Set the flag that an error occurred
				}
				Blah_Debug_Log_message(&fontLog, "left: %f, right: %f, bottom: %f, top: %f", coordLeft, coordRight, coordBottom, coordTop);
			}
			charCount++; //Increment loop counter
		}

		// If an error was encountered allocating character mapping, disable the current font and deallocate all memory that has been allocated
		if (error) {
            Blah_Font_Texture_disable(font);
        } else {
			result = true; // Set return flag as TRUE to indicate that initialisation succeeded.
		}
	}

	Blah_Debug_Log_disable(&fontLog);

	return result;
}

Blah_Font_Texture* Blah_Font_Texture_new(const char* fontName, const Blah_Image* source, const unsigned int charMap[BLAH_FONT_NUM_CHARS], int charWidth, int charHeight)
{	//Creates a new textured font structure from source image using index char map
	//and given width and height of each character.  Width and height of source
	//image must be a discreet multiple of character width and height. Index char
	//map begins with first character at position 1.  0 is ignored.
	//Implictly adds to the internal font tree.  Returns NULL on error.
	Blah_Font_Texture *newFont = (Blah_Font_Texture*)malloc(sizeof(Blah_Font_Texture)); //allocate extended structure and assign basic font properties
	if (newFont) //If new font structure was allocated successfully
	{
		if (!Blah_Font_Texture_init(newFont, fontName, source, charMap, charWidth, charHeight))
		{	// If initialisation of texture font fails, free the base structure and return NULL pointer
			free(newFont); // Bail out!  Free the allocated memory buffer and abort
			newFont = NULL; // Return Null pointer because font was not created.clean
		}
	} // else TODO - Error handling!

	return newFont;
}

// Prints a single text character using the given font at supplied screen
// coordinates, in 2D mode.  Text is rendered in 3D mode to appear as 2D.
void Blah_Font_Texture_printChar2d(const Blah_Font_Texture* font, char singleChar, int x, int y)
{
	Blah_Texture_Map *texMap = font->charMaps[(unsigned char)singleChar];
	if (texMap != NULL) {
        int coordLeft = x;
        int coordRight = x + (font->fontBase.width - 1);
        int coordBottom = y; int coordTop = y + (font->fontBase.height - 1);
        Blah_Vertex vertices[4] = { {{coordLeft, coordTop, 0}, {0,0,1}},
            {{coordRight, coordTop, 0}, {0,0,1}}, {{coordRight, coordBottom, 0}, {0,0,1}},
            {{coordLeft, coordBottom, 0}, {0,0,1}} };
        Blah_Vertex *vertexArray[5] = {&vertices[0], &vertices[1], &vertices[2], &vertices[3], NULL};

        Blah_Material mat;
        Blah_Material_setColour(&mat, 1,1,1,1);
        blah_draw_polygon2d(vertexArray,texMap,&mat);
    }
}

void Blah_Font_Texture_printString2d(const Blah_Font* font, const char* text, int x, int y)
{
    // TODO
    //Prints a text string using the given font at supplied screen coordinates,
	//in 2D mode.  Text is not rendered in 3D mode.
}

void Blah_Font_Texture_printChar3d(const Blah_Font* font, char singleChar, const Blah_Matrix* matrix)
{
    // TODO
    //Prints a single text character using the given font with orientation and position
	//denoted by 'matrix' in 3D space.
}

void Blah_Font_Texture_printString3d(const Blah_Font* font, const char* text, float x, float y, float z)
{
    // TODO
    //Prints a text string using the given font with orientation and position
	//denoted by 'matrix' in 3D space.
}
