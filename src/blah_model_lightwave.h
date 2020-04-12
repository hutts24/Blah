/* blah_model_lightwave.h
	Header file for blah_model_lightwave.c
	Defines structures and functions for manipulating lightwave models
*/

#ifndef _BLAH_MODEL_LIGHTWAVE

#define _BLAH_MODEL_LIGHTWAVE

#include <stdio.h>
#include "blah_model.h"
#include "blah_object.h"
#include "blah_image.h"
#include "blah_tree.h"
#include "blah_types.h"
#include "blah_texture.h"

/* Defines */

/* String length definitions */

#define BLAH_MODEL_LIGHTWAVE_TEXTURE_NAME_LENGTH 	50
#define BLAH_MODEL_LIGHTWAVE_TEXTURE_TYPE_LENGTH 	50
#define BLAH_MODEL_LIGHTWAVE_TEXTURE_FILENAME_LENGTH 200

/* Definitions for chunk id tags */
#define BLAH_MODEL_LIGHTWAVE_FORM			0x4D524F46	//"FORM" in unsigned long format
#define BLAH_MODEL_LIGHTWAVE_LWOB			0x424F574C	//"LWOB" in unsigned long format
#define BLAH_MODEL_LIGHTWAVE_POINTLIST		0x53544E50	//"PNTS" ... ... ... ... ... ...
#define BLAH_MODEL_LIGHTWAVE_SURFACELIST	0x53465253	//"SRFS" ... ... ... ... ... ...
#define BLAH_MODEL_LIGHTWAVE_FACELIST		0x534C4F50	//"POLS" ... ... ... ... ... ...
#define BLAH_MODEL_LIGHTWAVE_CURVE_LIST		"CRVS"
#define BLAH_MODEL_LIGHTWAVE_PATCH_LIST		"PCHS"
#define BLAH_MODEL_LIGHTWAVE_SURFACE		0x46525553	//"SURF"

/* Definitions for sub chunk id tags */

#define BLAH_MODEL_LIGHTWAVE_SURFACE_COLOUR			0x524C4F43	//"COLR"
#define BLAH_MODEL_LIGHTWAVE_SURFACE_FLAGS			0x47414C46	//"FLAG"
#define BLAH_MODEL_LIGHTWAVE_SURFACE_GLOSSINESS		0x534F4C47	//"GLOS"
#define BLAH_MODEL_LIGHWTAVE_SURFACE_REFLECTMODE	0x544C4652	//"RFLT"

#define BLAH_MODEL_LIGHTWAVE_SURFACE_LUMINOSITY		0x494D554C	//"LUMI"
#define BLAH_MODEL_LIGHTWAVE_SURFACE_DIFFUSE		0x46464944	//"DIFF"
#define BLAH_MODEL_LIGHTWAVE_SURFACE_SPECULAR		0x43455053	//"SPEC"
#define BLAH_MODEL_LIGHTWAVE_SURFACE_REFLECTION		0x4C464552	//"REFL"
#define BLAH_MODEL_LIGHTWAVE_SURFACE_TRANS			0x4E415254	//"TRAN"
/* 'V' float sub chunks overide their short integer counterparts */
#define BLAH_MODEL_LIGHTWAVE_SURFACE_VLUMINOSITY	0x4D554C56	//"VLUM"
#define BLAH_MODEL_LIGHTWAVE_SURFACE_VDIFFUSE		0x46494456	//"VDIF"
#define BLAH_MODEL_LIGHTWAVE_SURFACE_VSPECULAR		0x43505356	//"VSPC"
#define BLAH_MODEL_LIGHTWAVE_SURFACE_VREFLECTION	0x4C465256	//"VRFL"
#define BLAH_MODEL_LIGHTWAVE_SURFACE_VTRANS			0x4E525456	//"VTRN"


