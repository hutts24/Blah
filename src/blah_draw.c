/* blah_draw.c
	Drawing related routines */

#include <stdio.h>

#include "blah_scene.h"
#include "blah_font.h"
#include "blah_draw.h"
#include "blah_point.h"
#include "blah_draw_gl.h"
#include "blah_texture.h"
#include "blah_material.h"
#include "blah_object.h"
#include "blah_entity.h"
#include "blah_region.h"
#include "blah_stack.h"
#include "blah_debug.h"

/* Global Variables */

Blah_Material blah_draw_defaultMaterial;
	//The default drawing material holds default material settings to be used for
	//drawing operations when material parameters are not specified.

Blah_Draw_API *blah_draw_currentAPI = NULL;
	//pointer to current drawing API

Blah_Draw_Parameters blah_draw_currentParameters; //holds current drawing state
	//This structure should have values set during initialisationg of the drawing component

Blah_Draw_Capabilities blah_draw_currentCapabilities; //stores current subsystem capabilities

Blah_Scene *blah_draw_currentScene = NULL;
	//Pointer to the current scene to be rendered.  Defaults NULL;

Blah_Region blah_draw_viewport;
	//Draw port settings.  All 2D drawing operations are relative to the 'drawport'
Blah_Region blah_draw_drawport;
	//rectangular region.  Drawing is clipped beyond this region.

Blah_Stack blah_draw_drawportStack;
	//An internal stack for push pops

static Blah_Debug_Log blah_draw_log = { .filePointer = NULL };

/* Function Declarations */

void blah_draw_exit()
{	// This function is called when the engine exits and deallocates resources used
	// by the drawing engine component
	Blah_Debug_Log_message(&blah_draw_log, "Called blah_draw_exit\n");
	Blah_Stack_destroyBuffer(&blah_draw_drawportStack);
	blah_draw_gl_exit();
	Blah_Debug_Log_disable(&blah_draw_log);
}

bool blah_draw_init()
{	// Initialise drawing engine component
	Blah_Debug_Log_init(&blah_draw_log, "blah_draw");

	// Determine drawing capabilities
	// FIXME blah_draw_getCapabilities(&blah_draw_current_capabilities);

	// set default viewing parameters
	Blah_Point_set(&blah_draw_currentParameters.focalPoint, 0,0,1);
	Blah_Point_set(&blah_draw_currentParameters.viewpoint, 0,0,0);
	Blah_Vector_set(&blah_draw_currentParameters.viewNormal, 0,1,0);
	blah_draw_currentParameters.fieldOfVisionX = 1.6; //~90 degrees in radians
	blah_draw_currentParameters.fieldOfVisionY = 1.6;	//~90 degrees in radians
	blah_draw_currentParameters.depthOfVision = 1000;

	//set default drawing region parameters
	Blah_Region_init(&blah_draw_viewport, 0,0,0,0);
	Blah_Material_setDefaults(&blah_draw_defaultMaterial);

	//Set default material properties
	//FIXME - Reinstate this call once the gl_init functionis changed.  blah_draw_gl_init(); //Use OpenGL API for now...and for quite some time
	Blah_Debug_Log_message(&blah_draw_log, "Called blah_draw_init\n");
	return true;
}

void blah_draw_main()
{	//Main drawing routine.  Sets perspective and draws enitites/objects
	blah_draw_pushMatrix(); //Save the current
	blah_draw_updatePerspective();
	if (blah_draw_currentScene != NULL) { Blah_Scene_draw(blah_draw_currentScene); } //If a current scene has been defined, draw it
	blah_draw_popMatrix();	//Revert to previous drawing matrix
}

void blah_draw_pushMatrix()
{	//Preserves the current drawing matrix by pushing it on to the matrix stack
	blah_draw_gl_pushMatrix();
}

void blah_draw_popMatrix()
{	//Retrieve (pop) the most recently 'pushed' matrix off stack and make current
	blah_draw_gl_popMatrix();
}

void blah_draw_resetMatrix()
{	//Set the current matrix to the identity matrix
	blah_draw_gl_resetMatrix();
}

void blah_draw_multMatrix(Blah_Matrix *matrix)
{	//Multiplies the current matrix by given matrix
	blah_draw_gl_multMatrix(matrix);
}

void blah_draw_setCurrentScene(Blah_Scene *scene)
{	//Sets the pointer to the current scene to be rendered
	blah_draw_currentScene = scene;
}

/* Lighting Related Functions */

