/* blah_video_sdl.h
	SDL specific video routines */

#ifndef _BLAH_VIDEO_SDL

#define _BLAH_VIDEO_SDL

#include "blah_video.h"
#include "blah_types.h"


/* Public Prototypes */

#ifdef __cplusplus
	extern "C" {
#endif //__cplusplus

bool blah_video_sdl_init(const Blah_Video_Settings* settings); //Initialise SDL video
bool blah_video_sdl_exit(); //Shutdown SDL video component
void blah_video_sdl_swapBuffers();  //Swap video buffer when double buffering
void blah_video_sdl_clearBuffer(); //Clears drawing buffer
void blah_video_sdl_setDoubleBuffered(bool flag);
	//Turns double buffering on/off depending on flag

void blah_video_sdl_setFullScreen(bool fullFlag);
	//If parameter is true, puts video into full screen mode, else windowed

void blah_video_sdl_setSizeFullScreen(int width, int height);

void blah_video_sdl_updateBuffer();
	//Update all drawing changes to buffer

bool blah_video_sdl_setMode(const Blah_Video_Mode *mode);
	//Use SDL libraries to set display device in given mode

#ifdef __cplusplus
	}
#endif //__cplusplus

#endif
