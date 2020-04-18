/* blah_object.c
	Defines functions that operate upon objects.
	Objects are represented as a collection of primitives. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "blah_object.h"
#include "blah_macros.h"
#include "blah_entity.h"
#include "blah_primitive.h"
#include "blah_draw.h"
#include "blah_model.h"

/* Function Declarations */

Blah_Object *Blah_Object_fromModel(Blah_Model *model) {
	//Primitives and vertices are duplicated from model to create new object
	Blah_Object *newObject;
	Blah_List_Element *tempIndexElement, *tempFaceElement, *tempSurfaceElement, *tempVertexElement;
	Blah_Vertex **tempVerticesPointerArray;
	Blah_Vertex *tempVerticesArray[300];
	Blah_Vertex *currentVertex;
	Blah_Material *tempMaterial;
	Blah_Point mappingArray[300]; //big dodgy temporary array to store mapping coords
	const Blah_Point* mappingIndices[300]; //big dodgy temporary array to hold mapping indices
	Blah_Model_Surface *currentSurface;
	Blah_Model_Texture_Map *texMap=NULL;

	long vertexCount;
	int vertexIndex;
	int tempIndex;
	float x=0,y=0,z=0;
	Blah_Point textureOrigin;
	Blah_Vector delta;
	Blah_Vector normal;

	newObject = Blah_Object_new();
	//Create a temporary array with pointers to all vertices for indexing purposes
	tempVerticesPointerArray = malloc(sizeof(Blah_Vertex*) * model->vertices.length);
	tempVertexElement = model->vertices.first;
	vertexCount = 0;
	while (tempVertexElement) {
		currentVertex = (Blah_Vertex*)tempVertexElement->data;
		tempVerticesPointerArray[vertexCount] = Blah_Object_addVertex(newObject,
			currentVertex->location.x, currentVertex->location.y, currentVertex->location.z);
		tempVertexElement = tempVertexElement->next;
		vertexCount++;
	}
	//loop through all surfaces and create primitives from the surfaces contained
	tempSurfaceElement = model->surfaces.first;
	while (tempSurfaceElement) {
		currentSurface = (Blah_Model_Surface*)tempSurfaceElement->data;
		//Create material from current surface and add it to the new model
		tempMaterial = Blah_Material_fromSurface(currentSurface);
		Blah_Object_addMaterial(newObject, tempMaterial);
		//Create object primitives from model faces
		tempFaceElement = currentSurface->faces.first;
		while (tempFaceElement) { //while not yet end of model faces
			tempIndexElement = ((Blah_Model_Face*)tempFaceElement->data)->indices.first;
			vertexCount = 0;

			// FIXME - Need to allow for multiple textures
			while (tempIndexElement) { //create temp_indices_array as an array of
				vertexIndex = tempIndexElement->data - NULL;
				currentVertex = tempVerticesPointerArray[vertexIndex];
				tempVerticesArray[vertexCount] = currentVertex;
				if (currentSurface->textures.first) { //do texture mapping_array
					texMap = (Blah_Model_Texture_Map*)currentSurface->textures.first->data;

					//calculate texture origin (0,0)
					textureOrigin.x = texMap->textureCenter.x - texMap->textureSize.x/2.0;
					textureOrigin.y = texMap->textureCenter.y - texMap->textureSize.y/2.0;
					textureOrigin.z = texMap->textureCenter.z - texMap->textureSize.z/2.0;

					Blah_Point_deltaPoint(&textureOrigin, &currentVertex->location, &delta);

					switch (texMap->projectionAxis) {
						case 'x' :
							x = fabs(delta.z / texMap->textureSize.z);
							y = fabs(delta.y / texMap->textureSize.y);
							break;
						case 'y' :
							x = fabs(delta.x / texMap->textureSize.x);
							y = fabs(delta.z / texMap->textureSize.z);
							break;
						case 'z' :
							x = fabs(delta.x / texMap->textureSize.x);
							y = fabs(delta.y / texMap->textureSize.y);
					}
					z=0;
					Blah_Point_set(&mappingArray[vertexCount], x, y, z);
					mappingIndices[vertexCount] = &mappingArray[vertexCount];
				} else {
					texMap = NULL;
				}

				tempIndexElement = tempIndexElement->next;
				vertexCount++;
			}

			tempVerticesArray[vertexCount] = NULL; //Add terminating NULL pointer
			mappingIndices[vertexCount] = NULL; //terminate array of mapping coordinates
			blah_primitive_type primType;
			switch (vertexCount) {
                case 3: primType = BLAH_PRIMITIVE_TRIANGLE; break;
                case 4: primType = BLAH_PRIMITIVE_QUADRILATERAL; break;
                default: primType = BLAH_PRIMITIVE_POLYGON; break;
            }
			Blah_Primitive* newPrim = Blah_Primitive_new(primType, tempVerticesArray, vertexCount);


			//Set material property for new Primitive
			Blah_Primitive_setMaterial(newPrim, tempMaterial);
			Blah_Object_addPrimitive(newObject, newPrim);

            // Map texture if appropriate
			if (texMap) { Blah_Primitive_mapTexture(newPrim, texMap->texture, mappingIndices); }

            // get next face to make into primitive
			/* FIXME - BIG UGLY MESS TO MAKE NORMALS FOR VERTICES */
			if (((Blah_Model_Face*)tempFaceElement->data)->indices.length > 2) { Blah_Primitive_getNormal(newPrim, &normal); }
			tempIndex = 0;
			currentVertex = tempVerticesArray[0];

			while (currentVertex) {
				Blah_Vector_addVector(&currentVertex->normal, &normal);
				Blah_Vector_normalise(&currentVertex->normal);
				tempIndex++;
				currentVertex = tempVerticesArray[tempIndex];
			}

			tempFaceElement = tempFaceElement->next;
		}
		tempSurfaceElement = tempSurfaceElement->next;
	}

	free(tempVerticesPointerArray);

	//Free all temp memory buffers
	Blah_Object_updateBounds(newObject);

	return newObject;
}

