/* blah_overlay_text.c
	Defines functions which operate overlay text. Overlay text is associated
	with a parent overlay region, and is positioned within the overlay region and
	relative to the overlay origin.  */

#include <malloc.h>
#include <string.h>

#include "blah_text_2d.h"
#include "blah_overlay_text.h"
#include "blah_types.h"
#include "blah_macros.h"
#include "blah_util.h"

/* Structure Function declarations */

void Blah_Overlay_Text_destroy(Blah_Overlay_Text *text) {
	//Destroys an overlay text object.  Frees memory occupied by overlay text
	//structure and also destroys attached text structure
	free(text->stringBuffer);
	free(text);
}

void Blah_Overlay_Text_draw(Blah_Overlay_Text *text) {
	//Draw text in 2D space within overlay region
	if (text->visible) {
		//If the text defines a special draw function, use it
		if (text->drawFunction)
			text->drawFunction(text);
		else //Just use the standard font draw function
			//FIXME - Either add current drawing position functionality,
			//add coordinate properties to text object,
			//or add coordinate parameters Blah_Text_2D_draw(text->text);
			Blah_Font_printString2d(text->fontStyle, text->stringBuffer, text->position.x, text->position.y);
	}
}

void Blah_Overlay_Text_init(Blah_Overlay_Text* text, const char* name, unsigned int size, const Blah_Font* fontStyle) {
    //Initialise overlay text object.
    blah_util_strncpy(text->name, name, BLAH_OVERLAY_TEXT_NAME_LENGTH);
	text->parent = NULL; //By default text does not belong to any parent
	text->stringBuffer = (char*)malloc(size+1); //allocate space for <size> characters plus one NULL
	text->bufferSize = size;
	strcpy(text->stringBuffer,""); //set string buffer to empty string
	Blah_Point_set(&text->position, 0,0,0);
	text->drawFunction = NULL;
	text->visible = true; //visible by default
	text->fontStyle = fontStyle;
}

bool Blah_Overlay_Text_isVisible(Blah_Overlay_Text *text) {
	//Returns TRUE if the text is visible, else FALSE
	return text->visible;
}

Blah_Overlay_Text* Blah_Overlay_Text_new(const char* name, unsigned int size, const Blah_Font* fontStyle) {
	// Create an overlay text object using supplied name, size and
	// font style.  Alloc a new Structure and return pointer
	Blah_Overlay_Text* newText = malloc(sizeof(Blah_Overlay_Text));
    // Ensure memory allocation succeeded before attempting initialisation
    if (newText != NULL) { Blah_Overlay_Text_init(newText, name, size, fontStyle); }
	return newText;
}

void Blah_Overlay_Text_setDrawFunction(Blah_Overlay_Text* text, blah_overlay_text_draw_func* function) {
	//set pointer for draw function
	text->drawFunction = function;
}

void Blah_Overlay_Text_setPosition(Blah_Overlay_Text *text, float x, float y) {
	//set object's position indicated by 2D coordinates
	Blah_Point_set(&text->position, x,y,0);
}

void Blah_Overlay_Text_setText(Blah_Overlay_Text *text, char *string) {
	//Copies the character string pointed to by 'string', to the overlay text buffer
	blah_util_strncpy(text->stringBuffer,string, text->bufferSize);
}

void Blah_Overlay_Text_setVisible(Blah_Overlay_Text *text, bool visFlag) {
	//Sets the visibility flag of the overlay text to the value given by vis_flag
	//True will make the text visible and drawn, FALSE will make it invisible
	text->visible = visFlag;
}

