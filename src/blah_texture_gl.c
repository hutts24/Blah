/* blah_texture_gl.c
	Defines OpenGL specific functions for manipulating textures  */
#include <GL/gl.h>
#include <GL/glext.h>

#include "blah_texture.h"

/* Function Declarations */

blah_texture_handle Blah_Texture_gl_new(Blah_Image *sourceImage) {
	//Creates a new texture from a source image
	GLuint newTextureName;	//OpenGL texture name
	GLenum texturePixelFormat;
	GLenum sourceFormat;
	
	switch(sourceImage->pixelFormat) {
		case BLAH_PIXEL_FORMAT_RGBA :
			texturePixelFormat = GL_RGBA; sourceFormat = GL_RGBA; break;
		case BLAH_PIXEL_FORMAT_BGRA :
			texturePixelFormat = GL_RGBA; sourceFormat = GL_BGRA; break;
		case BLAH_PIXEL_FORMAT_RGB :
			texturePixelFormat = GL_RGB; sourceFormat = GL_RGB; break;
		case BLAH_PIXEL_FORMAT_BGR :
			texturePixelFormat = GL_RGB; sourceFormat = GL_BGR; break;
		default : 
			texturePixelFormat = GL_RGBA; sourceFormat = GL_RGBA; break;
	}
	
	glGenTextures(1,&newTextureName); //Get a new texture name using OpenGL API
	glBindTexture(GL_TEXTURE_2D, newTextureName);
	glTexImage2D(GL_TEXTURE_2D, 0, texturePixelFormat, sourceImage->width,
		sourceImage->height, 0, sourceFormat, GL_UNSIGNED_BYTE,
		sourceImage->pixelData);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	// Linear Filtering
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	// Linear Filtering
	glBindTexture(GL_TEXTURE_2D, 0);
	
	return (blah_texture_handle)newTextureName;
}

void Blah_Texture_gl_destroy(blah_texture_handle handle) {
	//Destroys a texture
	GLuint temp = (GLuint)handle;
	glDeleteTextures(1,&temp);
}
