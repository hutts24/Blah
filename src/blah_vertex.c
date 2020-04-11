/* blah_vertex.c
	Defines functions which perform operations upon vertex structures */

#include "blah_vertex.h"
#include "malloc.h"

/* Forward Declarations */

struct Blah_Vector;

/* Function Declarations */

void Blah_Vertex_setLocation(Blah_Vertex *vertex, float x, float y, float z) {
	//sets coordinates of vertex position
	Blah_Point_set(&vertex->location, x,y,z);
}
	
void Blah_Vertex_setNormal(Blah_Vertex *vertex, float x, float y, float z) { 
	//sets the normal vector for the given vertex
	Blah_Vector_set(&vertex->normal, x,y,z);
}

void Blah_Vertex_deltaVertex(Blah_Vertex *vertexA, Blah_Vertex *vertexB, struct Blah_Vector *vector) {
	//Stores a delta vector from a to b in *vector
	Blah_Point_deltaPoint(&vertexA->location, &vertexB->location, vector);
}
		
		
float Blah_Vertex_distanceVertex(Blah_Vertex *vertexA, Blah_Vertex *vertexB) { 
	//True distance from a to b
	return Blah_Point_distancePoint(&vertexA->location, &vertexB->location);
}

void Blah_Vertex_translateByVector(Blah_Vertex *vertex, struct Blah_Vector *vector) {
	//Translates position of vertex by given vector
	Blah_Point_translateByVector(&vertex->location, vector);
}
	
void Blah_Vertex_translateByQuaternion(Blah_Vertex *vertex, Blah_Quaternion *quat) {
	//translates the location of a vertex by a rotation about an arbitrary axis,
	//represented by a quaternion
	Blah_Point_translateByQuaternion(&vertex->location, quat);
}

void Blah_Vertex_multiply_matrix(Blah_Vertex *vertex, struct Blah_Matrix *matrix) {
	//Multiplies location coordinates of vertex by a given matrix
	Blah_Point_multiplyMatrix(&vertex->location, matrix);
}

void Blah_Vertex_init(Blah_Vertex *vertex, float x, float y, float z) {
	//Initialises vertex structure by setting location to given coordinates, and zeroing normal vector
	Blah_Point_set(&vertex->location, x,y,z);
	vertex->normal.x = vertex->normal.y = vertex->normal.z = 0;
}

Blah_Vertex *Blah_Vertex_new(float x, float y, float z) {
	//Creates a new vertex structure and returns pointer
	Blah_Vertex *newVertex = (Blah_Vertex*)malloc(sizeof(Blah_Vertex));
	if (newVertex)
		Blah_Vertex_init(newVertex, x, y, z);  //initialise new vertext structure if memory allocation succeeded
	return newVertex;
}

void Blah_Vertex_scale(Blah_Vertex *vertex, float scaleFactor) {
	//Multiplies each coordinate of the position by scale_factor
	Blah_Point_scale(&vertex->location, scaleFactor);
}

void Blah_Vertex_rotateAxis(Blah_Vertex *vertex, Blah_Vector *axis, float angle) {
	//Translates a vertex position by rotation of a given angle (in rads) around an 
	//abitrary axis as represented by given quaternion
	Blah_Point_rotateAxis(&vertex->location, axis, angle);
}
