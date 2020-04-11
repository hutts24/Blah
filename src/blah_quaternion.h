/* blah_quaternion.h */

#ifndef _BLAH_QUATERNION

#define _BLAH_QUATERNION

#include "blah_vector.h"

/* Structure Definitions */

typedef struct Blah_Quaternion {
	float x,y,z,w;
} Blah_Quaternion;

/* Public Function Prototypes */

#ifdef __cplusplus
	extern "C" {
#endif //__cplusplus

Blah_Quaternion *Blah_Quaternion_new(Blah_Vector *axis, float angle);
	//Generate new quaternion based on given axis and rotation angle

void Blah_Quaternion_init(Blah_Quaternion *quat, Blah_Vector *axis, float angle);
	//Same as Blah_Quaternion_formatAxisAngle

void Blah_Quaternion_formatAxisAngle(Blah_Quaternion *quat, Blah_Vector *axis, float angle);
	//Format a quaternion given axis and rotation angle

void Blah_Quaternion_multiplyQuaternion(Blah_Quaternion *quat1, Blah_Quaternion *quat2);
	//Multiplies quat_1 by quat_2 and stores result in quat_1

void Blah_Quaternion_setIdentity(Blah_Quaternion *quat);
	//Set quaternion to identity multiplication matrix (no rotation)

void Blah_Quaternion_formatEuler(Blah_Quaternion * quat, float x, float y, float z);
	//Format a quaternion given 3 euler angles x,y and z

#ifdef __cplusplus
	}
#endif //__cplusplus		
		
#endif
