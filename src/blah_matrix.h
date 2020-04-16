/* blah_matrix.h */

#ifndef _BLAH_MATRIX

#define _BLAH_MATRIX

#include "blah_vector.h"
#include "blah_point.h"
#include "blah_quaternion.h"

typedef struct Blah_Matrix { //Vector is a complex array of float values representing location and orientation in 3d space
	Blah_Vector axisX; float scaleX;
	Blah_Vector axisY; float scaleY;
	Blah_Vector axisZ; float scaleZ;
	Blah_Point location; float scale;	//array of 16 floating point values
} Blah_Matrix;

/* Matrix Function Prototypes */

#ifdef __cplusplus
	extern "C" {
#endif //__cplusplus

void Blah_Matrix_formatEuler(Blah_Matrix *matrix, float xAngle, float yAngle, float zAngle);
	//Formats the given matrix to be a 4x4 rotation matrix from euler angles x,y and z

void Blah_Matrix_formatQuaternion(Blah_Matrix *matrix, Blah_Quaternion *quat);
	//Formats the given matrix to be a 4x4 rotation matrix from an axis and angle

void Blah_Matrix_init(Blah_Matrix *matrix);
	//Initialises a matrix structure (just sets to identity matrix)

// void Blah_Matrix_multiply(Blah_Matrix *matrix1, Blah_Matrix *matrix2);
	//multiplies matrix_1 by matrix_2

Blah_Matrix *Blah_Matrix_new();
	//constructs a new identity matrix

void Blah_Matrix_setAxisX(Blah_Matrix *matrix, float x, float y, float z);
	//Sets the x axis of the matrix to supplied values

void Blah_Matrix_setAxisY(Blah_Matrix *matrix, float x, float y, float z);
	//Sets the y axis of the matrix to supplied values

void Blah_Matrix_setAxisZ(Blah_Matrix *matrix, float x, float y, float z);
	//Sets the z axis of the matrix to supplied values

void Blah_Matrix_setIdentity(Blah_Matrix *matrix);
	//sets the given matrix to the identity values

void Blah_Matrix_setRotationQuat(Blah_Matrix *matrix, Blah_Quaternion *quat);
	//Sets the rotation of given matrix according to given quaternion

void Blah_Matrix_setTranslation(Blah_Matrix *matrix, float x, float y, float z);
	//Sets the translation (point position) of the matrix

void Blah_Matrix_sprintf(char *dest, Blah_Matrix *matrixSrc);

#ifdef __cplusplus
	}
#endif //__cplusplus

#endif
