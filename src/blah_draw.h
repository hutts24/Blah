/* blah_draw.h
	The drawing state implements a concept of a current scene.  The current scene is rendered by the video processing
	in the main engine routine.

	Drawing related routines */

#ifndef _BLAH_DRAW

#define _BLAH_DRAW

#include "blah_scene.h"
#include "blah_matrix.h"
#include "blah_colour.h"
#include "blah_list.h"
#include "blah_texture.h"
#include "blah_material.h"
#include "blah_types.h"
#include "blah_region.h"

#define BLAH_DRAW_API_NAME_LENGTH 20
#define BLAH_DRAW_DRAWPORT_STACK_SIZE 100

/* Function Type Declarations */

typedef void blah_draw_api_init_func();
	//This type of function is called to initialise the drawing subsystem

typedef void blah_draw_api_main_func();
	//This type of function is called as the main routine to effect all
	//drawing operations

typedef void blah_draw_api_exit_func();
	//This type of function is called to exit and shutdown the drawing subsystem

/* Structure Declarations */
typedef struct Blah_Draw_API { //Defines functions to use with a specific drawing API e.g OpenGL, D3D
	char name[BLAH_DRAW_API_NAME_LENGTH+1]; //name of API
	blah_draw_api_init_func *initFunction;
	blah_draw_api_main_func *mainFunction;
	blah_draw_api_exit_func *exitFunction;
} Blah_Draw_API;

typedef struct Blah_Draw_Parameters {
	Blah_Vector viewNormal; //Direction of 'up vector' e.g. 0,1,0
	Blah_Point focalPoint; //Viewing reference point, center of front viewing plane
	Blah_Point viewpoint;	//Point from which view is perceived
	float fieldOfVisionX;  	//X axis angle of viewing area in radians
	float fieldOfVisionY;  	//Y axis angle of viewing area in radians
	float depthOfVision;		//depth of viewing area, distance from eye
} Blah_Draw_Parameters;

typedef struct Blah_Draw_Capabilities { //Represents drawing system/hardware capabilities.
	bool lighting; //This flag indicates whether the drawing system supports Lighting
} Blah_Draw_Capabilities;

/* Function Prototypes */

