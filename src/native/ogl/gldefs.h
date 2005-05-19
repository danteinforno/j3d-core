/*
 * $RCSfile$
 *
 * Copyright (c) 2005 Sun Microsystems, Inc. All rights reserved.
 *
 * Use is subject to license terms.
 *
 * $Revision$
 * $Date$
 * $State$
 */

#ifndef _Java3D_gldefs_h_
#define _Java3D_gldefs_h_

/*
 * Portions of this code were derived from work done by the Blackdown
 * group (www.blackdown.org), who did the initial Linux implementation
 * of the Java 3D API.
 */

#include <math.h>
#include <stdlib.h>
#include <string.h>

#if defined(SOLARIS) || defined(LINUX)
#define GLX_GLEXT_PROTOTYPES
#define GLX_GLXEXT_PROTOTYPES
#define UNIX

#include <limits.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include "glext.h"
#endif

#ifdef WIN32
#include <windows.h>

#ifndef _WIN32
#define _WIN32
#endif

#define M_PI 3.14159265358979323846

/* Some constant defined in those javax_media_j3d_*.h */
/* TODO: remove those constant when D3D automatically include those *.h files */
#ifdef D3D

/* used in GeometryArrayRetained, execute geometry in by REFERENCE case */
#ifndef javax_media_j3d_GeometryArrayRetained_COORD_FLOAT
#define javax_media_j3d_GeometryArrayRetained_COORD_FLOAT 1L
#endif
#ifndef javax_media_j3d_GeometryArrayRetained_COORD_DOUBLE
#define javax_media_j3d_GeometryArrayRetained_COORD_DOUBLE 2L
#endif
#ifndef javax_media_j3d_GeometryArrayRetained_COLOR_FLOAT
#define javax_media_j3d_GeometryArrayRetained_COLOR_FLOAT 4L
#endif
#ifndef javax_media_j3d_GeometryArrayRetained_COLOR_BYTE
#define javax_media_j3d_GeometryArrayRetained_COLOR_BYTE 8L
#endif
#ifndef javax_media_j3d_GeometryArrayRetained_NORMAL_FLOAT
#define javax_media_j3d_GeometryArrayRetained_NORMAL_FLOAT 16L
#endif
#ifndef javax_media_j3d_GeometryArrayRetained_TEXCOORD_FLOAT
#define javax_media_j3d_GeometryArrayRetained_TEXCOORD_FLOAT 32L
#endif

#endif /* end of ifdef D3D */

#ifndef D3D
#include <GL/gl.h>
#include "wglext.h"
#include "glext.h"
#endif

#endif /* WIN32 */

/* include those .h files generated by javah */
#include "javax_media_j3d_Background.h"
#include "javax_media_j3d_Canvas3D.h"
#include "javax_media_j3d_ColoringAttributes.h"
#include "javax_media_j3d_ColoringAttributesRetained.h"
#include "javax_media_j3d_DepthComponentRetained.h"
#include "javax_media_j3d_DetailTextureImage.h"
#include "javax_media_j3d_DirectionalLightRetained.h"
#include "javax_media_j3d_DisplayListRenderMethod.h"
#include "javax_media_j3d_DrawingSurfaceObjectAWT.h"
#include "javax_media_j3d_ExponentialFogRetained.h"
#include "javax_media_j3d_GeometryRetained.h"
#include "javax_media_j3d_GeometryArray.h"
#include "javax_media_j3d_GeometryArrayRetained.h"
#include "javax_media_j3d_GraphicsContext3D.h"
#include "javax_media_j3d_ImageComponent.h"
#include "javax_media_j3d_ImageComponentRetained.h"
#include "javax_media_j3d_ImageComponent2DRetained.h"
#include "javax_media_j3d_IndexedGeometryArrayRetained.h"
#include "javax_media_j3d_LineAttributes.h"
#include "javax_media_j3d_LineAttributesRetained.h"
#include "javax_media_j3d_LinearFogRetained.h"
#include "javax_media_j3d_MasterControl.h"
#include "javax_media_j3d_Material.h"
#include "javax_media_j3d_MaterialRetained.h"
#include "javax_media_j3d_ModelClipRetained.h"
#include "javax_media_j3d_NativeConfigTemplate3D.h"
#include "javax_media_j3d_NodeRetained.h"
#include "javax_media_j3d_PointAttributesRetained.h"
#include "javax_media_j3d_PointLightRetained.h"
#include "javax_media_j3d_PolygonAttributes.h"
#include "javax_media_j3d_PolygonAttributesRetained.h"
#include "javax_media_j3d_Raster.h"
#include "javax_media_j3d_RasterRetained.h"
#include "javax_media_j3d_Renderer.h"
#include "javax_media_j3d_RenderingAttributes.h"
#include "javax_media_j3d_RenderingAttributesRetained.h"
#include "javax_media_j3d_RenderMolecule.h"
#include "javax_media_j3d_SpotLightRetained.h"
#include "javax_media_j3d_TexCoordGeneration.h"
#include "javax_media_j3d_TexCoordGenerationRetained.h"
#include "javax_media_j3d_Texture.h"
#include "javax_media_j3d_Texture2D.h"
#include "javax_media_j3d_Texture2DRetained.h"
#include "javax_media_j3d_Texture3DRetained.h"
#include "javax_media_j3d_TextureAttributes.h"
#include "javax_media_j3d_TextureAttributesRetained.h"
#include "javax_media_j3d_TextureCubeMapRetained.h"
#include "javax_media_j3d_TextureRetained.h"
#include "javax_media_j3d_TextureUnitStateRetained.h"
#include "javax_media_j3d_TransparencyAttributes.h"
#include "javax_media_j3d_TransparencyAttributesRetained.h"
#include "javax_media_j3d_GLSLShaderProgramRetained.h"
#include "javax_media_j3d_Shader.h"
#include "javax_media_j3d_ShaderError.h"

