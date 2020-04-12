/* blah_model.h
	Header file for blah_model.c
	Defines image structures and functions for creating and manipulating models
*/

#ifndef _BLAH_MODEL

#define _BLAH_MODEL

#include <stdio.h>

#include "blah_image.h"
#include "blah_point.h"
#include "blah_primitive.h"
#include "blah_colour.h"
#include "blah_vector.h"


/* Defines */

#define BLAH_MODEL_NAME_LENGTH 20
#define BLAH_MODEL_SURFACE_NAME_LENGTH		50

/* Type definitions */

typedef enum blah_model_texture_projection {BLAH_MODEL_TEXTURE_PROJECTION_PLANAR,
		BLAH_MODEL_TEXTURE_PROJECTION_CYLINDRICAL, BLAH_MODEL_TEXTURE_PROJECTION_SPHERICAL,
		BLAH_MODEL_TEXTURE_PROJECTION_CUBIC} blah_model_texture_projection;

/* Data Structures */

typedef struct Blah_Model_Surface {
	char name[BLAH_MODEL_SURFACE_NAME_LENGTH+1]; //FIXME - should be dynamic string
	Blah_Colour colour;
	float luminosity;			//floating point 0 - 1, describes luminosity of surface
	float diffuse;				//surface diffuse property
	float specular;				//surface specular attribute
	float reflection;			//surface reflection property
	float transparency;			//surface transparency, 0 - opaque, 1 fully transparent
	unsigned int glossiness;	//surface glossiness

	Blah_List textures;			//List of textures mapped to this surface
	Blah_List faces;			//List of all the faces composing this surface
} Blah_Model_Surface;

typedef struct Blah_Model_Texture_Map {
	char projectionAxis;		//'x', 'y', 'z' - denotes axis of projection
	Blah_Texture *texture; 		//pointer to texture used in mapping
	Blah_Point textureCenter;	//Coordinates of texture center, relative to model coordinates
	Blah_Vector textureSize;	//Size of area covered by texture in floats x,y,z
	blah_model_texture_projection projectionMode;
} Blah_Model_Texture_Map;

typedef struct Blah_Model_Face {
	Blah_List indices;		//List of indices (Indexes into the model's points listed
	short int surface;		//Index into the model's list of surfaces, 0 means ignore
} Blah_Model_Face;

typedef struct Blah_Model { //Represents a 3d model
	Blah_List vertices;		//List of (Blah_Vertex)vertices which model is constructed from
	Blah_List faces;		//List of (Blah_Model_Face)faces composing model
	Blah_List surfaces;		//List of (Blah_Model_Surface)surface types
	char name[BLAH_MODEL_NAME_LENGTH+1];
} Blah_Model;

/* Function Prototypes */

#ifdef __cplusplus
	extern "C" {
#endif //__cplusplus

/* Model Functions */

void Blah_Model_addFace(Blah_Model *model, Blah_Model_Face *newFace);
	//Adds new_face to the list of faces for model

void Blah_Model_addSurface(Blah_Model *model, Blah_Model_Surface *newSurface);
	//Adds new_surface to the list of surfaces for model

void Blah_Model_addVertex(Blah_Model *model, Blah_Vertex *newVertex);
	//Adds new_point to the list of points for model

void Blah_Model_destroy(Blah_Model *model);
	//Destroys a model structure and removes from garbage collection list

void blah_model_destroyAll();
	//Garbage cleanup function to destroy all models still in the tree in memory

bool Blah_Model_init(Blah_Model *model, char *modelName);
	//Initialises given model with specified name, and clears the list of vertices,
	//list of faces, and list of surfaces.

void Blah_Model_disable(Blah_Model *model);
	//Frees all allocated memory for structure internals

Blah_Model *Blah_Model_load(char *filename);
	//Creates a new model structure.  Memory is allocated etc

Blah_Model *Blah_Model_new(char *modelName);
	//Creates a new model structure with given name and returns pointer.  Adds new model to list for garbage collection

void Blah_Model_remove_face(Blah_Model *model, Blah_Model_Face *face);
	//Removes face from the list of faces for model

void Blah_Model_removeVertex(Blah_Model *model, Blah_Vertex *vertex);
	//Removes point from the list of points for model

void Blah_Model_scale(Blah_Model *model, float scaleFactor);
	//Alters every vertex in the model by multiplying each coordinate by scale_factor

/* Texture Map Functions */

Blah_Model_Texture_Map *Blah_Model_Texture_Map_new(Blah_Texture *texture, char projectionAxis, blah_model_texture_projection proj);
	//Construct a new texture map object and return pointer

void Blah_Model_Texture_Map_init(Blah_Model_Texture_Map *map, Blah_Texture *texture, char projectionAxis, blah_model_texture_projection proj);
	//Initialise map with given properties, and set other non-specified properties to defaults

void Blah_Model_Texture_Map_setSize(Blah_Model_Texture_Map *map, float x, float y, float z);
	//Sets the texture size in relation to the object size

void Blah_Model_Texture_Map_setCenter(Blah_Model_Texture_Map *map, float x, float y, float z);
	//Sets the position of texture center in relation to the object coordinates

/* Face function headers */


void Blah_Model_Face_addIndex(Blah_Model_Face *face, int index);
	//Adds a new point index to the face, given numberical index number

void Blah_Model_Face_destroy(Blah_Model_Face *face);
	//Destroys a face structure

void Blah_Model_Face_init(Blah_Model_Face *face);
	//Initialises a face structure by initialising face list contained within,
	//and setting the surface index to zero (N/A)

void Blah_Model_Face_disable(Blah_Model_Face *face);
	//Disables the face object by freeing all indices belonging to face.

Blah_Model_Face *Blah_Model_Face_new();
	//Construct a new face object without any vertex indices and return pointer.

void Blah_Model_Face_removeIndex(Blah_Model_Face *face, int index);
	//Removes given point index fromt the face's list of indexes

/* Surface function headers */

void Blah_Model_Surface_addFace(Blah_Model_Surface *surface, Blah_Model_Face *face);
	//Adds new_face to the list of faces for given surface

void Blah_Model_Surface_addTexture(Blah_Model_Surface *surface, Blah_Model_Texture_Map *map);
	//Adds the texture mapping to the list of texture maps for the given surface

void Blah_Model_Surface_destroy(Blah_Model_Surface *surface);
	//Destroys a model surface object

void Blah_Model_Surface_disable(Blah_Model_Surface *surface);
	//Frees all allocated resources for surface (textures, faces)

Blah_Model_Surface *Blah_Model_Surface_new(char *name);
	//Creates a new blank surface with given name and returns pointer.

void Blah_Model_Surface_init(Blah_Model_Surface *surface, char *name);
	//Assigns given name to surface and sets all other surface properties to defaults.

#ifdef __cplusplus
	}
#endif //__cplusplus

#endif
