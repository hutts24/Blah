/* blah_video.c
	Video routines to control display */

#include <stdio.h>
#include <string.h>
#include <SDL/SDL.h>

#ifdef BLAH_USE_GLUT
#include "blah_video_glut.h"
#endif

#include "blah_video.h"
#include "blah_draw.h"
#include "blah_video_sdl.h"
#include "blah_types.h"
#include "blah_macros.h"
#include "blah_entity.h"
#include "blah_list.h"
#include "blah_debug.h"
#include "blah_util.h"

/* Globals Variables */

Blah_Video_API blah_video_SDL = {
    .name = "SDL",
    .initFunction = blah_video_sdl_init,
    .exitFunction = blah_video_sdl_exit,
    .swapBuffersFunction = blah_video_sdl_swapBuffers,
    .clearBufferFunction = blah_video_sdl_clearBuffer,
    .setFullScreenFunction = blah_video_sdl_setFullScreen,
    .updateBufferFunction = blah_video_sdl_updateBuffer,
	.setDoubleBufferedFunction = blah_video_sdl_setDoubleBuffered,
	.setModeFunction = blah_video_sdl_setMode
};

#ifdef BLAH_USE_GLUT
Blah_Video_API blah_video_GLUT = {"GLUT",blah_video_glut_init, blah_video_glut_exit,
	blah_video_glut_swapBuffers, blah_video_sdl_clearBuffer, blah_video_glut_setFullScreen, NULL};
#endif

Blah_Video_API *blah_video_currentAPI = &blah_video_SDL;
	//Pointer to current API structure

Blah_Video_Mode *blah_video_currentMode = NULL;
	//A pointer to the current mode being used

Blah_List blah_video_modes = {"", NULL, NULL, 0};
	//Binary tree of all usable video modes. Extern in blah_video_sdl.h

Blah_Video_Settings blah_video_settings = {
	false	//initialised flag
};

static Blah_Debug_Log blah_video_log = { .filePointer = NULL };

/* Private static functions */

static int blah_video_modeCompare(Blah_Video_Mode *mode1, Blah_Video_Mode *mode2) {
	//Returns 0 if the two modes compared are identical, 1 if mode1 > mode2, -1 if mode1 < mode2
	//Compare mode1 against mode2
	if (mode1->colourDepth < mode2->colourDepth)
		return -1; //if colour depth is smaller, return -1
	else if (mode1->colourDepth > mode2->colourDepth)
		return 1; //if greater, return 1
	else { //if equal colour depth, compare on horiz resolution
		if (mode1->width < mode2->width)
			return -1; //return -1 if width smaller
		else if (mode1->width > mode2->width)
			return 1; //return 1 if width greater
		else { //if equal width compare depth
			if (mode1->height < mode2->height)
				return -1; //if height is smaller return -1
			else if (mode1->height > mode2->height)
				return 1;
			else //modes are identical as far as resolution and colour depth
				return 0;
		}
	}
}


static bool blah_video_modeSearch(Blah_Video_Mode *mode, char *modeName) {
	//Returns true if the video mode name matches the given name to search
	return !strcmp(mode->name, modeName) ? true : false;
}

/* Video Function Declarations */

void blah_video_clearBuffer() {  //Clears current drawing buffer
	blah_video_currentAPI->clearBufferFunction(NULL);
}

bool blah_video_init() {
	//Initialises the video component and sets current parameters to basic mode
	//Returns TRUE upon success, else false for error

	Blah_Debug_Log_init(&blah_video_log, "blah_video");

	if (!blah_video_currentAPI) {//If there is no API selected, raise error
		Blah_Debug_Log_message(&blah_video_log, "blah_video_init() failed.  No API is selected.");
	} else if (blah_video_settings.initialised) { //if video already initialised
		Blah_Debug_Log_message(&blah_video_log, "blah_video_init() failed.  Video is already initialised.");
	} else { //Call current API init
		if (blah_video_currentAPI->initFunction(&blah_video_settings) ) {
			//Sort video modes list
			Blah_List_sort(&blah_video_modes, (blah_list_sort_func)blah_video_modeCompare);
			blah_video_settings.initialised = true;
			Blah_Debug_Log_message(&blah_video_log, "blah_video_init() successful.");
			return true;
		}
	}
	Blah_Debug_Log_disable(&blah_video_log);
	return false;
}

