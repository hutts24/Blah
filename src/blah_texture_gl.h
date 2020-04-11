/* blah_texture_gl.h 
	Header file for blah_texture_gl.c
	Defines structure for textures and functions to manipulate textures */
	
#ifndef _BLAH_TEXTURE_GL

#define _BLAH_TEXTURE_GL

#include "blah_image.h"

/* Public Function Prototypes */

#ifdef __cplusplus
	extern "C" {
#endif //__cplusplus

blah_texture_handle Blah_Texture_gl_new(Blah_Image *sourceImage);
	//Creates a new internal GL texture from a source image

void Blah_Texture_gl_destroy(blah_texture_handle handle);
	//Destroys a GL texture given handle

#ifdef __cplusplus
	}
#endif //__cplusplus		
		
#endif