/* FLAG subchunk uses single bits to indicate surface attributes.  Mask definitions... */
#define BLAH_MODEL_LIGHTWAVE_SURFACE_FLAGS_LUMINOUS		0000000000000001
#define BLAH_MODEL_LIGHTWAVE_SURFACE_FLAGS_OUTLINE		0000000000000010
#define BLAH_MODEL_LIGHTWAVE_SURFACE_FLAGS_SMOOTH		0000000000000100
#define BLAH_MODEL_LIGHTWAVE_SURFACE_FLAGS_COLOURHIGH	0000000000001000
#define BLAH_MODEL_LIGHTWAVE_SURFACE_FLAGS_COLOURFILTER	0000000000010000
#define BLAH_MODEL_LIGHTWAVE_SURFACE_FLAGS_OPAQUEEDGE	0000000000100000 //should not be set?
#define BLAH_MODEL_LIGHTWAVE_SURFACE_FLAGS_TRANSEDGE	0000000001000000 //should not be set?
#define BLAH_MODEL_LIGHTWAVE_SURFACE_FLAGS_SHARPTERM	0000000010000000
#define BLAH_MODEL_LIGHTWAVE_SURFACE_FLAGS_DOUBLESIDED	0000000100000000
#define BLAH_MODEL_LIGHTWAVE_SURFACE_FLAGS_ADDITIVE		0000001000000000
#define BLAH_MODEL_LIGHTWAVE_SURFACE_FLAGS_SHADOWALPHA	0000010000000000

/* Texture subchunk ids */

#define BLAH_MODEL_LIGHTWAVE_COLOUR_TEXTURE			0x58455443	//"CTEX"
#define BLAH_MODEL_LIGHTWAVE_DIFFUSE_TEXTURE		0x58455444	//"DTEX"
#define BLAH_MODEL_LIGHTWAVE_SPECULAR_TEXTURE		0x58455453	//"STEX"
#define BLAH_MODEL_LIGHTWAVE_REFLECTION_TEXTURE		0x58455452	//"RTEX"
#define BLAH_MODEL_LIGHTWAVE_TRANSPARENCY_TEXTURE	0x58455454	//"TTEX"
#define BLAH_MODEL_LIGHTWAVE_LUMINOSITY_TEXTURE		0x5845544C	//"LTEX"
#define BLAH_MODEL_LIGHTWAVE_BUMP_TEXTURE			0x58455442	//"BTEX"

#define BLAH_MODEL_LIGHTWAVE_TEXTURE_SIZE			0x5A495354	//"TSIZ"
#define BLAH_MODEL_LIGHTWAVE_TEXTURE_CENTER			0x52544354	//"TCTR"
#define BLAH_MODEL_LIGHTWAVE_TEXTURE_FALLOFF		0x4C414654	//"TFAL"
#define BLAH_MODEL_LIGHTWAVE_TEXTURE_VELOCITY		0x4C455654	//"TVEL"
#define BLAH_MODEL_LIGHTWAVE_TEXTURE_IMAGE			0x474D4954	//"TIMG"
#define BLAH_MODEL_LIGHTWAVE_TEXTURE_COLOUR			0x524C4354	//"TCLR"
#define BLAH_MODEL_LIGHTWAVE_TEXTURE_FLAGS			0x474C4654	//"TFLG"
#define BLAH_MODEL_LIGHTWAVE_TEXTURE_WRAP			0x50525754	//"TWRP"

/* TFLG subchunk uses single bits to indicate texture attributes.  Mask definitions... */

#define BLAH_MODEL_LIGHTWAVE_TEXTURE_X_AXIS				0000000000000001
#define BLAH_MODEL_LIGHTWAVE_TEXTURE_Y_AXIS				0000000000000010
#define BLAH_MODEL_LIGHTWAVE_TEXTURE_Z_AXIS				0000000000000100
#define BLAH_MODEL_LIGHTWAVE_TEXTURE_WORLDCOORDS		0000000000001000
#define BLAH_MODEL_LIGHTWAVE_TEXTURE_NEGATIVEIMAGE		0000000000010000
#define BLAH_MODEL_LIGHTWAVE_TEXTURE_PIXELBLENDING		0000000000100000 //should not be set?
#define BLAH_MODEL_LIGHTWAVE_TEXTURE_ANTIALIASING		0000000001000000 //should not be set?