/*
 * Define these constants here as a workaround for conflicting
 * glext.h files between Mesa and Solaris
 */

#ifndef GL_CLAMP_TO_BORDER_SGIS
#define GL_CLAMP_TO_BORDER_SGIS           0x812D
#endif

#ifndef GL_VIDEO_RESIZE_COMPENSATION_SUN
#define GL_VIDEO_RESIZE_COMPENSATION_SUN        0x85CD
#endif

/*
 * End constant workaround
 */
                                                                                
                                                                                
/* Used to compare floating point values close to 0.0 */
#define  J3D_SMALL_FLOAT 0.00001f

/*
 * General purpose assertion macro
 */
#define J3D_ASSERT(expr)						\
    if (!(expr)) {							\
	fprintf(stderr,							\
	    "\nAssertion failed in module '%s' at line %d\n",		\
	    __FILE__, __LINE__);					\
	fprintf(stderr, "\t%s\n\n", #expr);				\
    }

#define EPSILON 1e-2
#define J3D_FNE(a,b) ((a)>((b)+EPSILON)||(a)<((b)-EPSILON))


/*
 * Macro to copy and transpose one matrix to another.
 *
 * NOTE: the source and dest must not be the same (no aliasing check
 * is performed).
 */
#define COPY_TRANSPOSE(src,dst) {					\
    (dst)[0] = (src)[0];						\
    (dst)[1] = (src)[4];						\
    (dst)[2] = (src)[8];						\
    (dst)[3] = (src)[12];						\
    (dst)[4] = (src)[1];						\
    (dst)[5] = (src)[5];						\
    (dst)[6] = (src)[9];						\
    (dst)[7] = (src)[13];						\
    (dst)[8] = (src)[2];						\
    (dst)[9] = (src)[6];						\
    (dst)[10] = (src)[10];						\
    (dst)[11] = (src)[14];						\
    (dst)[12] = (src)[3];						\
    (dst)[13] = (src)[7];						\
    (dst)[14] = (src)[11];						\
    (dst)[15] = (src)[15];						\
}


/*
 * These match the constants in GeometryRetained
 */

#define GEO_TYPE_NONE                   javax_media_j3d_GeometryRetained_GEO_TYPE_NONE               
#define GEO_TYPE_QUAD_SET               javax_media_j3d_GeometryRetained_GEO_TYPE_QUAD_SET
#define GEO_TYPE_TRI_SET                javax_media_j3d_GeometryRetained_GEO_TYPE_TRI_SET
#define GEO_TYPE_POINT_SET              javax_media_j3d_GeometryRetained_GEO_TYPE_POINT_SET
#define GEO_TYPE_LINE_SET               javax_media_j3d_GeometryRetained_GEO_TYPE_LINE_SET
#define GEO_TYPE_TRI_STRIP_SET          javax_media_j3d_GeometryRetained_GEO_TYPE_TRI_STRIP_SET
#define GEO_TYPE_TRI_FAN_SET            javax_media_j3d_GeometryRetained_GEO_TYPE_TRI_FAN_SET
#define GEO_TYPE_LINE_STRIP_SET         javax_media_j3d_GeometryRetained_GEO_TYPE_LINE_STRIP_SET
#define GEO_TYPE_INDEXED_QUAD_SET       javax_media_j3d_GeometryRetained_GEO_TYPE_INDEXED_QUAD_SET
#define GEO_TYPE_INDEXED_TRI_SET        javax_media_j3d_GeometryRetained_GEO_TYPE_INDEXED_TRI_SET
#define GEO_TYPE_INDEXED_POINT_SET      javax_media_j3d_GeometryRetained_GEO_TYPE_INDEXED_POINT_SET
#define GEO_TYPE_INDEXED_LINE_SET       javax_media_j3d_GeometryRetained_GEO_TYPE_INDEXED_LINE_SET
#define GEO_TYPE_INDEXED_TRI_STRIP_SET  javax_media_j3d_GeometryRetained_GEO_TYPE_INDEXED_TRI_STRIP_SET
#define GEO_TYPE_INDEXED_TRI_FAN_SET    javax_media_j3d_GeometryRetained_GEO_TYPE_INDEXED_TRI_FAN_SET
#define GEO_TYPE_INDEXED_LINE_STRIP_SET javax_media_j3d_GeometryRetained_GEO_TYPE_INDEXED_LINE_STRIP_SET
#define GEO_TYPE_RASTER                 javax_media_j3d_GeometryRetained_GEO_TYPE_RASTER
#define GEO_TYPE_TEXT3D                 javax_media_j3d_GeometryRetained_GEO_TYPE_TEXT3D
#define GEO_TYPE_COMPRESSED             javax_media_j3d_GeometryRetained_GEO_TYPE_COMPRESSED
#define GEO_TYPE_TOTAL                  javax_media_j3d_GeometryRetained_GEO_TYPE_TOTAL

