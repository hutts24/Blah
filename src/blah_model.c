/* blah_model.c
	Defines functions for creating and manipulating models
*/

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "blah_model.h"
#include "blah_model_lightwave.h"
#include "blah_point.h"
#include "blah_tree.h"
#include "blah_util.h"
#include "blah_file.h"

/* External Function Prototypes */

extern Blah_Model *Blah_Model_Lightwave_load(char *filename, FILE *fileStream); 
	//Creates a new Image structure.  Memory is allocated etc

/* Private internal globals */

Blah_Tree blah_model_tree = {"model tree", NULL, (blah_tree_element_dest_func*)Blah_Model_destroy, 0}; //For garbage collection purposes


/* Function Declarations */

void Blah_Model_addFace(Blah_Model *model, Blah_Model_Face *newFace) {
	Blah_List_appendElement(&model->faces, newFace);
}

void Blah_Model_addSurface(Blah_Model *model, Blah_Model_Surface *newSurface) {
	Blah_List_appendElement(&model->surfaces, newSurface);
}

void Blah_Model_addVertex(Blah_Model *model, Blah_Vertex *newVertex) {
	Blah_List_appendElement(&model->vertices, newVertex);
}

void Blah_Model_destroy(Blah_Model *model) {
	Blah_Tree_removeElement(&blah_model_tree, model->name); //remove from tree
	Blah_Model_disable(model);
	free(model);
}

blah_bool Blah_Model_init(Blah_Model *model, char *modelName) {
	blah_util_strncpy(model->name, modelName, BLAH_MODEL_NAME_LENGTH);
	Blah_List_init(&model->vertices,"model vertices list");
	Blah_List_init(&model->faces,"model faces list");
	Blah_List_setDestroyElementFunction(&model->faces, (blah_list_element_dest_func)Blah_Model_Face_destroy);
	Blah_List_init(&model->surfaces,"model surfaces list");
	return BLAH_TRUE;
}

void Blah_Model_disable(Blah_Model *model) {
	Blah_List_destroyElements(&model->vertices);
	Blah_List_destroyElements(&model->faces);
	Blah_List_destroyElements(&model->surfaces);
}


Blah_Model *Blah_Model_load(char *filename) {
	FILE *fileStream = blah_file_open(filename, "rb");
	Blah_Model *newModel;
	if (!fileStream) return NULL;
	newModel = Blah_Model_Lightwave_load(filename, fileStream);
	fclose(fileStream);
	return newModel;
}

Blah_Model *Blah_Model_new(char *modelName) {
	Blah_Model *newModel = malloc(sizeof(Blah_Model));
	if (newModel) { //Check if memory allocation succeeded and initialise new model
		if (!Blah_Model_init(newModel, modelName)) { //If for some reason initialisation failed, free allocated memory and return NULL pointer
			free(newModel);
			newModel = NULL;
		} else Blah_Tree_insertElement(&blah_model_tree, modelName, newModel);  //add new model to internal tree for garbage collection
	}

	return newModel;
}

void Blah_Model_removeVertex(Blah_Model *model, Blah_Vertex *vertex) {
	Blah_List_removeElement(&model->vertices, vertex);
}


void Blah_Model_removeFace(Blah_Model *model, Blah_Model_Face *face) {
	Blah_List_removeElement(&model->faces, face);
}

/* Model Face Functions */

void Blah_Model_Face_addIndex(Blah_Model_Face *face, int index) {
	//Use an integer as the data pointer for the list element.  This is a bit dodgy I guess.
	void *tempPointer = 0;
	tempPointer += index;
	Blah_List_appendElement(&face->indices, tempPointer);
}

void Blah_Model_Face_destroy(Blah_Model_Face *face) {
	Blah_Model_Face_disable(face);
	free(face);
}

void Blah_Model_Face_init(Blah_Model_Face *face) {
	Blah_List_init(&face->indices, "vertex indices");
	face->surface = 0;
}

void Blah_Model_Face_disable(Blah_Model_Face *face) {
	Blah_List_removeAll(&face->indices);
}

Blah_Model_Face *Blah_Model_Face_new() {
	Blah_Model_Face *newFace = malloc(sizeof(Blah_Model_Face));

	if (newFace) Blah_Model_Face_init(newFace); //Ensure memory allocated
	return newFace;
}

void Blah_Model_Face_removeIndex(Blah_Model_Face *face, int index) {
	//Use an integer as the data pointer for the list element.  This is a bit dodgy I guess.
	//FIXME - change method of storing index???
	void *tempPointer = 0;
	tempPointer += index;
	Blah_List_removeElement(&face->indices, tempPointer);
}

