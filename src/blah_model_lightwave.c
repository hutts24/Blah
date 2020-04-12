/* blah_model_lightwave.c
	Defines functions for using lightwave models
*/

#include <malloc.h>
#include <string.h>

#include "blah_model_lightwave.h"
#include "blah_debug.h"
#include "blah_util.h"
#include "blah_file.h"
#include "blah_types.h"
#include "blah_point.h"
#include "blah_object.h"
#include "blah_list.h"
#include "blah_iff.h"

/* Private Globals */

Blah_Debug_Log *blah_model_lightwave_log;

/* Private Function Prototypes */

/* Surface Function Prototypes */

static unsigned long Blah_Model_Lightwave_readSurfaceChunk(Blah_Model_Lightwave *model, Blah_IFF_Chunk *chunk);
	//Reads surface information from the surface chunk into the appropriate surface
	//in the surfaces list of the given model
	//Returns the number of bytes in the chunk data

static unsigned long Blah_Model_Lightwave_readColourSubchunk(Blah_Model_Lightwave_Surface *surface, Blah_IFF_Subchunk *subchunk);
	//Reads colour information from a colour subchunk and sets the colour
	//of the given surface accordingly

static unsigned long Blah_Model_Lightwave_readSurfaceFlagsSubchunk(Blah_Model_Lightwave_Surface *surface, Blah_IFF_Subchunk *subchunk);
	//Reads surface information from a surface flags subchunk and sets the attributes
	//of the given surface accordingly

static unsigned long Blah_Model_Lightwave_readVluminositySubchunk(Blah_Model_Lightwave_Surface *surface, Blah_IFF_Subchunk *subchunk);
	//Reads the luminosity value (in 32bit float) from a vluminosity subchunk
	//and sets the luminosity value of the given surface

static unsigned long Blah_Model_Lightwave_readLuminositySubchunk(Blah_Model_Lightwave_Surface *surface, Blah_IFF_Subchunk *subchunk);
	//Reads the luminosity value (in 32bit float) from a vluminosity subchunk
	//and if luminosity has not been set yet, sets the luminosity value of the
	//given surface

static unsigned long Blah_Model_Lightwave_readVdiffuseSubchunk(Blah_Model_Lightwave_Surface *surface, Blah_IFF_Subchunk *subchunk);
	//Reads the diffuse value (in 32bit float) from a vdiffuse subchunk
	//and sets the diffuse value of the given surface

static unsigned long Blah_Model_Lightwave_readDiffuseSubchunk(Blah_Model_Lightwave_Surface *surface, Blah_IFF_Subchunk *subchunk);
	//Reads the diffuse value (in 16bit integer) from a diffuse subchunk
	//and if diffuse has not been set yet, sets the diffuse value of the
	//given surface

static unsigned long Blah_Model_Lightwave_readVspecularSubchunk(Blah_Model_Lightwave_Surface *surface, Blah_IFF_Subchunk *subchunk);
	//Reads the specular value (in 32bit float) from a vspecular subchunk
	//and sets the specular value of the given surface

static unsigned long Blah_Model_Lightwave_readSpecularSubchunk(Blah_Model_Lightwave_Surface *surface, Blah_IFF_Subchunk *subchunk);
	//Reads the specular value (in 16bit integer) from a specular subchunk
	//and if specular has not been set yet, sets the specular value of the
	//given surface

static unsigned long Blah_Model_Lightwave_readVtransSubchunk(Blah_Model_Lightwave_Surface *surface, Blah_IFF_Subchunk *subchunk);
	//Reads the transparency value (in 32bit float) from a vtrans subchunk
	//and sets the trans value of the given surface

static unsigned long Blah_Model_Lightwave_readTransSubchunk(Blah_Model_Lightwave_Surface *surface, Blah_IFF_Subchunk *subchunk);
	//Reads the transparency value (in 16bit integer) from a trans subchunk
	//and if trans has not been set yet, sets the trans value of the
	//given surface

static unsigned long Blah_Model_Lightwave_readGlossinessSubchunk(Blah_Model_Lightwave_Surface *surface, Blah_IFF_Subchunk *subchunk);
	//Reads the glossiness value (in 16bit integer) from a glossiness subchunk
	//and if glossiness has not been set yet, sets the glossiness value of the
	//given surface

static unsigned long Blah_Model_Lightwave_readColourTextureSubchunk(Blah_Model_Lightwave_Surface_Texture *texture, Blah_IFF_Subchunk *subchunk);
	//Reads the type of the texture from a colour texture (CTEX) subchunk
	//into the current lightwave texture parameters

static unsigned long Blah_Model_Lightwave_readTextureSizeSubchunk(Blah_Model_Lightwave_Surface_Texture *texture, Blah_IFF_Subchunk *subchunk);
	//Reads the size of the texture from a texture size (TSIZ) subchunk
	//into the current lightwave texture parameters

static unsigned long Blah_Model_Lightwave_readTextureSizeSubchunk(Blah_Model_Lightwave_Surface_Texture *texture, Blah_IFF_Subchunk *subchunk);
	//Reads the size of the texture from a texture size (TSIZ) subchunk
	//into the current lightwave texture parameters

static unsigned long Blah_Model_Lightwave_readTextureCenterSubchunk(Blah_Model_Lightwave_Surface_Texture *texture, Blah_IFF_Subchunk *subchunk);
	//Reads the coordinates where the center of the texture will be placed apon the
	//surface, from a texture center (TCTR) subchunk into the current lightwave
	//texture parameters

static unsigned long Blah_Model_Lightwave_readTextureColourSubchunk(Blah_Model_Lightwave_Surface_Texture *texture, Blah_IFF_Subchunk *subchunk);
	//Reads the modifying texture colour as 3 usigned 8bit values followed by pad byte
	//from a texture colour (TCLR) subchunk into the current lightwave
	//texture parameters

