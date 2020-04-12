/* blah_engine.h */

#ifndef _BLAH_ENGINE

#define _BLAH_ENGINE

#ifdef __cplusplus
	extern "C" {
#endif //__cplusplus

#include "blah_types.h"

void blah_engine_exit();
	//Quit the engine.

bool blah_engine_init();
	//Initialise all engine components

void blah_engine_main();
	//Main processing function.  Invokes all component routines


#ifdef __cplusplus
	}
#endif //__cplusplus

#endif
