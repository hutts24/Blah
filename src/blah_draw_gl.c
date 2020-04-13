/* blah_draw_gl.c
	OpenGL specific drawing routines */

#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glu.h>
#include <stdio.h>
#include <math.h>

#ifdef BLAH_USE_GLUT
#include <GL/glut.h>
#endif

#include "blah_draw.h"
#include "blah_draw_gl.h"
#include "blah_texture.h"
#include "blah_video.h"
#include "blah_debug.h"

/* Externally Referenced Variables */

extern Blah_Draw_Parameters blah_draw_currentParameters;
extern Blah_Scene *blah_draw_currentScene;
extern Blah_Video_Mode *blah_video_currentMode;

/* Static Private Globals */

//Since OpenGL is a state machine, we should avoid setting the same state repeatedly

const Blah_Texture *blah_draw_gl_currentTexture;
	//Holds the pointer to the previously used texture

Blah_Material *blah_draw_gl_currentMaterial;
	//Holds the pointer to the previously used material

Blah_Matrix blah_draw_gl_drawportMatrix;
	//Cached matrix for 2D drawing operations

Blah_Matrix blah_draw_gl_2dProjectionMatrix;

static Blah_Debug_Log blah_draw_gl_log = { .filePointer = NULL };

int blah_draw_gl_activeLights = 0;
GLenum blah_draw_gl_lightSymbols[8] = {GL_LIGHT0, GL_LIGHT1, GL_LIGHT2, GL_LIGHT3,	GL_LIGHT4, GL_LIGHT5, GL_LIGHT6, GL_LIGHT7};

/* Static Function Prototypes */

static void blah_draw_gl_primitive(Blah_Vertex *vertices[], GLenum mode, Blah_Texture_Map *textureMap, Blah_Material *material);

static void blah_draw_gl_primitive2d(Blah_Vertex *vertices[], GLenum mode, Blah_Texture_Map *textureMap, Blah_Material *material);

static void blah_draw_gl_resetLights();

/* Function Declarations */

void blah_draw_gl_exit()
{	//Exit the opengl drawing engine component.  Deallocates resources
	Blah_Debug_Log_disable(&blah_draw_gl_log);
}

void blah_draw_gl_image2d(Blah_Image *image, int screenX, int screenY)
{	//Draw the given image in 2D mode at the position specified by
	//given physical screen coordinates.
	glRasterPos2i((GLint)screenX, (GLint)screenY);
	//glRasterPos3i((GLint)screen_x, (GLint)screen_y, 500);

	GLenum sourceFormat;

	switch(image->pixelFormat) { //SPEEDME?
		case BLAH_PIXEL_FORMAT_RGBA :
			sourceFormat = GL_RGBA; break;
		case BLAH_PIXEL_FORMAT_BGRA :
			sourceFormat = GL_BGRA; break;
		case BLAH_PIXEL_FORMAT_RGB :
			sourceFormat = GL_RGB; break;
		case BLAH_PIXEL_FORMAT_BGR :
			sourceFormat = GL_BGR; break;
		default :
			sourceFormat = GL_RGBA; break;
	}

	glDrawPixels(image->width, image->height, sourceFormat, GL_UNSIGNED_BYTE, (GLvoid*)image->pixelData);
}