static unsigned long Blah_Model_Lightwave_readTextureFilenameSubchunk(Blah_Model_Lightwave_Surface_Texture *texture, Blah_IFF_Subchunk *subchunk);
	//Reads the name of the file to be used as a texture image map from a texture
	//image (TIMG) subchunk into the given lightwave texture parameters structure

static unsigned long Blah_Model_Lightwave_readTextureFlagsSubchunk(Blah_Model_Lightwave_Surface_Texture *texture, Blah_IFF_Subchunk *subchunk);
	//Reads the flags for a surface texture from a texture
	//flags (TFLG) subchunk into the given lightwave texture parameters structure


/* Main Functions */

static unsigned long Blah_Model_Lightwave_getSize(FILE *fileStream);
	//Returns the size of the lightwave object data (in bytes).
	//Returns 0 if the file stream does not contain a valid lightwave object

static unsigned long Blah_Model_Lightwave_readPointsChunk(Blah_Model_Lightwave *model, Blah_IFF_Chunk *chunk);
	//Constructs a list of points in model, using data from the points chunk
	//Returns the number of bytes in the chunk data

static unsigned long Blah_Model_Lightwave_readFacesChunk(Blah_Model_Lightwave *model, Blah_IFF_Chunk *chunk);
	//Constructs a list of faces in model, using data from the faces chunk
	//Returns the number of bytes in the chunk data

static unsigned long Blah_Model_Lightwave_readSurfacelistChunk(Blah_Model_Lightwave *model, Blah_IFF_Chunk *chunk);
	//Constructs a list of empty surfaces in model, using data from the points chunk
	//Returns the number of bytes in the chunk data

/* Private Function Declarations */

static unsigned long Blah_Model_Lightwave_readTextureFlagsSubchunk(Blah_Model_Lightwave_Surface_Texture *texture, Blah_IFF_Subchunk *subchunk) {
	//Reads the flags for a surface texture from a texture
	//flags (TFLG) subchunk into the given lightwave texture parameters structure
	blah_unsigned16 textureFlags;

	//clear the flags
	texture->xAxis = texture->yAxis = texture->zAxis = false;

	//Read texture flags as a 16bit word
	Blah_IFF_Subchunk_readUnsigned16(subchunk, &textureFlags);
	//Now set the texture flags
	if (textureFlags & BLAH_MODEL_LIGHTWAVE_TEXTURE_X_AXIS) {
		Blah_Debug_Log_message(blah_model_lightwave_log, "Texture x axis");
		texture->xAxis = true;
	}
	if (textureFlags & BLAH_MODEL_LIGHTWAVE_TEXTURE_Y_AXIS) {
		Blah_Debug_Log_message(blah_model_lightwave_log, "Texture y axis");
		texture->yAxis = true;
	}
	if (textureFlags & BLAH_MODEL_LIGHTWAVE_TEXTURE_Z_AXIS) {
		Blah_Debug_Log_message(blah_model_lightwave_log, "Texture z axis");
		texture->zAxis = true;
	}

	return subchunk->subchunkLength;
}

static unsigned long Blah_Model_Lightwave_readTextureWrapSubchunk(Blah_Model_Lightwave_Surface_Texture *texture, Blah_IFF_Subchunk *subchunk) {
	//Reads the wrap options for a surface texture from a texture
	//wrap options (TWRP) subchunk into the given lightwave texture parameters structure
	char debugString[200], *modeString=NULL;
	blah_unsigned16 widthWrap, heightWrap;
	//read the width and height wrapping options as 16bit unsigned values
	Blah_IFF_Subchunk_readUnsigned16(subchunk, &widthWrap);
	Blah_IFF_Subchunk_readUnsigned16(subchunk, &heightWrap);

	texture->widthWrap = widthWrap; texture->heightWrap = heightWrap;

	switch (widthWrap) {
		case BLAH_MODEL_LIGHTWAVE_WRAP_BLACK :
			modeString = "black\0"; break;
		case BLAH_MODEL_LIGHTWAVE_WRAP_CLAMP :
			modeString = "clamp\0"; break;
		case BLAH_MODEL_LIGHTWAVE_WRAP_TILE :
			modeString = "tile\0"; break;
		case BLAH_MODEL_LIGHTWAVE_WRAP_MIRROR :
			modeString = "mirror\0"; break;
	}
	sprintf(debugString, "Texture width wrap: %s\n",modeString);
	Blah_Debug_Log_message(blah_model_lightwave_log, debugString);

	switch (heightWrap) {
		case BLAH_MODEL_LIGHTWAVE_WRAP_BLACK :
			modeString = "black\0"; break;
		case BLAH_MODEL_LIGHTWAVE_WRAP_CLAMP :
			modeString = "clamp\0"; break;
		case BLAH_MODEL_LIGHTWAVE_WRAP_TILE :
			modeString = "tile\0"; break;
		case BLAH_MODEL_LIGHTWAVE_WRAP_MIRROR :
			modeString = "mirror\0"; break;
	}
	sprintf(debugString, "Texture height wrap: %s\n",modeString);
	Blah_Debug_Log_message(blah_model_lightwave_log, debugString);

	return subchunk->subchunkLength;
}

static unsigned long Blah_Model_Lightwave_readTextureSizeSubchunk(Blah_Model_Lightwave_Surface_Texture *texture, Blah_IFF_Subchunk *subchunk) {
	//Reads the size of the texture from a texture size (TSIZ) subchunk
	//into the current lightwave texture parameters
	blah_float32 sizeX, sizeY, sizeZ;
	char debugString[200];

	//Read x,y, and z values as 32bit floating points
	Blah_IFF_Subchunk_readFloat32(subchunk, &sizeX);
	Blah_IFF_Subchunk_readFloat32(subchunk, &sizeY);
	Blah_IFF_Subchunk_readFloat32(subchunk, &sizeZ);

	sprintf(debugString, "Texture size: %f,%f,%f",sizeZ,sizeY,sizeZ);
	Blah_Debug_Log_message(blah_model_lightwave_log, debugString);

	Blah_Vector_set(&texture->size, sizeX, sizeY, sizeZ);

	return subchunk->subchunkLength;
}

