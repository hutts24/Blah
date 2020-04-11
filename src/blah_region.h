/* blah_region.h
	Defines a 2 dimensional rectangluar region with four corners */
	
#ifndef _BLAH_REGION

#define _BLAH_REGION

#include "blah_types.h"

/* Structure Definitions */

typedef struct { //Region structure, 4 values to specify dimensions 
	int left, right, bottom, top;
} Blah_Region;
	
/* Function Prototypes */

#ifdef __cplusplus
	extern "C" {
#endif //__cplusplus

void Blah_Region_init(Blah_Region *region, int left, int bottom, int right, int top);

#ifdef __cplusplus
	}
#endif //__cplusplus		

#endif
