/* blah_entity.h
 	The entity structure represents an entity in the world.  Entities can be added to scenes, so that they are
 	rendered to the screen when the scene is visible.
 	Regardless of whether they exist in a scene or not, all entities which are active will be processed for general behaviour
 	and collisions/interactions.
 */

#ifndef _BLAH_ENTITY

#define _BLAH_ENTITY

#include <malloc.h>

#include "blah_matrix.h"
#include "blah_vector.h"
#include "blah_point.h"
#include "blah_list.h"
#include "blah_quaternion.h"
#include "blah_entity_object.h"
#include "blah_types.h"


/* Definitions */
#define BLAH_ENTITY_NAME_LENGTH 20 //Does not include terminating NULL character
#define BLAH_ENTITY_EVENT_NAME_LENGTH 10 //Does not include terminating NULL character
#define BLAH_ENTITY_TYPE_LENGTH 10 //Does not include terminating NULL character

/* Forward Declarations */

struct Blah_Entity;
struct Blah_Entity_Event;

/* Function Type Declarations */

typedef bool blah_entity_event_func(struct Blah_Entity* entity, struct Blah_Entity_Event* event);
	// This function type is responsible for applying the affect of the given event
	// to the given entity.  Function should return false, if subsequent events
	// for the entity should not be processed (e.g. entity has been destroyed).
	// Else function should return true if all is ok.

typedef void blah_entity_event_destroy_func(struct Blah_Entity_Event* event);
	// This type of function takes care of custom destroy routine for an entity event

typedef void blah_entity_draw_func(struct Blah_Entity* entity);
	// This type of function renders the entity in the 3d scene

typedef void blah_entity_move_func(struct Blah_Entity* entity);
	// This type of function controls the movement of the entity at regular intervals

typedef void blah_entity_collision_func(struct Blah_Entity* thisEntity, struct Blah_Entity* otherEntity);
	// This function type should handle the event of the object colliding with another

typedef void blah_entity_destroy_func(struct Blah_Entity *entity);
	//This type of function takes care of custom destroy routine for an entity


/* Data structures */

typedef struct Blah_Entity {
	//Begin fake Matrix structure - orientation of entity in 3d space
	union {
		Blah_Matrix fakeMatrix;
		struct {
			Blah_Vector axisX; float scaleX;
			Blah_Vector axisY; float scaleY;
			Blah_Vector axisZ; float scaleZ;	//entity's own primary axes x,y, and z
			Blah_Point location; float scale;	//Location of entity, 3d space world coordinates
		};
	};
	//End fake Matrix structure
	Blah_List objects;				//List of objects representing entity
	Blah_Vector velocity;			//velocity of entity.  Direction and magnitude specified together
	float rotationAxisX;		//rate of turn around x axis
	float rotationAxisY;		//rate of turn around y axis
	float rotationAxisZ;		//rate of turn around z axis
	blah_entity_draw_func* drawFunction;	//pointer to function to draw entity
	//void *draw_function_data;
	blah_entity_move_func* moveFunction;	//pointer to function to control movement
	//void *move_function_data;
	blah_entity_collision_func* collisionFunction;	//pointer to function to call when colliding with other entity
	//void *collision_function_data;
	blah_entity_destroy_func* destroyFunction;
	//void *destroy_function_data;
	char name[BLAH_ENTITY_NAME_LENGTH + 1];
	int type;					//The entity type
	Blah_Quaternion orientation;		//Entity orientation (rotation only)
	void *entityData;			//Entity specific information
	Blah_List events;				//List of pending events waiting to be processed
	bool activeCollision;
} Blah_Entity;

typedef struct Blah_Entity_Event {
	char name[BLAH_ENTITY_EVENT_NAME_LENGTH+1];
	const Blah_Entity* sender;
	blah_entity_event_func* eventFunction;
	blah_entity_event_destroy_func* destroyFunction;
	void* eventData;
} Blah_Entity_Event;

/* Entity Function prototpyes */