void blah_video_main() { //Handles video buffer swapping and drawing

	blah_video_clearBuffer(); //Clear the video to begin new frame

	blah_draw_main();	//Call the main drawing routine to set perspective and draw
						//all objects and entities with automated drawing.
	blah_video_updateBuffer(); //Update all the changes from the drawing buffer to
								//video memory for new frame

	if (blah_video_currentMode->doubleBuffered)
		blah_video_swapBuffers(); //If double buffering enabled, swap buffers

}

void blah_video_exit() { //Exit video engine component
	if (!blah_video_settings.initialised) { //if video not yet initialised
		fprintf(stderr,"blah_video_exit() failed.  Video is not initialised.\n");
	} else {
		Blah_Debug_Log_message(&blah_video_log, "blah_video_exit() called.");
		blah_video_currentAPI->exitFunction(); //Call current API exit()
		blah_video_settings.initialised = false;
		Blah_List_destroyElements(&blah_video_modes);
		Blah_Debug_Log_disable(&blah_video_log);
	}
}

void blah_video_updateBuffer() {
	//Update all changes from drawing buffer into current buffer
	blah_video_currentAPI->updateBufferFunction(NULL);
}

void blah_video_swapBuffers() { //Calls the appropriate video function to swap buffers
	blah_video_currentAPI->swapBuffersFunction(NULL);
	//Call current API swap buffers
}

void blah_video_setFullScreen(bool fullFlag) {
	//If parameter is true, puts video into full screen mode, else windowed
	//Does nothing if display as not been set to a mode

	if (blah_video_currentMode) { //FIXME
	/* 	if (full_flag && !(blah_video_current_mode->full_screen)) {
			blah_video_current_mode->full_screen = true;
			if (blah_video_settings.initialised) //Make the setting apply immediately if video initialised
				blah_video_current_api->set_full_screen_function(true);
		} else if (!full_flag && blah_video_current_mode->full_screen) { //Don't change if not necessarry
			blah_video_current_mode->full_screen = false;
			if (blah_video_settings.initialised)
				blah_video_current_api->set_full_screen_function(false);
		}  */
	}
}

void blah_video_setDoubleBuffered(bool flag) {
	//Turns double buffering on/off depending on flag
	//Does nothing if display as not been set to a mode
	/* blah_video_current_mode->double_buffered = flag;
	blah_video_current_api->set_double_buffered_function(flag); */
}

bool blah_video_isFullScreen() { //Returns true if in full screen mode
	return blah_video_currentMode->fullScreen;
}

bool blah_video_isDoubleBuffered() { //Returns true if in full double buffering active
	return blah_video_currentMode->doubleBuffered;
}

void blah_video_setSizeWindowed(int width, int height) {;}

void blah_video_setSizeFullScreen(int width, int height) {;}

bool blah_video_setMode(Blah_Video_Mode *mode) {
	//Sets the display device to the given mode.  Returns TRUE upon success, else false
	Blah_Debug_Log_message(&blah_video_log, "Calling blah_video_sdl_set_mode()");
	if (blah_video_sdl_setMode(mode)) {
		blah_video_currentMode = mode;
		blah_draw_setViewport(0,0,mode->width-1, mode->height-1);
		return true;
	} else {
		return false;
	}
}

Blah_Video_Mode *blah_video_getCurrentMode() {
	//Returns a pointer to the video mode structure representing the current mode
	return blah_video_currentMode;
}

