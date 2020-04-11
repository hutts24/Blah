#include <math.h>
#include <malloc.h>
#include <string.h>
#include <stdio.h>

#include "blah_matrix.h"
#include "blah_quaternion.h"


/* Function Definitions */


void Blah_Matrix_formatEuler(Blah_Matrix *matrix, float xAngle, float yAngle, float zAngle) {
	//Formats the given matrix to be a 4x4 rotation matrix from euler angles x,y and z
	float cosX, sinX, cosY, sinY, cosZ, sinZ;
	
	cosX = cos(xAngle); sinX = sin(xAngle);
    cosY = cos(yAngle); sinY = sin(yAngle);
    cosZ = cos(zAngle); sinZ = sin(zAngle);
    Blah_Vector_set(&matrix->axisX,cosY * cosZ, -sinX * sinY * cosZ + cosX * sinZ, cosX * sinY * cosZ + sinX * sinZ);
	Blah_Vector_set(&matrix->axisY, -cosY * sinZ, sinX * sinY * sinZ + cosX * cosZ, -cosX * sinY * sinZ + sinX * cosZ);
	Blah_Vector_set(&matrix->axisZ, -sinY, -sinX * cosY, cosX * cosY);
	Blah_Point_set(&matrix->location, 0, 0, 0);
	matrix->scaleX = matrix->scaleY = matrix->scaleZ = 0;
	matrix->scale = 1;
}

void Blah_Matrix_formatQuaternion(Blah_Matrix *matrix, Blah_Quaternion *quat) {
	//Formats the given matrix to be a 4x4 rotation matrix from a quaternion
	Blah_Matrix_setRotationQuat(matrix, quat);
	
    Blah_Point_set(&matrix->location, 0, 0, 0);
	matrix->scaleX = matrix->scaleY = matrix->scaleZ = 0;
	matrix->scale = 1;
}

void Blah_Matrix_init(Blah_Matrix *matrix)
{	//Initialises a matrix structure (just sets to identity matrix)
	Blah_Matrix_setIdentity(matrix);
}

void Blah_Matrix_multiply(Blah_Matrix *matrix1, Blah_Matrix *matrix2) {  //multiplies matrix_1 by matrix_2
	int resultCell, matrix1Cell, matrix2Cell, count;
	float result[16]={0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0};
	
	for (resultCell=0;resultCell<16;resultCell++) {
		//result[result_cell] = 0;
		matrix1Cell=resultCell & 3;	//result_cell mod 4
		matrix2Cell=resultCell & 252; //result_cell integer div 4 * 4
		for (count=0;count<4;count++) {
			result[resultCell] += ((float*)matrix1)[matrix1Cell] * ((float*)matrix2)[matrix2Cell];
			matrix1Cell+=4;
			matrix2Cell++;
		} //sum products of m2 * m1
	}
	
	memcpy(matrix1, result, sizeof(float)*16); //copy result data into matrix_1
}

Blah_Matrix *Blah_Matrix_new()
{	//constructs a new identity matrix
	Blah_Matrix *newMatrix = (Blah_Matrix*)malloc(sizeof(Blah_Matrix));  //allocate memory
	if (newMatrix) //Ensure memory allocation succeeded
	{
		Blah_Matrix_init(newMatrix); //initialise to identity matrix
	}
	
	return newMatrix;
}

void Blah_Matrix_setAxisX(Blah_Matrix *matrix, float x, float y, float z) {
	//Sets the x axis of the matrix to supplied values
	Blah_Vector_set(&matrix->axisX, x,y,z);
}

void Blah_Matrix_setAxisY(Blah_Matrix *matrix, float x, float y, float z) {
	//Sets the y axis of the matrix to supplied values
	Blah_Vector_set(&matrix->axisY, x,y,z);
}

void Blah_Matrix_setAxisZ(Blah_Matrix *matrix, float x, float y, float z) {
	//Sets the z axis of the matrix to supplied values
	Blah_Vector_set(&matrix->axisZ, x,y,z);
}

void Blah_Matrix_setIdentity(Blah_Matrix *matrix) { //sets the given matrix to the identity values
	static Blah_Matrix identity = {{1,0,0},0, {0,1,0},0, {0,0,1},0, {0,0,0},1};
	memcpy(matrix, &identity, sizeof(Blah_Matrix));
}

void Blah_Matrix_setRotationQuat(Blah_Matrix *matrix, Blah_Quaternion *quat) {
	//Sets the rotation of the given matrix from a quaternion
	float x = quat->x; float y = quat->y; float z = quat->z; float w = quat->w;

	float xx = x * x; float xy = x * y; float xz = x * z; float xw = x * w;
	float yy = y * y; float yz = y * z; float yw = y * w;
	float zz = z * z; float zw = z * w;
	
	Blah_Vector_set(&matrix->axisX, 1 - 2 * (yy+zz), 2 * (xy+zw), 2 * (xz-yw));
	Blah_Vector_set(&matrix->axisY, 2 * (xy-zw), 1 - 2 * (xx+zz), 2 * (yz+xw));
	Blah_Vector_set(&matrix->axisZ, 2 * (xz+yw), 2 * (yz-xw), 1 - 2 * (xx+yy));
}

void Blah_Matrix_setTranslation(Blah_Matrix *matrix, float x, float y, float z) {  //Simply set the translation coordinates
	Blah_Point_set(&matrix->location, x,y,z);
}

void Blah_Matrix_sprintf(char *dest, Blah_Matrix *matrixSrc) {
	float *matrix = (float*)matrixSrc;
	
	sprintf(dest, "%f:%f:%f:%f\n%f:%f:%f:%f\n%f:%f:%f:%f\n%f:%f:%f:%f\n",
		matrix[0],matrix[4],matrix[8],matrix[12],
		matrix[1],matrix[5],matrix[9],matrix[13],
		matrix[2],matrix[6],matrix[10],matrix[14],
		matrix[3],matrix[7],matrix[11],matrix[15]);
}