static unsigned long Blah_Model_Lightwave_readTextureCenterSubchunk(Blah_Model_Lightwave_Surface_Texture *texture, Blah_IFF_Subchunk *subchunk) {
	//Reads the coordinates where the center of the texture will be placed apon the
	//surface, from a texture center (TCTR) subchunk into the current lightwave
	//texture parameters
	blah_float32 centerX, centerY, centerZ;
	char debugString[200];

	//Read x,y, and z values as 32bit floating points
	Blah_IFF_Subchunk_readFloat32(subchunk, &centerX);
	Blah_IFF_Subchunk_readFloat32(subchunk, &centerY);
	Blah_IFF_Subchunk_readFloat32(subchunk, &centerZ);

	sprintf(debugString, "Texture center: %f,%f,%f",centerX,centerY,centerZ);
	Blah_Debug_Log_message(blah_model_lightwave_log, debugString);

	Blah_Point_set(&texture->center, centerX, centerY, centerZ);

	return subchunk->subchunkLength;
}

static unsigned long Blah_Model_Lightwave_readTextureColourSubchunk(Blah_Model_Lightwave_Surface_Texture *texture, Blah_IFF_Subchunk *subchunk) {
	//Reads the modifying texture colour as 3 8bit unsigned values followed by pad byte
	//from a texture colour (TCLR) subchunk into the current lightwave
	//texture parameters
	blah_unsigned8 red, green, blue;
	char debugString[200];

	Blah_IFF_Subchunk_readUnsigned8(subchunk, &red);
	Blah_IFF_Subchunk_readUnsigned8(subchunk, &green);
	Blah_IFF_Subchunk_readUnsigned8(subchunk, &blue);
	Blah_IFF_Subchunk_seek(subchunk, 1);

	Blah_Colour_set(&texture->colour, (float)red/255.0, (float)green/255.0, (float)blue/255.0, 1);

	sprintf(debugString, "Texture colour: %f,%f,%f,%f",texture->colour.red,
			texture->colour.green,texture->colour.blue,texture->colour.alpha);
	Blah_Debug_Log_message(blah_model_lightwave_log, debugString);

	return subchunk->subchunkLength;
}


static unsigned long Blah_Model_Lightwave_readTextureFilenameSubchunk(Blah_Model_Lightwave_Surface_Texture *lwTexture, Blah_IFF_Subchunk *subchunk) {
	//Reads the name of the file to be used as a texture image map from a texture
	//image (TIMG) subchunk into the given lightwave texture parameters structure
	char *tempString, debugString[200];
	Blah_Image *tempImage;
	Blah_Texture *texture;

	Blah_Debug_Log_message(blah_model_lightwave_log, "Reading texture filename");
	sprintf(debugString, "Length of texture filename chunk:%u",subchunk->subchunkLength);
	Blah_Debug_Log_message(blah_model_lightwave_log, debugString);

	tempString = Blah_IFF_Subchunk_readString(subchunk);
	sprintf(debugString, "image filename read:%s",tempString);
	Blah_Debug_Log_message(blah_model_lightwave_log, debugString);
	blah_util_strncpy(lwTexture->fileName, tempString, BLAH_MODEL_LIGHTWAVE_TEXTURE_FILENAME_LENGTH);
	//Try to locate an existing texture from same image
	texture = blah_texture_find(tempString);
	Blah_Debug_Log_message(blah_model_lightwave_log, "looking for texture");

	if (!texture) { //if no texture found
		Blah_Debug_Log_message(blah_model_lightwave_log, "texture not found, looking for image");

		tempImage = blah_image_find(tempString); //try to locate existing copy of image
		if (!tempImage) { //if no copy of same image found
			Blah_Debug_Log_message(blah_model_lightwave_log, "image not found");
			tempImage = Blah_Image_fromFile(tempString); //load it from file
		}

		Blah_Debug_Log_message(blah_model_lightwave_log, "tried to load file");
		if (tempImage) { //if loading image successful, create new texture from it
			Blah_Debug_Log_message(blah_model_lightwave_log, "image loaded, creating texture");
			texture = Blah_Texture_fromImage(tempImage); //create texture from image
		}
	}

	lwTexture->texture = texture;
	free(tempString);

	if (subchunk->padBytePresent)
		Blah_IFF_Subchunk_seek(subchunk, 1); //If length is odd, then seek one pad byte

	return subchunk->subchunkLength;
}

static unsigned long Blah_Model_Lightwave_readColourTextureSubchunk(Blah_Model_Lightwave_Surface_Texture *texture, Blah_IFF_Subchunk *subchunk) {
	//Reads the type of the texture from a colour texture (CTEX) subchunk
	//into the current lightwave texture parameters
	char *tempString, debugString[200];

	Blah_Debug_Log_message(blah_model_lightwave_log, "Reading colour texture");
	sprintf(debugString, "Length of colour texture chunk:%u",subchunk->subchunkLength);
	Blah_Debug_Log_message(blah_model_lightwave_log, debugString);

	tempString = Blah_IFF_Subchunk_readString(subchunk);
	if (!strcmp(tempString, "Planar Image Map"))
		texture->projectionMode = BLAH_MODEL_TEXTURE_PROJECTION_PLANAR;
	sprintf(debugString, "texture type read:%s",tempString);
	Blah_Debug_Log_message(blah_model_lightwave_log, debugString);
	blah_util_strncpy(texture->type, tempString, BLAH_MODEL_LIGHTWAVE_TEXTURE_TYPE_LENGTH);
	free(tempString);

	if (subchunk->padBytePresent)
		Blah_IFF_Subchunk_seek(subchunk, 1); //If length is odd, then seek one pad byte

	return subchunk->subchunkLength;
}

