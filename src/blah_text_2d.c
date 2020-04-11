/* blah_text_2d.c - Defines functions which operate upon text objects.*/

#include "blah_colour.h"
#include "blah_image.h"
#include "blah_types.h"	
#include "blah_text_2d.h"
#include "blah_util.h"

/* Text Function Declarations */

Blah_Text_2D *Blah_Text_2D_new(char *textString, Blah_Font *font) {
	//Creates a new Text object structure with given string and specified font.
	//Returns NULL on error.
	
	Blah_Text_2D *newText = malloc(sizeof(Blah_Text_2D));
	if (newText != NULL) //Ensure memory allocation succeeded
		Blah_Text_2D_init(newText, textString, font);
	return newText;	
}

void Blah_Text_2D_destroy(Blah_Text_2D *text) {
	//Destroys a text structure
	free(text);
}
	
void Blah_Text_2D_draw(Blah_Text_2D *text) {
	//Draws the text on the screen using all parameters contained in the
	//text structure

	Blah_Font_printString2d(text->fontStyle, text->textString, text->posX, text->posY);
}

void Blah_Text_2D_init(Blah_Text_2D *text, char *textString, Blah_Font *font) {
	Blah_Text_2D_setPosition(text, 0, 0); //new_text->pos_x = 0; new_text->pos_y = 0;
	Blah_Text_2D_setFont(text, font);
	//new_text->font_style = font;
	Blah_Text_2D_setString(text, textString);
}

void Blah_Text_2D_setString(Blah_Text_2D *text, char *textString) {
	//Copies the given string into the string of the text object
	blah_util_strncpy(text->textString, textString, BLAH_TEXT_2D_LENGTH);
}
	
	
void Blah_Text_2D_setFont(Blah_Text_2D *text, Blah_Font *font) {
	//Sets the font used by the text object to draw the text
	text->fontStyle = font;
}
	
void Blah_Text_2D_setPosition(Blah_Text_2D *text, unsigned int posX, unsigned int posY) {
	//Sets the drawing position of the text
	text->posX = posX;
	text->posY = posY;
}