#ifdef __cplusplus
	extern "C" {
#endif //__cplusplus

void blah_draw_exit();
	//This function is called when the engine exits and deallocates resources used
	//by the drawing engine component.  Exits video mode etc

bool blah_draw_init();
	//Initialise drawing engine component.  Returns true on success.

void blah_draw_main();
	//Main drawing routine.  Sets perspective and draws enitites/objects

void blah_draw_multMatrix(Blah_Matrix *matrix);
	//Multiplies the current matrix by given matrix

void blah_draw_popMatrix();
	//Retrieve (pop) the most recently 'pushed' matrix off stack and make current

void blah_draw_pushMatrix();
	//Preserves the current drawing matrix by pushing it on to the matrix stack

void blah_draw_resetMatrix();
	//Set the current matrix to the identity matrix

void blah_draw_setCurrentScene(Blah_Scene *scene);
	//Sets the pointer to the current scene to be rendered

void blah_draw_updatePerspective();
	//Updates viewing perspective before rendering world.  Arranges view
	//according to the viewing parameters in the current drawing parameters


/* Lighting related functions */
void blah_draw_setAmbientLight(float red, float green, float blue, float alpha);
	//Sets the properties of the ambient light used to render current drawing

bool blah_draw_setLight(Blah_Point *location, Blah_Colour *diffuse, Blah_Colour *ambient,	Blah_Vector *direction, float intensity, float spread);
	//Enables a light source at specified location in 3D space, with given qualities

/* Drawport related functions */

void blah_draw_getDrawport(unsigned int *left, unsigned int *bottom, unsigned int *right, unsigned int *top);
	//Gets the active coordinates of the drawport and stores them
	//in the memory locations as indicated by supplied parameters

bool blah_draw_popDrawport();
	//Pushes current drawport data onto internal stack to preserve it

bool blah_draw_pushDrawport();
	//Pushes current drawport data onto internal stack to preserve it

void blah_draw_setDrawport(unsigned int left, unsigned int bottom, unsigned int right, unsigned int top);
	//Sets the drawport 2D physical screen coordinates to which all 2D
	//drawing operations are relative.  Default is 0,0, width, height.
	//Coordinates specified are relative to the origin 0,0, bottom left of screen.

/* Viewport related functions */

void blah_draw_getViewport(unsigned int *left, unsigned int *bottom, unsigned int *right, unsigned int *top);
	//Gets the active coordinates of the viewport and stores them
	//in the memory locations as indicated by supplied parameters

void blah_draw_setViewport(unsigned int left, unsigned int bottom, unsigned int right, unsigned int top);
	//Sets the viewport 2D physical screen coordinates to which all visuals
	//are mapped.  Default is 0,0, width, height.  Coordinates specified are
	//relative to the origin 0,0, bottom left of screen.

/* Viewing perspective related functions */

void blah_draw_setDepthOfVision(float depth);
	//Sets the depth of the viewable area as a given distance from the viewing point

void blah_draw_setFieldOfVision(float radsX, float radsY);
	//Sets the width and height of the field of vision specified by the given angles
	//in radians

void blah_draw_setFocalPoint(float x, float y, float z);
	//Sets the point of focus in world coordinates.  The focal point defines the
	//center of the viewing area.

void blah_draw_setViewpoint(float x, float y, float z);
	//Sets the viewing point (eye) from which the scene is perceived

void blah_draw_setViewNormal(float x, float y, float z);
	//Sets the direction of the viewing normal (up) vector.  Generally this will be
	//0,1,0 (straight along positive y axis)

/* Drawing functions */

void blah_draw_image(Blah_Image *image, int screenX, int screenY);
	//Draw the given image in 2D mode at the position specified by
	//given physical screen coordinates.

void blah_draw_line(Blah_Point *point1, Blah_Point *point2, Blah_Material *material);
	//Draws a line from point1 to point2 in given colour

void blah_draw_lines(Blah_Vertex *points[], Blah_Material *material);
	//Draw multiple lines using given series of vertex couples and material.

void blah_draw_lineStrip(Blah_Vertex *points[], Blah_Material *material);
	//Draws a sequence of lines connected to each other, using single point for next line to

void blah_draw_pixels2d(void *source, blah_pixel_format format, unsigned int width, unsigned int height, int screenX, int screenY);
	//Draws a rectangle of pixels to the video in 2d mode.  Pixels are copied from
	//memory pointed to by source, and drawn to video as a block of width * height
	//pixels, represented in given pixel format and colour depth.  Coordinates
	//specified are relative to current drawport region.

void blah_draw_point(float x, float y, float z, Blah_Material *material);
	//Draws a point with given colour at coordinates specified by x, y and z

void blah_draw_points(Blah_Vertex *points[], Blah_Material *material);
	//Draws multiple points given null pointer terminated array

void blah_draw_polygon(Blah_Vertex *vertices[], Blah_Texture_Map *textureMap, Blah_Material *material);
	//Draw a polygon with vertices specified by array of points

void blah_draw_polygon2d(Blah_Vertex *vertices[],  Blah_Texture_Map *textureMap, Blah_Material *material);
	//Draw a polygon in 2d mode with vertices specified by array of points.
	//Vertex coordinates are rendered relative to current drawport.

void blah_draw_solidSphere(float radius, int slices, int stacks, Blah_Material *material);
	//Draw a solid sphere in given colour with given characteristics

void blah_draw_solidCone(float base, float height, int slices, int stacks, Blah_Material *material);

// Draws a trianlge with vertices specified by points[]
void blah_draw_triangle(Blah_Vertex *points[], Blah_Texture_Map *textureMap, Blah_Material *material);

void blah_draw_quadrilateral(Blah_Vertex* points[], Blah_Texture_Map* textureMap, Blah_Material* material);

// Draws a trianlge strip with vertices specified by points[]
void blah_draw_triangleStrip(Blah_Vertex *points[], Blah_Texture_Map *textureMap, Blah_Material *material);

void blah_draw_wireCube(float side_length,  Blah_Material *material);
	//Draws a wire cube with sides of given length

#ifdef __cplusplus
	}
#endif //__cplusplus

#endif
