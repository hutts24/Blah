/* blah_draw_gl
	OpenGL specific routines for drawing video */

#ifndef _BLAH_DRAW_GL

#define _BLAH_DRAW_GL

#include "blah_point.h"
#include "blah_colour.h"
#include "blah_matrix.h"
#include "blah_list.h"
#include "blah_texture.h"
#include "blah_material.h"
#include "blah_video.h"

/* Function prototypes */

/* Lighting related functions */

#ifdef __cplusplus
	extern "C" {
#endif //__cplusplus

void blah_draw_gl_exit();
	//Exit the opengl drawing engine component.  Deallocates resources

void blah_draw_gl_image2d(Blah_Image *image, int screenX, int screenY);
	//Draw the given image in 2D mode at the position specified by
	//given physical screen coordinates.

void blah_draw_gl_init();
	//Initialise and configure OpenGL

void blah_draw_gl_line(Blah_Point *point1, Blah_Point *point2, Blah_Material *material);

void blah_draw_gl_lines(Blah_Vertex *points[], Blah_Material *material);
	//Draw multiple lines given array of points

void blah_draw_gl_lineStrip(Blah_Vertex *points[], Blah_Material *material);

void blah_draw_gl_multMatrix(Blah_Matrix *matrix);
 	//Multiply current matrix by supplied matrix

void blah_draw_gl_pixels2d(void *source, blah_pixel_format format, unsigned int width, unsigned int height, int screenX, int screenY);
	//Draws a rectangle of pixels to the video in 2d mode.  Pixels are copied from
	//memory pointed to by source, and drawn to video as a block of width * height
	//pixels, represented in given pixel format and colour depth.

void blah_draw_gl_point(float x, float y, float z, Blah_Material *material);
	//Draws a point with given colour at coordinates specified by x, y and z

void blah_draw_gl_points(Blah_Vertex *points[], Blah_Material *material);
	//Draws multiple points given null pointer terminated array

void blah_draw_gl_polygon(Blah_Vertex *vertices[], Blah_Texture_Map *textureMap, Blah_Material *material);
	//Draws a polygon with vertices specified by vertices NULL pointer terminated array

void blah_draw_gl_polygon2d(Blah_Vertex *vertices[], Blah_Texture_Map *textureMap, Blah_Material *material);
	//Draw a polygon in 2d mode with vertices specified by array of points.
	//Vertex coordinates are rendered relative to current drawport.

void blah_draw_gl_popMatrix();
	//Pop OpenGL matrix and restore previous state

void blah_draw_gl_printError();
	//Print information about current GL error to standard error out

void blah_draw_gl_pushMatrix();
	//Push OpenGL matrix and save video state

void blah_draw_gl_resetMatrix();
	//Set the current matrix to the identity matrix

void blah_draw_gl_setAmbientLight(float red, float green, float blue, float alpha);
	//Sets the properties of the ambient light used to render current drawing

void blah_draw_gl_setDrawport(unsigned int left, unsigned int bottom, unsigned int right, unsigned int top);
	//Updates the local GL specific 2D drawport matrix
	//2D drawport matrix is identity matrix with simple translation

bool blah_draw_gl_setLight(Blah_Point *location, Blah_Colour *diffuse, Blah_Colour *ambient, Blah_Vector *direction, float intensity, float spread);
	//Enables a light source at specified location in 3D space, with given qualities

void blah_draw_gl_setViewport(unsigned int left, unsigned int bottom, unsigned int right, unsigned int top);
	//Sets the viewport 2D drawing region coordinates to which all 2D drawing
	//operations are relative.  Coordinates specified are relative to the origin
	//0,0, bottom left of screen.

void blah_draw_gl_solidCone(float base, float height, int slices, int stacks, Blah_Material *material);

void blah_draw_gl_solidSphere(float radius, int slices, int stacks, Blah_Material *material);

// Draws a trianlge with points specified by points
void blah_draw_gl_triangle(Blah_Vertex *points[], Blah_Texture_Map *textureMap, Blah_Material *material);

void blah_draw_gl_quadrilateral(Blah_Vertex* points[], Blah_Texture_Map* textureMap, Blah_Material* material);

void blah_draw_gl_triangleStrip(Blah_Vertex *points[], Blah_Texture_Map *textureMap, Blah_Material *material);
	// Draws a trianlge strip with points specified by points

void blah_draw_gl_update2dProjection(const Blah_Video_Mode* mode);
	// Calculates and updates the internal 2d projection matrix using physical
	// dimensions of viewing area (video mode width/height)
	// This function is called when the video mode is set and should not be called
	// at any other time.

void blah_draw_gl_updatePerspective();
	// Sets up the viewing perspective by setting up view clipping planes and
	// applying a transform matrix to the modelview matrix to simulate a vantage point

void blah_draw_gl_wireCube(float sideLength, Blah_Material *material);


#ifdef __cplusplus
	}
#endif //__cplusplus

#endif