void blah_draw_gl_init()
{	//Initialise and configure OpenGL
	//FIXME - Need to research OpenGL initialisation a bit more.  This stuff seems to crash if called before context is created
	//FIXME - This stuff needs to be moved to an area where it is called after the opengl mode has been set
	Blah_Debug_Log_init(&blah_draw_gl_log, "blah_draw_gl");
	Blah_Debug_Log_message(&blah_draw_gl_log, "Entering blah_draw_gl_init\n");

	Blah_Debug_Log_message(&blah_draw_gl_log, "Enabling lighting\n");
	glEnable(GL_LIGHTING);

	//glEnable(GL_COLOR_MATERIAL);
	//glColorMaterial(GL_FRONT, GL_AMBIENT);
	Blah_Debug_Log_message(&blah_draw_gl_log, "Configuring Depth\n");
	Blah_Debug_Log_message(&blah_draw_gl_log, "Enabling Depth Test\n");
	glEnable(GL_DEPTH_TEST);

	glShadeModel(GL_SMOOTH);
	glClearDepth(1.0);				// Enables Clearing Of The Depth Buffer
  	glDepthFunc(GL_LESS);				// The Type Of Depth Test To Do

	/* Texturing */
	Blah_Debug_Log_message(&blah_draw_gl_log, "Enabling texturing 2D\n");
	glEnable(GL_TEXTURE_2D);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	/* Blending */
	Blah_Debug_Log_message(&blah_draw_gl_log, "Enabling blending\n");
	//glEnable(GL_ALPHA_TEST);
	//glAlphaFunc(GL_GREATER,0);
	glEnable(GL_BLEND);

	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFunc(GL_ONE, GL_ONE);

	/* Line antialiasing */
	Blah_Debug_Log_message(&blah_draw_gl_log, "Enabling antialiasing\n");
	glEnable(GL_LINE_SMOOTH);
	//glEnable(GL_POLYGON_SMOOTH);
	glLineWidth(2);

	//Set initial states
	Blah_Debug_Log_message(&blah_draw_gl_log, "Setting initial states\n");
	blah_draw_gl_currentMaterial = NULL; blah_draw_gl_currentTexture = NULL;
	Blah_Matrix_setIdentity(&blah_draw_gl_drawportMatrix);

	Blah_Debug_Log_message(&blah_draw_gl_log, "Exiting blah_draw_gl_init\n");
}

void blah_draw_gl_line(Blah_Point *point1, Blah_Point *point2, Blah_Material *material) {
	//FIXME - This is just asking to break, when Point structure is changed.  SHould not be direct reference
	Blah_Vertex vertex1 = { {point1->x, point1->y, point1->z}, {0,0,1}};
	Blah_Vertex vertex2 = { {point2->x, point2->y, point2->z}, {0,0,1}};
	Blah_Vertex *vertices[3] = {&vertex1, &vertex2, NULL};
	blah_draw_gl_primitive(vertices, GL_LINES, NULL, material);
}

void blah_draw_gl_lines(Blah_Vertex *points[], Blah_Material *material)
{	//draws lines with every couple of points.  points is a null pointer terminated array
	blah_draw_gl_primitive(points, GL_LINES, NULL, material);
}

void blah_draw_gl_lineStrip(Blah_Vertex *points[], Blah_Material *material)
{	//draws a line strip joing each new line with previous. points is a null pointer terminated array
	blah_draw_gl_primitive(points, GL_LINE_STRIP, NULL, material);
}

void blah_draw_gl_multMatrix(Blah_Matrix *matrix)
{	//Multiply current matrix by supplied matrix
	glMultMatrixf((GLfloat*)matrix);
}

void blah_draw_gl_pixels2d(void *source, blah_pixel_format format, unsigned int width, unsigned int height, int screenX, int screenY)
{	//Draws a rectangle of pixels to the video in 2d mode.  Pixels are copied from
	//memory pointed to by source, and drawn to video as a block of width * height
	//pixels, represented in given pixel format and colour depth.
	GLenum sourceFormat;

	glPushMatrix(); //save model view
	//Set up 2D drawport with clipping planes
	glLoadMatrixf((GLfloat*)&blah_draw_gl_drawportMatrix);
	//Set up proper pseudo 2D viewing parameters
	glMatrixMode(GL_PROJECTION);
	glPushMatrix(); //save projection matrix on stack
	glLoadMatrixf((GLfloat*)&blah_draw_gl_2dProjectionMatrix);
	//Do the actual drawing
	glRasterPos2i((GLint)screenX, (GLint)screenY);

	switch(format) { //SPEEDME? - should this be a function or an array index?
		case BLAH_PIXEL_FORMAT_RGBA :
			sourceFormat = GL_RGBA; break;
		case BLAH_PIXEL_FORMAT_BGRA :
			sourceFormat = GL_BGRA; break;
		case BLAH_PIXEL_FORMAT_RGB :
			sourceFormat = GL_RGB; break;
		case BLAH_PIXEL_FORMAT_BGR :
			sourceFormat = GL_BGR; break;
		default :
			sourceFormat = GL_RGBA; break;
	}

	glDrawPixels(width, height, sourceFormat, GL_UNSIGNED_BYTE, (GLvoid*)source);

	glPopMatrix(); //restore projection matrix
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix(); //restore model view matrix
}

