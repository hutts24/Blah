/* blah_light.h
	Defines a structure to represent a light source */

#ifndef _BLAH_LIGHT	

#define _BLAH_LIGHT

#include "blah_point.h"
#include "blah_vector.h"
#include "blah_colour.h"

/* Structure Definitions */

typedef struct Blah_Light { //A structure to represent a vertex of a primitive
	Blah_Point location;	//Location of vertex in 3D space
	Blah_Vector direction;	//Vector representing direction in which light is projected
	Blah_Colour diffuse;	//Colour properties for diffuse light properties
	Blah_Colour ambient;	//Colour properties for diffuse light properties
	float intensity;		//0 .. 1, 1 is maximum intensity, 0 is uniform
	float spread;			//spread angle, from 0 to 360 degrees, 360 is uniform
} Blah_Light;

/* Function Prototypes */

#ifdef __cplusplus
	extern "C" {
#endif //__cplusplus

void Blah_Light_init(Blah_Light *light);
	//Initialises light structure given its pointer, by assigning default
	//values to internal properties.

Blah_Light *Blah_Light_new();
	//Creates a new light source structure and itialise properties to default values.
	//Function returns pointer to new light structure on success, or NULL pointer
	//if an error occurred.

void Blah_Light_setLocation(Blah_Light *light, float x, float y, float z); 
	//sets position coordinates of light source

void Blah_Light_setAmbient(Blah_Light *light, float red, float green, float blue, float alpha);
	//Sets the ambient properties of the light source

void Blah_Light_setDiffuse(Blah_Light *light, float red, float green, float blue, float alpha);
	//Sets the diffuse properties of the light source

void Blah_Light_setDirection(Blah_Light *light, float x, float y, float z);
	//Sets direction of light

void Blah_Light_setIntensity(Blah_Light *light, float intensity);
	//Sets the intensity of the specified light

void Blah_Light_setSpread(Blah_Light *light, float spread);
	//Sets the full spread angle of the specified light from 0 to 360 degrees

#ifdef __cplusplus
	}
#endif //__cplusplus

#endif