static unsigned long Blah_Model_Lightwave_readColourSubchunk(Blah_Model_Lightwave_Surface *surface, Blah_IFF_Subchunk *subchunk) {
	//Reads colour information from a colour subchunk and sets the colour
	//of the given surface accordingly
	blah_unsigned8 red,green,blue;

	Blah_IFF_Subchunk_readUnsigned8(subchunk, &red);
	Blah_IFF_Subchunk_readUnsigned8(subchunk, &green);
	Blah_IFF_Subchunk_readUnsigned8(subchunk, &blue);
	Blah_IFF_Subchunk_seek(subchunk, 1);

	Blah_Colour_set(&surface->colour, (float)red/255.0, (float)green/255.0, (float)blue/255.0, 1);

	return subchunk->subchunkLength;
}

static unsigned long Blah_Model_Lightwave_readSurfaceFlagsSubchunk(Blah_Model_Lightwave_Surface *surface, Blah_IFF_Subchunk *subchunk) {
	//Reads surface information from a surface flags subchunk and sets the attributes
	//of the given surface accordingly
	blah_unsigned16 surfaceFlags;

	Blah_IFF_Subchunk_readUnsigned16(subchunk, &surfaceFlags);
	//grab surface_flags as first 2 bytes of the sub chunk data

	surface->luminosityFlag = surfaceFlags & BLAH_MODEL_LIGHTWAVE_SURFACE_FLAGS_LUMINOUS ? true : false;
	surface->outlineFlag = surfaceFlags & BLAH_MODEL_LIGHTWAVE_SURFACE_FLAGS_OUTLINE ? true : false;
	surface->smoothFlag = surfaceFlags & BLAH_MODEL_LIGHTWAVE_SURFACE_FLAGS_SMOOTH ? true : false;
	surface->colourHighlightsFlag = surfaceFlags & BLAH_MODEL_LIGHTWAVE_SURFACE_FLAGS_COLOURHIGH ? true : false;
	surface->colourFilterFlag = surfaceFlags & BLAH_MODEL_LIGHTWAVE_SURFACE_FLAGS_COLOURFILTER ? true : false;
	surface->opaqueEdgeFlag = surfaceFlags & BLAH_MODEL_LIGHTWAVE_SURFACE_FLAGS_OPAQUEEDGE ? true : false;
	surface->transparentEdgeFlag = surfaceFlags & BLAH_MODEL_LIGHTWAVE_SURFACE_FLAGS_TRANSEDGE ? true : false;
	surface->sharpTerminatorFlag = surfaceFlags & BLAH_MODEL_LIGHTWAVE_SURFACE_FLAGS_SHARPTERM ? true : false;
	surface->doubleSidedFlag = surfaceFlags & BLAH_MODEL_LIGHTWAVE_SURFACE_FLAGS_DOUBLESIDED ? true : false;
	surface->additiveFlag = surfaceFlags & BLAH_MODEL_LIGHTWAVE_SURFACE_FLAGS_ADDITIVE ? true : false;
	surface->shadowAlphaFlag = surfaceFlags & BLAH_MODEL_LIGHTWAVE_SURFACE_FLAGS_SHADOWALPHA ? true : false;

	return subchunk->subchunkLength; //Add two bytes for the sub chunk length
}

static unsigned long Blah_Model_Lightwave_readVluminositySubchunk(Blah_Model_Lightwave_Surface *surface, Blah_IFF_Subchunk *subchunk) {
	//Reads the luminosity value (in 32bit float) from a vluminosity subchunk
	//and sets the luminosity value of the given surface
	Blah_IFF_Subchunk_readFloat32(subchunk, &surface->vluminosity);
	//grab luminosity as first 4 bytes of the sub chunk data
	return subchunk->subchunkLength; //Add two bytes for the sub chunk length
}

static unsigned long Blah_Model_Lightwave_readVdiffuseSubchunk(Blah_Model_Lightwave_Surface *surface, Blah_IFF_Subchunk *subchunk) {
	//Reads the diffuse value (in 32bit float) from a vdiffuse subchunk
	//and sets the diffuse value of the given surface
	Blah_IFF_Subchunk_readFloat32(subchunk, &surface->vdiffuse);
	//grab luminosity as first 4 bytes of the sub chunk data
	return subchunk->subchunkLength; //Add two bytes for the sub chunk length
}

static unsigned long Blah_Model_Lightwave_readVspecularSubchunk(Blah_Model_Lightwave_Surface *surface, Blah_IFF_Subchunk *subchunk) {
	//Reads the specular value (in 32bit float) from a vspecular subchunk
	//and sets the specular value of the given surface
	Blah_IFF_Subchunk_readFloat32(subchunk, &surface->vspecular);
	//grab luminosity as first 4 bytes of the sub chunk data
	return subchunk->subchunkLength; //Add two bytes for the sub chunk length
}

static unsigned long Blah_Model_Lightwave_readVtransSubchunk(Blah_Model_Lightwave_Surface *surface, Blah_IFF_Subchunk *subchunk) {
	//Reads the trans value (in 32bit float) from a vtrans subchunk
	//and sets the trans value of the given surface
	Blah_IFF_Subchunk_readFloat32(subchunk, &surface->vtransparency);
	//grab luminosity as first 4 bytes of the sub chunk data
	return subchunk->subchunkLength; //Add two bytes for the sub chunk length
}

