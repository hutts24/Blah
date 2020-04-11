#include <malloc.h>
#include <math.h>
#include <string.h>
#include <stdio.h>

#include "blah_vector.h"
#include "blah_matrix.h"

Blah_Vector *Blah_Vector_new(float x, float y, float z) { //Creates new vector struct and returns pointer
	Blah_Vector *newVector = (Blah_Vector*)malloc(sizeof(Blah_Vector));
	
	Blah_Vector_set(newVector, x, y, z);
	
	/* new_vector->x = x;
	new_vector->y = y;
	new_vector->z = z; */
	
	return newVector;
}

void Blah_Vector_set(Blah_Vector *vector, float x, float y, float z) { 
	//Sets the 3 coordinate values of a vector
	vector->x = x;
	vector->y = y;
	vector->z = z;
}

void Blah_Vector_normalise(Blah_Vector *vector) { //Normalises a vector so that its norm is 1
	Blah_Vector_setMagnitude(vector, 1);
}

void Blah_Vector_scale(Blah_Vector *vector, float scaleFactor) {
	//Scales the magnitude of the vector by scale_factor
	vector->x *= scaleFactor;
	vector->y *= scaleFactor;
	vector->z *= scaleFactor;
}	
	
void Blah_Vector_multiplyMatrix(Blah_Vector *vector, Blah_Matrix *matrix) { //Multiplys a vector by a given matrix
	int resultCell, matrixCell, count;
	float result[3]={0,0,0};
	
	for (resultCell=0;resultCell<3;resultCell++) {
		//result[result_cell] = 0;
		matrixCell=resultCell & 3;	//result_cell mod 4
		
		for (count=0;count<3;count++) {
			result[resultCell] += ((float*)vector)[count] * ((float*)matrix)[matrixCell];
			matrixCell+=4;
		} //sum products of v * m
		result[resultCell] += ((float*)matrix)[matrixCell];
	}
	
	memcpy(vector, result, sizeof(float)*3); //copy result data into vector
}

void Blah_Vector_sprintf(char *dest, Blah_Vector *vector) {
	sprintf(dest, "%f\n%f\n%f\n", ((float*)vector)[0],((float*)vector)[1],((float*)vector)[2]);
}

float Blah_Vector_getMagnitude(Blah_Vector *vector) {
	return sqrtf( (vector->x*vector->x) + (vector->y*vector->y) + (vector->z*vector->z));
}

void Blah_Vector_setMagnitude(Blah_Vector *vector, float mag) {
	float norm = Blah_Vector_getMagnitude(vector);  //get size of vector
	float scalar = mag / norm;  //find scaling factor
	
	vector->x *= scalar;
	vector->y *= scalar;
	vector->z *= scalar;		//apply scaling factor to all elements of vector
}

void Blah_Vector_addVector(Blah_Vector *vector1, Blah_Vector *vector2) {
	vector1->x += vector2->x;
	vector1->y += vector2->y;
	vector1->z += vector2->z;
}

void Blah_Vector_invert(Blah_Vector *vector) {
	//transforms a vector to its inverse (multiplies by -1)
	vector->x = -vector->x;
	vector->y = -vector->y;
	vector->z = -vector->z;
}

void blah_vector_crossProduct(Blah_Vector *vector1, Blah_Vector *vector2, Blah_Vector *result) {
	//Calculates the cross product of vector_1 and vector_2 and stores in
	//Vector structure pointed to by result
	result->x = (vector1->y * vector2->z) - (vector2->y * vector1->z);
	result->y = (vector2->x * vector1->z) - (vector1->x * vector2->z);
	result->z = (vector1->x * vector2->y) - (vector2->x * vector1->y);
}