void blah_draw_gl_point(float x, float y, float z, Blah_Material *material)
{	//Draws a point with given colour at coordinates specified by x, y and z
	//Blah_Point *points[2];
	//Blah_Point the_point;
	Blah_Vertex vertex = { {x,y,z}, {0,0,1}};
	Blah_Vertex *vertices[2] = {&vertex, NULL};
	//Blah_Vertex_set_location(&the_vertex, x, y, z);  //SPEEDME?
	//points[0] = &the_point; points[1] = NULL;
	blah_draw_gl_primitive(vertices, GL_POINTS, NULL, material);
}

void blah_draw_gl_points(Blah_Vertex *points[], Blah_Material *material)
{	//Draws multiple points given null pointer terminated array
	blah_draw_gl_primitive(points, GL_POINTS, NULL, material);
}

void blah_draw_gl_polygon(Blah_Vertex *vertices[], Blah_Texture_Map *textureMap, Blah_Material *material)
{	//Draw a polygon with a list of vertices
	blah_draw_gl_primitive(vertices, GL_POLYGON, textureMap, material);
}

void blah_draw_gl_polygon2d(Blah_Vertex *vertices[], Blah_Texture_Map *textureMap, Blah_Material *material)
{	//Draw a polygon in 2d mode with vertices specified by array of points.
	//Vertex coordinates are rendered relative to current drawport.

	float ambientLightRed = blah_draw_currentScene->ambientLightRed;
	float ambientLightBlue = blah_draw_currentScene->ambientLightBlue;
	float ambientLightGreen = blah_draw_currentScene->ambientLightGreen;
	float ambientLightAlpha = blah_draw_currentScene->ambientLightAlpha;

	blah_draw_gl_setAmbientLight(1,1,1,1);

	blah_draw_gl_primitive2d(vertices, GL_POLYGON, textureMap, material);

	blah_draw_gl_setAmbientLight(ambientLightRed, ambientLightBlue, ambientLightGreen, ambientLightAlpha);
}

void blah_draw_gl_popMatrix()
{	//Pop OpenGL matrix and restore previous state
	glPopMatrix();
}