static unsigned long Blah_Model_Lightwave_readLuminositySubchunk(Blah_Model_Lightwave_Surface *surface, Blah_IFF_Subchunk *subchunk) {
	//Reads the luminosity value (in 16bit integer) from a luminosity subchunk
	//and if luminosity has not been set yet, sets the luminosity value of the
	//given surface
	blah_unsigned16 luminosityInt;
	Blah_IFF_Subchunk_readUnsigned16(subchunk, &luminosityInt);
	surface->luminosity = (float)luminosityInt / 65536.0f;
	return subchunk->subchunkLength;
}

static unsigned long Blah_Model_Lightwave_readDiffuseSubchunk(Blah_Model_Lightwave_Surface *surface, Blah_IFF_Subchunk *subchunk) {
	//Reads the diffuse value (in 16bit integer) from a diffuse subchunk
	//and if diffuse has not been set yet, sets the diffuse value of the
	//given surface
	blah_unsigned16 diffuseInt;
	Blah_IFF_Subchunk_readUnsigned16(subchunk, &diffuseInt);
	surface->diffuse = (float)diffuseInt / 65536.0f;
	return subchunk->subchunkLength;
}

static unsigned long Blah_Model_Lightwave_readSpecularSubchunk(Blah_Model_Lightwave_Surface *surface, Blah_IFF_Subchunk *subchunk) {
	//Reads the specular value (in 16bit integer) from a specular subchunk
	//and if specular has not been set yet, sets the specular value of the
	//given surface
	blah_unsigned16 specularInt;
	Blah_IFF_Subchunk_readUnsigned16(subchunk, &specularInt);
	surface->specular = (float)specularInt / 65536.0f;
	return subchunk->subchunkLength;
}

static unsigned long Blah_Model_Lightwave_readTransSubchunk(Blah_Model_Lightwave_Surface *surface, Blah_IFF_Subchunk *subchunk) {
	//Reads the trans value (in 16bit integer) from a trans subchunk
	//and if trans has not been set yet, sets the trans value of the
	//given surface
	blah_unsigned16 transInt;
	Blah_IFF_Subchunk_readUnsigned16(subchunk, &transInt);
	surface->transparency = (float)transInt / 65536.0f;
	return subchunk->subchunkLength;
}

static unsigned long Blah_Model_Lightwave_readGlossinessSubchunk(Blah_Model_Lightwave_Surface *surface, Blah_IFF_Subchunk *subchunk) {
	//Reads the glossiness value (in 32bit float) from a glossiness subchunk
	//and sets the glossiness value of the given surface
	blah_unsigned16 glossInt;
	Blah_IFF_Subchunk_readUnsigned16(subchunk, &glossInt);
	surface->glossiness = glossInt >> 3;
	return subchunk->subchunkLength; //Add two bytes for the sub chunk length
}

static unsigned long Blah_Model_Lightwave_skipSubchunk(Blah_IFF_Subchunk *subchunk) {
	//Skips the given subchunk by skipping forward the size of the
	//subchunk less the subchunk header (it has already been read)
	//Returns number of bytes skipped or 0 if failure
	blah_unsigned32 skipLength;
	char tempString[BLAH_DEBUG_MESSAGE_LENGTH];

	skipLength = subchunk->subchunkLength - BLAH_IFF_SUBCHUNK_HEADER_LENGTH;
	sprintf(tempString, "Skipping subchunk length:%u",skipLength);
	Blah_Debug_Log_message(blah_model_lightwave_log, tempString);


	if (Blah_IFF_Chunk_seek(subchunk->parentChunk, skipLength)) //Skip subchunk length
		return 0; //Positive return from seek means failure, return 0
	else
		return subchunk->subchunkLength;	//Return the size of the subchunk skipped
}

static unsigned long Blah_Model_Lightwave_getSize(FILE *fileStream) {
	//Returns the size of the lightwave object data (in bytes).
	//Returns 0 if the file stream does not contain a valid lightwave object
	char tempString[BLAH_DEBUG_MESSAGE_LENGTH];
	blah_unsigned32 fileTag, lwobTag, lwobLength;
	unsigned long returnLength = 0;

	fread(&lwobTag, sizeof(blah_unsigned32), 1, fileStream);
	blah_file_readUnsigned32(fileStream, &lwobLength);

	if (lwobTag != BLAH_MODEL_LIGHTWAVE_FORM) {
		sprintf(tempString, "IFF form tag is:%x\n",lwobTag);
		Blah_Debug_Log_message(blah_model_lightwave_log, tempString);
		Blah_Debug_Log_message(blah_model_lightwave_log, "File is not an IFF file");
	} else {
		Blah_Debug_Log_message(blah_model_lightwave_log, "File header conforms to IFF format");
		sprintf(tempString, "IFF data length is: %u\n",lwobLength);
		Blah_Debug_Log_message(blah_model_lightwave_log, tempString);

		fread(&fileTag, sizeof(blah_unsigned32), 1, fileStream);

		if (fileTag != BLAH_MODEL_LIGHTWAVE_LWOB)
			Blah_Debug_Log_message(blah_model_lightwave_log, "IFF data is not a lightwave object");
		else {
			Blah_Debug_Log_message(blah_model_lightwave_log, "Identified file as lightwave format");
			returnLength = lwobLength - 4;
		}
	}
	return returnLength; //Return the length of remaining lightwave data chunks
}

static unsigned long Blah_Model_Lightwave_skipChunk(Blah_IFF_Chunk *chunk) {
	//Skips the next chunk in the file by skipping forward the size of the
	//chunk - the chunk header (it has already been read)
	//Returns number of bytes skipped or 0 if failure
	blah_unsigned32 skipLength;

	skipLength = chunk->chunkLength - BLAH_IFF_CHUNK_HEADER_LENGTH;

	if (fseek(chunk->filePointer, skipLength, SEEK_CUR)) //Skip chunk length
		return 0; //Positive return from seek means failure, return 0
	else
		return chunk->chunkLength;	//Return the size of the chunk skipped
}