void Blah_Object_destroy(Blah_Object *object) {//standard destroy routine for object
	Blah_List_destroyElements(&object->primitives);
	Blah_List_destroyElements(&object->vertices);
	Blah_List_destroyElements(&object->materials);
	free(object);
}

void Blah_Object_draw(Blah_Object *object) {
	//Draw object in space using the current drawing matrix
	if (object->drawFunction != NULL) { // if draw function defined, use it
		object->drawFunction(object);
 	} else { // call primitive draw function to draw all primitives
		Blah_List_callFunction(&object->primitives,(blah_list_element_func*)Blah_Primitive_draw);
 	}
}

void Blah_Object_init(Blah_Object *object) {
	Blah_Point_set(&object->frameTopLeftFront, 0, 0, 0);
	Blah_Point_set(&object->frameBottomRightBack, 0, 0, 0);
	object->boundRadius = 0;
	Blah_Object_setDrawFunction(object,NULL);
	Blah_List_init(&object->primitives, "object primitives");
	Blah_List_init(&object->vertices, "resource vertices");
	Blah_List_init(&object->materials, "resource materials");
	object->primitives.destroyElementFunction = (blah_list_element_dest_func*)Blah_Primitive_destroy;
}

Blah_Object *Blah_Object_new() {
	Blah_Object *newObject = malloc(sizeof(Blah_Object));
	if (newObject != NULL) // Ensure memory allocation succeeded before initialising
		Blah_Object_init(newObject);

	return newObject;
}

void Blah_Object_setDrawFunction(Blah_Object* object, blah_object_draw_func* function) {
	object->drawFunction = function;
}


void Blah_Object_setColour(Blah_Object *object, float red, float green, float blue,	float alpha) {
	//Sets the colour of all an object's materials
	Blah_List_Element *materialElement = object->materials.first;

	while (materialElement) {
		Blah_Material_setColour((Blah_Material*)materialElement->data, red, green, blue, alpha);
		materialElement = materialElement->next;
	}
}

void Blah_Object_setMaterial(Blah_Object* object, Blah_Material* material) {
	//Set the material used by all primitives belonging to the object
	Blah_List_callWithArg(&object->primitives, (blah_list_element_func_1arg*)Blah_Primitive_setMaterial, material);
}

void Blah_Object_mapTextureAuto(Blah_Object *obj, Blah_Texture *texture) {
	//Map given texture to all primitives of given object
	Blah_List_callWithArg(&obj->primitives, (blah_list_element_func_1arg*)Blah_Primitive_mapTextureAuto, texture);

	/* prim->texture = texture;
	if (prim->texture_mapping) { //If there is a pre-existing mapping, need to destroy it
		free(prim->texture_mapping);
		prim->texture_mapping = NULL; //Assign null pointer for no mapping
	} */
}


Blah_Vertex *Blah_Object_addVertex(Blah_Object *object, float x, float y, float z) {
	//Convenience function to add a vertex to the list of vertices
	//Returns handle to new vertex
	Blah_Vertex *newVertex = Blah_Vertex_new(x,y,z);
	Blah_List_appendElement(&object->vertices,newVertex);
	return newVertex;
}

void Blah_Object_addVertices(Blah_Object *object, Blah_Point *vertices[]) {
	//Adds multiple vertices given a NULL pointer terminated array of *Blah_PointS */
	//Memory is not duplicated; Do not free the supplied vertex structures
	int vertexIndex = 0;

	while (vertices[vertexIndex]) {
		Blah_List_appendElement(&object->vertices,vertices[vertexIndex]);
		vertexIndex++;
	}
}

void Blah_Object_addPrimitive(Blah_Object *object, Blah_Primitive *primitive) {
	//Adds a 3d primitive to an object's list of primitives
	Blah_List_appendElement(&object->primitives, primitive);
	//Blah_Object_update_bounds(object);
}

void Blah_Object_addMaterial(Blah_Object *object, Blah_Material *material) {
	//Adds the given material to the list of the object's own materials
	//Note that all materials in the object list belong to the object and will be
	//destroyed with the object.
	Blah_List_appendElement(&object->materials, material);
}

void Blah_Object_updateBounds(Blah_Object* object) {
	//Calculates the collision boundaries of an object

	Blah_List_Element *primElement;
	Blah_Vertex **vertexList;
	Blah_Point origin = {0,0,0};
	int vertexIndex;
	float maxRadius = 0;
	float tempRadius;

	primElement = object->primitives.first;

	while (primElement) {
		vertexList = ((Blah_Primitive*)primElement->data)->sequence;
		if (vertexList) { //if there is a vertex list
			vertexIndex = 0;

			while (vertexList[vertexIndex]) {
				tempRadius = Blah_Point_distancePoint(&origin, &vertexList[vertexIndex]->location);
				if (tempRadius > maxRadius) { maxRadius = tempRadius; } // update max radius
				vertexIndex++;
			}
		}
		primElement = primElement->next;
	}
	object->boundRadius = maxRadius;
}

static void Blah_Object_scalePoint(Blah_Point *point, float *scaleFactor) {
	Blah_Point_scale(point, *scaleFactor);
}

void Blah_Object_scale(Blah_Object* object, float scaleFactor) {
	//Alters every vertex in the object by multiplying each coordinate by scale_factor
	Blah_List_callWithArg(&object->vertices, (blah_list_element_func_1arg*)Blah_Object_scalePoint, &scaleFactor);
	Blah_Object_updateBounds(object);
}
