/* blah_video.h */

#ifndef _BLAH_VIDEO
#define _BLAH_VIDEO

#include "blah_types.h"
#include "blah_list.h"

/* Definitions */

#define BLAH_VIDEO_API_NAME_LENGTH 20

#define BLAH_VIDEO_MODE_NAME_LENGTH 20

/* Forward Declarations */

struct Blah_Video_Settings;
struct Blah_Video_Mode;

/* Function Type Declarations */

typedef blah_bool blah_video_api_init_func(struct Blah_Video_Settings *settings);
	//This type of function is called to initialise the video subsystem

typedef void blah_video_api_main_func();
	//This type of function is called as the main routine to control video management

typedef blah_bool blah_video_api_exit_func();
	//This type of function is called to exit and shutdown the video subsystem

typedef void blah_video_api_swap_func();
	//This type of function is called to swap the buffers of the video subsystem

typedef void blah_video_api_clear_func();
	//This type of function is called to clear the current drawing buffer of the video subsystem

typedef void blah_video_api_fs_func();
	//This type of function is called to set the video subsystem to full screen mode

typedef void blah_video_api_update_func();
	//This type of function is called to update changes to the video subsystem

typedef void blah_video_api_db_func(blah_bool fullFlag);
	//This type of function is called to establish double buffering of video subsystem

typedef blah_bool blah_video_api_mode_func(struct Blah_Video_Mode *mode);
	//This type of function is called to change the mode of the video subsystem

/* Data Structures */

typedef struct Blah_Video_API { //Defines functions to use with a specific API
	char name[BLAH_VIDEO_API_NAME_LENGTH+1]; //name of API
	blah_video_api_init_func *initFunction;
	blah_video_api_exit_func *exitFunction;
	blah_video_api_swap_func *swapBuffersFunction;
	blah_video_api_clear_func *clearBufferFunction;
	blah_video_api_fs_func *setFullScreenFunction;
	blah_video_api_update_func *updateBufferFunction;
	blah_video_api_db_func *setDoubleBufferedFunction;
	blah_video_api_mode_func *setModeFunction;
} Blah_Video_API;

typedef struct Blah_Video_Settings { //Stores all current configuration settings for video
	blah_bool initialised;
} Blah_Video_Settings;

typedef struct Blah_Video_Mode { 	//Represents a physical display video mode
	char name[BLAH_VIDEO_MODE_NAME_LENGTH+1]; //name of video mode
	blah_bool fullScreen;  		//Default to windowed video
	blah_bool doubleBuffered;		//If using double buffering
	unsigned int width;  			//Width of video mode in pixels
	unsigned int height; 			//Height of video mode in pixels
	unsigned int colourDepth;		//Pixel colour depth, in bits per pixel windowed
} Blah_Video_Mode;

/* Video Function Prototypes */

//Video functions

#ifdef __cplusplus
	extern "C" {
#endif //__cplusplus

void blah_video_swapBuffers(); //Swap video buffers
void blah_video_clearBuffer();
blah_bool blah_video_init();	//Initialise video.  Returns TRUE on success, else false
void blah_video_main(); //Handles video buffer swapping and drawing
void blah_video_exit(); //Exit video engine component

void blah_video_setFullScreen(blah_bool fullFlag);
	//If parameter is true, puts video into full screen mode, else windowed
	//Does nothing if display as not been set to a mode

void blah_video_setDoubleBuffered(blah_bool flag);
	//Turns double buffering on/off depending on flag
	//Does nothing if display as not been set to a mode

void blah_video_setSizeWindowed(int width, int height);
void blah_video_setSizeFullScreen(int width, int height);

void blah_video_updateBuffer();
	//Update all changes from drawing buffer into current buffer

blah_bool blah_video_isFullScreen();
	//Returns BLAH_TRUE if in full screen mode

blah_bool blah_video_isDoubleBuffered();
	//Returns BLAH_TRUE if in full double buffering active

blah_bool blah_video_setMode(Blah_Video_Mode *mode);
	//Sets the display device to the given mode.  Returns TRUE upon success, else false

Blah_Video_Mode *blah_video_getCurrentMode();
	//Returns a pointer to the video mode structure representing the current mode

Blah_Video_Mode *blah_video_getMode(int width, int height, int bppDepth);
	//Searches for an available mode with the given attributes and returns handle
	//to Blah_Video_Mode structure if one exists, otherwise NULL.  Note that this structure is
	//internal to blah video and must not be altered in any way!

Blah_Video_Mode *blah_video_getIdealMode(int width, int height, int bppDepth);
	//Searches for an available mode with the given attributes and returns handle
	//to Blah_Video_Mode structure if one exists, otherwise the next best mode is
	//returned.  Note that this structure is internal to blah video and must
	//not be altered in any way!


Blah_Video_Mode *blah_video_getNextMode(Blah_Video_Mode *mode);
	//Searches for a higher resolution mode than the given mode, using the same
	//colour depth.  Returns a pointer to the new mode found or NULL if there is no
	//higher resolution mode available with requested colour depth;

Blah_Video_Mode *blah_video_getPrevMode(Blah_Video_Mode *mode);
	//Searches for a lower resolution mode than the given mode, using the same
	//colour depth.  Returns a pointer to the new mode found or NULL if there is no
	//lower resolution mode available with requested colour depth;

Blah_Video_Mode *blah_video_getLeastMode();
	//Returns a pointer to the lowest resolution mode using the lowest
	//available colour depth

Blah_Video_Mode *blah_video_getBestMode();
	//Returns a pointer to the highest resolution mode using the highest
	//available colour depth

//Video Mode functions

Blah_Video_Mode *Blah_Video_Mode_new(char *name, blah_bool fullScreen,
	blah_bool doubleBuffered, int width, int height, int bppDepth);
	//Creates a new video mode with given properties supplied in params.
	//Allocates memory and returns new structure.

#ifdef __cplusplus
}
#endif //__cplusplus

#endif