/* 
 * These match the constants in ImageComponent   
 */ 
 
#define FORMAT_RGB          javax_media_j3d_ImageComponent_FORMAT_RGB
#define FORMAT_RGBA         javax_media_j3d_ImageComponent_FORMAT_RGBA
#define FORMAT_RGB5         javax_media_j3d_ImageComponent_FORMAT_RGB5 
#define FORMAT_RGB5_A1      javax_media_j3d_ImageComponent_FORMAT_RGB5_A1
#define FORMAT_RGB4         javax_media_j3d_ImageComponent_FORMAT_RGB4
#define FORMAT_RGBA4        javax_media_j3d_ImageComponent_FORMAT_RGBA4
#define FORMAT_LUM4_ALPHA4  javax_media_j3d_ImageComponent_FORMAT_LUM4_ALPHA4
#define FORMAT_LUM8_ALPHA8  javax_media_j3d_ImageComponent_FORMAT_LUM8_ALPHA8
#define FORMAT_R3_G3_B2     javax_media_j3d_ImageComponent_FORMAT_R3_G3_B2
#define FORMAT_CHANNEL8     javax_media_j3d_ImageComponent_FORMAT_CHANNEL8


/* now the imagecomponent formats are reduced the ones below */
#define FORMAT_BYTE_RGBA    javax_media_j3d_ImageComponentRetained_BYTE_RGBA
#define FORMAT_BYTE_ABGR    javax_media_j3d_ImageComponentRetained_BYTE_ABGR
#define FORMAT_BYTE_GRAY    javax_media_j3d_ImageComponentRetained_BYTE_GRAY
#define FORMAT_USHORT_GRAY  javax_media_j3d_ImageComponentRetained_USHORT_GRAY
#define FORMAT_BYTE_LA      javax_media_j3d_ImageComponentRetained_BYTE_LA
#define FORMAT_BYTE_BGR     javax_media_j3d_ImageComponentRetained_BYTE_BGR
#define FORMAT_BYTE_RGB     javax_media_j3d_ImageComponentRetained_BYTE_RGB


/* These match the definitions in GeometryArray.java */
/* These have a GA prefix to avoid confusion with TEXTURE_COORDINATE_2 above */
#define GA_COORDINATES		 javax_media_j3d_GeometryArray_COORDINATES
#define GA_NORMALS 		 javax_media_j3d_GeometryArray_NORMALS 
#define GA_COLOR 		 javax_media_j3d_GeometryArray_COLOR
#define GA_WITH_ALPHA            javax_media_j3d_GeometryArray_WITH_ALPHA       
#define GA_TEXTURE_COORDINATE_2  javax_media_j3d_GeometryArray_TEXTURE_COORDINATE_2	
#define GA_TEXTURE_COORDINATE_3  javax_media_j3d_GeometryArray_TEXTURE_COORDINATE_3	
#define GA_TEXTURE_COORDINATE_4  javax_media_j3d_GeometryArray_TEXTURE_COORDINATE_4	
#define GA_TEXTURE_COORDINATE    javax_media_j3d_GeometryArray_TEXTURE_COORDINATE
#define GA_VERTEX_ATTRIBUTES     javax_media_j3d_GeometryArray_VERTEX_ATTRIBUTES
#define GA_BY_REFERENCE          javax_media_j3d_GeometryArray_BY_REFERENCE
				
				

/*
 * These match the constants in NativeConfigTemplate3D
 */

