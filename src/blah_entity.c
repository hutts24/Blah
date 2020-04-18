/* blah_entity.c
	Defines functions which operate upon entities.  See blah_entity.h for reference.
*/

#include <stdlib.h>
#include <stdio.h>

#ifdef BLAH_USE_GLUT
#include <GL/glut.h>
#endif

#include <string.h>

#include "blah_entity.h"
#include "blah_macros.h"
#include "blah_matrix.h"
#include "blah_list.h"
#include "blah_draw.h"
#include "blah_util.h"

/* Static Globals - Private to entity.c */

// List of those entities which were dynamically allocated, defaults to empty
Blah_List blah_entity_list = {
    .name = "entities",
    .destroyElementFunction = (blah_list_element_dest_func*)Blah_Entity_destroy,
};

/* Static Prototypes */

static void Blah_Entity_animate(Blah_Entity *entity);
	//Alters entity's location and orientation

static void Blah_Entity_checkCollision(Blah_Entity *entity);
	//Checks if given entity is colliding against all other entities

static bool Blah_Entity_processEvent(Blah_Entity *entity, Blah_Entity_Event *event);
	//Deals with pending event


/* Main Entity Functions */

static void Blah_Entity_animate(Blah_Entity *entity)
{	//Alters entity's location and orientation
	//translate current position by velocity vector
	Blah_Point_translateByVector(&entity->location, &entity->velocity);

	//Calculate entity's orientation and update in private matrix
	Blah_Entity_rotateEuler(entity, entity->rotationAxisX, entity->rotationAxisY, entity->rotationAxisZ);
}

void blah_entity_destroyAll()
{  	//Cleanup routine to do garbage collection for dynamically allocated entities apon exit
	Blah_List_destroyElements(&blah_entity_list);
}

void blah_entity_main()
{	//Main routine to process all entities
	blah_entity_processAll();
}

void blah_entity_processAll()
{
	// fprintf(stderr, "blah_entity_process_all()\n");
	Blah_List_callFunction(&blah_entity_list, (blah_list_element_func*)Blah_Entity_process);
}

/* Entity Function Definitions */

Blah_Entity_Object *Blah_Entity_addObject(Blah_Entity *entity, Blah_Object *object)
{	// Adds an object to an entity's list of composing objects
	Blah_Entity_Object *newEntObj = Blah_Entity_Object_new("an object",object);
	newEntObj->entity = entity;
	Blah_List_appendElement(&entity->objects, newEntObj);
	return newEntObj;
}

static void Blah_Entity_checkCollision(Blah_Entity *entity)
{	// Checks if given entity is colliding against all other entities.
    // If a collision is detected with another entity, call the collision handling function
	Blah_List_Element* currentElement = blah_entity_list.first;

	while (currentElement) {
        Blah_Entity* currentEntity = (Blah_Entity*)currentElement->data;
        if (currentEntity != entity) { // Don't check collision with itself!
            blah_entity_collision_func* colFunc = currentEntity->collisionFunction;
            Blah_Point impact;
            if (colFunc != NULL && currentEntity->activeCollision && Blah_Entity_checkCollisionEntity(entity, currentEntity, &impact)) {
                colFunc(currentEntity, entity); // call collision handler for recipient object
            }
        }
		currentElement = currentElement->next;
	}

}

bool Blah_Entity_checkCollisionEntity(Blah_Entity *entity1, Blah_Entity *entity2, Blah_Point *impact)
{
    // Returns true if entity_1 is colliding with entity_2
    // TODO - return impact point
	(void)impact;
	Blah_List_Element *entity1Obj = entity1->objects.first;
	Blah_List_Element *entity2Obj = entity2->objects.first; // Get the first object in list of each entity
	bool collision = false; // assume no collision yet
	Blah_Point collisionPoint1, collisionPoint2;

	while (entity1Obj && !collision) {
		while (entity2Obj && !collision) {
			if (Blah_Entity_Object_checkCollision((Blah_Entity_Object*)entity1Obj->data,
				(Blah_Entity_Object*)entity2Obj->data, &collisionPoint1, &collisionPoint2)) {
				collision = true;
			} else {
				entity2Obj = entity2Obj->next;  // go through all ent2 objects
			}
		}
		entity2Obj = entity2->objects.first; // rewind to start of object list for ent2
		entity1Obj = entity1Obj->next;  // advance to next object for ent1
	}

	return collision;
}

// Standard destroy routine for entity
void Blah_Entity_destroy(Blah_Entity *entity)
{
	fprintf(stderr, "Blah_Entity_destroy %p\n", (void*)entity);

	// TODO - Only remove enity from the list if it was dynamically allocated
	Blah_List_removeElement(&blah_entity_list, entity);  // Remove from list of entities


	if (entity->destroyFunction) { // call custom destroy function if there is one defined
		entity->destroyFunction(entity);
	} else {
		fprintf(stderr, "removed entity from main list\n");
		Blah_Entity_disable(entity);
		free(entity);
		fprintf(stderr, "freed entity\n");
	}
}