static void blah_draw_gl_primitive(Blah_Vertex *vertices[], GLenum mode, Blah_Texture_Map *textureMap, Blah_Material *material) {

	int vertexIndex = 0;
	Blah_Point *mapping;
	const Blah_Texture *texture;

	//fprintf(stderr,"Enter blah_draw_gl_primitive\n");

	if (material!=blah_draw_gl_currentMaterial) {
		//If using a different material than previous, change opengl state
		blah_draw_gl_currentMaterial = material;
		glMaterialfv(GL_FRONT, GL_AMBIENT, (GLfloat*)&material->ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, (GLfloat*)&material->diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, (GLfloat*)&material->specular);
		glMaterialfv(GL_FRONT, GL_EMISSION, (GLfloat*)&material->emission);
		glMateriali(GL_FRONT, GL_SHININESS, (GLint)material->shininess);
	}

	if (textureMap) { //If there is a texture supplied
		texture = textureMap->texture;
		if (texture != blah_draw_gl_currentTexture) {
			if (glIsTexture((GLuint)(texture->handle))) {
				blah_draw_gl_currentTexture = texture;
				glBindTexture(GL_TEXTURE_2D, (GLuint)(texture->handle));
			} else
				fprintf(stderr,"invalid texture id\n");
		}

		glBegin(mode);  //Begin GL primitive

		mapping = textureMap->mapping;
		//use mapping coordinates provided
		while (vertices[vertexIndex]) {
			//fprintf(stderr,"processing vertex index %d\n",vertex_index);
			glTexCoord2fv((GLfloat*)&mapping[vertexIndex]);
			glVertex3fv((GLfloat*)&vertices[vertexIndex]->location);
			//fprintf(stderr,"begin normal");
			glNormal3fv((GLfloat*)&vertices[vertexIndex]->normal);
			//fprintf(stderr, "using normal %f,%f,%f\n",vertices[vertex_index]->normal.x,
			//	vertices[vertex_index]->normal.y,vertices[vertex_index]->normal.z);
			vertexIndex++;  //For each vertex there is a corresponding texture coord
		}
		glEnd(); //End GL primitive

	} else { //Draw primitive without texture
		glBindTexture(GL_TEXTURE_2D, 0);  //Bind to default texture (should be none)
		blah_draw_gl_currentTexture = NULL; //mark current texture as being none
		glBegin(mode);  //Begin GL primitive

		while (vertices[vertexIndex]) {
			//fprintf(stderr,"processing vertex\n");
			glVertex3fv((GLfloat*)&vertices[vertexIndex]->location);
			//fprintf(stderr,"begin normal");
			glNormal3fv((GLfloat*)&vertices[vertexIndex]->normal);
			//fprintf(stderr, "using normal %f,%f,%f\n",vertices[vertex_index]->normal.x,
			//	vertices[vertex_index]->normal.y,vertices[vertex_index]->normal.z);
			vertexIndex++;
		}	//Call glVertex3fv for all Blah_PointS in vertex_list

		glEnd(); //End GL primitive
	}

}

static void blah_draw_gl_primitive2d(Blah_Vertex *vertices[], GLenum mode, Blah_Texture_Map *textureMap, Blah_Material *material) {
	//Use OpenGL to draw a primitive of specified type (mode parameter)
	//with a given list of vertices in 2D mode (infront of eye)
	glPushMatrix(); //save model view
	//Set up 2D drawport with clipping planes
	glLoadMatrixf((GLfloat*)&blah_draw_gl_drawportMatrix);
	//Set up proper pseudo 2D viewing parameters
	glMatrixMode(GL_PROJECTION);
	glPushMatrix(); //save projection matrix on stack
	glLoadMatrixf((GLfloat*)&blah_draw_gl_2dProjectionMatrix);
	//Use standard 3D routine to do the actual drawing once we have pseudo 2D
	blah_draw_gl_primitive(vertices, GL_POLYGON, textureMap, material);

	glPopMatrix(); //restore projection matrix
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix(); //restore model view matrix
}

void blah_draw_gl_printError()
{	//Print information about current GL error to standard error out
	fprintf(stderr,"GL ERROR code:%d\n",glGetError());
	switch (glGetError()) {
		case GL_NO_ERROR :
			fprintf(stderr,"None\n");
			break;
		case GL_INVALID_ENUM :
			fprintf(stderr,"Invalid enum\n");
			break;
		case GL_INVALID_VALUE :
			fprintf(stderr,"Invalid value\n");
			break;
		case GL_INVALID_OPERATION :
			fprintf(stderr,"Invalid operation\n");
			break;
		case GL_STACK_OVERFLOW :
			fprintf(stderr,"Stack Overflow\n");
			break;
		case GL_STACK_UNDERFLOW :
			fprintf(stderr,"Stack underflow\n");
			break;
		case GL_OUT_OF_MEMORY :
			fprintf(stderr,"Out of memory\n");
			break;
		case GL_TABLE_TOO_LARGE :
			fprintf(stderr,"Table too large\n");
			break;
	}
}

