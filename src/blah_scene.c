/* blah_scene.c
	Defines functions which operate upon scene structures.
	A scene contains all elements to be rendered.
	to screen as visible components of a final rendered picture.	*/

#include "blah_types.h"
#include "blah_primitive.h"
#include "blah_list.h"
#include "blah_scene.h"
#include "blah_scene_object.h"
#include "blah_object.h"
#include "blah_entity.h"
#include "blah_draw.h"

/* Global variables, private to blah_scene.c */

//Blah_List blah_scene_list = {"", NULL, NULL, (blah_list_element_dest_func)Blah_Scene_destroy};  //List of all scenes, defaults to empty


/* Forward declarations */
static void Blah_Scene_setupLight(Blah_Light *light);
	//Initialise default lighting parameters for scene - fixme big time


/* Scene Function Definitions */

void Blah_Scene_addEntity(Blah_Scene *scene, Blah_Entity *entity) {
	//Adds the given entity to the scene's internal collection of entities
	Blah_List_appendElement(&scene->entities, entity);
}

void Blah_Scene_addLight(Blah_Scene *scene, Blah_Light *light) {
	//Adds given light source to the scene's list of lights
	Blah_List_appendElement(&scene->lights, light);
}

Blah_Scene_Object *Blah_Scene_addObject(Blah_Scene *scene, Blah_Object *object) {
	//Adds the given base object to the scene and returns a pointer to the new
	//scene object representing it within the scene, which is implicitly created.
	Blah_Scene_Object *newSceneObj = Blah_Scene_Object_new("scene object", object);
	Blah_Scene_addSceneObject(scene, newSceneObj);
	return newSceneObj;
}

void Blah_Scene_addOverlay(Blah_Scene *scene, Blah_Overlay *overlay) {
	//Adds given overlay to the scene's list of overlays
	Blah_List_appendElement(&scene->overlays, overlay);
}

void Blah_Scene_addSceneObject(Blah_Scene *scene, Blah_Scene_Object *sceneObject) {
	//Adds the given scene object to the scene's internal collection
	//of scene objects.
	Blah_List_appendElement(&scene->objects, sceneObject);
}

void Blah_Scene_destroy(Blah_Scene *scene) {
	//Destroys the scene object, the lists it contains within, and all
	//entities and scene_objects belonging to the scene.  Everything goes.
	Blah_Scene_disable(scene);
	free(scene);
}

void Blah_Scene_disable(Blah_Scene *scene) {
	//Destroy all entities and scene_objects belonging to the scene.
	Blah_List_destroyElements(&scene->objects);
	fprintf(stderr,"Destroyed scene objects\n");
	Blah_List_destroyElements(&scene->entities);
	fprintf(stderr,"Destroyed scene entities\n");
	Blah_List_destroyElements(&scene->overlays);
	fprintf(stderr,"Destroyed scene overlays\n");
	Blah_List_destroyElements(&scene->lights);
	fprintf(stderr,"Destroyed scene lights\n");
}

void Blah_Scene_draw(Blah_Scene *scene) {
	//Setup lighting parameters
	blah_draw_setAmbientLight(scene->ambientLightRed, scene->ambientLightGreen,	scene->ambientLightBlue, scene->ambientLightAlpha);
	Blah_List_callFunction(&scene->lights, (blah_list_element_func*)Blah_Scene_setupLight);

	//Draw the scene with all scene objects, entities and overlays contained within
	Blah_List_callFunction(&scene->objects, (blah_list_element_func*)Blah_Scene_Object_draw);
	Blah_List_callFunction(&scene->entities, (blah_list_element_func*)Blah_Entity_draw);
	Blah_List_callFunction(&scene->overlays, (blah_list_element_func*)Blah_Overlay_draw);
}

/* void Blah_Scene_draw_all() {
	//Draws all active scenes
	Blah_List_call_function(&blah_scene_list, (blah_list_element_func)Blah_Scene_draw);
} */

