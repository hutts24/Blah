/* blah_video_sdl.c
	SDL specific video routines */

#include <SDL/SDL.h>
#include <GL/gl.h>
#include <stdlib.h>
#include <stdio.h>

#include "blah_video.h"
#include "blah_draw.h"
#include "blah_video_sdl.h"
#include "blah_draw_gl.h"
#include "blah_debug.h"
#include "blah_types.h"

/* Externally available variables and functions that should not be exposed beyond this library */
extern Blah_List blah_video_modes;
extern Blah_Video_Mode *blah_video_currentMode;

extern Blah_Video_Mode *Blah_Video_Mode_new(char *name, bool fullScreen, bool doubleBuffered, int width, int height, int bppDepth);
    // Creates a new video mode with given properties supplied in params.
    // Allocates memory and returns new structure.

/* Globals */

static bool blah_video_sdl_initialised = false;  //state flag
static Blah_Debug_Log blah_video_sdl_log = { .filePointer = NULL };

/* Function Declarations */

bool blah_video_sdl_init(const Blah_Video_Settings* settings) {  // Initialise SDL video subsystem
	char modeName[BLAH_VIDEO_MODE_NAME_LENGTH+1];
	int modeIndex = 0; int bpp;
	int curResX = 0, curResY = 0;
	Blah_Video_Mode *tempMode;
	SDL_VideoInfo *videoInfo; SDL_Rect **sdlModes, *sdlMode;;

	Blah_Debug_Log_init(&blah_video_sdl_log, "blah_video_sdl"); //Set up SDL video log

	if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0) { // Initialise SDL with no extra subsystems
		Blah_Debug_Log_message(&blah_video_sdl_log, "Failed to initialise SDL: %s\n", SDL_GetError());
		Blah_Debug_Log_disable(&blah_video_sdl_log);
		return false;
	} else {
		/* Get video info */
		Blah_Debug_Log_message(&blah_video_sdl_log,"Getting a list of available video modes through SDL");
		videoInfo = (SDL_VideoInfo*)SDL_GetVideoInfo(); // Get pointer to SDL video info
		bpp = videoInfo->vfmt->BitsPerPixel; // get bits per pixel in best colour mode
		sdlModes = SDL_ListModes(NULL, SDL_FULLSCREEN|SDL_OPENGL);

		/* Construct a valid list of modes in blah_video_modes */
		if (sdlModes[0]) {
			curResX = sdlModes[0]->w;
			curResY = sdlModes[0]->h;
		}
		while(sdlModes[modeIndex]) {
			sdlMode = sdlModes[modeIndex];
			if (sdlMode->w != curResX || sdlMode->h != curResY) {
				sprintf(modeName, "%dx%dx%d",curResX, curResY, bpp);
				Blah_Debug_Log_message(&blah_video_sdl_log, modeName);
				tempMode = Blah_Video_Mode_new(modeName, true, true, curResX, curResY, bpp);
				//Construct new mode structure and add to tree
				Blah_List_appendElement(&blah_video_modes, tempMode);
			}
			modeIndex++;
			curResX = sdlMode->w;
			curResY = sdlMode->h;

		}
		sprintf(modeName, "%dx%dx%d", curResX, curResY, bpp);
		Blah_Debug_Log_message(&blah_video_sdl_log, modeName);
		tempMode = Blah_Video_Mode_new(modeName, true, true, curResX, curResY, bpp);
		//Construct new mode structure and add to tree
		Blah_List_appendElement(&blah_video_modes, tempMode);
		blah_video_sdl_initialised = true;
		return true;
	}
}

bool blah_video_sdl_exit() { //Shutdown SDL video component
	if (blah_video_sdl_initialised) {
		Blah_Debug_Log_message(&blah_video_sdl_log, "Begin SDL Video shutdown blah_video_sdl_exit()");
		SDL_ShowCursor(SDL_ENABLE); // Show cursor
		Blah_Debug_Log_message(&blah_video_sdl_log, "Enabled SDL cursor");
		SDL_WM_GrabInput(SDL_GRAB_OFF); // Allow mouse to roam free outside window
		Blah_Debug_Log_message(&blah_video_sdl_log, "Turned input grabbing off");
		SDL_QuitSubSystem(SDL_INIT_VIDEO);
		blah_video_sdl_initialised = false;  //set state flag off
		Blah_Debug_Log_message(&blah_video_sdl_log, "Video shutdown with SDL_QuitSubSystem()");
		Blah_Debug_Log_disable(&blah_video_sdl_log); //deallocate log memory
		return true;
	} else {
		return false;
	}
}

void blah_video_sdl_updateBuffer() {
	glFinish(); //Wait for all GL operations to finish
}

void blah_video_sdl_swapBuffers() { //Calls the appropriate vide function to swap buffers
	SDL_GL_SwapBuffers();
}

void blah_video_sdl_clearBuffer() {  //Clears current drawing buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void blah_video_sdl_setDoubleBuffered(bool flag) {
	//Turns double buffering on/off depending on flag
	fprintf(stderr, "setting double buffering:%d\n",flag);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, blah_video_currentMode->doubleBuffered);
	//FIXME blah_video_sdl_set_mode();
}

void blah_video_sdl_setFullScreen(bool fullFlag) {
	//Required information is already in video settings
	//FIXME blah_video_sdl_set_mode();
}

void blah_video_sdl_setSizeFullScreen(int width, int height) {
	//FIXME blah_video_sdl_set_mode();  //Values we need are already in the video settings sructure
}

bool blah_video_sdl_setMode(const Blah_Video_Mode* mode) {
	//Set SDL video settings based apon ptr_video_settings
	//This function exists because basically SDL has no way of controlling individual settings
	Uint32 sdlFlags = SDL_OPENGL;
	int sdlBpp;

	//Calculate size based apon full screen attribute and size settings
	const int width = mode->width;
	const int height = mode->height;
	const int bpp = mode->colourDepth;

	if (mode->fullScreen) {
		//Calculate flags to send to SDL init
		sdlFlags |= SDL_FULLSCREEN;
		SDL_ShowCursor(SDL_DISABLE); //Hide cursor
		SDL_WM_GrabInput(SDL_GRAB_ON); //confine keyboard and mouse to SDL window
	} else {
		SDL_ShowCursor(SDL_ENABLE); //Show cursor
		SDL_WM_GrabInput(SDL_GRAB_OFF); //Allow mouse to roam free outside window
	}

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, mode->doubleBuffered);

	Blah_Debug_Log_message(&blah_video_sdl_log, "Setting video to %dx%dx%d\n", width, height, bpp);
	sdlBpp = SDL_VideoModeOK(width, height, bpp, sdlFlags);
	Blah_Debug_Log_message(&blah_video_sdl_log, "SDL bpp is %d", sdlBpp);
	Blah_Debug_Log_message(&blah_video_sdl_log, "Suggested bpp for %dx%d is %d", width, height, bpp);
	if (SDL_SetVideoMode(width, height, bpp, sdlFlags)) {
		//blah_video_current_mode = mode;  //FIXME??
		glViewport(0, 0, width, height);
		blah_draw_gl_init();
		blah_draw_gl_update2dProjection(mode);
		Blah_Debug_Log_message(&blah_video_sdl_log, "Video mode set successful");
		return true;
	} else {
		Blah_Debug_Log_message(&blah_video_sdl_log, "Failed to set video mode");
		return false;
	}
}
