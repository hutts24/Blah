/* blah_point.c */

#include <malloc.h>
#include <math.h>
#include <string.h>

#include "blah_point.h"
#include "blah_vector.h"
#include "blah_matrix.h"

/* Function Declarations */

void Blah_Point_init(Blah_Point *point, float x, float y, float z) {
	Blah_Point_set(point, x, y, z);
	//point->normal.x = 1/x; point->normal.y = 1/y; point->normal.z = 1/z;
}

Blah_Point *Blah_Point_new(float x, float y, float z){ 
	//Creates a new point structure - very simple
	Blah_Point *newPoint = (Blah_Point*)malloc(sizeof(Blah_Point));
	if (newPoint != NULL) //Ensure memory allocation succeeded before initialising
		Blah_Point_init(newPoint, x, y ,z);
	
	return newPoint;
}

/* void Blah_Point_destroy(Blah_Point *point) {
	//Destroys a point structure
	free(point); //Simply free memory occupied by point structure
} */

void Blah_Point_set(Blah_Point *point, float x, float y, float z) { 
	//sets coordinates of point structure - also sad, but compiled into an inline function becomes quick!
	point->x=x;
	point->y=y;
	point->z=z;
}

void Blah_Point_deltaPoint(Blah_Point *pointA, Blah_Point *pointB, Blah_Vector *vector) {
	//Stores the delta vector from point_a to point_b
	vector->x = pointB->x - pointA->x;
	vector->y = pointB->y - pointA->y;
	vector->z = pointB->z - pointA->z;
}

float Blah_Point_distancePoint(Blah_Point *pointA, Blah_Point *pointB) { //Distance from a to b
	Blah_Vector tempVector;
	
	Blah_Point_deltaPoint(pointA, pointB, &tempVector);
	return sqrt(tempVector.x*tempVector.x + tempVector.y*tempVector.y + tempVector.z*tempVector.z);
}

void Blah_Point_translateByVector(Blah_Point *point, Blah_Vector *vector) {
	//Adds x,y,z values of vector to x,y,z values of point
	
	point->x += vector->x;
	point->y += vector->y;
	point->z += vector->z;
}

void Blah_Point_multiplyMatrix(Blah_Point *point, Blah_Matrix *matrix) { //Multiplies point coordinates by a given matrix
	int resultCell, matrixCell, count;
	float result[3]={0,0,0};
	
	for (resultCell=0;resultCell<3;resultCell++) {
		//result[result_cell] = 0;
		matrixCell=resultCell & 3;	//result_cell mod 4
		
		for (count=0;count<3;count++) {
			result[resultCell] += ((float*)point)[count] * ((float*)matrix)[matrixCell];
			matrixCell+=4;
		} //sum products of v * m
		result[resultCell] += ((float*)matrix)[matrixCell];
	}
	
	memcpy(point, result, sizeof(float)*3); //copy result data into vector
}

void Blah_Point_scale(Blah_Point *point, float scaleFactor) {
	//Multiplies each coordinate of the point by scale_factor
	point->x*=scaleFactor;point->y*=scaleFactor;point->z*=scaleFactor;
}

void Blah_Point_translateByQuaternion(Blah_Point *point, Blah_Quaternion *quat) {
	//translates a point by a rotation about an arbitrary axis, represented
	//by a quaternion
	Blah_Matrix tempMatrix; 
	
	Blah_Matrix_formatQuaternion(&tempMatrix, quat); //create translation matrix from quaternion
	Blah_Point_multiplyMatrix(point, &tempMatrix); //apply translation matrix to point 
}

void Blah_Point_rotateAxis(Blah_Point *point, Blah_Vector *axis, float angle) {
	//Translates a point position by rotation of a given angle (in rads) around an 
	//abitrary axis as represented by given quaternion
	Blah_Quaternion tempQuat;
	
	Blah_Quaternion_formatAxisAngle(&tempQuat, axis, angle); //create new quaternion representing axis and angle
	Blah_Point_translateByQuaternion(point, &tempQuat); //Apply new quaternion to point
}