static unsigned long Blah_Model_Lightwave_readPointsChunk(Blah_Model_Lightwave *model, Blah_IFF_Chunk *chunk) {
	//Parses a points list chunk from a given chunk structure
	//Creates an array of pointers to allocated POINT structures in model
	//Returns number of bytes read
	blah_unsigned32 numPoints, pointCount;
	blah_float32 tempX, tempY, tempZ;

	char tempString[100];

	Blah_Debug_Log_message(blah_model_lightwave_log, "Reading points list");
	numPoints = chunk->dataLength / 12; //divide by 12 bytes for num points
	sprintf(tempString, "Number of points:%u",numPoints);
	Blah_Debug_Log_message(blah_model_lightwave_log, tempString);
	Blah_Debug_Log_message(blah_model_lightwave_log, "called list init - blah points");

	for (pointCount = 0; pointCount < numPoints; pointCount++) {
		Blah_IFF_Chunk_readFloat32(chunk, &tempX);
		Blah_IFF_Chunk_readFloat32(chunk, &tempY);
		Blah_IFF_Chunk_readFloat32(chunk, &tempZ);
		Blah_Model_addVertex(model->newModel, Blah_Vertex_new(tempX, tempY, tempZ));
	}

	return chunk->chunkLength; //Return the size of the data parsed (and pad byte if present)
}

static unsigned long Blah_Model_Lightwave_readFacesChunk(Blah_Model_Lightwave *model, Blah_IFF_Chunk *chunk) {
	//Parses a polygon list chunk from an IFF chunk
	//Creates an array of pointers to allocated PRIMITVE structures in model
	//Returns number of bytes read
	blah_unsigned16 numVertices, vertexCount, vertexIndex;
	blah_int16 surfaceIndex;
	Blah_Model_Face *tempFace;
	char tempString[100];
	Blah_Model_Surface **surfacePointers; //temporary pointer array for indexing

	Blah_Debug_Log_message(blah_model_lightwave_log, "Reading facess list");
	sprintf(tempString, "Length of faces chunk:%u",chunk->chunkLength);
	Blah_Debug_Log_message(blah_model_lightwave_log, tempString);
	surfacePointers = (Blah_Model_Surface**)Blah_List_createPointerstring(&model->newModel->surfaces);

	while (chunk->currentOffset < chunk->dataLength-1) { //While end of data not reached
		Blah_IFF_Chunk_readUnsigned16(chunk, &numVertices);
		//Read number of vertices for next polygon

		tempFace = Blah_Model_Face_new(); //create a new face

		for (vertexCount = 0; vertexCount < numVertices; vertexCount++) {
			Blah_IFF_Chunk_readUnsigned16(chunk, &vertexIndex);
			//Read index into vertex list for next vertex point
			Blah_Model_Face_addIndex(tempFace, vertexIndex); //Add index to face
		}

		Blah_IFF_Chunk_readInt16(chunk, &surfaceIndex);
		//Read surface index from file
		if (surfaceIndex < 0)
			Blah_Debug_Log_message(blah_model_lightwave_log,"negative surface index - detail polygons\n");

		tempFace->surface = surfaceIndex;

		Blah_Model_addFace(model->newModel, tempFace);
		if (surfaceIndex > 0)
			Blah_Model_Surface_addFace(surfacePointers[surfaceIndex-1], tempFace);
	}

	free(surfacePointers);
	sprintf(tempString, "Faces found:%d",model->newModel->faces.length);
	Blah_Debug_Log_message(blah_model_lightwave_log, tempString);

	return chunk->chunkLength; //return length of chunk read
}

static unsigned long Blah_Model_Lightwave_readSurfacelistChunk(Blah_Model_Lightwave *model, Blah_IFF_Chunk *chunk) {
	//Parses a surface list chunk from a lightwave file
	//Creates an list of empty surfaces.  Returns number of bytes read
	char *tempString, debugString[200];
	Blah_Model_Surface *newSurface;

	Blah_Debug_Log_message(blah_model_lightwave_log, "Reading surfaces list");
	sprintf(debugString, "Length of surface list chunk:%u",chunk->chunkLength);
	Blah_Debug_Log_message(blah_model_lightwave_log, debugString);

	while (chunk->currentOffset < chunk->dataLength -1) {
		//While end of chunk data not reached
		tempString = Blah_IFF_Chunk_readString(chunk);
		sprintf(debugString, "Surface name read:%s",tempString);
		Blah_Debug_Log_message(blah_model_lightwave_log, debugString);
		newSurface = Blah_Model_Surface_new(tempString);
		Blah_Model_addSurface(model->newModel, newSurface);
		//Add the new surface to the list of surfaces in new model
		Blah_Tree_insertElement(&model->surfacesTree, tempString, newSurface);
		//Also add the same pointer to the temporary surface tree (for name searching)
		free(tempString);
	} //Parse all surface name strings from the chunk

	if (chunk->padBytePresent)
		Blah_IFF_Chunk_seek(chunk, 1); //If length is odd, then seek one pad byte

	return chunk->chunkLength;
}


