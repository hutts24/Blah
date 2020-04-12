/* blah_scene_object.c
	Defines functions which operate upon scene objects. A scene object is a passive
	object, appropriate for scenery and	simple features.
	It does not receive events or actively collide etc.	*/

#include <malloc.h>
#include <string.h>

#include "blah_draw.h"
#include "blah_scene_object.h"
#include "blah_matrix.h"
#include "blah_types.h"
#include "blah_macros.h"
#include "blah_util.h"

/* Structure Function declarations */

Blah_Scene_Object *Blah_Scene_Object_new(char *name, Blah_Object *objectPtr) {
	//Create a new scene object object using supplied object pointer.
	//Alloc a new Structure and return pointer
	Blah_Scene_Object *newSceneObject = malloc(sizeof(Blah_Scene_Object));
	if (newSceneObject != NULL) // Ensure memory allocation succeeded
		Blah_Scene_Object_init(newSceneObject, name, objectPtr);
	return newSceneObject;
}

void Blah_Scene_Object_setPosition(Blah_Scene_Object *sceneObject, float x, float y, float z) {
	//set scene object's position indicated by 3 coordinates
	Blah_Point_set(&sceneObject->position, x,y,z);
}

void Blah_Scene_Object_destroy(Blah_Scene_Object *sceneObject) {
	Blah_Scene_Object_disable(sceneObject);
	free(sceneObject);
}

void Blah_Scene_Object_disable(Blah_Scene_Object *sceneObject) {
	Blah_Object_destroy(sceneObject->object);
}

void Blah_Scene_Object_draw(Blah_Scene_Object *sceneObject) {
	//Draw scene_object in scene
	if (sceneObject->visible) {
		blah_draw_pushMatrix();
		blah_draw_multMatrix(&sceneObject->objectMatrix);
		//If the entity object defines a special draw function, use it
		if (sceneObject->drawFunction)
			sceneObject->drawFunction(sceneObject);
		else //Just use the standard object draw function
			Blah_Object_draw(sceneObject->object);
		blah_draw_popMatrix();
	}
}

void Blah_Scene_Object_init(Blah_Scene_Object *sceneObject, char *name, Blah_Object *objectPtr) {
	blah_util_strncpy(sceneObject->name, name, BLAH_SCENE_OBJECT_NAME_LENGTH);
	sceneObject->object = objectPtr;
	Blah_Point_set(&sceneObject->position, 0,0,0);
	Blah_Matrix_setIdentity(&sceneObject->objectMatrix);
	Blah_Vector_set(&sceneObject->axisX, 0,0,0);
	Blah_Vector_set(&sceneObject->axisY, 0,0,0);
	Blah_Vector_set(&sceneObject->axisZ, 0,0,0);
	sceneObject->drawFunction = NULL;
	sceneObject->visible = true; //visible by default
}

void Blah_Scene_Object_setDrawFunction(Blah_Scene_Object *sceneObject, blah_scene_object_draw_func *function) {
	//set pointer for draw function
	sceneObject->drawFunction = function;
}

void Blah_Scene_Object_setVisible(Blah_Scene_Object *sceneObject, bool visFlag) {
	//Sets the visibility flag of the scene_object to the value given by vis_flag
	//True will make the scene_object visible and drawn, FALSE will make it invisible
	sceneObject->visible = visFlag;
}

bool Blah_Scene_Object_isVisible(Blah_Scene_Object *sceneObject) {
	//Returns TRUE if the scene_object is visible, else FALSE
	return sceneObject->visible;
}
