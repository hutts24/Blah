/* blah_vector.h */

#ifndef _BLAH_VECTOR

#define _BLAH_VECTOR

/* Structure Definition */

/* A vector is a simple structure with x,y, and z values */
typedef struct Blah_Vector { 
	float x,y,z;
} Blah_Vector;

/* Forward Declarations */
struct Blah_Matrix;
	
/* Function Headers */

#ifdef __cplusplus
	extern "C" {
#endif //__cplusplus

Blah_Vector *Blah_Vector_new(float x, float y, float z); //Creates new vector struct and returns pointer

void Blah_Vector_set(Blah_Vector *vector, float x, float y, float z); //Sets the x,y, and z values of a vector

void Blah_Vector_normalise(Blah_Vector *vector);

void Blah_Vector_scale(Blah_Vector *vector, float scaleFactor);
	//Scales the magnitude of the vector by scale_factor

void Blah_Vector_multiplyMatrix(Blah_Vector *vector, struct Blah_Matrix *matrix);

void Blah_Vector_sprintf(char *dest, Blah_Vector *vector);

float Blah_Vector_getMagnitude(Blah_Vector *vector);

void Blah_Vector_setMagnitude(Blah_Vector *vector, float mag);

void Blah_Vector_addVector(Blah_Vector *vector1, Blah_Vector *vector2);
	//Adds vector_2 to vector_1

void Blah_Vector_invert(Blah_Vector *vector);
	//transforms a vector to its inverse (multiplies by -1)

void blah_vector_crossProduct(Blah_Vector *vector1, Blah_Vector *vector2, Blah_Vector *result);
	//Calculates the cross product of vector_1 and vector_2 and stores in
	//Vector structure pointed to by result

#ifdef __cplusplus
	}
#endif //__cplusplus

#endif
