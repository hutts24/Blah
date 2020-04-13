/* blah_entity_object.h
	An entity object is an object constituting all or part of an entity.
	Entity objects have a reference to an underlying geometric object	*/

#ifndef _BLAH_ENTITY_OBJECT

#define _BLAH_ENTITY_OBJECT

#include <malloc.h>

#include "blah_matrix.h"
#include "blah_object.h"
#include "blah_types.h"

/* Forward Structure Declarations */

struct Blah_Entity;
struct Blah_Entity_Object;

/* Function Type Definitions */

typedef void blah_entity_object_draw_func(struct Blah_Entity_Object* object); // This function type should draw the given object in the scene

/* Symbol Definitions */

#define BLAH_ENTITY_OBJECT_NAME_LENGTH 50

/* Data Structure definitions */

typedef struct Blah_Entity_Object { //represents an object constituting an entity part
	char name[BLAH_ENTITY_OBJECT_NAME_LENGTH+1];
	struct Blah_Entity *entity;	//Pointer to parent owner entity
	Blah_Object *object;	//Pointer to the object
	Blah_Point position; //objects's position in 3D, relative to entity center
	Blah_Matrix objectMatrix; //structure's relative matrix.  Don't mess with it directly.
	Blah_Vector axisX, axisY, axisZ; //structure's own primary axes x,y, and z
	blah_entity_object_draw_func* drawFunction;
	bool visible;		//Visibility flag; If TRUE, then structure is drawn
} Blah_Entity_Object;

/* Entity Object Function prototypes */

#ifdef __cplusplus
	extern "C" {
#endif //__cplusplus

bool Blah_Entity_Object_checkCollision(Blah_Entity_Object *entityObject1, Blah_Entity_Object *entityObject2, Blah_Point *impact1, Blah_Point *impact2);
	//Checks if obejct_1 and object_2 are colliding and stores point of
	//contact in 'impact'

void Blah_Entity_Object_deltaEntityObject(Blah_Entity_Object *entityObject1, Blah_Entity_Object *entityObject2, Blah_Vector *delta);
	//Returns a vector from object_1 to object_2

void Blah_Entity_Object_destroy(Blah_Entity_Object* entityObject);
	//Destroys an entity object structure.  Frees memory occupied by entity object
	//structure and also destroys the referenced base object.

void Blah_Entity_Object_disable(Blah_Entity_Object* entityObject);
	//This function deinitialises the given entity object, by removing any allocated resources
	//associated with it, apart from the memory structure containing the entity object itself.

float Blah_Entity_Object_distanceObject(Blah_Entity_Object* entityObject1, Blah_Entity_Object* entityObject2);
	//Returns true distance between two entity objects

void Blah_Entity_Object_draw(Blah_Entity_Object* entityObject);
	//Draw structure in scene

void Blah_Entity_Object_init(Blah_Entity_Object* entityObject, const char *name, Blah_Object *objectPtr);
	//Initialise an entity structure using supplied name and object pointer.

bool Blah_Entity_Object_isVisible(Blah_Entity_Object* entityObject);
	//Returns TRUE if the entity_object is visible, else FALSE

Blah_Entity_Object *Blah_Entity_Object_new(const char* name, Blah_Object* objectPtr);
	//Create a new entity object using supplied name and object pointer.
	//Alloc a new entity object data structure and return pointer.
	//Returns NULL on failure.  Defaults position to 0,0,0, visible True.

void Blah_Entity_Object_setDrawFunction(Blah_Entity_Object* entityObject, blah_entity_object_draw_func* function);
	//set pointer for draw function used by given entity object

void Blah_Entity_Object_setPosition(Blah_Entity_Object* entityObject, float x, float y, float z);
	//Alters entity object's position, relative to parent entity center.

void Blah_Entity_Object_setVisible(Blah_Entity_Object* entityObject, bool visFlag);
	//Sets the visibility flag of the entity object to the value given by vis_flag
	//Tue will make the object visible and drawn as part of the parent entity,
	//False will make it invisible

#ifdef __cplusplus
	}
#endif //__cplusplus

#endif