void blah_draw_gl_pushMatrix()
{	//Push OpenGL matrix and save video state
	glPushMatrix();
}

static void blah_draw_gl_resetLights()
{
	int lightCount;

	for (lightCount = 0; lightCount < 8; lightCount++)
		glDisable(blah_draw_gl_lightSymbols[lightCount]);

	blah_draw_gl_activeLights = 0;
}

void blah_draw_gl_resetMatrix()
{	//Set the current matrix to the identity matrix
	glLoadIdentity();
}

void blah_draw_gl_setAmbientLight(float red, float green, float blue, float alpha)
{	//Sets the properties of the ambient light used to render current drawing
	GLfloat lightParams[] = {red,green,blue,alpha};

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lightParams);
}

void blah_draw_gl_setDrawport(unsigned int left, unsigned int bottom, unsigned int right, unsigned int top)
{	//Updates the local GL specific 2D drawport matrix
	//2D drawport matrix is identity matrix with simple translation
	Blah_Matrix_setTranslation(&blah_draw_gl_drawportMatrix,
		(int)left - (int)(blah_video_currentMode->width >> 1),
		(int)bottom - (int)(blah_video_currentMode->height >> 1), 0);
}

bool blah_draw_gl_setLight(Blah_Point *location, Blah_Colour *diffuse, Blah_Colour *ambient, Blah_Vector *direction, float intensity, float spread)
{	//Enables a light source at specified location in 3D space, with given qualities
	GLenum lightSymbol = blah_draw_gl_lightSymbols[blah_draw_gl_activeLights];

	glEnable(lightSymbol); //enable next available GL light
	glLightfv(lightSymbol, GL_POSITION, (GLfloat*)location); //set location of light
	glLightfv(lightSymbol, GL_AMBIENT, (GLfloat*)ambient); //use colour structure as array of floats
	glLightfv(lightSymbol, GL_DIFFUSE, (GLfloat*)diffuse); //set diffuse properties
	glLightfv(lightSymbol, GL_SPOT_DIRECTION, (GLfloat*)direction);
	glLightf(lightSymbol, GL_SPOT_EXPONENT, (GLfloat)(1.0f/128.0f * intensity));
	glLightf(lightSymbol, GL_SPOT_CUTOFF, (GLfloat)(spread / 2.0f));


	blah_draw_gl_activeLights++; //increment number of lights in use

	return true;
}

void blah_draw_gl_setViewport(unsigned int left, unsigned int bottom, unsigned int right, unsigned int top)
{	//Sets the viewport 2D drawing region coordinates to which all 2D drawing
	//operations are relative.  Coordinates specified are relative to the origin
	//0,0, bottom left of screen.
	glViewport((GLint)left, (GLint)bottom, (GLsizei)(right-left+1), (GLsizei)(top-bottom+1));
}

void blah_draw_gl_solidCone(float base, float height, int slices, int stacks, Blah_Material *material)
{	//FIXME - glutSolidCone((GLdouble)base, (GLdouble)height, (GLint)slices, (GLint)stacks);
}

void blah_draw_gl_solidSphere(float radius, int slices, int stacks, Blah_Material *material) {
	//FIXME - Need a better method of drawing sphere
	//glutSolidSphere((GLdouble)radius, slices, stacks);
}

void blah_draw_gl_triangle(Blah_Vertex *points[], Blah_Texture_Map *textureMap, Blah_Material *material)
{	//Draws a trianlge with points specified by points
	blah_draw_gl_primitive(points, GL_TRIANGLES, textureMap, material);
}

