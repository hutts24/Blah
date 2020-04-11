/* blah_input_keyboard_glut.h
	GLUT specific functions
	Handle keyboard input GLUT */
	
#ifndef _BLAH_INPUT_KEYBOARD_GLUT

#define _BLAH_INPUT_KEYBOARD_GLUT

#include <GL/glut.h>

#include "blah_types.h"

/* Definitions */

/* Types */

/* Structure Definitions */

/* Function Prototypes */

#ifdef __cplusplus
	extern "C" {
#endif //__cplusplus

void blah_input_keyboard_glut_init (); //initialises GLUT keyboard input
void blah_input_keyboard_glut_exit(); //shutdown GLUT keyboard input component
void blah_input_keyboard_glut_main();  //update keyboard status via GLUT

#ifdef __cplusplus
	}
#endif //__cplusplus		
		
#endif