void Blah_Entity_disable(Blah_Entity *entity)
{	//Disables entity.  Nullifies its existence.  Removes all objects and events associated with it.
	Blah_List_destroyElements(&entity->objects);  //Destroy all objects composing entity
	fprintf(stderr,"destroyed child objects\n");
	Blah_List_destroyElements(&entity->events);  //Destroy any events in the queue for the entity
	fprintf(stderr,"destroyed events\n");
	if (entity->entityData) {//if there is an allocated memory block for entity data
		fprintf(stderr,"free entity data\n");
		free(entity->entityData);  //free it
	}
}

float Blah_Entity_distanceEntity(Blah_Entity *entity1, Blah_Entity *entity2)
{	//Returns the distance from center of entity_1 to center of entity_2
	return Blah_Point_distancePoint(&entity1->location, &entity2->location);
}

void Blah_Entity_draw(Blah_Entity *entity)
{
	//fprintf(stderr,"Blah_Entity_draw");

	//fprintf(stderr,"Entity name is %s\n",entity->name);

	if (entity->drawFunction) {
		//fprintf(stderr,"custom draw func:%p\n",entity->draw_function);
		entity->drawFunction(entity); //, entity->drawFunctionData); //Call custom draw function if it exists for entity
	} else {
		//fprintf(stderr,"standard draw func");
		blah_draw_pushMatrix();
		//Blah_Matrix_set_translation(&entity->current_matrix,&entity->location);
		blah_draw_multMatrix(&entity->fakeMatrix);
		Blah_List_callFunction(&entity->objects, (blah_list_element_func*)Blah_Entity_Object_draw); //call Object_draw for all entity objects
		blah_draw_popMatrix();
	}

}

void *Blah_Entity_getData(Blah_Entity *entity)
{
	return entity->entityData;
}

void Blah_Entity_getLocation(Blah_Entity *entity,Blah_Point *p)
{
	//Gets entity's location in 3D space in 3 coordinates
	Blah_Point_set(p,entity->location.x,entity->location.y,entity->location.z);
}

int Blah_Entity_getType(Blah_Entity *entity)
{
	return entity->type;
}

void Blah_Entity_getVelocity(Blah_Entity *entity, Blah_Vector *v)
{
	//Gets entity's velocity in 3 vector float values
	Blah_Vector_set(v,entity->velocity.x,entity->velocity.y,entity->velocity.z);
}

// Initialises a new plain entity without objects, positioned at origin
void Blah_Entity_init(Blah_Entity *newEntity, char *name, int type, size_t dataSize)
{
	newEntity->entityData = (dataSize > 0) ? malloc(dataSize) : NULL; // Allocate entity data if required
	blah_util_strncpy(newEntity->name, name, blah_countof(newEntity->name)); //copy name
	newEntity->type = type;	//Set new entity type
	Blah_List_init(&newEntity->objects, "Objects");
	Blah_List_init(&newEntity->events,"Events");
	newEntity->objects.destroyElementFunction = (blah_list_element_dest_func*)Blah_Entity_Object_destroy;
	newEntity->activeCollision = false;

	Blah_Entity_setLocation(newEntity,0,0,0); //set location to origin
	Blah_Entity_setVelocity(newEntity,0,0,0); //going nowhere
	Blah_Vector_set(&newEntity->axisX, 1, 0, 0);
	Blah_Vector_set(&newEntity->axisY, 0, 1, 0);
	Blah_Vector_set(&newEntity->axisZ, 0, 0, 1);
	Blah_Matrix_setIdentity(&newEntity->fakeMatrix); //set matrix to identity matrix
	newEntity->drawFunction = NULL;
	newEntity->moveFunction = NULL;
	newEntity->collisionFunction = NULL;
	newEntity->destroyFunction = NULL;
	newEntity->rotationAxisX = newEntity->rotationAxisY = newEntity->rotationAxisZ = 0;
	Blah_Quaternion_setIdentity(&newEntity->orientation);
}

Blah_Entity *Blah_Entity_new(char *name, int type, size_t dataSize)
{	//constructs a new plain entity without objects, positioned at origin
	Blah_Entity *newEntity = (Blah_Entity*)malloc(sizeof(Blah_Entity));

	if (newEntity) {
		Blah_Entity_init(newEntity, name, type, dataSize);
		Blah_List_appendElement(&blah_entity_list, newEntity);  //Add to list of entities
	} else {
		fprintf(stderr,"Blah_Entity_new - Failed to allocate memory for entity\n");  //FIXME - add proper log debugging
	}
	return newEntity;
}

