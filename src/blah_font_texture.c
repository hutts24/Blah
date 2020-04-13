/* blah_font_texture.c - Extension of blah_font data structure to represent a
	//font style rendered using	textured polygons.  Textured fonts are stored as 2D texture objects */

#include <malloc.h>
#include <string.h>

#include "blah_font_texture.h"
#include "blah_debug.h"
#include "blah_material.h"
#include "blah_draw.h"

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
		while (!error && charCount < sizeof(font->charMaps) / sizeof(Blah_Texture_Map*)) //Loop through all characters in font whilst an error has not been encountered
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

void Blah_Font_Texture_printChar2d(const Blah_Font_Texture* font, char singleChar, int x, int y)
{	//Prints a single text character using the given font at supplied screen
	//coordinates, in 2D mode.  Text is rendered in 3D mode to appear as 2D.
	Blah_Texture_Map *texMap = font->charMaps[(unsigned char)singleChar];
	//Blah_Point points[4];
	int coordLeft = x;	int coordRight = x + (font->fontBase.width - 1);
	int coordBottom = y; int coordTop = y + (font->fontBase.height - 1);
	Blah_Vertex vertices[4] = { {{coordLeft, coordTop, 0}, {0,0,1}},
		{{coordRight, coordTop, 0}, {0,0,1}}, {{coordRight, coordBottom, 0}, {0,0,1}},
		{{coordLeft, coordBottom, 0}, {0,0,1}} };
	Blah_Vertex *vertexArray[5] = {&vertices[0], &vertices[1], &vertices[2], &vertices[3], NULL};
	//Blah_Point *point_array[5] = {&points[0], &points[1], &points[2], &points[3], NULL};

	Blah_Material mat;
	//int count;

	//fprintf(stderr,"begin text_print_char_2d\n");

	Blah_Material_setColour(&mat, 1,1,1,1);
	/* Blah_Material_set_ambient(&mat, 1, 1, 1, 1);
	Blah_Material_set_diffuse(&mat, 1, 1, 1, 1);
	Blah_Material_set_specular(&mat, 1,1,1,1);
	Blah_Material_set_emission(&mat, 1, 1, 1, 1);
	Blah_Material_set_shininess(&mat, 1); */
	//Sets the shininess propertie of the material

	if (texMap) {
		/* coord_left = x;
		coord_right = x + (font->font_base.width - 1);
		coord_bottom = y;
		coord_top = y + (font->font_base.height - 1);

		points[0].x = points[3].x = coord_left;
		points[1].x = points[2].x = coord_right;
		points[0].y = points[1].y = coord_top;
		points[2].y = points[3].y = coord_bottom;
		points[0].z = points[1].z = points[2].z = points[3].z = 0;

		for (count=0;count<4;count++)
			fprintf(stderr,"Point %d - %f,%f,%f\n",count,points[count].x,
				points[count].y, points[count].z);
		*/

		blah_draw_polygon2d(vertexArray,texMap,&mat);
	}

	//fprintf(stderr,"end font_print_char\n");
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
