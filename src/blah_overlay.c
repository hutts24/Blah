/* blah_overlay.c - Defines functions which operate upon overlays. */


#include <malloc.h>
#include <string.h>

#include "blah_overlay_text.h"
#include "blah_draw.h"
#include "blah_overlay.h"
#include "blah_colour.h"
#include "blah_image.h"
#include "blah_types.h"
#include "blah_util.h"

/* Function Declarations */

void Blah_Overlay_init(Blah_Overlay *overlay, unsigned int layerNum, char *name, unsigned int width, unsigned int height) {
	blah_util_strncpy(overlay->name, name, BLAH_OVERLAY_NAME_LENGTH);
	overlay->width = width;
	overlay->height = height;
	overlay->posX = overlay->posY = 0; //default position to 0,0
	overlay->layerNum = layerNum;
	Blah_List_init(&overlay->textList, "overlay text list");
	Blah_List_init(&overlay->imageList, "overlay image list");
	overlay->visible = true;
}

Blah_Overlay *Blah_Overlay_new(unsigned int layerNum, char *name, unsigned int width, unsigned int height) {
	//Creates a new overlay structure with given name.
	//Returns NULL on error.
	Blah_Overlay *newOverlay = malloc(sizeof(Blah_Overlay));
	if (newOverlay != NULL) //Ensure memory allocation succeeded before initialising structure
		Blah_Overlay_init(newOverlay, layerNum, name, width, height);

	return newOverlay;
}

Blah_Overlay_Text *Blah_Overlay_addText(Blah_Overlay *overlay, char *name,
	char *text,	Blah_Font *fontStyle, unsigned int posX, unsigned int posY) {
	//Convenience function to create a new overlay text structure and implictly
	//add it to the list of text in the overlay, positioning it at given
	//coordinates within the parent overlay.
	Blah_Overlay_Text *newText = Blah_Overlay_Text_new(name, strlen(text), fontStyle);
	Blah_Overlay_Text_setText(newText, text);
	Blah_Overlay_Text_setPosition(newText, posX, posY);
	Blah_Overlay_addOverlayText(overlay,newText);

	return newText;
}


void Blah_Overlay_addOverlayText(Blah_Overlay *overlay, Blah_Overlay_Text *text) {
	//Adds the overlay text object to the list of text in the given overlay
	Blah_List_appendElement(&overlay->textList, text);
}

void Blah_Overlay_draw(Blah_Overlay *overlay) {
	//Draws the overlay in 2D space infront of the rendered 3D scene
	//FIXME - need to set clipping planes
	if (overlay->visible) { //only draw overlay if it is visible
		blah_draw_pushDrawport();
		blah_draw_setDrawport(overlay->posX, overlay->posY, overlay->posX+overlay->width-1, overlay->posX+overlay->height-1);
		Blah_List_callFunction(&overlay->textList, (blah_list_element_func)Blah_Overlay_Text_draw);
		blah_draw_popDrawport();
	}
}

void Blah_Overlay_destroy(Blah_Overlay *overlay) {
	//Destroys an overlay structure and all the text and image structures
	//contained in the overlay's internal lists.
	Blah_List_destroyElements(&overlay->textList);
	Blah_List_destroyElements(&overlay->imageList);
	free(overlay);
}

void Blah_Overlay_setVisible(Blah_Overlay *overlay, bool vis) {
	//Sets the visibility flag of the given overlay
	overlay->visible = vis;
}

void Blah_Overlay_setPosition(Blah_Overlay *overlay, unsigned int x, unsigned int y) {
	//Sets the position of the overlay given physical screen coordinates, relative
	//to physical origin bottom left corner (0,0)
	overlay->posX = x; overlay->posY = y;
}


bool Blah_Overlay_isVisible(Blah_Overlay *overlay) {
	//Returns the visibility flag (TRUE or FALSE)
	return overlay->visible;
}
