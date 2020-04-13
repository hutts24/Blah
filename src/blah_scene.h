/* blah_scene.h
	Defines the scene structure.  A scene contains all elements to be rendered
	to screen as visible components of a final rendered picture.	*/

#ifndef _BLAH_SCENE

#define _BLAH_SCENE

#include "blah_types.h"
#include "blah_primitive.h"
#include "blah_list.h"
#include "blah_scene_object.h"
#include "blah_object.h"
#include "blah_entity.h"
#include "blah_overlay.h"
#include "blah_light.h"

/* Symbol Definitions */

#define BLAH_SCENE_DEFAULT_AMBIENT_LIGHT_RED 0.2f
#define BLAH_SCENE_DEFAULT_AMBIENT_LIGHT_BLUE 0.2f
#define BLAH_SCENE_DEFAULT_AMBIENT_LIGHT_GREEN 0.2f
#define BLAH_SCENE_DEFAULT_AMBIENT_LIGHT_ALPHA 1.0f

/* Forward Declarations */

struct Blah_Scene;

/* Function Type Definitions */

typedef void blah_scene_draw_func(struct Blah_Scene* scene); //This function type should render the scene in the 3d world

/* Structure definitions */

typedef struct Blah_Scene { //represents an object in the world
	Blah_Point origin; //center point of scene
	Blah_Matrix sceneMatrix; //scene's matrix.  Don't mess with it directly.
	Blah_List objects;	//List of passive objects in the scene
	Blah_List entities;		//List of active entities in scene
	Blah_List overlays;		//List of 2D overlays, drawn infront of rendered 3D
	Blah_List lights;		//List of light sources used to render scene (Blah_Light)
	//Lighting info
	float ambientLightRed, ambientLightBlue, ambientLightGreen, ambientLightAlpha;

} Blah_Scene;

/* Scene Function prototypes */

#ifdef __cplusplus
	extern "C" {
#endif //__cplusplus

void Blah_Scene_addEntity(Blah_Scene *scene, Blah_Entity *entity);
	//Adds the given entity to the scene's internal collection of entities

void Blah_Scene_addLight(Blah_Scene *scene, Blah_Light *light);
	//Adds given light source to the scene's list of lights

Blah_Scene_Object *Blah_Scene_addObject(Blah_Scene *scene, Blah_Object *object);
	//Adds the given base object to the scene and returns a pointer to the new
	//scene object representing it within the scene, which is implicitly created.

void Blah_Scene_addOverlay(Blah_Scene *scene, Blah_Overlay *overlay);
	//Adds given overlay to the scene's list of overlays

void Blah_Scene_addSceneObject(Blah_Scene *scene, Blah_Scene_Object *sceneObject);
	//Adds the given scene object to the scene's internal collection
	//of scene objects.

void Blah_Scene_destroy(Blah_Scene *scene);
	//Destroys the scene object, the lists it contains within, and all
	//entities and scene_objects belonging to the scene.  Everything goes.

void Blah_Scene_disable(Blah_Scene *scene);
	//Destroy all entities and scene_objects belonging to the scene.

void Blah_Scene_draw(Blah_Scene *scene);
	//Draw the scene with all scene objects and entities contained within

void Blah_Scene_init(Blah_Scene* scene);
	//Initialise a scene to empty contents with default properties

Blah_Scene *Blah_Scene_new();
	//Alloc a new Scene structure and return pointer. Returns NULL on error

/* void Blah_Scene_process_all();
	//Processes all active scenes */

void Blah_Scene_removeEntity(Blah_Scene *scene, Blah_Entity *entity);
	//Removes the given entity from the scene's internal collection of entities

void Blah_Scene_removeLight(Blah_Scene *scene, Blah_Light *light);
	//Removes specified light from the scene

void Blah_Scene_removeObject(Blah_Scene *scene, Blah_Object *sceneobject);
	//Removes the given scene object from the scene's internal collection of scene objects

void Blah_Scene_removeOverlay(Blah_Scene *scene, Blah_Overlay *overlay);
    // Removes specified overlay from the scene

void Blah_Scene_removeSceneObject(Blah_Scene *scene, Blah_Scene_Object *sceneObject);
    // Removes the given scene object from the scene's internal collection of scene objects

void Blah_Scene_setAmbientLight(Blah_Scene *scene, float red, float green, float blue, float alpha);
    // Sets the ambient light properties of the scene

void Blah_Scene_setDrawFunction(Blah_Scene* scene, blah_scene_draw_func* function);
	// set pointer for draw function


#ifdef __cplusplus
	}
#endif //__cplusplus

#endif
