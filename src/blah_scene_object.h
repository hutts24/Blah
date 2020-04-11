/* blah_scene_object.h
	A scene object is a passive object, appropriate for scenery and
	simple features.  It does not receive events or actively collide etc.	*/

#ifndef _BLAH_SCENE_OBJECT	

#define _BLAH_SCENE_OBJECT

#include "blah_matrix.h"
#include "blah_object.h"
#include "blah_types.h"

/* Symbol Definitions */

#define BLAH_SCENE_OBJECT_NAME_LENGTH 50

/* Forward Declarations */
struct Blah_Scene_Object;

/* Function Type Definitions */

typedef void blah_scene_object_draw_func(struct Blah_Scene_Object *object);
	//This function type should draw the given object in the scene in relation
	//to the parent entity.

/* Data Structure definitions */

typedef struct Blah_Scene_Object { //represents an scene object in the world
	char name[BLAH_SCENE_OBJECT_NAME_LENGTH+1];
	Blah_Object *object;	//Pointer the object that constitutes this
	Blah_Point position; //object's position in 3d space relative to world coordinates
	Blah_Matrix objectMatrix; //object's relative matrix.  Don't mess with it directly.
	Blah_Vector axisX, axisY, axisZ; //object's own primary axes x,y, and z
	blah_scene_object_draw_func *drawFunction;
	blah_bool visible;		//Visibility flag; If TRUE, then object is drawn
} Blah_Scene_Object;

/* Structure Function prototypes */

#ifdef __cplusplus
	extern "C" {
#endif //__cplusplus

Blah_Scene_Object *Blah_Scene_Object_new(char *name, Blah_Object *object_ptr); 
	//Create a new scene object object and initialise, using supplied name and object pointer.
	//Alloc a new Structure and return pointer

void Blah_Scene_Object_setPosition(Blah_Scene_Object *sceneObject, float x, float y, float z); 
	//set object's position indicated by 3 coordinates

void Blah_Scene_Object_destroy(Blah_Scene_Object *sceneObject);
	//Destroys a scene object.  Frees memory occupied by scene object and
	//also destroys attached object
	
void Blah_Scene_Object_disable(Blah_Scene_Object *sceneObject);
	//Destroy attached object only	

void Blah_Scene_Object_draw(Blah_Scene_Object *sceneObject); 
	//Draw object in 3D space relative to parent entity

void Blah_Scene_Object_init(Blah_Scene_Object *sceneObejct, char *name, Blah_Object *object_ptr);
	//Initialise scene object with supplied name and object pointer.  Object will be made visible by default.

void Blah_Scene_Object_setDrawFunction(Blah_Scene_Object *sceneObject, blah_scene_object_draw_func *function); 
	//set pointer for draw function

void Blah_Scene_Object_setVisible(Blah_Scene_Object *sceneObject, blah_bool visFlag);
	//Sets the visibility flag of the scene object to the value given by vis_flag
	//True will make the scene object visible and drawn, FALSE will make it invisible
	
blah_bool Blah_Scene_Object_isVisible(Blah_Scene_Object *sceneObject);
	//Returns TRUE if the scene object is visible, else FALSE

#ifdef __cplusplus
	}
#endif //__cplusplus

#endif
