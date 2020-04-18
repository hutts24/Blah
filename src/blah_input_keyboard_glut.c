/* blah_input_keyboard_glut.c
	GLUT Specific Keyboard functions.
	Functions to handle keyboard input via GLUT */

#ifdef BLAH_USE_GLUT //comment whole file

#include <stdio.h>

#include "blah_input_keyboard_glut.h"
#include "blah_input_keyboard.h"
#include "blah_video_glut.h"


/* Defines */

#define BLAH_INPUT_KEYBOARD_GLUT_NUM_KEYSYMS 256

/* Globals */

bool blahInputKeyboardGLUTInitialised = false;  //state flag

blah_input_key_symbol blahInputKeyboardGLUTMap[BLAH_INPUT_KEYBOARD_GLUT_NUM_KEYSYMS];
	//FIXME an array to map GLUT ascii key strokes to engine key syms

/* Private Function Prototypes */

static void blah_input_keyboard_glut_handler(unsigned char key, int x, int y);

/* Function Declarations */

void blah_input_keyboard_glut_init () { //initialises GLUT keyboard input
	int symCount;
	int activeKeyIndex=0;
	int glutActiveKeys[] = {27,BLAH_INPUT_KEY_ESCAPE, 'w',BLAH_INPUT_KEY_UP, ' ',BLAH_INPUT_KEY_SPACE, -1};

	//GLUT Video is required for GLUT keyboard input.  (Window must catch events)
	if (!blahVideoGLUTInitialised) //if SDL Video hasn't been initialised
		blah_video_glut_init(); //Initialise SDL with video subsystem and we get keyboard also

	//Set all mappings to -1, meaning not mapped to engine key symbols
	for (symCount=0; symCount < BLAH_INPUT_KEYBOARD_GLUT_NUM_KEYSYMS; symCount++)
		blahInputKeyboardGLUTMap[symCount] = -1;

	//Set up active mappings
	while (glutActiveKeys[activeKeyIndex] !=-1) { //while end of mapping list hasn't been reached
		blahInputKeyboardGLUTMap[glutActiveKeys[activeKeyIndex]] = glutActiveKeys[activeKeyIndex+1];
		activeKeyIndex+=2;
	}

	glutKeyboardFunc(blah_input_keyboard_glut_handler);
	blahInputKeyboardGLUTInitialised = true;  //set state flag on
}

void blah_input_keyboard_glut_exit() { //shutdown SDL keyboard input component
	glutKeyboardFunc(NULL); //Clear GLUT keyboard handler function pointer
	blahInputKeyboardGLUTinitialised = false;  //set state flag off
}

static void blah_input_keyboard_glut_handler(unsigned char key, int x, int y) {
	blah_input_key_symbol keySym;
	//key pressed down
	keySym = blahInputKeyboardGLUTMap[key];
	if (keySym !=-1)  //if the GLUT key symbol is actively mapped
		blahInputKeys[keySym].depressed = true;  //update key status

}

void blah_input_keyboard_glut_main() {  //update keyboard status via SDL
	blah_input_key_symbol keySym;

	//NOTE: input_keyboard_glut_handler is responsible for acknowledging key presses

	//Clear depressed state of all keys, since GLUT sucks and only gives us notification when a key is pressed
	//Wait for the glut handler to set flags to true (If it wants to)
	for (keySym = 0; keySym < BLAH_INPUT_KEYBOARD_NUM_KEYS; keySym++) {
		//input_keys[key_sym].old_depressed = input_keys[key_sym].depressed;
		blahInputKeys[keySym].depressed = false;
	}
}

#endif
