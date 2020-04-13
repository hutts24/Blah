/* blah_object.h
	Objects are represented as a collection of primitives. */

#ifndef _BLAH_OBJECT

#define _BLAH_OBJECT

#include "blah_colour.h"
#include "blah_types.h"
#include "blah_primitive.h"
#include "blah_list.h"
#include "blah_model.h"

/* Forward Declarations */

struct Blah_Object;

/* Function Type Definitions */

typedef void blah_object_draw_func(struct Blah_Object *object);
	//This function type should draw the given object in the scene

/* Structure definitions */

typedef struct Blah_Object { //represents a single object as a collection of primitives
	blah_object_draw_func* drawFunction;
	Blah_Point frameTopLeftFront, frameBottomRightBack;
	float boundRadius;
	Blah_List primitives;	//List of primitives that compose object
	Blah_List vertices;		//List of resource vertices for possible use to construct primitives
	Blah_List materials;	//List of materials used to draw object
} Blah_Object;

/* Object Function prototypes */

#ifdef __cplusplus
	extern "C" {
#endif //__cplusplus

void Blah_Object_draw(Blah_Object *object);
	//Draw object in space using the current drawing matrix

void Blah_Object_init(Blah_Object *object);
	//Initialise object structure with default values

Blah_Object *Blah_Object_new();
	//Alloc a new Object structure and return pointer

Blah_Object *Blah_Object_fromModel(Blah_Model *model);
	//Produces an object with all the details of the supplied model
	//The model is not altered from this process in any way

void Blah_Object_setDrawFunction(Blah_Object* object, blah_object_draw_func* function);
	//set pointer for draw function

void Blah_Object_destroy(Blah_Object *object);
	//Destroys an object and all primitives attached

void Blah_Object_setColour(Blah_Object *object, float red, float green, float blue, float alpha);
	//Sets the colour of all an object's materials, which are used by its primitives

void Blah_Object_setMaterial(Blah_Object *object, Blah_Material *material);
	//Set the material used by all primitives belonging to the object

Blah_Vertex *Blah_Object_addVertex(Blah_Object *object, float x, float y, float z);
	//Convenience function to add a vertex to the list of vertices
	//Returns handle to new vertex

void Blah_Object_addVertices(Blah_Object *object, Blah_Point *vertices[]);
	//Adds multiple vertices given a NULL pointer terminated array of *Blah_PointS */
	//Memory is not duplicated; Do not free the supplied vertex structures

void Blah_Object_addPrimitive(Blah_Object *object, Blah_Primitive *prim);
	//Adds a 3d primitive to an object's list of primitives

void Blah_Object_addMaterial(Blah_Object *object, Blah_Material *material);
	//Adds the given material to the list of the object's own materials
	//Note that all materials in the object list belong to the object and will be
	//destroyed with the object.

void Blah_Object_updateBounds(Blah_Object* object);
	//Calculates the collision boundaries of an object

void Blah_Object_scale(Blah_Object *object, float scaleFactor);
	//Alters every vertex in the object by multiplying each coordinate by scale_factor

#ifdef __cplusplus
	}
#endif //__cplusplus

#endif
