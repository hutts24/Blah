/* blah_input.h */

#ifndef _BLAH_INPUT

#define _BLAH_INPUT

/* Definitions */

/* Data Structures */

/* Function Prototypes */

#ifdef __cplusplus
	extern "C" {
#endif //__cplusplus

#include "blah_types.h"


bool blah_input_init(); //initialises input component.  Returns true on success
void blah_input_exit(); //shutdown input component
void blah_input_main(); //updates current status of all monitored user input devices

#ifdef __cplusplus
	}
#endif //__cplusplus

#endif