void blah_draw_setAmbientLight(float red, float green, float blue, float alpha)
{	//FIXME - use drawing API
	//Sets the properties of the ambient light used to render current drawing
	blah_draw_gl_setAmbientLight(red, green, blue, alpha);
}

bool blah_draw_setLight(Blah_Point *location, Blah_Colour *diffuse, Blah_Colour *ambient, Blah_Vector *direction, float intensity, float spread)
{	//Enables a light source at specified location in 3D space, with given qualities
	return blah_draw_gl_setLight(location, diffuse, ambient, direction, intensity, spread);
}

/* Drawport related functions */

void blah_draw_getDrawport(unsigned int *left, unsigned int *bottom, unsigned int *right, unsigned int *top)
{	//Gets the active coordinates of the drawport and stores them
	//in the memory locations as indicated by supplied parameters
	*left = blah_draw_drawport.left;
	*bottom = blah_draw_drawport.bottom;
	*right = blah_draw_drawport.right;
	*top = blah_draw_drawport.top;
}

bool blah_draw_popDrawport()
{	//Pushes current drawport data onto internal stack to preserve it
	if (Blah_Stack_pop(&blah_draw_drawportStack, &blah_draw_drawport,sizeof(Blah_Region))) {
		blah_draw_gl_setDrawport(blah_draw_drawport.left, blah_draw_drawport.bottom,
			blah_draw_drawport.right, blah_draw_drawport.top);
		return true;
	} else {
		return false;
	}
}

bool blah_draw_pushDrawport()
{	//Pushes current drawport data onto internal stack to preserve it
	return Blah_Stack_push(&blah_draw_drawportStack, &blah_draw_drawport, sizeof(Blah_Region));
}

void blah_draw_setDrawport(unsigned int left, unsigned int bottom, unsigned int right, unsigned int top)
{	//Sets the drawport 2D physical screen coordinates to which all 2D
	//drawing operations are relative.  Default is 0,0, width, height.
	//Coordinates specified are relative to the origin 0,0, bottom left of screen.
	Blah_Region_init(&blah_draw_drawport, left, bottom, right, top);
	blah_draw_gl_setDrawport(left, bottom, right, top);
}


/* Viewport related functions */

void blah_draw_getViewport(unsigned int *left, unsigned int *bottom, unsigned int *right, unsigned int *top)
{	//Gets the active coordinates of the viewport and stores them
	//in the memory locations as indicated by supplied parameters
	*left = blah_draw_viewport.left;
	*bottom = blah_draw_viewport.bottom;
	*right = blah_draw_viewport.right;
	*top = blah_draw_viewport.top;
}

void blah_draw_setViewport(unsigned int left, unsigned int bottom, unsigned int right, unsigned int top)
{	//Sets the viewport 2D physical screen coordinates to which all visuals
	//are mapped.  Default is 0,0, width, height.  Coordinates specified are
	//relative to the origin 0,0, bottom left of screen.
	Blah_Region_init(&blah_draw_viewport, left, bottom, right, top);
	blah_draw_gl_setViewport(left, bottom, right, top);
}


/* Viewing perspective related functions */

void blah_draw_setDepthOfVision(float depth)
{	//Sets the depth of the viewable area as a given distance from the viewing point
	blah_draw_currentParameters.depthOfVision = depth;
}

void blah_draw_setFieldOfVision(float radsX, float radsY)
{	//Sets the width and height of the field of vision specified by the given angles
	//in radians
	blah_draw_currentParameters.fieldOfVisionX = radsX;
	blah_draw_currentParameters.fieldOfVisionY = radsY;
}

void blah_draw_setFocalPoint(float x, float y, float z)
{	//Sets the point of focus in world coordinates.  The focal point defines the
	//center of the viewing area.
	Blah_Point_set(&blah_draw_currentParameters.focalPoint, x,y,z);
}

void blah_draw_setViewpoint(float x, float y, float z)
{	//Sets the viewing point (eye) from which the scene is perceived
	Blah_Point_set(&blah_draw_currentParameters.viewpoint, x,y,z);
}

void blah_draw_setViewNormal(float x, float y, float z)
{	//Sets the direction of the viewing normal (up) vector.  Generally this will be
	//0,1,0 (straight along positive y axis)
	Blah_Vector_set(&blah_draw_currentParameters.viewNormal, x,y,z);
}

void blah_draw_updatePerspective()
{	//Updates viewing perspective before rendering world.  Arranges view
	//according to the viewing parameters in the current drawing parameters
	blah_draw_gl_updatePerspective();
}


/* Drawing functions */