/* Type Definitions */

typedef enum Blah_Model_Lightwave_Texture_Category {BLAH_MODEL_LIGHTWAVE_NOTEX = 0,
	BLAH_MODEL_LIGHTWAVE_CTEX, BLAH_MODEL_LIGHTWAVE_DTEX, BLAH_MODEL_LIGHTWAVE_STEX,
	BLAH_MODEL_LIGHTWAVE_RTEX, BLAH_MODEL_LIGHTWAVE_TTEX, BLAH_MODEL_LIGHTWAVE_LTEX,
	BLAH_MODEL_LIGHTWAVE_BTEX} blah_model_lightwave_texture_category;

typedef enum Blah_Model_Lightwave_Texture_Wrap {BLAH_MODEL_LIGHTWAVE_WRAP_BLACK=0,
	BLAH_MODEL_LIGHTWAVE_WRAP_CLAMP = 1, BLAH_MODEL_LIGHTWAVE_WRAP_TILE = 2,
	BLAH_MODEL_LIGHTWAVE_WRAP_MIRROR = 3} blah_model_lightwave_texture_wrap;

/* Structure Definitions */

typedef struct Blah_Model_Lightwave {
	Blah_Tree surfacesTree;	//Binary Tree of surface names, index into model surfaces
	Blah_Model *newModel;
} Blah_Model_Lightwave;

typedef struct Blah_Model_Lightwave_Surface {
	char name[BLAH_MODEL_SURFACE_NAME_LENGTH+1];
	Blah_Colour colour;
	blah_float32 vluminosity;		//floating point luminosity of surface
	blah_float32 vdiffuse;			//floating point surface diffuse property
	blah_float32 vspecular;			//floating point surface specular attribute
	blah_float32 vreflection;		//floating point surface reflection property
	blah_float32 vtransparency;		//surface transparency, 0 - opaque, 1 fully transparent
	blah_int16 luminosity;			//16bit integer describes luminosity of surface
	blah_int16 diffuse;				//16bit integer surface diffuse property
	blah_int16 specular;			//16bit integer surface specular attribute
	blah_int16 reflection;			//16bit integer surface reflection property
	blah_int16 transparency;		//16bit integer surface transparency
	unsigned int glossiness;	//surface glossiness
	/* Surface Flags */
	bool luminosityFlag;
	bool outlineFlag;
	bool smoothFlag;
	bool colourHighlightsFlag;
	bool colourFilterFlag;
	bool opaqueEdgeFlag;
	bool transparentEdgeFlag;
	bool sharpTerminatorFlag;
	bool doubleSidedFlag;
	bool additiveFlag;
	bool shadowAlphaFlag;

} Blah_Model_Lightwave_Surface;

typedef struct Blah_Model_Lightwave_Surface_Texture {
	blah_model_lightwave_texture_category category;
	//Texture category according to lightwave design
	char type[BLAH_MODEL_LIGHTWAVE_TEXTURE_TYPE_LENGTH+1]; //Type of texture as per lightwave control panel
	char name[BLAH_MODEL_LIGHTWAVE_TEXTURE_NAME_LENGTH+1];
	char fileName[BLAH_MODEL_LIGHTWAVE_TEXTURE_FILENAME_LENGTH+1];
	Blah_Vector size;
	Blah_Point center;
	Blah_Vector falloff;
	Blah_Vector velocity;
	Blah_Texture *texture; //pointer to the texture used for this surface
	Blah_Colour colour;
	/* Texture flags */
	bool xAxis;
	bool yAxis;
	bool zAxis;
	bool worldCoords;
	bool negativeImage;
	bool pixelBlending;
	bool antialiasing;
	blah_model_texture_projection projectionMode;
	blah_model_lightwave_texture_wrap widthWrap,heightWrap;
} Blah_Model_Lightwave_Surface_Texture;


#endif
