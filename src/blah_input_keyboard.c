/* blah_input_keyboard.c

	Functions to handle keyboard input */

#include <stdio.h>

#include "blah_input_keyboard.h"
#include "blah_input_keyboard_sdl.h"
#include "blah_debug.h"

#ifdef BLAH_USE_GLUT
#include "blah_input_keyboard_glut.h"
#endif

#include "blah_macros.h"

/* Public globals */

Blah_Input_Key blahInputKeys[BLAH_INPUT_KEYBOARD_NUM_KEYS];
Blah_Debug_Log blah_input_keyboard_log = { .filePointer = NULL };

/* Private statics */
static Blah_Input_Keyboard_API blah_input_keyboard_SDL = {"SDL", blah_input_keyboard_sdl_init,
	blah_input_keyboard_sdl_main, blah_input_keyboard_sdl_exit};

#ifdef BLAH_USE_GLUT
static Blah_Input_Keyboard_API blah_input_keyboard_GLUT = {"GLUT",blah_input_keyboard_glut_init,
	blah_input_keyboard_glut_main, blah_input_keyboard_glut_exit};
#endif

static Blah_Input_Keyboard_API *blah_input_keyboard_currentAPI = &blah_input_keyboard_SDL; // Pointer to current API structure


/* Private function declarations */

// Sets the monitored flag to true if atleast one state handler
// is assigned, else false.
static void Blah_Input_Key_updateMonitored(Blah_Input_Key *key) {
	key->monitored = key->depressFunction || key->releaseFunction || key->holdFunction;
}

/* Public Function Declarations */

// initialises keyboard input configuration
void blah_input_keyboard_init () {
    Blah_Debug_Log_init(&blah_input_keyboard_log, "blah_input_keyboard");
    Blah_Debug_Log_message(&blah_input_keyboard_log, "Initialising keyboard input");
	// take care of the key status data
	for (int keySymbol = 0; keySymbol < BLAH_INPUT_KEYBOARD_NUM_KEYS; keySymbol++) {
		blahInputKeys[keySymbol].key = keySymbol;
		blahInputKeys[keySymbol].monitored = false;
		blahInputKeys[keySymbol].depressed = false;
		blahInputKeys[keySymbol].oldDepressed = false;
		blahInputKeys[keySymbol].depressFunction = NULL;
		blahInputKeys[keySymbol].releaseFunction = NULL;
		blahInputKeys[keySymbol].holdFunction = NULL;
	}
	blah_input_keyboard_currentAPI->initFunction(NULL);
}

// update keyboard status
void blah_input_keyboard_main() {
	//input_keyboard_sdl_main();  //Update key status via SDL
	blah_input_keyboard_currentAPI->mainFunction(NULL);
	//Invoke key handlers where applicable

	for (int keyCount = 0; keyCount < BLAH_INPUT_KEYBOARD_NUM_KEYS; keyCount++) {
		Blah_Input_Key *tempKey = &blahInputKeys[keyCount];
		if (tempKey->monitored) {  //check for change of state
			if (tempKey->depressed != tempKey->oldDepressed) {
				if (tempKey->depressed && tempKey->depressFunction) {
					tempKey->depressFunction(&blahInputKeys[keyCount]);
			 	} else if (!tempKey->depressed && tempKey->releaseFunction) {
					tempKey->releaseFunction(&blahInputKeys[keyCount]);
			 	}
				//update the old status to preserve state before next inspection
				tempKey->oldDepressed = tempKey->depressed;
			} else if (tempKey->depressed && tempKey->holdFunction) {
				tempKey->holdFunction(&blahInputKeys[keyCount]);
            }
				//if key status has not changed and is still held, call hold_function
		}
	}
}

// shutdown keyboard input component
void blah_input_keyboard_exit()
{
	blah_input_keyboard_currentAPI->exitFunction(NULL);
	Blah_Debug_Log_disable(&blah_input_keyboard_log);
}

void blah_input_keyboard_setDepressFunction(blah_input_key_symbol keySymbol, blah_input_key_depress_func* function) {
	//attaches a handler function to be called when key is initially depressed
	//Function will be called with a single argument of type Blah_Input_key representing
	//the key which has been depressed
	blahInputKeys[keySymbol].depressFunction = function;
	Blah_Input_Key_updateMonitored(&blahInputKeys[keySymbol]);
}

void blah_input_keyboard_setHoldFunction(blah_input_key_symbol keySymbol, blah_input_key_hold_func* function) {
	//attaches a handler function to be called while a key is still depressed
	//after the first initial detection.  Function will be called with a single
	//argument of type Blah_Input_key representing the key which is depressed
	blahInputKeys[keySymbol].holdFunction = function;
	Blah_Input_Key_updateMonitored(&blahInputKeys[keySymbol]);
}
