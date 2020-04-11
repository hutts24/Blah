/* blah_input.c *

	Routines for handling user input */

#include <stdio.h>

#include "blah_input.h"
#include "blah_input_keyboard.h"
#include "blah_debug.h"

/* Global Variables */

Blah_Debug_Log *blahInputLog = NULL;

/* Function Declarations */

blah_bool blah_input_init() { //initialises input component.  Returns true on success
	blahInputLog = Blah_Debug_Log_new("blah_input");
	
	Blah_Debug_Log_message(blahInputLog,"enter input_init\n");
	
	blah_input_keyboard_init();
	Blah_Debug_Log_message(blahInputLog,"Called input_init\n");
	return BLAH_TRUE;
}

void blah_input_exit() { //shutdown input component
	blah_input_keyboard_exit();  //shutdown keyboard input component
	Blah_Debug_Log_destroy(blahInputLog);
}

void blah_input_main() { //updates current status of all monitored user input devices
	
	blah_input_keyboard_main();  //update keyboard status
}
