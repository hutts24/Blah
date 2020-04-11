/* blah_text_2d.h - Data structure to represent a piece of text.  Text structure
	contains a pointer to the font used to draw the text, and 2D positional
	coordinates, increasing from bottom left.*/

#ifndef _BLAH_TEXT_2D

#define _BLAH_TEXT_2D

#include "blah_types.h"
#include "blah_font.h"
#include "malloc.h"
#include "string.h"

/* Symbol Definitions */
#define BLAH_TEXT_2D_LENGTH	100	//Max length for a piece of text

/* Structure Definitions */

typedef struct Blah_Text_2D {
	Blah_Font *fontStyle; //Font used to draw this piece of text
	char textString[BLAH_TEXT_2D_LENGTH+1]; //storage for text string
	unsigned int posX,posY;	//2D coordinates of text position 
} Blah_Text_2D;

/* Text Function Prototypes */

#ifdef __cplusplus
	extern "C" {
#endif //__cplusplus

Blah_Text_2D *Blah_Text_2D_new(char *text_string, Blah_Font *font);
	//Creates a new Text object structure with given string and specified font.
	//Returns NULL on error.

void Blah_Text_2D_destroy(Blah_Text_2D *text);
	//Destroys a text structure

void Blah_Text_2D_draw(Blah_Text_2D *text);
	//Draws the text on the screen using all parameters contained in the
	//text structure

void Blah_Text_2D_init(Blah_Text_2D *text, char *text_string, Blah_Font *font);
	//Intiialise 2d text with given properties
	
void Blah_Text_2D_setString(Blah_Text_2D *text, char *textString);
	//Copies the given string into the string of the text object

void Blah_Text_2D_setFont(Blah_Text_2D *text, Blah_Font *font);
	//Sets the font used by the text object to draw the text

void Blah_Text_2D_setPosition(Blah_Text_2D *text, unsigned int posX, unsigned int posY);
	//Sets the drawing position of the text

#ifdef __cplusplus
	}
#endif //__cplusplus

#endif
