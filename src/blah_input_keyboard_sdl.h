/* blah_input_keyboard_sdl.h
	
	SDL specific functions to handle keyboard input */
	
#ifndef _BLAH_INPUT_KEYBOARD_SDL

#define _BLAH_INPUT_KEYBOARD_SDL

#include <SDL/SDL.h>

#include "blah_types.h"

/* Definitions */

/* Types */

/* Structure Definitions */

/* Function Prototypes */

#ifdef __cplusplus
	extern "C" {
#endif //__cplusplus

void blah_input_keyboard_sdl_init (); //initialises SDL keyboard input
void blah_input_keyboard_sdl_exit(); //shutdown SDL keyboard input component
void blah_input_keyboard_sdl_main();  //update keyboard status via SDL

#ifdef __cplusplus
	}
#endif //__cplusplus
		
#endif