void blah_draw_image(Blah_Image *image, int screenX, int screenY)
{	//Draw the given image in 2D mode at the position specified by
	//given physical screen coordinates.
	blah_draw_pixels2d(image->pixelData, image->pixelFormat,
		image->width, image->height, screenX, screenY);
}

void blah_draw_line(Blah_Point *point1, Blah_Point *point2, Blah_Material *material)
{	//Draw single line.
	blah_draw_gl_line(point1, point2, !material ? &blah_draw_defaultMaterial : material); //If material not specified, use default material
}

void blah_draw_lines(Blah_Vertex *points[], Blah_Material *material)
{	//Draw multiple lines using given null terminated series of vertex couples and material.
	blah_draw_gl_lines(points, !material ? &blah_draw_defaultMaterial : material); //If material not specified, use default material
}

void blah_draw_lineStrip(Blah_Vertex *points[], Blah_Material *material)
{
	blah_draw_gl_lineStrip(points, !material ? &blah_draw_defaultMaterial : material); //If material not specified, use default material
}

void blah_draw_pixels2d(void *source, blah_pixel_format format, unsigned int width,	unsigned int height, int screenX, int screenY)
{	//Draws a rectangle of pixels to the video in 2d mode.  Pixels are copied from
	//memory pointed to by source, and drawn to video as a block of width * height
	//pixels, represented in given pixel format and colour depth.  Coordinates
	//specified are relative to current active drawing region (drawport).
	blah_draw_gl_pixels2d(source, format, width, height, screenX, screenY);
}

void blah_draw_point(float x, float y, float z,	Blah_Material *material)
{	//Draws a point with given colour at coordinates specified by x, y and z
	blah_draw_gl_point(x,y,z, !material ? &blah_draw_defaultMaterial : material); //If material not specified, use default material
}

void blah_draw_points(Blah_Vertex *points[], Blah_Material *material)
{	//Draws multiple points given null pointer terminated array
	blah_draw_gl_points(points, !material ? &blah_draw_defaultMaterial : material); //If material not specified, use default material
}

void blah_draw_polygon(Blah_Vertex *vertices[], Blah_Texture_Map *textureMap, Blah_Material *material)
{
	blah_draw_gl_polygon(vertices, textureMap, !material ? &blah_draw_defaultMaterial : material); //If material not specified, use default material
}

void blah_draw_polygon2d(Blah_Vertex *vertices[], Blah_Texture_Map *textureMap, Blah_Material *material)
{	//Draw a polygon in 2d mode with vertices specified by array of points.
	//Vertex coordinates are rendered relative to current drawport.
	blah_draw_gl_polygon2d(vertices, textureMap, !material ? &blah_draw_defaultMaterial : material); //If material not specified, use default material
}

void blah_draw_solidCone(float base, float height, int slices, int stacks, Blah_Material *material)
{	//Draw solid cone.  Use default material if no pointer to a material is given
	blah_draw_gl_solidCone(base, height, slices, stacks, !material ? &blah_draw_defaultMaterial : material);
}

void blah_draw_solidSphere(float radius, int slices, int stacks, Blah_Material *material)
{	//Draw solid sphere.  Use default material if no pointer to a material is given.
	blah_draw_gl_solidSphere(radius, slices, stacks, !material ? &blah_draw_defaultMaterial : material);
}

// Draws a trianlge with points specified by points
void blah_draw_triangle(Blah_Vertex *points[], Blah_Texture_Map *textureMap, Blah_Material *material )
{
	blah_draw_gl_triangle(points, textureMap, !material ? &blah_draw_defaultMaterial : material); //If material not specified, use default material
}

void blah_draw_quadrilateral(Blah_Vertex* points[], Blah_Texture_Map* textureMap, Blah_Material* material)
{
    blah_draw_gl_quadrilateral(points, textureMap, !material ? &blah_draw_defaultMaterial : material);
}

void blah_draw_triangleStrip(Blah_Vertex *points[], Blah_Texture_Map *textureMap, Blah_Material *material )
{	//Draws a trianlge strip with points specified by points
	blah_draw_gl_triangleStrip(points, textureMap, !material ? &blah_draw_defaultMaterial : material); //If material not specified, use default material
}

void blah_draw_wireCube(float side_length, Blah_Material *material)
{	//Draws a wire cube with sides of given length
	//FIXME
	(void)side_length;
	if (!material) { material = &blah_draw_defaultMaterial; }//If material not specified, use default material

	//blah_draw_gl_wire_cube(side_length, material);
}

