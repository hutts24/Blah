/* blah_overlay_text.h
	This structure represents a string of text in a given font, at specified
	coordinates within an overlay region.	*/

#ifndef _BLAH_OVERLAY_TEXT

#define _BLAH_OVERLAY_TEXT

#include "blah_font.h"
#include "blah_point.h"
#include "blah_types.h"

/* Symbol Definitions */

#define BLAH_OVERLAY_TEXT_NAME_LENGTH 50
#define BLAH_OVERLAY_TEXT_LENGTH 100

/* Forward Declarations */

struct Blah_Overlay;
struct Blah_Overlay_Text;

/* Function Type Definitions */

typedef void blah_overlay_text_draw_func(struct Blah_Overlay_Text *text);
	//This function type should draw the given given text within the overlay region

/* Data Structure definitions */

typedef struct Blah_Overlay_Text { //represents an scene object in the world
	char name[BLAH_OVERLAY_TEXT_NAME_LENGTH+1];
	unsigned int bufferSize; //current size of text buffer (in characters)
	Blah_Font *fontStyle; //Font used to draw this piece of text
	char *stringBuffer; //pointer to storage buffer for text string
	Blah_Point position; //object's position within the overlay, relative to overlay origin
	blah_overlay_text_draw_func *drawFunction;
	blah_bool visible;	//Visibility flag; If TRUE, then text is drawn
	struct Blah_Overlay *parent;
} Blah_Overlay_Text;

/* Structure Function prototypes */

#ifdef __cplusplus
	extern "C" {
#endif //__cplusplus

void Blah_Overlay_Text_destroy(Blah_Overlay_Text *text);
	//Destroys an overlay text object.  Frees memory occupied by overlay text
	//structure and also destroys attached text structure

void Blah_Overlay_Text_draw(Blah_Overlay_Text *text);
	//Draw text in 2D space within overlay region

void Blah_Overlay_Text_init(Blah_Overlay_Text *text);
    //Initialise overlay text object.

blah_bool Blah_Overlay_Text_isVisible(Blah_Overlay_Text *text);
	//Returns TRUE if the text is visible, else FALSE

Blah_Overlay_Text *Blah_Overlay_Text_new(char *name, unsigned int size, Blah_Font *fontStyle);
	//Create an overlay text object using supplied name, size and
	//font style.  Alloc a new Structure and return pointer

void Blah_Overlay_Text_setDrawFunction(Blah_Overlay_Text *text, blah_overlay_text_draw_func *func);
	//set pointer for draw function

void Blah_Overlay_Text_setPosition(Blah_Overlay_Text *text, float x, float y);
	//set object's position indicated by 2D coordinates

void Blah_Overlay_Text_setText(Blah_Overlay_Text *text, char *string);
	//Copies the character string pointed to by 'string', to the overlay text buffer

void Blah_Overlay_Text_setVisible(Blah_Overlay_Text *text, blah_bool visFlag);
	//Sets the visibility flag of the overlay text to the value given by vis_flag
	//True will make the text visible and drawn, FALSE will make it invisible


#ifdef __cplusplus
	}
#endif //__cplusplus

#endif