#define RED_SIZE	javax_media_j3d_NativeConfigTemplate3D_RED_SIZE
#define GREEN_SIZE	javax_media_j3d_NativeConfigTemplate3D_GREEN_SIZE
#define BLUE_SIZE	javax_media_j3d_NativeConfigTemplate3D_BLUE_SIZE
#define ALPHA_SIZE	javax_media_j3d_NativeConfigTemplate3D_ALPHA_SIZE
#define ACCUM_BUFFER	javax_media_j3d_NativeConfigTemplate3D_ACCUM_BUFFER
#define DEPTH_SIZE	javax_media_j3d_NativeConfigTemplate3D_DEPTH_SIZE
                                           /* this maps to GLX_ACCUM_RED,  */
                                           /* GLX_ACCUM_GREEN and          */
                                           /* GLX_ACCUM_BLUE so NUM_ITEMS  */
                                           /* must be incremented by 3 for */
                                           /* this attribute.              */
#define DOUBLEBUFFER	javax_media_j3d_NativeConfigTemplate3D_DOUBLEBUFFER
#define STEREO		javax_media_j3d_NativeConfigTemplate3D_STEREO
#define ANTIALIASING	javax_media_j3d_NativeConfigTemplate3D_ANTIALIASING


/* set this to the number of indices (from above) */
#define NUM_ITEMS	(javax_media_j3d_NativeConfigTemplate3D_NUM_ITEMS + 2)
                          /* total + 2 beacause of       */
                          /*  DEPTH_SIZE                 */

/* values for "enum" entries for GraphicsConfiguration */
#define REQUIRED			1
#define PREFERRED			2
#define UNNECESSARY			3



#define INTENSITY        javax_media_j3d_Texture_INTENSITY
#define LUMINANCE        javax_media_j3d_Texture_LUMINANCE
#define ALPHA            javax_media_j3d_Texture_ALPHA
#define LUMINANCE_ALPHA  javax_media_j3d_Texture_LUMINANCE_ALPHA
#define J3D_RGB          javax_media_j3d_Texture_RGB
#define J3D_RGBA         javax_media_j3d_Texture_RGBA

#ifndef D3D
#if defined(UNIX)
extern void APIENTRY glBlendColor (GLclampf, GLclampf, GLclampf, GLclampf);
extern void APIENTRY glBlendColorEXT (GLclampf, GLclampf, GLclampf, GLclampf);
extern void APIENTRY glColorTable (GLenum, GLenum, GLsizei, GLenum, GLenum, const GLvoid *);
extern void APIENTRY glColorTableSGI (GLenum, GLenum, GLsizei, GLenum, GLenum, const GLvoid *);
extern void APIENTRY glGetColorTableParameterivSGI (GLenum, GLenum, GLint *);
extern void APIENTRY glGetColorTableParameterfv (GLenum, GLenum, GLfloat *);
extern void APIENTRY glMultiDrawArraysEXT (GLenum, GLint *, GLsizei *, GLsizei);
extern void APIENTRY glMultiDrawArraysSUN (GLenum, GLint *, GLsizei *, GLsizei);
extern void APIENTRY glMultiDrawElementsEXT (GLenum, GLsizei *, GLenum, const GLvoid**, GLsizei);
extern void APIENTRY glMultiDrawElementsSUN (GLenum, GLsizei *, GLenum, const GLvoid**, GLsizei);
extern void APIENTRY glLockArraysEXT (GLint first, GLsizei count);
extern void APIENTRY glUnlockArraysEXT (void);


extern void APIENTRY glClientActiveTextureARB (GLenum);
extern void APIENTRY glMultiTexCoord2fvARB (GLenum, const GLfloat *);
extern void APIENTRY glMultiTexCoord3fvARB (GLenum, const GLfloat *);
extern void APIENTRY glMultiTexCoord4fvARB (GLenum, const GLfloat *);
extern void APIENTRY glGlobalAlphaFactorfSUN (GLfloat);
extern void APIENTRY glLoadTransposeMatrixdARB (const GLdouble *);
extern void APIENTRY glMultTransposeMatrixdARB (const GLdouble *);
extern void APIENTRY glActiveTextureARB (GLenum);
extern void APIENTRY glSharpenTexFuncSGIS(GLenum, GLsizei, const GLfloat *);
extern void APIENTRY glDetailTexFuncSGIS(GLenum, GLsizei, const GLfloat *);
extern void APIENTRY glTexFilterFuncSGIS(GLenum, GLenum, GLsizei, const GLfloat *);
extern void APIENTRY glCombinerInputNV (GLenum, GLenum, GLenum, GLenum, GLenum, GLenum);
extern void APIENTRY glCombinerOutputNV (GLenum, GLenum, GLenum, GLenum, GLenum, GLenum, GLenum, GLboolean, GLboolean, GLboolean);
extern void APIENTRY glFinalCombinerInputNV (GLenum, GLenum, GLenum, GLenum);
extern void APIENTRY glCombinerParameterfvNV (GLenum, const GLfloat *);
extern void APIENTRY glCombinerParameterivNV (GLenum, const GLint *);
extern void APIENTRY glCombinerParameterfNV (GLenum, GLfloat);
extern void APIENTRY glCombinerParameteriNV (GLenum, GLint);

