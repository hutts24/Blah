/* blah_colour.c 
	Colour related stuff */
	
#include "blah_colour.h"

/* Function Declarations */

void Blah_Colour_init(Blah_Colour *colour, float red, float green, float blue, float alpha)
{	//Initialises colour structure with RGB and Alpha component values, given its pointer
	Blah_Colour_set(colour, red, green, blue, alpha);
}

void Blah_Colour_set(Blah_Colour *colour, float red, float green, float blue, float alpha)
{	//Sets the red, green, blue and alpha component values of the given colour
	colour->red = red;
	colour->green = green;
	colour->blue = blue;
	colour->alpha = alpha;
}