/* void Blah_Scene_main() {
	//Processes active scenes and draws visible scenes to the rendering display
	Blah_Scene_process_all();
	Blah_Scene_draw_all(); //Draw all visible scenes
} */

void Blah_Scene_init(Blah_Scene* scene) {
	Blah_Point_set(&scene->origin, 0,0,0);
	Blah_Matrix_setIdentity(&scene->sceneMatrix);
	Blah_List_init(&scene->entities, "Scene Entities");
	Blah_List_setDestroyElementFunction(&scene->entities, (blah_list_element_dest_func*)Blah_Entity_destroy);
	Blah_List_init(&scene->objects, "Scene Objects");
	Blah_List_setDestroyElementFunction(&scene->objects, (blah_list_element_dest_func*)Blah_Scene_Object_destroy);
	Blah_List_init(&scene->overlays, "Scene Overlays");
	Blah_List_setDestroyElementFunction(&scene->overlays, (blah_list_element_dest_func*)Blah_Overlay_destroy);
	Blah_List_init(&scene->lights, "Scene Lights");

	scene->ambientLightRed = BLAH_SCENE_DEFAULT_AMBIENT_LIGHT_RED;
	scene->ambientLightBlue = BLAH_SCENE_DEFAULT_AMBIENT_LIGHT_BLUE;
	scene->ambientLightGreen = BLAH_SCENE_DEFAULT_AMBIENT_LIGHT_GREEN;
	scene->ambientLightAlpha = BLAH_SCENE_DEFAULT_AMBIENT_LIGHT_ALPHA;
}

Blah_Scene *Blah_Scene_new() {
	//Alloc a new Scene structure and return pointer. Returns NULL on error
	Blah_Scene *newScene = malloc(sizeof(Blah_Scene));
	if (newScene != NULL)
		Blah_Scene_init(newScene);

	return newScene;
}

/* void Blah_Scene_process(Blah_Scene *scene) {
	//Processes all entities an interactions within the given scene
	fprintf(stderr, "Blah_Scene_process()\n");
	Blah_List_call_function(&scene->entities, (blah_list_element_func)Blah_Entity_process);
} */

/* void Blah_Scene_process_all() {
	//Processes all active scenes
	Blah_List_call_function(&blah_scene_list, (blah_list_element_func)Blah_Scene_process);
} */

void Blah_Scene_removeLight(Blah_Scene *scene, Blah_Light *light) {
	//Removes specified light from the scene
	Blah_List_removeElement(&scene->overlays, light);
}

void Blah_Scene_removeOverlay(Blah_Scene *scene, Blah_Overlay *overlay) {
	//Removes specified overlay from the scene
	Blah_List_removeElement(&scene->overlays, overlay);
}

void Blah_Scene_removeEntity(Blah_Scene *scene, Blah_Entity *entity) {
	//Removes the given entity from the scene's internal collection of entities
	Blah_List_removeElement(&scene->entities, entity);
}

/* void Blah_Scene_removeObject(Blah_Scene *scene, Blah_Object *sceneObject);
	//Removes the given scene object from the scene's internal collection of scene
	//objects */

void Blah_Scene_removeSceneObject(Blah_Scene *scene, Blah_Scene_Object *sceneObject) {
	//Removes the given scene object from the scene's internal collection of scene
	//objects
	Blah_List_removeElement(&scene->objects, sceneObject);
}

void Blah_Scene_setAmbientLight(Blah_Scene *scene, float red, float green, float blue, float alpha) {
	//Sets the ambient light properties of the scene
	scene->ambientLightRed = red;
	scene->ambientLightGreen = green;
	scene->ambientLightBlue = blue;
	scene->ambientLightAlpha = alpha;
}

void Blah_Scene_setDrawFunction(Blah_Scene* scene, blah_scene_draw_func* function);
	//set pointer for draw function

static void Blah_Scene_setupLight(Blah_Light *light) {
	//Initialise default lighting parameters for scene - fixme big time
	blah_draw_setLight(&light->location, &light->diffuse, &light->ambient, &light->direction, light->intensity, light->spread);
}