extern void APIENTRY glTexImage3DEXT (GLenum, GLint, GLenum, GLsizei, GLsizei, GLsizei, GLint, GLenum, GLenum, const GLvoid *);
extern void APIENTRY glTexSubImage3DEXT (GLenum, GLint, GLint, GLint, GLint, GLsizei, GLsizei, GLsizei, GLenum, GLenum, const GLvoid *);



#ifndef GLX_SUN_video_resize
#define GLX_SUN_video_resize 1
extern int glXVideoResizeSUN( Display *, GLXDrawable, float);
#endif

#endif /* UNIX_ */

#ifndef APIENTRY
#define APIENTRY
#endif

/* define function prototypes */
typedef void (APIENTRY * MYPFNGLBLENDCOLORPROC) (GLclampf red, GLclampf green,
						GLclampf blue, GLclampf alpha);
typedef void (APIENTRY * MYPFNGLBLENDCOLOREXTPROC) (GLclampf red, GLclampf green,
						    GLclampf blue, GLclampf alpha);
typedef void (APIENTRY * MYPFNGLCOLORTABLEPROC) (GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const GLvoid *table);
typedef void (APIENTRY * MYPFNGLGETCOLORTABLEPARAMETERIVPROC) (GLenum target, GLenum pname, GLint *params);
typedef void (APIENTRY * MYPFNGLGETCOLORTABLEPROC) (GLenum target, GLenum format, GLenum type, GLvoid *table);
typedef void (APIENTRY * MYPFNGLCLIENTACTIVETEXTUREARBPROC) (GLenum texture);
typedef void (APIENTRY * MYPFNGLMULTIDRAWARRAYSEXTPROC) (GLenum mode, GLint *first, GLsizei *count, GLsizei primcount);
typedef void (APIENTRY * MYPFNGLMULTIDRAWELEMENTSEXTPROC) (GLenum, GLsizei *, GLenum, const GLvoid**, GLsizei);
typedef void (APIENTRY * MYPFNGLLOCKARRAYSEXTPROC) (GLint first, GLsizei count);
typedef void (APIENTRY * MYPFNGLUNLOCKARRAYSEXTPROC) (void);

typedef void (APIENTRY * MYPFNGLMULTITEXCOORD2FVARBPROC) (GLenum target, const GLfloat *v);
typedef void (APIENTRY * MYPFNGLMULTITEXCOORD3FVARBPROC) (GLenum target, const GLfloat *v);
typedef void (APIENTRY * MYPFNGLMULTITEXCOORD4FVARBPROC) (GLenum target, const GLfloat *v);
typedef void (APIENTRY * MYPFNGLLOADTRANSPOSEMATRIXDARBPROC) (const GLdouble *m);
typedef void (APIENTRY * MYPFNGLMULTTRANSPOSEMATRIXDARBPROC) (const GLdouble *m);
typedef void (APIENTRY * MYPFNGLACTIVETEXTUREARBPROC) (GLenum texture);
typedef void (APIENTRY * MYPFNGLTEXIMAGE3DPROC) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
typedef void (APIENTRY * MYPFNGLTEXSUBIMAGE3DPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid *pixels);

typedef void (APIENTRY * MYPFNGLGLOBALALPHAFACTORFSUNPROC) (GLfloat factor);

typedef void (APIENTRY * MYPFNGLCOMBINERINPUTNV) (GLenum stage, GLenum portion, GLenum variable, GLenum input, GLenum mapping, GLenum componentUsage);
typedef void (APIENTRY * MYPFNGLFINALCOMBINERINPUTNV) (GLenum variable, GLenum input, GLenum mapping, GLenum componentUsage);
typedef void (APIENTRY * MYPFNGLCOMBINEROUTPUTNV) (GLenum stage, GLenum portion, GLenum abOutput, GLenum cdOutput, GLenum sumOutput, GLenum scale, GLenum bias, GLboolean abDotProduct, GLboolean cdDotProduct, GLboolean muxSum);
typedef void (APIENTRY * MYPFNGLCOMBINERPARAMETERFVNV) (GLenum pname, const GLfloat *params);
typedef void (APIENTRY * MYPFNGLCOMBINERPARAMETERIVNV) (GLenum pname, const GLint *params);
typedef void (APIENTRY * MYPFNGLCOMBINERPARAMETERFNV) (GLenum pname, GLfloat param);
typedef void (APIENTRY * MYPFNGLCOMBINERPARAMETERINV) (GLenum pname, GLint param);
typedef void (APIENTRY * MYPFNGLSHARPENTEXFUNCSGI) (GLenum target, GLsizei n, const GLfloat *points);
typedef void (APIENTRY * MYPFNGLDETAILTEXFUNCSGI) (GLenum target, GLsizei n, const GLfloat *points);
typedef void (APIENTRY * MYPFNGLTEXFILTERFUNCSGI) (GLenum target, GLenum filter, GLsizei n, const GLfloat *points);