static unsigned long Blah_Model_Lightwave_readSurfaceChunk(Blah_Model_Lightwave *model, Blah_IFF_Chunk *chunk) {
	//Reads surface information from the surface chunk into the appropriate surface
	//in the surfaces list of the given model
	//Returns the number of bytes in the chunk data

	char tempString[BLAH_DEBUG_MESSAGE_LENGTH];
	char *surfaceName;
	Blah_Model_Lightwave_Surface tempSurface;
	Blah_Model_Lightwave_Surface_Texture tempTexture;
	Blah_Model_Surface *currentSurface;
	Blah_IFF_Subchunk tempSubchunk;
	Blah_Model_Texture_Map *newMap;

	memset(&tempSurface, 0, sizeof(Blah_Model_Lightwave_Surface));
	memset(&tempTexture, 0, sizeof(Blah_Model_Lightwave_Surface_Texture));

	Blah_Debug_Log_message(blah_model_lightwave_log, "Reading surface chunk");
	sprintf(tempString, "Length of surface chunk:%u",chunk->chunkLength);
	Blah_Debug_Log_message(blah_model_lightwave_log, tempString);

	surfaceName = Blah_IFF_Chunk_readString(chunk); //Read surface name from chunk
	sprintf(tempString, "Read surface name:%s",surfaceName);
	Blah_Debug_Log_message(blah_model_lightwave_log, tempString);

	Blah_Debug_Log_message(blah_model_lightwave_log, "Searching for a surface matching name");
	currentSurface = Blah_Tree_findElement(&model->surfacesTree, surfaceName)->data;

	if (currentSurface)
		Blah_Debug_Log_message(blah_model_lightwave_log, "Found a surface matching name");
	else
		Blah_Debug_Log_message(blah_model_lightwave_log, "No matching surface found");

	while (chunk->currentOffset < chunk->dataLength-1) {
		//While end of chunk not reached
		Blah_IFF_Subchunk_get(&tempSubchunk, chunk); //read next subchunk header
		sprintf(tempString, "Subchunk format:%c%c%c%c",
			((unsigned char*)&tempSubchunk.idTag)[0], ((unsigned char*)&tempSubchunk.idTag)[1],
			((unsigned char*)&tempSubchunk.idTag)[2], ((unsigned char*)&tempSubchunk.idTag)[3]);

		Blah_Debug_Log_message(blah_model_lightwave_log, tempString);

		switch (tempSubchunk.idTag) { //Switch depending apon sub chunk type
			case BLAH_MODEL_LIGHTWAVE_SURFACE_COLOUR :
				Blah_Model_Lightwave_readColourSubchunk(&tempSurface, &tempSubchunk);
				break;
			case BLAH_MODEL_LIGHTWAVE_SURFACE_FLAGS :
				Blah_Model_Lightwave_readSurfaceFlagsSubchunk(&tempSurface, &tempSubchunk);
				break;
			case BLAH_MODEL_LIGHTWAVE_SURFACE_VLUMINOSITY  :
				Blah_Model_Lightwave_readVluminositySubchunk(&tempSurface, &tempSubchunk);
				break;
			case BLAH_MODEL_LIGHTWAVE_SURFACE_LUMINOSITY  :
				Blah_Model_Lightwave_readLuminositySubchunk(&tempSurface, &tempSubchunk);
				break;
			case BLAH_MODEL_LIGHTWAVE_SURFACE_VDIFFUSE  :
				Blah_Model_Lightwave_readVdiffuseSubchunk(&tempSurface, &tempSubchunk);
				break;
			case BLAH_MODEL_LIGHTWAVE_SURFACE_DIFFUSE  :
				Blah_Model_Lightwave_readDiffuseSubchunk(&tempSurface, &tempSubchunk);
				break;
			case BLAH_MODEL_LIGHTWAVE_SURFACE_VSPECULAR  :
				Blah_Model_Lightwave_readVspecularSubchunk(&tempSurface, &tempSubchunk);
				break;
			case BLAH_MODEL_LIGHTWAVE_SURFACE_SPECULAR  :
				Blah_Model_Lightwave_readSpecularSubchunk(&tempSurface, &tempSubchunk);
				break;
			case BLAH_MODEL_LIGHTWAVE_SURFACE_VTRANS  :
				Blah_Model_Lightwave_readVtransSubchunk(&tempSurface, &tempSubchunk);
				break;
			case BLAH_MODEL_LIGHTWAVE_SURFACE_TRANS  :
				Blah_Model_Lightwave_readTransSubchunk(&tempSurface, &tempSubchunk);
				break;
			case BLAH_MODEL_LIGHTWAVE_SURFACE_GLOSSINESS  :
				Blah_Model_Lightwave_readGlossinessSubchunk(&tempSurface, &tempSubchunk);
				break;
			case BLAH_MODEL_LIGHTWAVE_COLOUR_TEXTURE :
				Blah_Model_Lightwave_readColourTextureSubchunk(&tempTexture, &tempSubchunk);
				break;
			case BLAH_MODEL_LIGHTWAVE_TEXTURE_SIZE :
				Blah_Model_Lightwave_readTextureSizeSubchunk(&tempTexture, &tempSubchunk);
				break;
			case BLAH_MODEL_LIGHTWAVE_TEXTURE_CENTER :
				Blah_Model_Lightwave_readTextureCenterSubchunk(&tempTexture, &tempSubchunk);
				break;
			case BLAH_MODEL_LIGHTWAVE_TEXTURE_COLOUR :
				Blah_Model_Lightwave_readTextureColourSubchunk(&tempTexture, &tempSubchunk);
				break;
			case BLAH_MODEL_LIGHTWAVE_TEXTURE_IMAGE :
				Blah_Model_Lightwave_readTextureFilenameSubchunk(&tempTexture, &tempSubchunk);
				break;
			case BLAH_MODEL_LIGHTWAVE_TEXTURE_FLAGS :
				Blah_Model_Lightwave_readTextureFlagsSubchunk(&tempTexture, &tempSubchunk);
				break;
			case BLAH_MODEL_LIGHTWAVE_TEXTURE_WRAP :
				Blah_Model_Lightwave_readTextureWrapSubchunk(&tempTexture, &tempSubchunk);
				break;

			default: //Skip unhandled sub chunk
				Blah_Debug_Log_message(blah_model_lightwave_log, "Skipping Subchunk");
				Blah_Model_Lightwave_skipSubchunk(&tempSubchunk);
				break;
		}

	}

	Blah_Debug_Log_message(blah_model_lightwave_log, tempString);

	if (chunk->padBytePresent)
		Blah_IFF_Chunk_seek(chunk, 1);	//If length is odd, then skip pad byte

	free(surfaceName);

	//Set new surface properties
	currentSurface->luminosity = tempSurface.vluminosity ? tempSurface.vluminosity :
		(tempSurface.luminosity ? tempSurface.luminosity / 256 :
		(tempSurface.luminosityFlag ? 1 :0));
	currentSurface->diffuse = tempSurface.vdiffuse ? tempSurface.vdiffuse :
		(tempSurface.diffuse ? tempSurface.diffuse / 256 : 0);
	currentSurface->specular = tempSurface.vspecular ? tempSurface.vspecular :
		(tempSurface.specular ? tempSurface.specular / 256 : 0);
	currentSurface->reflection = tempSurface.vreflection ? tempSurface.vreflection :
		(tempSurface.reflection ? tempSurface.reflection / 256 : 0);
	currentSurface->transparency = tempSurface.vtransparency ? tempSurface.vtransparency :
		(tempSurface.transparency ? tempSurface.transparency / 256 : 0);
	currentSurface->glossiness = tempSurface.glossiness / 1024;
	memcpy(&currentSurface->colour, &tempSurface.colour, sizeof(Blah_Colour));

	//Must NOT forget this -  have to make sure there is a valid texture first before
	//mapping.  Sleep depravation error caused much debugging work to find this duh.

	if (tempTexture.texture) { //check for valid texture
		newMap = Blah_Model_Texture_Map_new(tempTexture.texture,
			tempTexture.xAxis ? 'x' : (tempTexture.yAxis ? 'y' : 'z'),
			tempTexture.projectionMode);
		Blah_Model_Texture_Map_setCenter(newMap, tempTexture.center.x,
			tempTexture.center.y, tempTexture.center.z);
		Blah_Model_Texture_Map_setSize(newMap, tempTexture.size.x,
			tempTexture.size.y, tempTexture.size.z); //map texture and add to surface
		Blah_Model_Surface_addTexture(currentSurface, newMap);
	}

	return chunk->chunkLength; //Add four bytes for chunk length value
}

