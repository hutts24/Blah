/* blah_colour.h
	Colour routines */

#ifndef _BLAH_COLOUR

#define _BLAH_COLOUR

#include "blah_types.h"

/* Structure Definitions */

typedef struct
{	//Colour structure, 3 component values for colour, 1 alpha component
	float red, green, blue, alpha;
} Blah_Colour;

/* Function Prototypes */

#ifdef __cplusplus
	extern "C" {
#endif //__cplusplus

void Blah_Colour_init(Blah_Colour *colour, float red, float green, float blue, float alpha);
	//Initialises colour structure with RGB and Alpha component values, given its pointer

void Blah_Colour_set(Blah_Colour *colour, float red, float green, float blue, float alpha );
	//Sets the red, green, blue and alpha component values of the given colour

#ifdef __cplusplus
	}
#endif //__cplusplus

#endif
