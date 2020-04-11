/* blah_material.c
	Functions relating to drawing materials.  Materials describe the properties
	of drawn visuals.	*/

#include <malloc.h>

#include "blah_types.h"
#include "blah_material.h"
#include "blah_model.h"

/* Function Declarations */

Blah_Material *Blah_Material_fromSurface(Blah_Model_Surface *surface) {
	//Creates a new material from a model surface
	Blah_Material *newMaterial = malloc(sizeof(Blah_Material));

	float matTransparency = 1.0f - surface->transparency;
	Blah_Material_setAmbient(newMaterial, surface->colour.red, surface->colour.green,
		surface->colour.blue, matTransparency); //Set material ambient colour
	Blah_Material_setDiffuse(newMaterial, surface->colour.red * surface->diffuse,
		surface->colour.green * surface->diffuse, surface->colour.blue * surface->diffuse, 1);
	Blah_Material_setSpecular(newMaterial, surface->colour.red * surface->specular,
		surface->colour.green * surface->specular, surface->colour.blue * surface->specular, 1);
	Blah_Material_setEmission(newMaterial, surface->colour.red * surface->luminosity,
		surface->colour.green * surface->luminosity, surface->colour.blue * surface->luminosity, 1);
	Blah_Material_setShininess(newMaterial, surface->glossiness);

	return newMaterial;
}

void Blah_Material_init(Blah_Material *material)
{	//Initialises given material structure to defaults
	Blah_Material_setDefaults(material);	//For now, all this function does is call the setDefaults function
}

Blah_Material *Blah_Material_new() {
	//Creates a new material object and sets all attributes to defaults
	Blah_Material *newMaterial = malloc(sizeof(Blah_Material));

	if (newMaterial) //Ensure that memory allocation succeeded
	{
		Blah_Material_init(newMaterial);  //Perform initialisation of new allocated object
	}

	return newMaterial; //Allocate new material object and set default properties
}

void Blah_Material_setAmbient(Blah_Material *material, float red, float green, float blue, float alpha) {
	//Sets the ambient RGBA colour properties of the material
	Blah_Colour_set(&material->ambient, red, green, blue, alpha);
}

void Blah_Material_setColour(Blah_Material *material, float red, float green, float blue, float alpha) {
	//Sets all colour properties to standard default variations of the specified colour.
	Blah_Material_setAmbient(material, red, green, blue, alpha);
	Blah_Material_setDiffuse(material, red * BLAH_MATERIAL_DEFAULT_DIFFUSE,
		green * BLAH_MATERIAL_DEFAULT_DIFFUSE, blue * BLAH_MATERIAL_DEFAULT_DIFFUSE, 1);
	Blah_Material_setSpecular(material, red * BLAH_MATERIAL_DEFAULT_SPECULAR,
		green * BLAH_MATERIAL_DEFAULT_SPECULAR, blue * BLAH_MATERIAL_DEFAULT_SPECULAR, 1);
	Blah_Material_setEmission(material, red * BLAH_MATERIAL_DEFAULT_EMISSION,
		green * BLAH_MATERIAL_DEFAULT_EMISSION, blue * BLAH_MATERIAL_DEFAULT_EMISSION, 1);
	Blah_Material_setShininess(material, BLAH_MATERIAL_DEFAULT_SHININESS);
}

void Blah_Material_setDefaults(Blah_Material *material) {
	//Sets the attributes of the given material to the defaults
	Blah_Colour_set(&material->ambient, BLAH_MATERIAL_DEFAULT_AMBIENT,
		BLAH_MATERIAL_DEFAULT_AMBIENT, BLAH_MATERIAL_DEFAULT_AMBIENT, 1);
	Blah_Colour_set(&material->diffuse, BLAH_MATERIAL_DEFAULT_DIFFUSE,
		BLAH_MATERIAL_DEFAULT_DIFFUSE, BLAH_MATERIAL_DEFAULT_DIFFUSE, 1);
	Blah_Colour_set(&material->specular, BLAH_MATERIAL_DEFAULT_SPECULAR,
		BLAH_MATERIAL_DEFAULT_SPECULAR, BLAH_MATERIAL_DEFAULT_SPECULAR, 1);
	Blah_Colour_set(&material->emission, BLAH_MATERIAL_DEFAULT_EMISSION,
		BLAH_MATERIAL_DEFAULT_EMISSION, BLAH_MATERIAL_DEFAULT_EMISSION, 1);
	material->shininess = BLAH_MATERIAL_DEFAULT_SHININESS;
}

void Blah_Material_setDiffuse(Blah_Material *material, float red, float green, float blue, float alpha) {
	//Sets the diffuse RGBA colour properties of the material
	Blah_Colour_set(&material->diffuse, red, green, blue, alpha);
}

void Blah_Material_setEmission(Blah_Material *material, float red, float green, float blue, float alpha) {
	//Sets the emission RGBA colour properties of the material
	Blah_Colour_set(&material->emission, red, green, blue, alpha);
}

void Blah_Material_setShininess(Blah_Material *material, unsigned int shininess) {
	//Sets the shininess propertie of the material
	material->shininess = shininess;
}

void Blah_Material_setSpecular(Blah_Material *material, float red, float green, float blue, float alpha) {
	//Sets the specular RGBA colour properties of the material
	Blah_Colour_set(&material->specular, red, green, blue, alpha);
}

