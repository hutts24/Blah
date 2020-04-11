/* blah_overlay.h - Data structure to represent a 2 dimensional overlay pane.
	Overlays can contain images and text.  Overlays have a position relative to
	2D physical screen dimensions, increasing from bottom left corner.  Elements
	are positioned within the overlay, using coordinates in the same manner. */
	
#ifndef _BLAH_OVERLAY

#define _BLAH_OVERLAY

#include "blah_colour.h"
#include "blah_image.h"
#include "blah_types.h"	
#include "blah_list.h"
#include "blah_types.h"
#include "blah_font.h"
#include "blah_overlay_text.h"

/* Symbol Definitions */	
	
#define BLAH_OVERLAY_NAME_LENGTH	20  //number of characters allowed for name property	

/* Type Definitions */

/* Structure Definitions */

typedef struct Blah_Overlay {
	char name[BLAH_OVERLAY_NAME_LENGTH+1];//name of font
	unsigned int height;		//height of overlay in pixels
	unsigned int width;			//width of overlay in pixels
	unsigned int posX,posY;	//2D coordinates of overlay position 
	unsigned int layerNum; //Overlay layer number.  Level 0 is drawn first,then 1 over 0 and so on
	Blah_List textList; //List of text objects within overlay
	Blah_List imageList; //List of images within overlay
	blah_bool visible;	//If TRUE, overlay is drawn
} Blah_Overlay;

/* Font Function Prototypes */

#ifdef __cplusplus
	extern "C" {
#endif //__cplusplus


void Blah_Overlay_addOverlayText(Blah_Overlay *overlay, Blah_Overlay_Text *text);
	//Adds the overlay text object to the list of text in the given overlay

Blah_Overlay_Text *Blah_Overlay_addText(Blah_Overlay *overlay, char *name,
	char *text,	Blah_Font *fontStyle, unsigned int posX, unsigned int posY);
	//Convenience function to create a new overlay text structure and implictly
	//add it to the list of text in the overlay, positioning it at given
	//coordinates within the parent overlay.

void Blah_Overlay_destroy(Blah_Overlay *overlay);
	//Destroys an overlay structure and all the text and image structures
	//contained in the overlay's internal lists.

void Blah_Overlay_draw(Blah_Overlay *overlay);
	//Draws the overlay in 2D space infront of the rendered 3D scene

void Blah_Overlay_init(Blah_Overlay *overlay, unsigned int layerNum, char *name, unsigned int width, unsigned int height);
	//Initialise overlay structure with defaults.

blah_bool Blah_Overlay_isVisible(Blah_Overlay *overlay);
	//Returns the visibility flag (TRUE or FALSE)

Blah_Overlay *Blah_Overlay_new(unsigned int layerNum, char *name, unsigned int width, unsigned int height);
	//Creates a new overlay structure with given name.
	//Returns NULL on error.
		
void Blah_Overlay_setPosition(Blah_Overlay *overlay, unsigned int x, unsigned int y);
	//Sets the position of the overlay given physical screen coordinates, relative
	//to physical origin bottom left corner (0,0)

void Blah_Overlay_setVisible(Blah_Overlay *overlay, blah_bool vis);
	//Sets the visibility flag of the given overlay

#ifdef __cplusplus
	}
#endif //__cplusplus

#endif
