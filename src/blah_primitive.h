/* blah_primitive.h
	Primitives are the very basic shape elements used to construct geometric
	figures.  Types of primitives include lines, triangles, points etc */

#ifndef _BLAH_PRIMITIVE

#define _BLAH_PRIMITIVE

#include "blah_list.h"
#include "blah_colour.h"
#include "blah_vertex.h"
#include "blah_point.h"
#include "blah_texture.h"
#include "blah_material.h"

/* Definitions */

enum Blah_Primitive_Type {BLAH_PRIMITIVE_POLYGON, BLAH_PRIMITIVE_POINT, BLAH_PRIMITIVE_LINE,
	BLAH_PRIMITIVE_LINE_STRIP, BLAH_PRIMITIVE_TRIANGLE, BLAH_PRIMITIVE_TRIANLGE_STRIP, BLAH_PRIMITIVE_QUADRILATERAL};

typedef enum Blah_Primitive_Type blah_primitive_type;

typedef struct Blah_Primitive {
	blah_primitive_type type; //Denotes what kind of primitve (based apon OpenGL primitives)
	Blah_Vertex **sequence;  //A dynamically allocated array of pointers to vertices used to draw the primitive
	Blah_Texture_Map *textureMap; //Pointer to texture mapping
	Blah_Material *material;	//Pointer to material properties for this primitive
} Blah_Primitive;

/* Function Prototypes */

#ifdef __cplusplus
	extern "C" {
#endif //__cplusplus

bool Blah_Primitive_init(Blah_Primitive *prim, blah_primitive_type newType, Blah_Vertex* vertexArray[], unsigned int vertexCount);
	// Initialise primitive structure with given type and allocates memory for vertex sequence.
	// Vertex data is copied from parameter 'vertexArray'. Function returns false in case of error.

Blah_Primitive *Blah_Primitive_new(blah_primitive_type newType, Blah_Vertex *vertexArray[], unsigned int vertexCount);  //Creates a new primitive structure

void Blah_Primitive_destroy(Blah_Primitive *prim);
	// Destroys a primitive and frees all memory used by it

void Blah_Primitive_draw(Blah_Primitive *prim);
	// Draws a primitive using the current matrix

void Blah_Primitive_setMaterial(Blah_Primitive *prim, Blah_Material *material);
	// Assigns the specified material to the given primitive by assigning the material
	// pointer value of the primitive object.

void Blah_Primitive_mapTextureAuto(Blah_Primitive *prim, Blah_Texture *texture);
	// Maps a texture to a primtive using automatic texture coordinates

void Blah_Primitive_mapTexture(Blah_Primitive *prim, const Blah_Texture* texture, const Blah_Point* mapping[]);
	// Maps a texture to a primitive using the given null terminated array of
	// texture coordinates (mapping)

void Blah_Primitive_getNormal(Blah_Primitive *prim, Blah_Vector *normal);
	// Calculates normal for surface represented by primitive and stores
	// in Vector structure pointed to by normal

#ifdef __cplusplus
	}
#endif //__cplusplus

#endif
