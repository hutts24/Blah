/* blah_input_keyboard.h

	Functions to handle keyboard input */

#ifndef _BLAH_INPUT_KEYBOARD

#define _BLAH_INPUT_KEYBOARD

#include "blah_types.h"


/* Definitions */

#define BLAH_INPUT_KEYBOARD_NUM_KEYS 16
#define BLAH_INPUT_KEYBOARD_API_NAME_LENGTH 20

/* Forward Declarations */

struct Blah_Input_Key;

/* Types */

enum Blah_Input_Key_Symbol { BLAH_INPUT_KEY_LEFT, BLAH_INPUT_KEY_RIGHT,
	BLAH_INPUT_KEY_UP, BLAH_INPUT_KEY_DOWN,	BLAH_INPUT_KEY_SPACE,
	BLAH_INPUT_KEY_RETURN, BLAH_INPUT_KEY_LEFT_SHIFT, BLAH_INPUT_KEY_ESCAPE,
	BLAH_INPUT_KEY_V, BLAH_INPUT_KEY_D, BLAH_INPUT_KEY_H, BLAH_INPUT_KEY_X,
	BLAH_INPUT_KEY_T, BLAH_INPUT_KEY_Z, BLAH_INPUT_KEY_EQUALS, BLAH_INPUT_KEY_MINUS};

typedef enum Blah_Input_Key_Symbol blah_input_key_symbol;

/* Function Type Declarations */

typedef void blah_input_key_depress_func(struct Blah_Input_Key *key);
	//This type of function is called once when a key is depressed
typedef void blah_input_key_release_func(struct Blah_Input_Key *key);
	//This type of function is called once when a key is released
typedef void blah_input_key_hold_func(struct Blah_Input_Key *key);
	//This type of function is called whilst the key is depressed after the first
	//call to the initial depress func;

typedef void blah_input_api_kb_init_func();
	//This type of function is called to initialise the keyboard subsystem
typedef void blah_input_api_kb_main_func();
	//This type of function is called as the main routine to control keyboard
	//polling for events etc
typedef void blah_input_api_kb_exit_func();
	//This type of function is called to exit and shutdown the keyboard subsystem


/* Structure Definitions */

typedef struct Blah_Input_Key {
	blah_input_key_symbol key; 		//Which key this is exactly (e.g. BLAH_INPUT_KEY_UP)
	bool depressed;  		// true if key is currently pressed down
	bool oldDepressed;		//Old value used to compare if button state has changed
								//Used during keyboard inspection in input_keyboard_main()
								//Don't mess with this!
	bool monitored;  //If flag is false, then input for this key will be ignored
	blah_input_key_depress_func *depressFunction; //Function to invoke (only once) when key is pressed
	blah_input_key_release_func *releaseFunction; //Function to invoke when key is released
	blah_input_key_hold_func *holdFunction;	//Function to invoke when key is held down
									//This function is not called when key is intially
									//detected as being depressed.  See depress_function.
} Blah_Input_Key;

typedef struct Blah_Input_Keyboard_API { //Defines functions to use with a specific API
	char name[BLAH_INPUT_KEYBOARD_API_NAME_LENGTH+1]; //name of API
	blah_input_api_kb_init_func *initFunction;
	blah_input_api_kb_main_func *mainFunction;
	blah_input_api_kb_exit_func *exitFunction;
} Blah_Input_Keyboard_API;

/* Function Prototypes */

#ifdef __cplusplus
	extern "C" {
#endif //__cplusplus

void blah_input_keyboard_init(); //initialises keyboard input component
void blah_input_keyboard_exit(); //shutdown keyboard input component
void blah_input_keyboard_main();  //update keyboard status

void blah_input_keyboard_setDepressFunction(blah_input_key_symbol keySymbol, blah_input_key_depress_func *function);
	//attaches a handler function to be called when key is initially depressed
	//Function will be called with a single argument of type BLAH_INPUT_KEY representing
	//the key which has been depressed

void blah_input_keyboard_setHoldFunction(blah_input_key_symbol keySymbol, blah_input_key_hold_func *function);
	//attaches a handler function to be called while a key is still depressed
	//after the first initial detection.  Function will be called with a single
	//argument of type BLAH_INPUT_KEY representing the key which is depressed

#ifdef __cplusplus
	}
#endif //__cplusplus

#endif
