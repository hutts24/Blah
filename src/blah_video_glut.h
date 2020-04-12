/* blah_video_glut.h
	GLUT specific video routines */

#ifndef _BLAH_VIDEO_GLUT

#define _BLAH_VIDEO_GLUT

#include "blah_types.h"


/* Public Prototypes */

#ifdef __cplusplus
	extern "C" {
#endif //__cplusplus

void blah_video_glut_init(); //Initialise GLUT video
void blah_video_glut_exit(); //Shutdown GLUT video component
void blah_video_glut_swapBuffers();  //Swap video buffer when double buffering
void blah_video_glut_clearBuffer(); //Clears drawing buffer
void blah_video_glut_setFullScreen(bool fullFlag);
	//If parameter is true, puts video into full screen mode, else windowed

#ifdef __cplusplus
	}
#endif //__cplusplus

#endif
