/* blah_primitive.c */

#include <malloc.h>
#include <stdio.h>
#include <string.h>

#include "blah_primitive.h"
#include "blah_draw.h"

/* Function Declarations */

bool Blah_Primitive_init(Blah_Primitive *prim, blah_primitive_type newType, Blah_Vertex* vertexArray[], unsigned int vertexCount) {
	bool bSuccess = true;
	prim->type=newType;
	if (vertexArray != NULL) {
		Blah_Vertex **newSequence = (Blah_Vertex**)malloc(sizeof(Blah_Vertex*) * (vertexCount+1));
		if (newSequence != NULL) {
			memcpy(newSequence, vertexArray, sizeof(Blah_Vertex*) * (vertexCount));
			newSequence[vertexCount] = NULL;
			prim->sequence = newSequence;
		} else {
			bSuccess = false;
		}
	} else {
		prim->sequence = NULL;
	}

	prim->textureMap = NULL; //Default no texture mapping coordinates
	prim->material = NULL; //Default to no material, use default
	return bSuccess;
}

Blah_Primitive *Blah_Primitive_new(blah_primitive_type newType, Blah_Vertex* vertexArray[], unsigned int vertexCount) {
	//Creates a new primitive structure
	Blah_Primitive *newPrim = (Blah_Primitive*)malloc(sizeof(Blah_Primitive));
	if (newPrim != NULL) //Ensure memory allocation succeeded
		if (Blah_Primitive_init(newPrim, newType, vertexArray, vertexCount) != true) {
			//If init failed, then free memory and return NULL
			free(newPrim);
			newPrim = NULL;
		}

	return newPrim;
}

void Blah_Primitive_draw(Blah_Primitive *prim) { //Draws a primitive using the current matrix

	if (prim->sequence) {
		switch (prim->type) {
			case BLAH_PRIMITIVE_POLYGON :
				blah_draw_polygon(prim->sequence, prim->textureMap, prim->material);
				break;
			case BLAH_PRIMITIVE_LINE :
				blah_draw_lines(prim->sequence, prim->material);
				break;
			case BLAH_PRIMITIVE_LINE_STRIP :
				blah_draw_lineStrip(prim->sequence, prim->material);
				break;
			case BLAH_PRIMITIVE_TRIANGLE :
				blah_draw_triangle(prim->sequence, prim->textureMap, prim->material);
				break;
			case BLAH_PRIMITIVE_TRIANLGE_STRIP :
				blah_draw_triangleStrip(prim->sequence, prim->textureMap, prim->material);
				break;

			default:
				break;
		}
	}
}

void Blah_Primitive_destroy(Blah_Primitive *prim) { //Destroys a primitive and all vertices contained
	if (prim->sequence)
		free(prim->sequence);
	free(prim);
}

void Blah_Primitive_setMaterial(Blah_Primitive *prim, Blah_Material *material) {
	//Assigns the specified material to the given primitive by assigning the material
	//pointer value of the primitive object.
	prim->material = material;
}

void Blah_Primitive_mapTextureAuto(Blah_Primitive *prim, Blah_Texture *texture) {
	//Map given texture to specified primitive
	int vertexCount = 0, vertexIndex;
	Blah_Point topLeft = {0,1,0};Blah_Point topRight = {1,1,0};
	Blah_Point bottomLeft = {0,0,0}; Blah_Point bottomRight = {1,0,1};

	while(prim->sequence[vertexCount]) { vertexCount++; }

	const Blah_Point** texCoordIndices = (const Blah_Point**)malloc(sizeof(Blah_Point*)*(vertexCount + 1));

	for (vertexIndex=0;vertexIndex < vertexCount;vertexIndex++) {
		switch (vertexIndex & 3) {
			case 0 : texCoordIndices[vertexIndex]=&topLeft;break;
			case 1 : texCoordIndices[vertexIndex]=&topRight;break;
			case 2 : texCoordIndices[vertexIndex]=&bottomRight;break;
			case 3 : texCoordIndices[vertexIndex]=&bottomLeft;break;
		}
	}
	texCoordIndices[vertexCount]=NULL; //Add terminating pointer
	//Call normal mapping routine with temp array
	Blah_Primitive_mapTexture(prim, texture, texCoordIndices);
	free(texCoordIndices);
}

void Blah_Primitive_mapTexture(Blah_Primitive *prim, const Blah_Texture* texture, const Blah_Point* mapping[]) {
	//Maps a texture to a primitive using the given array of texture coordinates (mapping)
	//Creates a new primitive texture map structure and assigns it the the given primitive
	//Replaces current mapping if one already exists
	if (prim->textureMap) { Blah_Texture_Map_destroy(prim->textureMap); }
	prim->textureMap = Blah_Texture_Map_new(texture, mapping);
}

void Blah_Primitive_getNormal(Blah_Primitive *prim, Blah_Vector *normal) {
	//Calculates normal for surface represented by primitive and stores
	//in Vector structure pointed to by normal
	Blah_Vector vector1, vector2;

	Blah_Vertex_deltaVertex(prim->sequence[0], prim->sequence[1], &vector1);
	Blah_Vertex_deltaVertex(prim->sequence[1], prim->sequence[2], &vector2);
	blah_vector_crossProduct(&vector1, &vector2, normal);
}
