/* blah_vertex.h
	Defines a structure to represent a vertex of a primitive */

#ifndef _BLAH_VERTEX	

#define _BLAH_VERTEX

#include "blah_point.h"
#include "blah_vector.h"

/* Forward Declarations */

typedef struct Blah_Vertex { //A structure to represent a vertex of a primitive
	Blah_Point location;	//Location of vertex in 3D space
	Blah_Vector normal;		//normal vector for this point
} Blah_Vertex;

/* Function Prototypes */

#ifdef __cplusplus
	extern "C" {
#endif //__cplusplus

void Blah_Vertex_setLocation(Blah_Vertex *vertex, float x, float y, float z); 
	//sets coordinates of vertex position

void Blah_Vertex_setNormal(Blah_Vertex *vertex, float x, float y, float z); 
	//sets the normal vector for the given vertex

void Blah_Vertex_deltaVertex(Blah_Vertex *vertexA, Blah_Vertex *vertexB, struct Blah_Vector *vector);
	//Stores a delta vector from a to b in *vector

float Blah_Vertex_distanceVertex(Blah_Vertex *vertexA, Blah_Vertex *vertexB); 
	//True distance from a to b

void Blah_Vertex_translateByVector(Blah_Vertex *vertex, struct Blah_Vector *vector);
	//Translates position of vertex by given vector

void Blah_Vertex_translateByQuaternion(Blah_Vertex *vertex, Blah_Quaternion *quat);
	//translates the location of a vertex by a rotation about an arbitrary axis,
	//represented by a quaternion

void Blah_Vertex_multiplyMatrix(Blah_Vertex *vertex, struct Blah_Matrix *matrix);
	//Multiplies location coordinates of vertex by a given matrix

void Blah_Vertex_init(Blah_Vertex *vertex, float x, float y, float z);
	//Initialises vertex structure by setting location to given coordinates, and zeroing normal vector

Blah_Vertex *Blah_Vertex_new(float x, float y, float z);
	//Creates a new vertex structure and returns pointer

void Blah_Vertex_scale(Blah_Vertex *vertex, float scaleFactor);
	//Multiplies each coordinate of the position by scale_factor

void Blah_Vertex_rotateAxis(Blah_Vertex *vertex, Blah_Vector *axis, float angle);
	//Translates a vertex position by rotation of a given angle (in rads) around an 
	//abitrary axis as represented by given quaternion

#ifdef __cplusplus
	}
#endif //__cplusplus

#endif
