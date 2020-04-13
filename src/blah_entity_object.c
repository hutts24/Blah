/* blah_entity_object.c
	An entity object is an object constituting all or part of an entity. */

#include <string.h>

#include "blah_entity.h"
#include "blah_entity_object.h"
#include "blah_types.h"
#include "blah_draw.h"
#include "blah_macros.h"
#include "blah_util.h"

/* Entity Object Function Declarations */

void Blah_Entity_Object_deltaEntityObject(Blah_Entity_Object *entityObject1, Blah_Entity_Object *entityObject2, Blah_Vector *delta)
{	//Returns a vector from object_1 to object_2
	Blah_Point tempPoint1, tempPoint2;
	Blah_Entity *owner1 = entityObject1->entity;
	Blah_Entity *owner2 = entityObject2->entity;

	tempPoint1.x = owner1->location.x + entityObject1->position.x;
	tempPoint1.y = owner1->location.y + entityObject1->position.y;
	tempPoint1.z = owner1->location.z + entityObject1->position.z;

	tempPoint2.x = owner2->location.x + entityObject2->position.x;
	tempPoint2.y = owner2->location.y + entityObject2->position.y;
	tempPoint2.z = owner2->location.z + entityObject2->position.z;

	Blah_Point_deltaPoint(&tempPoint1, &tempPoint2, delta);
}

void Blah_Entity_Object_destroy(Blah_Entity_Object *entityObject)
{	//Destroys an entity object structure.  Frees memory occupied by entity object
	//structure and also destroys the referenced base object.
	Blah_Entity_Object_disable(entityObject); //Perform deinitialisation
	free(entityObject); //free the referring structure
}

void Blah_Entity_Object_disable(Blah_Entity_Object *entityObject)
{	//This function deinitialises the given entity object, by removing any allocated resources
	//associated with it, apart from the memory structure containing the entity object itself.
	Blah_Object_destroy(entityObject->object); //Destroy base object
}

float Blah_Entity_Object_distanceObject(Blah_Entity_Object *entityObject1, Blah_Entity_Object *entityObject2)
{	//Returns true distance between two objects
	Blah_Point tempPoint1, tempPoint2;
	Blah_Entity *owner1 = entityObject1->entity;
	Blah_Entity *owner2 = entityObject2->entity;

	tempPoint1.x = owner1->location.x + entityObject1->position.x;
	tempPoint1.y = owner1->location.y + entityObject1->position.y;
	tempPoint1.z = owner1->location.z + entityObject1->position.z;

	tempPoint2.x = owner2->location.x + entityObject2->position.x;
	tempPoint2.y = owner2->location.y + entityObject2->position.y;
	tempPoint2.z = owner2->location.z + entityObject2->position.z;

	return Blah_Point_distancePoint(&tempPoint1, &tempPoint2);
}

void Blah_Entity_Object_draw(Blah_Entity_Object *entityObject)
{	//Draw object in 3D space relative to parent entity
	if (entityObject->visible) {
		blah_draw_pushMatrix();
		blah_draw_multMatrix(&entityObject->objectMatrix);
		//If the entity object defines a special draw function, use it
		if (entityObject->drawFunction)
			entityObject->drawFunction(entityObject);
		else //Just use the standard object draw function
			Blah_Object_draw(entityObject->object);
		blah_draw_popMatrix();
	}
}

void Blah_Entity_Object_init(Blah_Entity_Object *entityObject, const char *name, Blah_Object *objectPtr)
{	//Initialise an entity structure using supplied name and object pointer.
	blah_util_strncpy(entityObject->name, name, sizeof(entityObject->name));
	entityObject->entity = NULL; //Set parent entity pointer to NULL
	entityObject->object = objectPtr;
	entityObject->drawFunction = NULL;
	entityObject->visible = true;
	Blah_Point_set(&entityObject->position, 0, 0, 0);
	Blah_Matrix_setIdentity(&entityObject->objectMatrix);
	Blah_Vector_set(&entityObject->axisX, 1, 0, 0);
	Blah_Vector_set(&entityObject->axisY, 0, 1, 0);
	Blah_Vector_set(&entityObject->axisZ, 0, 0, 1);
}

bool Blah_Entity_Object_isVisible(Blah_Entity_Object *entityObject)
{	//Returns TRUE if the entity_object is visible, else FALSE
	return entityObject->visible;
}

Blah_Entity_Object *Blah_Entity_Object_new(const char* name, Blah_Object* objectPtr)
{
    // Create a new entity object using supplied name and object pointer.
	// Alloc a new entity object data structure, initialise it and return pointer.
	// Returns NULL on failure.
	// TODO - Error handling!
	Blah_Entity_Object* newEntityObject = malloc(sizeof(Blah_Entity_Object));
	if (newEntityObject) { Blah_Entity_Object_init(newEntityObject, name, objectPtr); } //Initialse new memory structure
	return newEntityObject;
}

void Blah_Entity_Object_setDrawFunction(Blah_Entity_Object *entityObject, blah_entity_object_draw_func* function) {
	//set pointer for draw function used by given entity object
	entityObject->drawFunction = function;
}

void Blah_Entity_Object_setPosition(Blah_Entity_Object *entityObject, float x, float y, float z)
{	//Alters entity object's position, relative to parent entity center.
	Blah_Point_set(&entityObject->position, x,y,z);
}

void Blah_Entity_Object_setVisible(Blah_Entity_Object *entityObject, bool visFlag) {
	//Sets the visibility flag of the entity object to the value given by vis_flag
	//Tue will make the object visible and drawn as part of the parent entity,
	//False will make it invisible
	entityObject->visible = visFlag;
}

bool Blah_Entity_Object_checkCollision(Blah_Entity_Object *entityObject1,
	Blah_Entity_Object *entityObject2, Blah_Point *impact1, Blah_Point *impact2) {
	//Checks if object_1 and object_2 are colliding and stores point of contact in 'impact'
	float distance;
	bool colliding = false;
	Blah_Entity_Object dummyObject;
	Blah_Vector tempVector;
	Blah_Object *object1 = entityObject1->object;
	Blah_Object *object2 = entityObject2->object;

	//get vector from obj1 to obj2
	Blah_Entity_Object_deltaEntityObject(entityObject1, entityObject2, &tempVector);
	distance = Blah_Vector_getMagnitude(&tempVector);  //get distance(size) of vector

	if (distance < object1->boundRadius + object2->boundRadius) {
		//if both objects are close enough to be colliding
		memcpy(&dummyObject.position, &entityObject1->position, sizeof(Blah_Point));
		dummyObject.entity = entityObject1->entity;
		Blah_Point_translateByVector(&dummyObject.position, &entityObject1->entity->velocity);
		//Calculate the location of object_1 next frame
		if (Blah_Entity_Object_distanceObject(&dummyObject, entityObject2) < distance) {
			colliding = true;  //If distance next frame is less,
			Blah_Vector_setMagnitude(&tempVector, object1->boundRadius);
			impact1->x = tempVector.x; impact1->y = tempVector.y; impact1->z = tempVector.z;
			//get point of impact on object_1
			Blah_Vector_invert(&tempVector);
			Blah_Vector_setMagnitude(&tempVector, object2->boundRadius);
			impact2->x=tempVector.x; impact2->y=tempVector.y; impact2->z=tempVector.z;
			//get point of impact on object_2
		}

	}
	//return distance < object_1->bound_radius + object_2->bound_radius ? true : false;
	return colliding;
}