#if defined(UNIX)
typedef GLXFBConfig * (APIENTRY * MYPFNGLXCHOOSEFBCONFIG) (Display *dpy, int screen, const int *attrib_list, int *nelements);
typedef int (APIENTRY * MYPFNGLXVIDEORESIZESUN) (Display * dpy, GLXDrawable draw, float factor);
#endif /* UNIX_ */


/* define the structure to hold the properties of graphics context */
typedef struct {
    jlong context;

    /* version and extension info */
    char *versionStr;
    char *vendorStr;
    char *rendererStr;
    char *extensionStr;
    int versionNumbers[2];
	 
    /* both in 1.2 core part and 1.1 extensions */
    /* GL_EXT_rescale_normal or GL_RESCALE_NORMAL */ 
    jboolean rescale_normal_ext;
    GLenum rescale_normal_ext_enum;

    /* GL_BGR_EXT or GL_BGR */
    jboolean bgr_ext;
    GLenum bgr_ext_enum;
	 
    /* GL_EXT_texture3D or GL_TEXTURE3D */
    jboolean texture3DAvailable;
    GLenum texture_3D_ext_enum;
    GLenum texture_wrap_r_ext_enum;

    /* GL_ARB_imaging subset */
    /* GL_EXT_blend_color or GL_BLEND_COLOR */
    jboolean blend_color_ext;
    GLenum blendFunctionTable[8]; 
    /* GL_SGI_color_table or GL_COLOR_TABLE */
    jboolean color_table_ext;

    /* GL_EXT_separate_specular_color */
    jboolean seperate_specular_color;
    GLenum light_model_color_control_enum;
    GLenum single_color_enum;
    GLenum seperate_specular_color_enum;

    /* GL_CLAMP_TO_EDGE or GL_EXT_texture_edge_clamp or 
				GL_SGIS_texture_edge_clamp */
    GLenum texture_clamp_to_edge_enum;


    /* GL_SGIS_texture_lod */
    jboolean textureLodAvailable;
    GLenum texture_min_lod_enum;
    GLenum texture_max_lod_enum;
    GLenum texture_base_level_enum;
    GLenum texture_max_level_enum;


    /* ***********1.1 extension or 1.2 extensions ********************/

    /* GL_ARB_texture_border_clamp or GL_SGIS_texture_border_clamp */
    GLenum texture_clamp_to_border_enum;

    /* GL_SUN_multi_draw_arrays */
    jboolean multi_draw_arrays_sun;

    /* GLX_SUN_video_resize */
    jboolean videoResizeAvailable;
    
    /* GL_SUN_global_alpha */
    jboolean global_alpha_sun;
    /* GL_SUNX_constant_data */
    jboolean constant_data_sun;
	 
    /* GL_EXT_abgr */
    jboolean abgr_ext;
    /* GL_EXT_multi_draw_arrays */
    jboolean multi_draw_arrays_ext;

    /* GL_EXT_compiled_vertex_array */
    jboolean compiled_vertex_array_ext;

    /* GL_ARB_transpose_matrix */
    jboolean arb_transpose_matrix;

    /* GL_ARB_multitexture */
    jboolean arb_multitexture;
    int textureUnitCount;

    /* GL_SGI_texture_color_table */
    jboolean textureColorTableAvailable;
    int textureColorTableSize;

    /* GL_ARB_texture_env_combine */
    /* GL_EXT_texture_env_combine */
    jboolean textureEnvCombineAvailable;
    jboolean textureCombineDot3Available;
    jboolean textureCombineSubtractAvailable;

    /* GL_NV_register_combiners */
    jboolean textureRegisterCombinersAvailable;
    GLenum currentTextureUnit;
    GLenum currentCombinerUnit;
 
    /* save the enum for the combine modes since the enums between
       ARB & EXT might be different.
     */
    GLenum combine_enum;
    GLenum combine_add_signed_enum;
    GLenum combine_interpolate_enum;
    GLenum combine_subtract_enum;
    GLenum combine_dot3_rgb_enum;
    GLenum combine_dot3_rgba_enum;

    /* GL_ARB_texture_cube_map */
    /* GL_EXT_texture_cube_map */
    jboolean textureCubeMapAvailable;
    GLenum texture_cube_map_ext_enum;

    /* GL_ARB_mulitsample */
    jboolean arb_multisample; 

    /*
      By default, full scene antialiasing is disable if
      multisampling pixel format (or visual) is chosen.
      To honor display driver multisampling antialiasing
      setting (e.g. force scene antialiasing), set the 
      implicit multisample flag to true in this case. 
      This cause Java3D not to invoke any native 
      multisampling API to enable/disable scene antialiasing. 
    */    
    jboolean implicit_multisample;    
    
    /* by MIK OF CLASSX */
    /*
      Used by transparentOffScreen feature.
      This is the value of the alpha channel
      of the background.
    */  
    GLfloat alphaClearValue;

    /* GL_SGIS_sharpen_texture */
    jboolean textureSharpenAvailable;
    GLenum linear_sharpen_enum;
    GLenum linear_sharpen_rgb_enum;
    GLenum linear_sharpen_alpha_enum;

    /* GL_SGIS_detail_texture */
    jboolean textureDetailAvailable;
    GLenum texture_detail_ext_enum;
    GLenum linear_detail_enum;
    GLenum linear_detail_rgb_enum;
    GLenum linear_detail_alpha_enum;
    GLenum texture_detail_mode_enum;
    GLenum texture_detail_level_enum;

    /* GL_SGIS_texture_filter4 */
    jboolean textureFilter4Available;
    GLenum filter4_enum;

    /* GL_EXT_texture_filter_anisotropic */
    jboolean textureAnisotropicFilterAvailable;
    GLenum texture_filter_anisotropic_ext_enum;
    GLenum max_texture_filter_anisotropy_enum;

    /* GL_SGIX_texture_lod_bias */
    jboolean textureLodBiasAvailable;

    /* extension mask */
    jint extMask;
    jint textureExtMask;

    /* shader language  support */
    jboolean  shadingLanguageGLSL;
    jboolean  shadingLanguageCg;
    
    /* function pointers */
    MYPFNGLBLENDCOLORPROC glBlendColor;
    MYPFNGLBLENDCOLOREXTPROC glBlendColorEXT;
    MYPFNGLCOLORTABLEPROC glColorTable;    
    MYPFNGLGETCOLORTABLEPARAMETERIVPROC glGetColorTableParameteriv;
    MYPFNGLTEXIMAGE3DPROC               glTexImage3DEXT;
    MYPFNGLTEXSUBIMAGE3DPROC               glTexSubImage3DEXT;
    MYPFNGLCLIENTACTIVETEXTUREARBPROC glClientActiveTextureARB;
    MYPFNGLACTIVETEXTUREARBPROC glActiveTextureARB; 
    MYPFNGLMULTIDRAWARRAYSEXTPROC  glMultiDrawArraysEXT;
    MYPFNGLMULTIDRAWELEMENTSEXTPROC  glMultiDrawElementsEXT;
    MYPFNGLLOCKARRAYSEXTPROC  glLockArraysEXT;
    MYPFNGLUNLOCKARRAYSEXTPROC  glUnlockArraysEXT;
    MYPFNGLMULTITEXCOORD2FVARBPROC glMultiTexCoord2fvARB;
    MYPFNGLMULTITEXCOORD3FVARBPROC glMultiTexCoord3fvARB;
    MYPFNGLMULTITEXCOORD4FVARBPROC glMultiTexCoord4fvARB;
    MYPFNGLLOADTRANSPOSEMATRIXDARBPROC glLoadTransposeMatrixdARB;
    MYPFNGLMULTTRANSPOSEMATRIXDARBPROC glMultTransposeMatrixdARB;
    MYPFNGLGLOBALALPHAFACTORFSUNPROC glGlobalAlphaFactorfSUN;

    MYPFNGLCOMBINERINPUTNV glCombinerInputNV;
    MYPFNGLCOMBINEROUTPUTNV glCombinerOutputNV;
    MYPFNGLFINALCOMBINERINPUTNV glFinalCombinerInputNV;
    MYPFNGLCOMBINERPARAMETERFVNV glCombinerParameterfvNV;
    MYPFNGLCOMBINERPARAMETERIVNV glCombinerParameterivNV;
    MYPFNGLCOMBINERPARAMETERFNV glCombinerParameterfNV;
    MYPFNGLCOMBINERPARAMETERINV glCombinerParameteriNV;

    MYPFNGLSHARPENTEXFUNCSGI glSharpenTexFuncSGIS;
    MYPFNGLDETAILTEXFUNCSGI glDetailTexFuncSGIS;
    MYPFNGLTEXFILTERFUNCSGI glTexFilterFuncSGIS;

    /* Programmable Shader */
    PFNGLATTACHOBJECTARBPROC pfnglAttachObjectARB;
    PFNGLCOMPILESHADERARBPROC pfnglCompileShaderARB;
    PFNGLCREATEPROGRAMOBJECTARBPROC pfnglCreateProgramObjectARB;
    PFNGLCREATESHADEROBJECTARBPROC pfnglCreateShaderObjectARB;
    PFNGLDELETEOBJECTARBPROC pfnglglDeleteObjectARB;
    PFNGLGETINFOLOGARBPROC pfnglGetInfoLogARB;
    PFNGLGETOBJECTPARAMETERIVARBPROC pfnglGetObjectParameterivARB;
    PFNGLLINKPROGRAMARBPROC pfnglLinkProgramARB;
    PFNGLSHADERSOURCEARBPROC pfnglShaderSourceARB;
    PFNGLUSEPROGRAMOBJECTARBPROC pfnglUseProgramObjectARB;
    PFNGLGETUNIFORMLOCATIONARBPROC pfnglGetUniformLocationARB;
    PFNGLGETATTRIBLOCATIONARBPROC pfnglGetAttribLocationARB;
    PFNGLBINDATTRIBLOCATIONARBPROC pfnglBindAttribLocationARB;
    PFNGLVERTEXATTRIB3FVARBPROC pfnglVertexAttrib3fvARB;
    PFNGLUNIFORM1IARBPROC pfnglUniform1iARB;
    PFNGLUNIFORM1FARBPROC pfnglUniform1fARB;
    PFNGLUNIFORM2IARBPROC pfnglUniform2iARB;
    PFNGLUNIFORM2FARBPROC pfnglUniform2fARB;
    PFNGLUNIFORM3IARBPROC pfnglUniform3iARB;
    PFNGLUNIFORM3FARBPROC pfnglUniform3fARB;
    PFNGLUNIFORM4IARBPROC pfnglUniform4iARB;
    PFNGLUNIFORM4FARBPROC pfnglUniform4fARB; 

    
#if defined(UNIX)
    MYPFNGLXVIDEORESIZESUN glXVideoResizeSUN;
#endif /* UNIX_ */

} GraphicsContextPropertiesInfo;