void Blah_Entity_process(Blah_Entity *entity)
{	//process entity, update position etc
	Blah_Entity_Event *temp_event;
	bool cont = true;


	if (entity->moveFunction != NULL) { entity->moveFunction(entity); } // If a movement control function is defined, call it
	Blah_Entity_animate(entity);	// Animate the entity
	if (entity->activeCollision) { Blah_Entity_checkCollision(entity); } //If entity is actively colliding, check collisions
    temp_event = (Blah_Entity_Event*)Blah_List_popElement(&entity->events);
	while (temp_event && cont) {//Take care of all pending events
		cont = Blah_Entity_processEvent(entity,temp_event); //process next event_data
		if (cont) { temp_event = Blah_List_popElement(&entity->events); } // If it is ok to keep processing events, get the next one
	}
}

void Blah_Entity_rotateEuler(Blah_Entity *entity, float x, float y, float z)
{
	Blah_Quaternion tempQuat;

	//Form a quaternion to represent all 3 euler rotations
	Blah_Quaternion_formatEuler(&tempQuat, x, y, z);
	//Multiply entity's current quaternion by new quaternion
	Blah_Quaternion_multiplyQuaternion(&entity->orientation, &tempQuat);
	//Recalculate orientation vectors in entity matrix
	Blah_Matrix_setRotationQuat(&entity->fakeMatrix, &entity->orientation);
}

void Blah_Entity_setActiveCollision(Blah_Entity *entity,bool flag)
{
	//Sets active collision flag
	entity->activeCollision = flag;
}

void Blah_Entity_setLocation(Blah_Entity *entity, float x, float y, float z)
{
	//Sets entity's location in 3D space given 3 coordinates
	Blah_Point_set(&entity->location, x, y, z);
}

void Blah_Entity_setRotationAxisX(Blah_Entity *entity, float x)
{
	entity->rotationAxisX = x;
}

void Blah_Entity_setRotationAxisY(Blah_Entity *entity, float y)
{
	entity->rotationAxisY = y;
}

void Blah_Entity_setType(Blah_Entity *entity, int type)
{
	entity->type = type;
}


void Blah_Entity_setCollisionFunction(Blah_Entity* entity, blah_entity_collision_func* function)
{
	entity->collisionFunction = function;
	//entity->collisionFunctionData = externData;
}

void Blah_Entity_setDestroyFunction(Blah_Entity* entity, blah_entity_destroy_func* function)
{
	entity->destroyFunction = function;
	//entity->destroyFunctionData = externData;
}

void Blah_Entity_setDrawFunction(Blah_Entity* entity, blah_entity_draw_func* function) //, void *externData)
{
	//fprintf(stderr,"entity set draw func %p, data:%p\n",function,extern_data);
	entity->drawFunction = function;
	//entity->drawFunctionData = externData;
}

void Blah_Entity_setMoveFunction(Blah_Entity* entity, blah_entity_move_func* function) //, void *externData)
{
	entity->moveFunction = function;
	//entity->moveFunctionData = externData;
}

void Blah_Entity_setVelocity(Blah_Entity *entity, float x, float y, float z)
{
	Blah_Vector_set(&entity->velocity, x, y, z);
}

/* Entity Event Functions */

void Blah_Entity_Event_destroy(Blah_Entity_Event *event)
{	//destroys an event structure
	//fprintf(stderr,"Blah_Entity_Event_destroy\n");
	if (event->destroyFunction) { // If custom destroy function pointer exists,
		event->destroyFunction(event); //, NULL); //Call it instead to destroy the event obj
	} else {
		if (event->eventData) { free(event->eventData); } // Free event data if allocated
		free(event);	//free event
	}
}

void *Blah_Entity_Event_getData(Blah_Entity_Event *event)
{
	return event->eventData;
}

void Blah_Entity_Event_init(Blah_Entity_Event* event, const char* name, const Blah_Entity* sender, blah_entity_event_func* function, size_t dataSize)
{
	event->eventData = (dataSize > 0) ? malloc(dataSize) : NULL;
	blah_util_strncpy(event->name, name, BLAH_ENTITY_EVENT_NAME_LENGTH);
	event->sender = sender;
	event->eventFunction = function;
	event->destroyFunction = NULL;
}

Blah_Entity_Event* Blah_Entity_Event_new(const char* name, const Blah_Entity* sender, blah_entity_event_func* function, size_t dataSize)
{
	Blah_Entity_Event *newEvent = malloc(sizeof(Blah_Entity_Event));
	if (newEvent) {	Blah_Entity_Event_init(newEvent, name, sender, function, dataSize); }
	return newEvent;
}

static bool Blah_Entity_processEvent(Blah_Entity *entity,  Blah_Entity_Event *event) {
	//Deals with pending event
	bool result = false;
	if (event->eventFunction) { result = event->eventFunction(entity, event); }
	Blah_Entity_Event_destroy(event);
	return result;
}

void Blah_Entity_sendEvent(Blah_Entity *recipient, Blah_Entity_Event *event) {
	//fprintf(stderr,"sending event to :%s\n",recipient->name);
	Blah_List_appendElement(&recipient->events, event); //Add the new event to the entity's list
}