/* Public Functions */

Blah_Model *Blah_Model_Lightwave_load(char *filename, FILE *fileStream) {
	//Creates a new model structure.  Memory is allocated etc
	Blah_Model_Lightwave lightwaveTemp;
	unsigned long lwobLength, bytesRemaining, bytesRead;
	//bytes_remaining holds the number of data bytes in the file, following the LWOB tag
	char tempString[BLAH_DEBUG_MESSAGE_LENGTH];
	Blah_IFF_Chunk dataChunk;

	Blah_Tree_init(&lightwaveTemp.surfacesTree, "LightwaveSurfaceTree");
	lightwaveTemp.newModel = Blah_Model_new(filename);
	//Create new model inside lightwave temp structure

	blah_model_lightwave_log = Blah_Debug_Log_new("blah_lightwave"); //Blah_Debug_Log_init(&blah_model_lightwave_log, "blah_lightwave");
	Blah_Debug_Log_message(blah_model_lightwave_log, "Begin reading lightwave file");

	bytesRemaining = lwobLength = Blah_Model_Lightwave_getSize(fileStream);

	while (bytesRemaining) { //Read chunks
		Blah_IFF_Chunk_get(&dataChunk, fileStream);
		sprintf(tempString, "Chunk format:%c%c%c%c",
			((unsigned char*)&dataChunk.idTag)[0], ((unsigned char*)&dataChunk.idTag)[1],
			((unsigned char*)&dataChunk.idTag)[2], ((unsigned char*)&dataChunk.idTag)[3]);
		Blah_Debug_Log_message(blah_model_lightwave_log, tempString);

		switch (dataChunk.idTag) {  //Switch depending apon chunk type
			case BLAH_MODEL_LIGHTWAVE_POINTLIST : //Load point list
				bytesRead = Blah_Model_Lightwave_readPointsChunk(&lightwaveTemp, &dataChunk);
				break;
			case BLAH_MODEL_LIGHTWAVE_FACELIST : //Read polygons
				bytesRead = Blah_Model_Lightwave_readFacesChunk(&lightwaveTemp, &dataChunk);
				break;
			case BLAH_MODEL_LIGHTWAVE_SURFACELIST : //Read surface list
				bytesRead = Blah_Model_Lightwave_readSurfacelistChunk(&lightwaveTemp, &dataChunk);
				break;
			case BLAH_MODEL_LIGHTWAVE_SURFACE : //Read surface chunk
				bytesRead = Blah_Model_Lightwave_readSurfaceChunk(&lightwaveTemp, &dataChunk);
				break;
			default: //Skip unhandled chunk
				Blah_Debug_Log_message(blah_model_lightwave_log, "Skipping Chunk");
				bytesRead = Blah_Model_Lightwave_skipChunk(&dataChunk);
				break;
		}

		if (bytesRemaining < bytesRead || !bytesRead)
			bytesRemaining = 0;
		else
			bytesRemaining -= bytesRead;

		sprintf(tempString, "Bytes remaining %lu", bytesRemaining);
		Blah_Debug_Log_message(blah_model_lightwave_log, tempString);
	}

	Blah_Tree_removeAll(&lightwaveTemp.surfacesTree);

	Blah_Debug_Log_destroy(blah_model_lightwave_log); //Blah_Debug_Log_close(&blah_model_lightwave_log);
	return lightwaveTemp.newModel; //Return pointer whether it be null or valid model
}