#ifdef __cplusplus
	extern "C" {
#endif //__cplusplus

Blah_Entity_Object *Blah_Entity_addObject(Blah_Entity *entity, Blah_Object *object);
	//Adds the given object to the specified entity.  Implicitly creates an entity
	//object structure and adds to the entity's collection of objects, returning
	//a pointer to the newly created entity_object structure

bool Blah_Entity_checkCollisionEntity(Blah_Entity *entity1, Blah_Entity *entity2, Blah_Point *impact);
	//Returns true if entity_1 is colliding with entity_2

void Blah_Entity_destroy(Blah_Entity *entity);
	//destroys entity

void blah_entity_destroyAll();
	//Cleanup routine to do garbage collection for dynamically allocated entities apon exit

void Blah_Entity_disable(Blah_Entity *entity);
	//Disables entity.  Nullifies its existence.  Removes all objects and events associated with it.

float Blah_Entity_distanceEntity(Blah_Entity *entity1, Blah_Entity *entity2);
	//Returns the distance from center of entity_1 to center of entity_2

void Blah_Entity_draw(Blah_Entity *entity);
	//Draws entity in 3d space

void *Blah_Entity_getData(Blah_Entity *entity);

void Blah_Entity_getLocation(Blah_Entity *entity, Blah_Point *p);
	//Gets entity's location in 3D space in 3 coordinates

int Blah_Entity_getType(Blah_Entity *entity);

void Blah_Entity_getVelocity(Blah_Entity *entity,Blah_Vector *v);
	//Gets entity's velocity in 3 vector float values

void Blah_Entity_init(Blah_Entity *newEntity, char *name, int type, size_t dataSize);
	//initialises a new plain entity without objects, positioned at origin, but does not allocate memory

void blah_entity_main();	//Main processing routine for entities

Blah_Entity *Blah_Entity_new(char *name, int type, size_t dataSize);
	//constructs a newly allocated plain entity without objects, positioned at origin

void Blah_Entity_process(Blah_Entity *entity);
	//process entity, update position etc

void blah_entity_processAll();

void Blah_Entity_rotateEuler(Blah_Entity* entity, float x, float y, float z);

void Blah_Entity_setLocation(Blah_Entity* entity, float x, float y, float z);
	//Sets entity's location in 3D space given 3 coordinates

void Blah_Entity_setRotationAxisX(Blah_Entity* entity, float x);

void Blah_Entity_setRotationAxisY(Blah_Entity* entity, float y);

void Blah_Entity_setVelocity(Blah_Entity *entity, float x, float y, float z);

void Blah_Entity_setDrawFunction(Blah_Entity* entity, blah_entity_draw_func* function); //, void *externData);

void Blah_Entity_setMoveFunction(Blah_Entity* entity, blah_entity_move_func* function); //, void *externData);

void Blah_Entity_setDestroyFunction(Blah_Entity* entity, blah_entity_destroy_func* function); //, void *externData);

void Blah_Entity_setCollisionFunction(Blah_Entity* entity, blah_entity_collision_func* function); //, void *externData);

void Blah_Entity_setActiveCollision(Blah_Entity* entity, bool flag);

void Blah_Entity_setType(Blah_Entity *entity, int type);


/* Event Function Prototypes */

void Blah_Entity_Event_destroy(Blah_Entity_Event *event);
	//destroys an event structure

void *Blah_Entity_Event_getData(Blah_Entity_Event *event);

void Blah_Entity_EventInit(Blah_Entity_Event* event, const char* name, const Blah_Entity* sender, blah_entity_event_func* function, size_t dataSize);

Blah_Entity_Event* Blah_Entity_Event_new(const char *name, const Blah_Entity* sender, blah_entity_event_func* function, size_t dataSize);
	// Creates a new event structure

void Blah_Entity_sendEvent(Blah_Entity* recipient, Blah_Entity_Event* event);
	// Sends an event to an entity


#ifdef __cplusplus
	}
#endif //__cplusplus

#endif