#ifdef WIN32

/* define the structure to hold the info. of a pixel format */
typedef struct PixelFormatInfoRec PixelFormatInfo;

struct PixelFormatInfoRec {
    /* Information about onScreen pixel format */
    int onScreenPFormat;        /* PixelFormat for onScreen */
    GLboolean  onScreenHasMultisample;  /* TRUE if WGL_SAMPLE_BUFFERS_ARB is TRUE and
					   WGL_SAMPLES_ARB > 1 */
    GLboolean  onScreenHasStereo;
    GLboolean  onScreenHasDoubleBuffer;
    GLboolean  onScreenHasAccum;
    /* Information about onScreen pixel format */
    int offScreenPFormat;       /* PixelFormat for offScreen */
    GLboolean  offScreenHasMultisample;  /* TRUE if WGL_SAMPLE_BUFFERS_ARB is TRUE and
					    WGL_SAMPLES_ARB > 1 */
    GLboolean  offScreenHasStereo;
    GLboolean  offScreenHasDoubleBuffer;
    GLboolean  offScreenHasAccum;
    GLboolean  drawToPbuffer;   /* value of DRAW_TO_PBUFFER attr for offScreenPFormat */
    
    /* Information about extension support */
    char* supportedExtensions;  /* list of supported ARB extensions */
    GLboolean  supportARB;	/* TRUE if wgl*PixelFormat*ARB functions supported */
    GLboolean  supportPbuffer;  /* TRUE if wgl*Pbuffer functions supported */
    
    
    /* handle to ARB functions */
    PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB; 
    PFNWGLGETPIXELFORMATATTRIBIVARBPROC wglGetPixelFormatAttribivARB;
    PFNWGLCREATEPBUFFERARBPROC  wglCreatePbufferARB;
    PFNWGLGETPBUFFERDCARBPROC wglGetPbufferDCARB;
    PFNWGLRELEASEPBUFFERDCARBPROC wglReleasePbufferDCARB;
    PFNWGLDESTROYPBUFFERARBPROC wglDestroyPbufferARB;
    PFNWGLQUERYPBUFFERARBPROC wglQueryPbufferARB;
};

#endif /* WIN32 */

/* define the structure to hold the info. of a offScreen buffer */
typedef struct OffScreenBufferInfoRec OffScreenBufferInfo;

struct OffScreenBufferInfoRec {
    GLboolean isPbuffer; /* GL_TRUE if Pbuffer is used. */

#if defined(UNIX)
#endif
    
#ifdef WIN32
    HPBUFFERARB hpbuf;  /* Handle to the Pbuffer */
#endif /* WIN32 */

};


#endif /* D3D */
#endif /* _Java3D_gldefs_h_ */
