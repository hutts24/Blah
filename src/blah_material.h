/* blah_material.h
	Functions relating to drawing materials.  Materials describe the properties
	of drawn visuals.	*/

#ifndef _BLAH_MATERIAL

#define _BLAH_MATERIAL

#include "blah_types.h"
#include "blah_colour.h"

/* Definitions */

#define BLAH_MATERIAL_DEFAULT -100	//If a material property is set to this value,
									//it is unassigned and uses defaults
#define BLAH_MATERIAL_DEFAULT_AMBIENT	0.2f
#define BLAH_MATERIAL_DEFAULT_DIFFUSE	0.8f
#define BLAH_MATERIAL_DEFAULT_SPECULAR	0.0f
#define BLAH_MATERIAL_DEFAULT_EMISSION	0.0f
#define BLAH_MATERIAL_DEFAULT_SHININESS	0.0f

/* Forward type declarations */

struct Blah_Model_Surface;

/* Structure Definitions */

typedef struct Blah_Material {
	//For all values, BLAH_MATERIAL_DEFAULT means use default value
	Blah_Colour ambient;		//Range -1..1, describes ambient light reflectance
	Blah_Colour diffuse;		//Range -1..1 ,describes diffuse light property
	Blah_Colour specular;		//Range -1..1 ,describes specular reflectance attribute
	Blah_Colour emission;		//Range -1..1 ,describes intensity of emmited light
	unsigned int shininess;			//Range 0..1, surface reflection property
} Blah_Material;


/* Function Prototypes */

#ifdef __cplusplus
	extern "C" {
#endif //__cplusplus

Blah_Material *Blah_Material_fromSurface(struct Blah_Model_Surface *surface);
	//Creates a new material from a model surface

void Blah_Material_init(Blah_Material *material);
	//Initialises given material structure to defaults

Blah_Material *Blah_Material_new();
	//Creates a new material object and sets all attributes to defaults
	//Return value is a pointer to the new material object, or NULL if error

void Blah_Material_setAmbient(Blah_Material *material, float red, float green, float blue, float alpha);
	//Sets the ambient RGBA colour properties of the material

void Blah_Material_setColour(Blah_Material *material, float red, float green, float blue, float alpha);
	//Sets all colour properties to standard default variations of the specified
	//colour.

void Blah_Material_setDefaults(Blah_Material *material);
	//Sets the attributes of the given material to the defaults

void Blah_Material_setDiffuse(Blah_Material *material, float red, float green, float blue, float alpha);
	//Sets the diffuse RGBA colour properties of the material

void Blah_Material_setEmission(Blah_Material *material, float red, float green, float blue, float alpha);
	//Sets the emission RGBA colour properties of the material

void Blah_Material_setShininess(Blah_Material *material, unsigned int shininess);
	//Sets the shininess propertie of the material

void Blah_Material_setSpecular(Blah_Material *material, float red, float green, float blue, float alpha);
	//Sets the specular RGBA colour properties of the material


#ifdef __cplusplus
	}
#endif //__cplusplus

#endif
