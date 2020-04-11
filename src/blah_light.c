/* blah_light.c
	Defines functions which perform operations upon light source structures */

#include "blah_light.h"
#include "malloc.h"

/* Function Declarations */

void Blah_Light_init(Blah_Light *light) {
	//Initialises light structure given its pointer, by assigning default
	//values to internal properties.
	Blah_Point_set(&light->location, 0,0,0);
	Blah_Colour_set(&light->ambient, 1,1,1,1);
	Blah_Colour_set(&light->diffuse, 1,1,1,1);
	Blah_Vector_set(&light->direction, 0,0,-1);
	light->spread = 360;
	light->intensity = 0;
}

Blah_Light *Blah_Light_new() {
	//Creates a new light source structure and itialise properties to default values.
	//Function returns pointer to new light structure on success, or NULL pointer
	//if an error occurred.
	Blah_Light *newLight = (Blah_Light*)malloc(sizeof(Blah_Light));
	
	if (newLight) //If memory allocation succeeded,
	{
		Blah_Light_init(newLight); //Initalise newly created light structure
	}
	
	return newLight;
}

void Blah_Light_setLocation(Blah_Light *light, float x, float y, float z) {
	//sets coordinates of light position
	Blah_Point_set(&light->location, x,y,z);
}
	
void Blah_Light_setAmbient(Blah_Light *light, float red, float green, float blue, float alpha) {
	//Sets the ambient properties of the light source
	Blah_Colour_set(&light->ambient, red, green, blue, alpha);
}

void Blah_Light_setDiffuse(Blah_Light *light, float red, float green, float blue, float alpha) {
	//Sets the diffuse properties of the light source
	Blah_Colour_set(&light->diffuse, red, green, blue, alpha);
}

void Blah_Light_setDirection(Blah_Light *light, float x, float y, float z) {
	//Sets direction of light
	Blah_Vector_set(&light->direction, x,y,z);
}

void Blah_Light_setIntensity(Blah_Light *light, float intensity) {
	//Sets the intensity of the specified light
	light->intensity = intensity;
}

void Blah_Light_setSpread(Blah_Light *light, float spread) {
	//Sets the full spread angle of the specified light from 0 to 360 degrees
	light->spread = spread;
}
