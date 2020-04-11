/* blah_point.h */

#ifndef _BLAH_POINT

#define _BLAH_POINT

#include "blah_quaternion.h"

/* Forward Declarations */
struct Blah_Matrix;
struct Blah_Vector;
	

typedef struct Blah_Point { //A simple point in 3D space, represented by 3 coordinates
	float x,y,z;
} Blah_Point;


/* Function Prototypes */

#ifdef __cplusplus
	extern "C" {
#endif //__cplusplus

void Blah_Point_deltaPoint(Blah_Point *pointA, Blah_Point *pointB, struct Blah_Vector *vector);
	//Stores a delta vector from a to b in *vector

/* THIS FUNCTION ISN'T NEEDED
	void Blah_Point_destroy(Blah_Point *point);
	//Destroys a point structure */

float Blah_Point_distancePoint(Blah_Point *pointA, Blah_Point *pointB); 
	//True distance from a to b

void Blah_Point_init(Blah_Point *point, float x, float y, float z);
	//Initialise point structure to specified coordinates

void Blah_Point_multiplyMatrix(Blah_Point *point, struct Blah_Matrix *matrix);
	//Multiplies point coordinates by a given matrix

Blah_Point *Blah_Point_new(float x, float y, float z);
	//Creates a new point structure

void Blah_Point_rotateAxis(Blah_Point *point, Blah_Vector *axis, float angle);
	//Translates a point position by rotation of a given angle (in rads) around an 
	//abitrary axis as represented by given quaternion

void Blah_Point_scale(Blah_Point *point, float scaleFactor);
	//Multiplies each coordinate of the point by scale_factor

void Blah_Point_set(Blah_Point *point, float x, float y, float z); 
	//sets coordinates of point structure

void Blah_Point_translateByQuaternion(Blah_Point *point, Blah_Quaternion *quat);
	//translates a point by a rotation about an arbitrary axis, represented
	//by a quaternion

void Blah_Point_translateByVector(Blah_Point *point, struct Blah_Vector *vector);
	//Adds vector to point coordinates

#ifdef __cplusplus
	}
#endif //__cplusplus

#endif
