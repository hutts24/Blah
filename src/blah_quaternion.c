/* blah_quaternion.c 
	Functions for quaternions */

#include <malloc.h>
#include <math.h>

#include "blah_quaternion.h"


/* Function Declarations */

Blah_Quaternion *Blah_Quaternion_new(Blah_Vector *axis, float angle) {
	Blah_Quaternion *newQuat = malloc(sizeof(Blah_Quaternion));
	
	if (newQuat != NULL) // Check that memory allocation succeeded
		Blah_Quaternion_formatAxisAngle(newQuat, axis, angle);
		
	return newQuat;
}

void Blah_Quaternion_init(Blah_Quaternion *quat, Blah_Vector *axis, float angle) {
	Blah_Quaternion_formatAxisAngle(quat, axis, angle);
}

static void Blah_Quaternion_set(Blah_Quaternion *quat, float x, float y, float z, float w) {
	quat->x = x; quat->y = y; quat->z = z; quat->w = w;
}

void Blah_Quaternion_formatAxisAngle(Blah_Quaternion *quat, Blah_Vector *axis, float angle) {
	float sinA = sin( angle / 2 );
    float cosA = cos( angle / 2 );

    quat->x = axis -> x * sinA;
    quat->y = axis -> y * sinA;
    quat->z = axis -> z * sinA;
    quat->w = cosA;
}

void Blah_Quaternion_formatEuler(Blah_Quaternion * quat, float x, float y, float z) {
	Blah_Quaternion tempQuat;
	
	if (!x) //if x angle is 0, then no need to calculate quaternion for it
		Blah_Quaternion_setIdentity(quat);  //Result is being stored in *quat
	else 
		Blah_Quaternion_set(quat, sin(x/2),0,0, cos(x/2)); //store quaternion for x rotation
		
	if (y) {
		Blah_Quaternion_set(&tempQuat, 0, sin(y/2),0, cos(y/2)); //make quaternion for y rotation
		Blah_Quaternion_multiplyQuaternion(quat, &tempQuat); //multiply current quat by new y quat
	}
	
	if (z) {
		Blah_Quaternion_set(&tempQuat, 0, 0, sin(z/2), cos(z/2)); //make quaternion for z rotation
		Blah_Quaternion_multiplyQuaternion(quat, &tempQuat); //multiply current quat by new z quat
	}
}

void Blah_Quaternion_setIdentity(Blah_Quaternion *quat) {
	quat->w = 1; quat->x = quat->y = quat->z = 0;
}
	

void Blah_Quaternion_multiplyQuaternion(Blah_Quaternion *quat1, Blah_Quaternion *quat2) {
	float w = (quat1->w * quat2->w) - (quat1->x * quat2->x) - (quat1->y * quat2->y) - (quat1->z * quat2->z);
	float x = (quat1->w * quat2->x) + (quat1->x * quat2->w) + (quat1->y * quat2->z)	- (quat1->z * quat2->y);
	float y = (quat1->w * quat2->y) + (quat1->y * quat2->w) + (quat1->z * quat2->x) - (quat1->x * quat2->z);
	float z = (quat1->w * quat2->z) + (quat1->z * quat2->w) + (quat1->x * quat2->y) - (quat1->y * quat2->x);
	
	quat1->w = w; quat1->x = x; quat1->y = y; quat1->z = z;
}
