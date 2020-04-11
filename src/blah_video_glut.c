/* blah_video_glut.c 
	GLUT specific video routines */

#ifdef BLAH_USE_GLUT //whole file commented

#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>

#include "blah_video.h"
#include "blah_engine.h"
 //FIXME --Allow parameter passing to control dimensions,
					//or put values into a "world" data structure
					//Also move glut_idle into video_glut.c

/* Globals */

blah_bool blah_video_glut_initialised = BLAH_FALSE;  //state flag


/* Private Function Prototypes */

void blah_video_glut_display(); //Glut video display function
void blah_video_glut_idle(); //Glut idle handler function
	
/* Function Declarations */

void blah_video_glut_display() { //Glut video display function
	blah_video_main();
}

void blah_video_glut_idle() { //Glut idle handler function
	blah_engine_main();
}

void blah_video_glut_reshape(int w, int h) {
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
}


void blah_video_glut_init() {  //Use crappy glut for now
	fprintf(stderr,"GLUT:call to glutinit in progress\n");
	glutInit(NULL, NULL);
	fprintf(stderr,"GLUT:called glutinit\n");
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); 
                        //| GLUT_ALPHA | GLUT_DEPTH);
	fprintf(stderr,"GLUT:set display mode\n");
	glutInitWindowSize (500, 500);
	fprintf(stderr,"set window size\n");
	glutInitWindowPosition (100, 100);
	fprintf(stderr,"window position\n");
	glutCreateWindow ("blah"); 
	//glutEnterGameMode();
	fprintf(stderr,"GLUT:created window\n"); 
	
	//Set up GLUT handler functions
	glutDisplayFunc(blah_video_glut_display);
	glutReshapeFunc(blah_video_glut_reshape);
	glutIdleFunc(blah_video_glut_idle);
	
	blah_video_glut_initialised = BLAH_TRUE;  //set state flag on
} 

void blah_video_glut_exit() { //Shutdown GLUT video component
	blah_video_glut_initialised = BLAH_FALSE;  //set state flag off
}

void blah_video_glut_swapBuffers() { //Calls the appropriate vide function to swap buffers
	glFinish();
	glutSwapBuffers();	//Using glut for now
}

void blah_video_glut_clearBuffer() {  //Clears current drawing buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void blah_video_glut_setFullScreen(blah_bool fullFlag) {
	//If parameter is true, puts video into full screen mode, else windowed
	//if (full_flag) 
	//	glutEnterGameMode();
	//else 
	//	glutEn;
	glutFullScreen();
}

#endif //whole file is commented