Blah_Video_Mode *blah_video_getMode(int width, int height, int bppDepth) {
	//Searches for an available mode with the given attributes and returns handle
	//to Blah_Video_Mode structure if one exists otherwise NULL.  Note that this structure is
	//internal to blah video and must not be altered in any way!
	char tempModeName[BLAH_VIDEO_MODE_NAME_LENGTH+1];
	/* construct a mode name from given attributes and search mode tree for match */
	sprintf(tempModeName, "%dx%dx%d", width, height, bppDepth);
	return (Blah_Video_Mode*)Blah_List_search(&blah_video_modes,
		(blah_list_search_func)blah_video_modeSearch, tempModeName);
}

Blah_Video_Mode *blah_video_getIdealMode(int width, int height, int bppDepth) {
	//Searches for an available mode with the given attributes and returns handle
	//to Blah_Video_Mode structure if one exists, otherwise the next best mode is
	//returned.  Note that this structure is internal to blah video and must
	//not be altered in any way!
	Blah_Video_Mode *idealMode=NULL;
	Blah_Video_Mode *tempMode;
	bool stopFlag = false;

	tempMode = blah_video_getLeastMode();

	while (!stopFlag && tempMode) {
		if (tempMode->width <= width && tempMode->height <= height) {
			idealMode = tempMode;
			tempMode = blah_video_getNextMode(tempMode);
		} else
			stopFlag = true;
	}
	return idealMode;
}

Blah_Video_Mode *blah_video_getNextMode(Blah_Video_Mode *mode) {
	//Searches for a higher resolution mode than the given mode, using the same
	//colour depth.  Returns a pointer to the new mode found or NULL if there is no
	//higher resolution mode available with requested colour depth;
	Blah_List_Element *modeElement = Blah_List_findElement(&blah_video_modes, mode);
	Blah_List_Element *nextElement = modeElement->next;
	Blah_Video_Mode *nextMode;
	if (nextElement) { //If there is a next mode in the list, continue
		nextMode = (Blah_Video_Mode*)nextElement->data;
		return mode->colourDepth == nextMode->colourDepth ? nextMode : NULL;
	} else
		return NULL;
}

Blah_Video_Mode *blah_video_getPrevMode(Blah_Video_Mode *mode) {
	//Searches for a lower resolution mode than the given mode, using the same
	//colour depth.  Returns a pointer to the new mode found or NULL if there is no
	//lower resolution mode available with requested colour depth;
	Blah_List_Element *modeElement = Blah_List_findElement(&blah_video_modes, mode);
	Blah_List_Element *prevElement = modeElement->prev;
	Blah_Video_Mode *prevMode;

	if (prevElement) { //If there is a prev mode in the list, continue
		prevMode = (Blah_Video_Mode*)prevElement->data;
		return mode->colourDepth == prevMode->colourDepth ? prevMode : NULL;
	} else
		return NULL;
}


Blah_Video_Mode *blah_video_getLeastMode() {
	//Returns a pointer to the lowest resolution mode using the lowest
	//available colour depth
	return blah_video_modes.first->data;
}

Blah_Video_Mode *blah_video_getBestMode() {
	//Returns a pointer to the highest resolution mode using the highest
	//available colour depth
	return blah_video_modes.last->data;
}

/* Video Mode Function Declarations */

Blah_Video_Mode *Blah_Video_Mode_new(char *name, bool fullScreen,
	bool doubleBuffered, int width, int height, int bppDepth) {
	//Creates a new video mode with given properties supplied in params.
	//Allocates memory and returns new structure.
	Blah_Video_Mode *newMode = (Blah_Video_Mode*)malloc(sizeof(Blah_Video_Mode));
	blah_util_strncpy(newMode->name, name, BLAH_VIDEO_MODE_NAME_LENGTH);  //set name property
	newMode->fullScreen = fullScreen;
	newMode->width = width;
	newMode->height = height;
	newMode->colourDepth = bppDepth;
	newMode->doubleBuffered = doubleBuffered;

	return newMode;
}