void blah_draw_gl_triangleStrip(Blah_Vertex *points[], Blah_Texture_Map *textureMap, Blah_Material *material)
{	//Draws a trianlge strip with points specified by points
	blah_draw_gl_primitive(points, GL_TRIANGLE_STRIP, textureMap, material);
}

void blah_draw_gl_update2dProjection(const Blah_Video_Mode* mode)
{	//Calculates and updates the internal 2d projection matrix using physical
	//dimensions of viewing area (video mode width/height)
	//This function is called when the video mode is set and should not be called
	//at any other time.
	Blah_Matrix_setIdentity(&blah_draw_gl_2dProjectionMatrix);
	Blah_Matrix_setAxisX(&blah_draw_gl_2dProjectionMatrix, (float)2/(float)mode->width,0,0);
	Blah_Matrix_setAxisY(&blah_draw_gl_2dProjectionMatrix, 0,(float)2/(float)mode->height,0);
	Blah_Matrix_setAxisZ(&blah_draw_gl_2dProjectionMatrix, 0,0,(float)2/(float)blah_draw_currentParameters.depthOfVision);
	Blah_Matrix_setTranslation(&blah_draw_gl_2dProjectionMatrix, 0, 0,-1);
}

void blah_draw_gl_updatePerspective()
{	//Sets up the viewing perspective by setting up view clipping planes and normal
	//etc in the projection matrix and applies
	//a transform matrix to the modelview matrix to simulate a vantage point
	float distance = Blah_Point_distancePoint(&blah_draw_currentParameters.viewpoint, &blah_draw_currentParameters.focalPoint); //get distance to focal point
	float halfAngleX = blah_draw_currentParameters.fieldOfVisionX/2;
	float halfAngleY = blah_draw_currentParameters.fieldOfVisionY/2;
	GLdouble halfWidth = distance * tan(halfAngleX);
	GLdouble halfHeight = distance * tan(halfAngleY);

	blah_draw_gl_resetLights(); //FIXME-SHOULD NOT BE IN THIS FUNCTION

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-halfWidth, halfWidth, -halfHeight, halfHeight, 0, blah_draw_currentParameters.depthOfVision);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(blah_draw_currentParameters.viewpoint.x, blah_draw_currentParameters.viewpoint.y,
		blah_draw_currentParameters.viewpoint.z, blah_draw_currentParameters.focalPoint.x,
		blah_draw_currentParameters.focalPoint.y, blah_draw_currentParameters.focalPoint.z,
		blah_draw_currentParameters.viewNormal.x, blah_draw_currentParameters.viewNormal.y,
		blah_draw_currentParameters.viewNormal.z);
}

/* void blah_draw_gl_wire_cube(float side_length, Blah_Material *material) {
	float half_side = side_length / 2;
	Blah_Point front_top_left = {-half_side, half_side, half_side};
	Blah_Point front_top_right = {half_side, half_side, half_side};
	Blah_Point front_bottom_right = {half_side, -half_side, half_side};
	Blah_Point front_bottom_left = {-half_side, -half_side, half_side};
	Blah_Point back_top_left = {-half_side, half_side, -half_side};
	Blah_Point back_top_right = {half_side, half_side, -half_side};
	Blah_Point back_bottom_right = {half_side, -half_side, -half_side};
	Blah_Point back_bottom_left = {-half_side, -half_side, -half_side};

	Blah_Point *cube_line_strip[] = {&front_top_left, &front_top_right, &front_bottom_right,
		&front_bottom_left, &front_top_left, &back_top_left, &back_top_right, &back_bottom_right,
		&back_bottom_left, &back_top_left, NULL};
	Blah_Point *cube_lines[] = {&front_top_right, &back_top_right, &front_bottom_right, &back_bottom_right,
		&front_bottom_left, &back_bottom_left,NULL};

	blah_draw_gl_primitive(cube_line_strip, GL_LINE_STRIP, NULL, material);
	blah_draw_gl_primitive(cube_lines, GL_LINES, NULL, material);
} */