/* Model Surface Functions */

void Blah_Model_Surface_addFace(Blah_Model_Surface *surface, Blah_Model_Face *face) {
	//Adds new_face to the list of faces for given surface
	Blah_List_appendElement(&surface->faces, face);
}

void Blah_Model_Surface_destroy(Blah_Model_Surface *surface) {
	Blah_Model_Surface_disable(surface);
	free(surface);
}

void Blah_Model_Surface_disable(Blah_Model_Surface *surface) {
	Blah_List_destroyElements(&surface->textures);
	Blah_List_removeAll(&surface->faces);
}

Blah_Model_Surface *Blah_Model_Surface_new(char *name) {
	Blah_Model_Surface *newSurface = malloc(sizeof(Blah_Model_Surface));
	if (newSurface != NULL) Blah_Model_Surface_init(newSurface, name); //Ensure memory allocation succeeded before initialising	
	return newSurface;
}

void Blah_Model_Surface_init(Blah_Model_Surface *surface, char *name) {
	Blah_Colour_set(&surface->colour, 1, 1, 1, 1);
	blah_util_strncpy(surface->name, name, BLAH_MODEL_SURFACE_NAME_LENGTH);
	surface->luminosity = 0;
	surface->diffuse = 0;
	surface->specular = 0;
	surface->reflection = 0;
	surface->transparency = 0;
	surface->glossiness = 0;
	Blah_List_init(&surface->textures, "texture list");
	Blah_List_init(&surface->faces, "face list");
}

void Blah_Model_scaleVertex(Blah_Vertex *vertex, float *scaleFactor) {
	vertex->location.x*=*scaleFactor;vertex->location.y*=*scaleFactor;
	vertex->location.z*=*scaleFactor;
}

Blah_Model_Texture_Map *Blah_Model_Texture_Map_new(Blah_Texture *texture, char projectionAxis, blah_model_texture_projection proj) {
	Blah_Model_Texture_Map *map = malloc(sizeof(Blah_Model_Texture_Map));
	//Ensure memory allocation succeeded before intiialising
	if (map != NULL) Blah_Model_Texture_Map_init(map, texture, projectionAxis, proj);
	return map;
}

void Blah_Model_Texture_Map_init(Blah_Model_Texture_Map *map, Blah_Texture *texture, char projectionAxis, blah_model_texture_projection proj) {
	map->texture = texture;
	map->projectionAxis = projectionAxis;
	map->projectionMode = proj;
	Blah_Point_set(&map->textureCenter, 0,0,0);
	Blah_Vector_set(&map->textureSize, 0,0,0);
}

static void Blah_Model_Texture_Map_scale(Blah_Model_Texture_Map *map, float *scaleFactor) {
	map->textureCenter.x*=*scaleFactor;map->textureCenter.y*=*scaleFactor;
	map->textureCenter.z*=*scaleFactor;map->textureSize.x*=*scaleFactor;
	map->textureSize.y*=*scaleFactor;map->textureSize.z*=*scaleFactor;
}

void Blah_Model_Texture_Map_setSize(Blah_Model_Texture_Map *map, float x, float y, float z) {
	Blah_Vector_set(&map->textureSize, x,y,z);
}


void Blah_Model_Texture_Map_setCenter(Blah_Model_Texture_Map *map, float x, float y, float z) {
	Blah_Point_set(&map->textureCenter, x,y,z);
}

static void Blah_Model_Surface_scale(Blah_Model_Surface *surface, float *scaleFactor) {
	Blah_List_callWithArg(&surface->textures, (blah_list_element_func_1arg)Blah_Model_Texture_Map_scale, scaleFactor);
}

void Blah_Model_Surface_addTexture(Blah_Model_Surface *surface, Blah_Model_Texture_Map *map) {
	Blah_List_appendElement(&surface->textures, map);
}


void Blah_Model_scale(Blah_Model *model, float scaleFactor) {
	//Alters every vertex in the model by multiplying each coordinate by scale_factor
	Blah_List_callWithArg(&model->vertices, (blah_list_element_func_1arg)Blah_Model_scaleVertex, &scaleFactor);
	Blah_List_callWithArg(&model->surfaces, (blah_list_element_func_1arg)Blah_Model_Surface_scale, &scaleFactor);
}

void blah_model_destroyAll() {
	//Garbage cleanup function to destroy all models still in the tree in memory
	Blah_Tree_destroyElements(&blah_model_tree);
}
