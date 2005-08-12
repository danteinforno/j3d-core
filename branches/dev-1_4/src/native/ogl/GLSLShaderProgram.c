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

#if defined(LINUX)
#define _GNU_SOURCE 1
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <jni.h>

#include "gldefs.h"

#if defined(UNIX)
#include <dlfcn.h>
#endif

#define TYPE_INTEGER javax_media_j3d_ShaderAttributeObjectRetained_TYPE_INTEGER
#define TYPE_FLOAT javax_media_j3d_ShaderAttributeObjectRetained_TYPE_FLOAT
#define TYPE_TUPLE2I javax_media_j3d_ShaderAttributeObjectRetained_TYPE_TUPLE2I
#define TYPE_TUPLE2F javax_media_j3d_ShaderAttributeObjectRetained_TYPE_TUPLE2F
#define TYPE_TUPLE3I javax_media_j3d_ShaderAttributeObjectRetained_TYPE_TUPLE3I
#define TYPE_TUPLE3F javax_media_j3d_ShaderAttributeObjectRetained_TYPE_TUPLE3F
#define TYPE_TUPLE4I javax_media_j3d_ShaderAttributeObjectRetained_TYPE_TUPLE4I
#define TYPE_TUPLE4F javax_media_j3d_ShaderAttributeObjectRetained_TYPE_TUPLE4F
#define TYPE_MATRIX3F javax_media_j3d_ShaderAttributeObjectRetained_TYPE_MATRIX3F
#define TYPE_MATRIX4F javax_media_j3d_ShaderAttributeObjectRetained_TYPE_MATRIX4F


extern char *strJavaToC(JNIEnv *env, jstring str);
extern jobject createShaderError(JNIEnv *env,
				 int errorCode,
				 const char *errorMsg,
				 const char *detailMsg);

extern int isExtensionSupported(const char *allExtensions, const char *extension);


/*
 * Called by getPropertiesFromCurrentContext to initialize the GLSL
 * shader function pointers and set the flag indicating whether GLSL
 * shaders are available.
 */
void
checkGLSLShaderExtensions(
    JNIEnv *env,
    jobject obj,
    char *tmpExtensionStr,
    GraphicsContextPropertiesInfo *ctxInfo,
    jboolean glslLibraryAvailable)
{

    if (glslLibraryAvailable &&
	isExtensionSupported(tmpExtensionStr, "GL_ARB_shader_objects") &&
	isExtensionSupported(tmpExtensionStr, "GL_ARB_shading_language_100")) {

#if defined(UNIX)
	ctxInfo->pfnglAttachObjectARB =
	    (PFNGLATTACHOBJECTARBPROC)dlsym(RTLD_DEFAULT, "glAttachObjectARB");
	ctxInfo->pfnglCompileShaderARB =
	    (PFNGLCOMPILESHADERARBPROC)dlsym(RTLD_DEFAULT, "glCompileShaderARB");
	ctxInfo->pfnglCreateProgramObjectARB =
	    (PFNGLCREATEPROGRAMOBJECTARBPROC)dlsym(RTLD_DEFAULT, "glCreateProgramObjectARB");
	ctxInfo->pfnglCreateShaderObjectARB =
	    (PFNGLCREATESHADEROBJECTARBPROC)dlsym(RTLD_DEFAULT, "glCreateShaderObjectARB");
	ctxInfo->pfnglglDeleteObjectARB =
	    (PFNGLDELETEOBJECTARBPROC)dlsym(RTLD_DEFAULT, "glDeleteObjectARB");
	ctxInfo->pfnglGetInfoLogARB =
	    (PFNGLGETINFOLOGARBPROC)dlsym(RTLD_DEFAULT, "glGetInfoLogARB");
	ctxInfo->pfnglGetObjectParameterivARB =
	    (PFNGLGETOBJECTPARAMETERIVARBPROC)dlsym(RTLD_DEFAULT, "glGetObjectParameterivARB");
	ctxInfo->pfnglLinkProgramARB =
	    (PFNGLLINKPROGRAMARBPROC)dlsym(RTLD_DEFAULT, "glLinkProgramARB");
	ctxInfo->pfnglShaderSourceARB =
	    (PFNGLSHADERSOURCEARBPROC)dlsym(RTLD_DEFAULT, "glShaderSourceARB");
	ctxInfo->pfnglUseProgramObjectARB =
	    (PFNGLUSEPROGRAMOBJECTARBPROC)dlsym(RTLD_DEFAULT, "glUseProgramObjectARB");
	ctxInfo->pfnglGetUniformLocationARB =
	    (PFNGLGETUNIFORMLOCATIONARBPROC)dlsym(RTLD_DEFAULT, "glGetUniformLocationARB");
	ctxInfo->pfnglGetAttribLocationARB =
	    (PFNGLGETATTRIBLOCATIONARBPROC)dlsym(RTLD_DEFAULT, "glGetAttribLocationARB");
	ctxInfo->pfnglBindAttribLocationARB =
	    (PFNGLBINDATTRIBLOCATIONARBPROC)dlsym(RTLD_DEFAULT, "glBindAttribLocationARB");
	ctxInfo->pfnglVertexAttrib3fvARB =
	    (PFNGLVERTEXATTRIB3FVARBPROC)dlsym(RTLD_DEFAULT, "glVertexAttrib3fvARB");
	ctxInfo->pfnglGetActiveUniformARB =
	    (PFNGLGETACTIVEUNIFORMARBPROC)dlsym(RTLD_DEFAULT, "glGetActiveUniformARB");
	ctxInfo->pfnglUniform1iARB =
	    (PFNGLUNIFORM1IARBPROC)dlsym(RTLD_DEFAULT, "glUniform1iARB");
	ctxInfo->pfnglUniform1fARB =
	    (PFNGLUNIFORM1FARBPROC)dlsym(RTLD_DEFAULT, "glUniform1fARB");
	ctxInfo->pfnglUniform2iARB =
	    (PFNGLUNIFORM2IARBPROC)dlsym(RTLD_DEFAULT, "glUniform2iARB");
	ctxInfo->pfnglUniform2fARB =
	    (PFNGLUNIFORM2FARBPROC)dlsym(RTLD_DEFAULT, "glUniform2fARB");
	ctxInfo->pfnglUniform3iARB =
	    (PFNGLUNIFORM3IARBPROC)dlsym(RTLD_DEFAULT, "glUniform3iARB");
	ctxInfo->pfnglUniform3fARB =
	    (PFNGLUNIFORM3FARBPROC)dlsym(RTLD_DEFAULT, "glUniform3fARB");
	ctxInfo->pfnglUniform4iARB =
	    (PFNGLUNIFORM4IARBPROC)dlsym(RTLD_DEFAULT, "glUniform4iARB");
	ctxInfo->pfnglUniform4fARB =
	    (PFNGLUNIFORM4FARBPROC)dlsym(RTLD_DEFAULT, "glUniform4fARB");
	ctxInfo->pfnglUniform1ivARB =
	    (PFNGLUNIFORM1IVARBPROC)dlsym(RTLD_DEFAULT, "glUniform1ivARB");
	ctxInfo->pfnglUniform1fvARB =
	    (PFNGLUNIFORM1FVARBPROC)dlsym(RTLD_DEFAULT, "glUniform1fvARB");
	ctxInfo->pfnglUniform2ivARB =
	    (PFNGLUNIFORM2IVARBPROC)dlsym(RTLD_DEFAULT, "glUniform2ivARB");
	ctxInfo->pfnglUniform2fvARB =
	    (PFNGLUNIFORM2FVARBPROC)dlsym(RTLD_DEFAULT, "glUniform2fvARB");
	ctxInfo->pfnglUniform3ivARB =
	    (PFNGLUNIFORM3IVARBPROC)dlsym(RTLD_DEFAULT, "glUniform3ivARB");
	ctxInfo->pfnglUniform3fvARB =
	    (PFNGLUNIFORM3FVARBPROC)dlsym(RTLD_DEFAULT, "glUniform3fvARB");
	ctxInfo->pfnglUniform4ivARB =
	    (PFNGLUNIFORM4IVARBPROC)dlsym(RTLD_DEFAULT, "glUniform4ivARB");
	ctxInfo->pfnglUniform4fvARB =
	    (PFNGLUNIFORM4FVARBPROC)dlsym(RTLD_DEFAULT, "glUniform4fvARB");
	ctxInfo->pfnglUniformMatrix3fvARB =
	    (PFNGLUNIFORMMATRIX3FVARBPROC)dlsym(RTLD_DEFAULT, "glUniformMatrix3fvARB");
	ctxInfo->pfnglUniformMatrix4fvARB =
	    (PFNGLUNIFORMMATRIX4FVARBPROC)dlsym(RTLD_DEFAULT, "glUniformMatrix4fvARB");
#endif
#ifdef WIN32
	ctxInfo->pfnglAttachObjectARB =
	    (PFNGLATTACHOBJECTARBPROC)wglGetProcAddress("glAttachObjectARB");
	ctxInfo->pfnglCompileShaderARB =
	    (PFNGLCOMPILESHADERARBPROC)wglGetProcAddress("glCompileShaderARB");
	ctxInfo->pfnglCreateProgramObjectARB =
	    (PFNGLCREATEPROGRAMOBJECTARBPROC)wglGetProcAddress("glCreateProgramObjectARB");
	ctxInfo->pfnglCreateShaderObjectARB =
	    (PFNGLCREATESHADEROBJECTARBPROC)wglGetProcAddress("glCreateShaderObjectARB");
	ctxInfo->pfnglglDeleteObjectARB =
	    (PFNGLDELETEOBJECTARBPROC)wglGetProcAddress("glDeleteObjectARB");
	ctxInfo->pfnglGetInfoLogARB =
	    (PFNGLGETINFOLOGARBPROC)wglGetProcAddress("glGetInfoLogARB");
	ctxInfo->pfnglGetObjectParameterivARB =
	    (PFNGLGETOBJECTPARAMETERIVARBPROC)wglGetProcAddress("glGetObjectParameterivARB");
	ctxInfo->pfnglLinkProgramARB =
	    (PFNGLLINKPROGRAMARBPROC)wglGetProcAddress("glLinkProgramARB");
	ctxInfo->pfnglShaderSourceARB =
	    (PFNGLSHADERSOURCEARBPROC)wglGetProcAddress("glShaderSourceARB");
	ctxInfo->pfnglUseProgramObjectARB =
	    (PFNGLUSEPROGRAMOBJECTARBPROC)wglGetProcAddress("glUseProgramObjectARB");
	ctxInfo->pfnglGetUniformLocationARB =
	    (PFNGLGETUNIFORMLOCATIONARBPROC)wglGetProcAddress("glGetUniformLocationARB");
	ctxInfo->pfnglGetAttribLocationARB =
	    (PFNGLGETATTRIBLOCATIONARBPROC)wglGetProcAddress("glGetAttribLocationARB");
	ctxInfo->pfnglBindAttribLocationARB =
	    (PFNGLBINDATTRIBLOCATIONARBPROC)wglGetProcAddress("glBindAttribLocationARB");
	ctxInfo->pfnglVertexAttrib3fvARB =
	    (PFNGLVERTEXATTRIB3FVARBPROC)wglGetProcAddress("glVertexAttrib3fvARB");
	ctxInfo->pfnglGetActiveUniformARB =
	    (PFNGLGETACTIVEUNIFORMARBPROC)wglGetProcAddress("glGetActiveUniformARB");
	ctxInfo->pfnglUniform1iARB =
	    (PFNGLUNIFORM1IARBPROC)wglGetProcAddress("glUniform1iARB");
	ctxInfo->pfnglUniform1fARB =
	    (PFNGLUNIFORM1FARBPROC)wglGetProcAddress("glUniform1fARB");
	ctxInfo->pfnglUniform2iARB =
	    (PFNGLUNIFORM2IARBPROC)wglGetProcAddress("glUniform2iARB");
	ctxInfo->pfnglUniform2fARB =
	    (PFNGLUNIFORM2FARBPROC)wglGetProcAddress("glUniform2fARB");
	ctxInfo->pfnglUniform3iARB =
	    (PFNGLUNIFORM3IARBPROC)wglGetProcAddress("glUniform3iARB");
	ctxInfo->pfnglUniform3fARB =
	    (PFNGLUNIFORM3FARBPROC)wglGetProcAddress("glUniform3fARB");
	ctxInfo->pfnglUniform4iARB =
	    (PFNGLUNIFORM4IARBPROC)wglGetProcAddress("glUniform4iARB");
	ctxInfo->pfnglUniform4fARB =
	    (PFNGLUNIFORM4FARBPROC)wglGetProcAddress("glUniform4fARB");
	ctxInfo->pfnglUniform1ivARB =
	    (PFNGLUNIFORM1IVARBPROC)wglGetProcAddress("glUniform1ivARB");
	ctxInfo->pfnglUniform1fvARB =
	    (PFNGLUNIFORM1FVARBPROC)wglGetProcAddress("glUniform1fvARB");
	ctxInfo->pfnglUniform2ivARB =
	    (PFNGLUNIFORM2IVARBPROC)wglGetProcAddress("glUniform2ivARB");
	ctxInfo->pfnglUniform2fvARB =
	    (PFNGLUNIFORM2FVARBPROC)wglGetProcAddress("glUniform2fvARB");
	ctxInfo->pfnglUniform3ivARB =
	    (PFNGLUNIFORM3IVARBPROC)wglGetProcAddress("glUniform3ivARB");
	ctxInfo->pfnglUniform3fvARB =
	    (PFNGLUNIFORM3FVARBPROC)wglGetProcAddress("glUniform3fvARB");
	ctxInfo->pfnglUniform4ivARB =
	    (PFNGLUNIFORM4IVARBPROC)wglGetProcAddress("glUniform4ivARB");
	ctxInfo->pfnglUniform4fvARB =
	    (PFNGLUNIFORM4FVARBPROC)wglGetProcAddress("glUniform4fvARB");
	ctxInfo->pfnglUniformMatrix3fvARB =
	    (PFNGLUNIFORMMATRIX3FVARBPROC)wglGetProcAddress("glUniformMatrix3fvARB");
	ctxInfo->pfnglUniformMatrix4fvARB =
	    (PFNGLUNIFORMMATRIX4FVARBPROC)wglGetProcAddress("glUniformMatrix4fvARB");
#endif
	
    }

    if (ctxInfo->pfnglCreateShaderObjectARB == NULL) {
	/*fprintf(stderr, "Java 3D : GLSLShader extension not available\n");*/
	ctxInfo->shadingLanguageGLSL = JNI_FALSE;	
	
    }
    else {
	/*fprintf(stderr, "Java 3D : GLSLShader extension is  available\n");*/
	ctxInfo->shadingLanguageGLSL = JNI_TRUE;	
    }

}


/*
 * Return the info log as a string. This is used as the detail message
 * for a ShaderError.
 */
static const char *
getInfoLog(
    GraphicsContextPropertiesInfo* ctxProperties,
    GLhandleARB obj)
{
    int infoLogLength = 0;
    int len = 0;
    GLcharARB *infoLog = NULL;

    static const char *allocMsg =
	"Java 3D ERROR: could not allocate infoLog buffer\n";

    ctxProperties->pfnglGetObjectParameterivARB(obj,
						GL_OBJECT_INFO_LOG_LENGTH_ARB,
						&infoLogLength);
    if (infoLogLength > 0) {
	infoLog = (GLcharARB *)malloc(infoLogLength);
	if (infoLog == NULL) {
	    return allocMsg;
	}

	ctxProperties->pfnglGetInfoLogARB(obj, infoLogLength, &len, infoLog);
    }

    return infoLog;
}


/*
 * Class:     javax_media_j3d_GLSLShaderProgramRetained
 * Method:    createNativeShader
 * Signature: (JI[J)Ljavax/media/j3d/ShaderError;
 */
JNIEXPORT jobject JNICALL
Java_javax_media_j3d_GLSLShaderProgramRetained_createNativeShader(
    JNIEnv *env,
    jobject obj,
    jlong ctxInfo,
    jint  shaderType,
    jlongArray shaderIdArray)
{

    jlong *shaderIdPtr;
    GLhandleARB shaderHandle = 0;
    
    GraphicsContextPropertiesInfo* ctxProperties =  (GraphicsContextPropertiesInfo* )ctxInfo;
    jobject shaderError = NULL;

    shaderIdPtr = (*env)->GetLongArrayElements(env, shaderIdArray, NULL);

    /* Process  shader */
    /*
    fprintf(stderr, "    shaderType == %d\n", shaderType);
    */
    if (shaderType == javax_media_j3d_Shader_SHADER_TYPE_VERTEX) { 
	/* create the vertex shader */
	shaderHandle = ctxProperties->pfnglCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
    }
    else if (shaderType == javax_media_j3d_Shader_SHADER_TYPE_FRAGMENT) { 
	    /* create the fragment shader */
	shaderHandle = ctxProperties->pfnglCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
    }
    
    if (shaderHandle == 0) {
	shaderError = createShaderError(env,
					javax_media_j3d_ShaderError_COMPILE_ERROR,
					"Unable to create native shader object",
					NULL);
    }

    shaderIdPtr[0] = (jlong) shaderHandle;
    (*env)->ReleaseLongArrayElements(env, shaderIdArray, shaderIdPtr, 0); 

    return shaderError;
}


/*
 * Class:     javax_media_j3d_GLSLShaderProgramRetained
 * Method:    destroyNativeShader
 * Signature: (JJ)Ljavax/media/j3d/ShaderError;
 */
JNIEXPORT jobject JNICALL
Java_javax_media_j3d_GLSLShaderProgramRetained_destroyNativeShader(
    JNIEnv *env,
    jobject obj,
    jlong ctxInfo,
    jlong shaderId)
{
    GraphicsContextPropertiesInfo* ctxProperties =  (GraphicsContextPropertiesInfo* )ctxInfo;

    ctxProperties->pfnglglDeleteObjectARB( (GLhandleARB) shaderId);
    
    return NULL;
}

/*
 * Class:     javax_media_j3d_GLSLShaderProgramRetained
 * Method:    compileNativeShader
 * Signature: (JJLjava/lang/String;)Ljavax/media/j3d/ShaderError;
 */
JNIEXPORT jobject JNICALL
Java_javax_media_j3d_GLSLShaderProgramRetained_compileNativeShader(
    JNIEnv *env,
    jobject obj,
    jlong ctxInfo,
    jlong shaderId,
    jstring program)
{    
    GLint status;
    
    GraphicsContextPropertiesInfo* ctxProperties =  (GraphicsContextPropertiesInfo* )ctxInfo;
    jobject shaderError = NULL;

    /* Null-terminated "C" strings */
    GLcharARB *shaderString = NULL;
    const GLcharARB *shaderStringArr[1];

    shaderString = (GLcharARB *)strJavaToC(env, program);
    if (shaderString == NULL) {	
	/* Just return, since strJavaToC will throw OOM if it returns NULL */
	return NULL;
    }

    shaderStringArr[0] = shaderString;
    ctxProperties->pfnglShaderSourceARB((GLhandleARB)shaderId, 1, shaderStringArr, NULL);
    ctxProperties->pfnglCompileShaderARB((GLhandleARB)shaderId);
    ctxProperties->pfnglGetObjectParameterivARB((GLhandleARB)shaderId,
						GL_OBJECT_COMPILE_STATUS_ARB,
						&status);
    if (!status) {
	const char *detailMsg = getInfoLog(ctxProperties, (GLhandleARB)shaderId);

	shaderError = createShaderError(env,
					javax_media_j3d_ShaderError_COMPILE_ERROR,
					"GLSL shader compile error",
					detailMsg);
    }

    free(shaderString);    
    return shaderError;
}


/*
 * Class:     javax_media_j3d_GLSLShaderProgramRetained
 * Method:    createNativeShaderProgram
 * Signature: (J[J)Ljavax/media/j3d/ShaderError;
 */
JNIEXPORT jobject JNICALL
Java_javax_media_j3d_GLSLShaderProgramRetained_createNativeShaderProgram(
    JNIEnv *env,
    jobject obj,
    jlong ctxInfo,
    jlongArray shaderProgramIdArray)    
{

    jlong *shaderProgramIdPtr;
    GLhandleARB shaderProgramHandle;
    jobject shaderError = NULL;

    GraphicsContextPropertiesInfo* ctxProperties =  (GraphicsContextPropertiesInfo* )ctxInfo;
    shaderProgramIdPtr = (*env)->GetLongArrayElements(env, shaderProgramIdArray, NULL);

    shaderProgramHandle = ctxProperties->pfnglCreateProgramObjectARB();

    if (shaderProgramHandle == 0) {
	shaderError = createShaderError(env,
					javax_media_j3d_ShaderError_LINK_ERROR,
					"Unable to create native shader program object",
					NULL);
    }

    shaderProgramIdPtr[0] = (jlong) shaderProgramHandle;
    (*env)->ReleaseLongArrayElements(env, shaderProgramIdArray, shaderProgramIdPtr, 0);
    
    return shaderError;
}

/*
 * Class:     javax_media_j3d_GLSLShaderProgramRetained
 * Method:    destroyNativeShaderProgram
 * Signature: (JJ)Ljavax/media/j3d/ShaderError;
 */
JNIEXPORT jobject JNICALL
Java_javax_media_j3d_GLSLShaderProgramRetained_destroyNativeShaderProgram(
    JNIEnv *env,
    jobject obj,
    jlong ctxInfo,
    jlong shaderProgramId)
{
    GraphicsContextPropertiesInfo* ctxProperties =  (GraphicsContextPropertiesInfo* )ctxInfo;

    ctxProperties->pfnglglDeleteObjectARB((GLhandleARB)shaderProgramId);

    return NULL;
}

/*
 * Class:     javax_media_j3d_GLSLShaderProgramRetained
 * Method:    linkNativeShaderProgram
 * Signature: (JJ[J)Ljavax/media/j3d/ShaderError;
 */
JNIEXPORT jobject JNICALL
Java_javax_media_j3d_GLSLShaderProgramRetained_linkNativeShaderProgram(
    JNIEnv *env,
    jobject obj,
    jlong ctxInfo,
    jlong shaderProgramId,
    jlongArray shaderIdArray)
{
    GLint status;
    int i;
    
    GraphicsContextPropertiesInfo* ctxProperties =  (GraphicsContextPropertiesInfo* )ctxInfo;
    jlong *shaderIdPtr = (*env)->GetLongArrayElements(env, shaderIdArray, NULL);
    jsize shaderIdArrayLength = (*env)->GetArrayLength(env,  shaderIdArray);
    jobject shaderError = NULL;

    /*
    fprintf(stderr, "linkShaderProgram: shaderIdArrayLength %d\n", shaderIdArrayLength);
    */
    
    for(i=0; i<shaderIdArrayLength; i++) {
	ctxProperties->pfnglAttachObjectARB((GLhandleARB)shaderProgramId,
					    (GLhandleARB)shaderIdPtr[i]);
    }

    ctxProperties->pfnglLinkProgramARB((GLhandleARB)shaderProgramId);
    ctxProperties->pfnglGetObjectParameterivARB((GLhandleARB)shaderProgramId,
						GL_OBJECT_LINK_STATUS_ARB,
						&status);

    if (!status) {
	const char *detailMsg = getInfoLog(ctxProperties, (GLhandleARB)shaderProgramId);

	shaderError = createShaderError(env,
					javax_media_j3d_ShaderError_LINK_ERROR,
					"GLSL shader program link error",
					detailMsg);
    }

    (*env)->ReleaseLongArrayElements(env, shaderIdArray, shaderIdPtr, JNI_ABORT); 

    return shaderError;
}


/*
 * Class:     javax_media_j3d_GLSLShaderProgramRetained
 * Method:    bindNativeVertexAttrName
 * Signature: (JJLjava/lang/String;I)Ljavax/media/j3d/ShaderError;
 */
JNIEXPORT jobject JNICALL
Java_javax_media_j3d_GLSLShaderProgramRetained_bindNativeVertexAttrName(
    JNIEnv * env,
    jobject obj,
    jlong ctxInfo,
    jlong shaderProgramId,
    jstring attrName,
    jint attrIndex)
{
    GraphicsContextPropertiesInfo* ctxProperties =  (GraphicsContextPropertiesInfo* )ctxInfo;
    jobject shaderError = NULL;
    GLcharARB *attrNameString = (GLcharARB *)strJavaToC(env, attrName);

    /*
    fprintf(stderr,
	    "GLSLShaderProgramRetained.bindNativeVertexAttrName: %s\n",
	    attrNameString);
    */

    ctxProperties->pfnglBindAttribLocationARB((GLhandleARB)shaderProgramId,
					      attrIndex + 1,
					      attrNameString);

    /* No error checking needed, so just return */

    return shaderError;
}


static jint
glslToJ3dType(GLint type)
{
    switch (type) {
    case GL_BOOL_ARB:
    case GL_INT:
	return TYPE_INTEGER;

    case GL_FLOAT:
	return TYPE_FLOAT;

    case GL_INT_VEC2_ARB:
    case GL_BOOL_VEC2_ARB:
	return TYPE_TUPLE2I;

    case GL_FLOAT_VEC2_ARB:
	return TYPE_TUPLE2F;

    case GL_INT_VEC3_ARB:
    case GL_BOOL_VEC3_ARB:
	return TYPE_TUPLE3I;

    case GL_FLOAT_VEC3_ARB:
	return TYPE_TUPLE3F;

    case GL_INT_VEC4_ARB:
    case GL_BOOL_VEC4_ARB:
	return TYPE_TUPLE4I;

    case GL_FLOAT_VEC4_ARB:
	return TYPE_TUPLE4F;

    /* case GL_FLOAT_MAT2_ARB: */

    case GL_FLOAT_MAT3_ARB:
	return TYPE_MATRIX3F;

    case GL_FLOAT_MAT4_ARB:
	return TYPE_MATRIX4F;
    }

    return -1;
}


/*
 * Class:     javax_media_j3d_GLSLShaderProgramRetained
 * Method:    lookupNativeShaderAttrNames
 * Signature: (JJI[Ljava/lang/String;[J[I[I[Z)V
 */
JNIEXPORT void JNICALL
Java_javax_media_j3d_GLSLShaderProgramRetained_lookupNativeShaderAttrNames(
    JNIEnv *env,
    jobject obj,
    jlong ctxInfo,
    jlong shaderProgramId,
    jint numAttrNames,
    jobjectArray attrNames,
    jlongArray locArr,
    jintArray typeArr,
    jintArray sizeArr,
    jbooleanArray isArrayArr)
{
    GraphicsContextPropertiesInfo* ctxProperties =  (GraphicsContextPropertiesInfo* )ctxInfo;
    GLcharARB **attrNamesString;
    jlong *locPtr;
    jint *typePtr;
    jint *sizePtr;
    jboolean *isArrayPtr;
    GLint loc;
    GLenum type;
    GLint size;
    GLcharARB *name;
    GLint maxStrLen;
    int numActiveUniforms;
    int i, j;
    
    JNIEnv table = *env;

    locPtr = (*env)->GetLongArrayElements(env, locArr, NULL);
    typePtr = (*env)->GetIntArrayElements(env, typeArr, NULL);
    sizePtr = (*env)->GetIntArrayElements(env, sizeArr, NULL);
    isArrayPtr = (*env)->GetBooleanArrayElements(env, isArrayArr, NULL);

    /*
     * Initialize the name array, also set the loc, type, and size
     * arrays to out-of-band values
     */
    attrNamesString = (GLcharARB **)malloc(numAttrNames * sizeof(GLcharARB *));
    for (i = 0; i < numAttrNames; i++) {
	jstring attrName;

        attrName = (*env)->GetObjectArrayElement(env, attrNames, i);
        attrNamesString[i] = (GLcharARB *)strJavaToC(env, attrName);

	locPtr[i] = -1;
	typePtr[i] = -1;
	sizePtr[i] = -1;
    }

    /*
     * Loop through the list of active uniform variables, one at a
     * time, searching for a match in the attrNames array.
     *
     * NOTE: Since attrNames isn't sorted, and we don't have a
     * hashtable of names to index locations, we will do a
     * brute-force, linear search of the array. This leads to an
     * O(n^2) algorithm (actually O(n*m) where n is attrNames.length
     * and m is the number of uniform variables), but since we expect
     * N to be small, we will not optimize this at this time.
     */
    ctxProperties->pfnglGetObjectParameterivARB((GLhandleARB) shaderProgramId,
						GL_OBJECT_ACTIVE_UNIFORMS_ARB,
						&numActiveUniforms);
    ctxProperties->pfnglGetObjectParameterivARB((GLhandleARB) shaderProgramId,
						GL_OBJECT_ACTIVE_UNIFORM_MAX_LENGTH_ARB,
						&maxStrLen);
    name = malloc(maxStrLen + 1);

    /*
    fprintf(stderr,
	    "numActiveUniforms = %d, maxStrLen = %d\n",
	    numActiveUniforms, maxStrLen);
    */

    for (i = 0; i < numActiveUniforms; i++) {
	ctxProperties->pfnglGetActiveUniformARB((GLhandleARB) shaderProgramId,
						i,
						maxStrLen,
						NULL,
						&size,
						&type,
						name);
	/*
	fprintf(stderr,
		"Uniform[%d] : name = %s, type = %d, size = %d\n",
		i, name, type, size);
	*/

	/* Now try to find the name */
	for (j = 0; j < numAttrNames; j++) {
	    if (strcmp(attrNamesString[j], name) == 0) {
		sizePtr[j] = (jint)size;
                isArrayPtr[j] = (size > 1);
		typePtr[j] = glslToJ3dType(type);
		break;
	    }
	}
    }

    free(name);

    /* Now lookup the location of each name in the attrNames array */
    for (i = 0; i < numAttrNames; i++) {
        /*
         * Get uniform attribute location
         */
        loc = ctxProperties->pfnglGetUniformLocationARB((GLhandleARB)shaderProgramId,
                                                        attrNamesString[i]);

	/*
        fprintf(stderr,
                "str = %s, loc = %d\n",
                attrNamesString[i], loc);
	*/

        locPtr[i] = (jlong)loc;
    }

    /* Free the array of strings */
    for (i = 0; i < numAttrNames; i++) {
        free(attrNamesString[i]);
    }
    free(attrNamesString);

    /* Release JNI arrays */
    (*env)->ReleaseLongArrayElements(env, locArr, locPtr, 0);
    (*env)->ReleaseIntArrayElements(env, typeArr, typePtr, 0);
    (*env)->ReleaseIntArrayElements(env, sizeArr, sizePtr, 0);
    (*env)->ReleaseBooleanArrayElements(env, isArrayArr, isArrayPtr, 0);
}


/*
 * Class:     javax_media_j3d_GLSLShaderProgramRetained
 * Method:    useShaderProgram
 * Signature: (JI)Ljavax/media/j3d/ShaderError;
 */
JNIEXPORT jobject
JNICALL Java_javax_media_j3d_GLSLShaderProgramRetained_useShaderProgram(
    JNIEnv *env,
    jobject obj,
    jlong ctxInfo,
    jlong shaderProgramId)
{
    GraphicsContextPropertiesInfo* ctxProperties =  (GraphicsContextPropertiesInfo* )ctxInfo;

    ctxProperties->pfnglUseProgramObjectARB((GLhandleARB)shaderProgramId);

    return NULL;
}

/*
 * Class:     javax_media_j3d_GLSLShaderProgramRetained
 * Method:    setUniform1i
 * Signature: (JJJI)Ljavax/media/j3d/ShaderError;
 */
JNIEXPORT jobject
JNICALL Java_javax_media_j3d_GLSLShaderProgramRetained_setUniform1i(
    JNIEnv *env,
    jobject obj,
    jlong ctxInfo,
    jlong shaderProgramId,
    jlong location,
    jint value)
{
    /* We do not need to use shaderProgramId because caller has already called
       useShaderProgram(). */

    GraphicsContextPropertiesInfo* ctxProperties =  (GraphicsContextPropertiesInfo* )ctxInfo;

    /* Load attribute */
    ctxProperties->pfnglUniform1iARB((GLint)location, value);

    /* TODO : We need to handle ShaderError. */
    return NULL;
}

/*
 * Class:     javax_media_j3d_GLSLShaderProgramRetained
 * Method:    setUniform1f
 * Signature: (JJJF)Ljavax/media/j3d/ShaderError;
 */
JNIEXPORT jobject
JNICALL Java_javax_media_j3d_GLSLShaderProgramRetained_setUniform1f(
    JNIEnv *env,
    jobject obj,
    jlong ctxInfo,
    jlong shaderProgramId,
    jlong location,
    jfloat value)
{
    /* We do not need to use shaderProgramId because caller has already called
       useShaderProgram(). */
    
    GraphicsContextPropertiesInfo* ctxProperties =  (GraphicsContextPropertiesInfo* )ctxInfo;
    
    /* Load attribute */
    ctxProperties->pfnglUniform1fARB((GLint)location, value);

    /* TODO : We need to handle ShaderError. */
    return NULL;
}

/*
 * Class:     javax_media_j3d_GLSLShaderProgramRetained
 * Method:    setUniform2i
 * Signature: (JJJ[I)Ljavax/media/j3d/ShaderError;
 */
JNIEXPORT jobject
JNICALL Java_javax_media_j3d_GLSLShaderProgramRetained_setUniform2i(
    JNIEnv *env,
    jobject obj,
    jlong ctxInfo,
    jlong shaderProgramId,
    jlong location,
    jintArray varray)
{    
    /* We do not need to use shaderProgramId because caller has already called
       useShaderProgram(). */
    
    jint *values;
	
    GraphicsContextPropertiesInfo* ctxProperties =  (GraphicsContextPropertiesInfo* )ctxInfo;
    
    /* Get array values */
    values = (*env)->GetIntArrayElements(env, varray, NULL);

    /* Load attribute */
    ctxProperties->pfnglUniform2iARB((GLint)location, values[0], values[1]);

    /* Release array values */
    (*env)->ReleaseIntArrayElements(env, varray, values, JNI_ABORT);

    /* TODO : We need to handle ShaderError. */
    return NULL;

}


/*
 * Class:     javax_media_j3d_GLSLShaderProgramRetained
 * Method:    setUniform2f
 * Signature: (JJJ[F)Ljavax/media/j3d/ShaderError;
 */
JNIEXPORT jobject
JNICALL Java_javax_media_j3d_GLSLShaderProgramRetained_setUniform2f(
    JNIEnv *env,
    jobject obj,
    jlong ctxInfo,
    jlong shaderProgramId,
    jlong location,
    jfloatArray varray)
{
    /* We do not need to use shaderProgramId because caller has already called
       useShaderProgram(). */

    jfloat *values;
    
    GraphicsContextPropertiesInfo* ctxProperties =  (GraphicsContextPropertiesInfo* )ctxInfo;

    /* Why shaderProgramId is not needed ? */
    
    /* Get array values */
    values = (*env)->GetFloatArrayElements(env, varray, NULL);

    /* Load attribute */
    ctxProperties->pfnglUniform2fARB((GLint)location, values[0], values[1]);

    /* Release array values */
    (*env)->ReleaseFloatArrayElements(env, varray, values, JNI_ABORT);

    /* TODO : We need to handle ShaderError. */
    return NULL;

}


/*
 * Class:     javax_media_j3d_GLSLShaderProgramRetained
 * Method:    setUniform3i
 * Signature: (JJJ[I)Ljavax/media/j3d/ShaderError;
 */
JNIEXPORT jobject
JNICALL Java_javax_media_j3d_GLSLShaderProgramRetained_setUniform3i(
    JNIEnv *env,
    jobject obj,
    jlong ctxInfo,
    jlong shaderProgramId,
    jlong location,
    jintArray varray)
{
    /* We do not need to use shaderProgramId because caller has already called
       useShaderProgram(). */

    jint *values;

    GraphicsContextPropertiesInfo* ctxProperties =  (GraphicsContextPropertiesInfo* )ctxInfo;

    /* Get array values */
    values = (*env)->GetIntArrayElements(env, varray, NULL);

    /* Load attribute */
    ctxProperties->pfnglUniform3iARB((GLint)location, values[0], values[1], values[2]);

    /* Release array values */
    (*env)->ReleaseIntArrayElements(env, varray, values, JNI_ABORT);

    /* TODO : We need to handle ShaderError. */
    return NULL;

}


/*
 * Class:     javax_media_j3d_GLSLShaderProgramRetained
 * Method:    setUniform3f
 * Signature: (JJJ[F)Ljavax/media/j3d/ShaderError;
 */
JNIEXPORT jobject
JNICALL Java_javax_media_j3d_GLSLShaderProgramRetained_setUniform3f(
    JNIEnv *env,
    jobject obj,
    jlong ctxInfo,
    jlong shaderProgramId,
    jlong location,
    jfloatArray varray)
{
    /* We do not need to use shaderProgramId because caller has already called
       useShaderProgram(). */

    jfloat *values;

    GraphicsContextPropertiesInfo* ctxProperties =  (GraphicsContextPropertiesInfo* )ctxInfo;
    
    /* Get array values */
    values = (*env)->GetFloatArrayElements(env, varray, NULL);

    /* Load attribute */
    ctxProperties->pfnglUniform3fARB((GLint)location, values[0], values[1], values[2]);

    /* Release array values */
    (*env)->ReleaseFloatArrayElements(env, varray, values, JNI_ABORT);
    
    /* TODO : We need to handle ShaderError. */
    return NULL;

}


/*
 * Class:     javax_media_j3d_GLSLShaderProgramRetained
 * Method:    setUniform4i
 * Signature: (JJJ[I)Ljavax/media/j3d/ShaderError;
 */
JNIEXPORT jobject
JNICALL Java_javax_media_j3d_GLSLShaderProgramRetained_setUniform4i(
    JNIEnv *env,
    jobject obj,
    jlong ctxInfo,
    jlong shaderProgramId,
    jlong location,
    jintArray varray)
{
    /* We do not need to use shaderProgramId because caller has already called
       useShaderProgram(). */

    jint *values;

    GraphicsContextPropertiesInfo* ctxProperties =  (GraphicsContextPropertiesInfo* )ctxInfo;    
    /* Get array values */
    values = (*env)->GetIntArrayElements(env, varray, NULL);

    /* Load attribute */
    ctxProperties->pfnglUniform4iARB((GLint)location, values[0], values[1], values[2], values[3]);

    /* Release array values */
    (*env)->ReleaseIntArrayElements(env, varray, values, JNI_ABORT);

    /* TODO : We need to handle ShaderError. */
    return NULL;
}


/*
 * Class:     javax_media_j3d_GLSLShaderProgramRetained
 * Method:    setUniform4f
 * Signature: (JJJ[F)Ljavax/media/j3d/ShaderError;
 */
JNIEXPORT jobject
JNICALL Java_javax_media_j3d_GLSLShaderProgramRetained_setUniform4f(
    JNIEnv *env,
    jobject obj,
    jlong ctxInfo,
    jlong shaderProgramId,
    jlong location,
    jfloatArray varray)
{
    /* We do not need to use shaderProgramId because caller has already called
       useShaderProgram(). */
    
    jfloat *values;

    GraphicsContextPropertiesInfo* ctxProperties =  (GraphicsContextPropertiesInfo* )ctxInfo;

    /* Get array values */
    values = (*env)->GetFloatArrayElements(env, varray, NULL);

    /* Load attribute */
    ctxProperties->pfnglUniform4fARB((GLint)location, values[0], values[1], values[2], values[3]);

    /* Release array values */
    (*env)->ReleaseFloatArrayElements(env, varray, values, JNI_ABORT);

    /* TODO : We need to handle ShaderError. */
    return NULL;

}

/*
 * Class:     javax_media_j3d_GLSLShaderProgramRetained
 * Method:    setUniformMatrix3f
 * Signature: (JJJ[F)Ljavax/media/j3d/ShaderError;
 */
JNIEXPORT jobject JNICALL Java_javax_media_j3d_GLSLShaderProgramRetained_setUniformMatrix3f(
    JNIEnv *env,
    jobject obj,
    jlong ctxInfo,
    jlong shaderProgramId,
    jlong location,
    jfloatArray varray)
{
    /* We do not need to use shaderProgramId because caller has already called
       useShaderProgram(). */
    
    jfloat *values;

    GraphicsContextPropertiesInfo* ctxProperties =  (GraphicsContextPropertiesInfo* )ctxInfo;

    /* Get array values */
    values = (*env)->GetFloatArrayElements(env, varray, NULL);

    /* Load attribute */
    /*  transpose is GL_TRUE : each matrix is supplied in row major order */
    ctxProperties->pfnglUniformMatrix3fvARB((GLint)location, 1, GL_TRUE, (GLfloat *)values);

    /* Release array values */
    (*env)->ReleaseFloatArrayElements(env, varray, values, JNI_ABORT);

    /* TODO : We need to handle ShaderError. */
    return NULL;
}

/*
 * Class:     javax_media_j3d_GLSLShaderProgramRetained
 * Method:    setUniformMatrix4f
 * Signature: (JJJ[F)Ljavax/media/j3d/ShaderError;
 */
JNIEXPORT jobject JNICALL Java_javax_media_j3d_GLSLShaderProgramRetained_setUniformMatrix4f(
    JNIEnv *env,
    jobject obj,
    jlong ctxInfo,
    jlong shaderProgramId,
    jlong location,
    jfloatArray varray)
{
    /* We do not need to use shaderProgramId because caller has already called
       useShaderProgram(). */
    
    jfloat *values;
    
    GraphicsContextPropertiesInfo* ctxProperties =  (GraphicsContextPropertiesInfo* )ctxInfo;

    /* Get array values */
    values = (*env)->GetFloatArrayElements(env, varray, NULL);
    
    /* Load attribute */
    /*  transpose is GL_TRUE : each matrix is supplied in row major order */
    ctxProperties->pfnglUniformMatrix4fvARB((GLint)location, 1, GL_TRUE, (GLfloat *)values);

    /* Release array values */
    (*env)->ReleaseFloatArrayElements(env, varray, values, JNI_ABORT);
    
    /* TODO : We need to handle ShaderError. */
    return NULL;
}

/*
 * Class:     javax_media_j3d_GLSLShaderProgramRetained
 * Method:    setUniform1iArray
 * Signature: (JJJI[I)Ljavax/media/j3d/ShaderError;
 */
JNIEXPORT jobject
JNICALL Java_javax_media_j3d_GLSLShaderProgramRetained_setUniform1iArray(
    JNIEnv *env,
    jobject obj,
    jlong ctxInfo,
    jlong shaderProgramId,
    jlong location,
    jint length,
    jintArray vArray)
{

    JNIEnv table = *env;
    jint *values;
    
    /* We do not need to use shaderProgramId because caller has already called
       useShaderProgram(). */

    GraphicsContextPropertiesInfo* ctxProperties =  (GraphicsContextPropertiesInfo* )ctxInfo;
    
    /* Get array values */
    values = (jint *)(*(table->GetPrimitiveArrayCritical))(env, vArray , NULL);
    
    /* Load attribute */
    ctxProperties->pfnglUniform1ivARB((GLint)location, length, values);

    /* Release array values */
    (*(table->ReleasePrimitiveArrayCritical))(env, vArray, values, 0);

    /* TODO : We need to handle ShaderError. */
    return NULL;

}


/*
 * Class:     javax_media_j3d_GLSLShaderProgramRetained
 * Method:    setUniform1fArray
 * Signature: (JJJI[F)Ljavax/media/j3d/ShaderError;
 */
JNIEXPORT jobject
JNICALL Java_javax_media_j3d_GLSLShaderProgramRetained_setUniform1fArray(
    JNIEnv *env,
    jobject obj,
    jlong ctxInfo,
    jlong shaderProgramId,
    jlong location,
    jint length,
    jfloatArray vArray)
{
    
    JNIEnv table = *env;
    jfloat *values;
    
    /* We do not need to use shaderProgramId because caller has already called
       useShaderProgram(). */
    
    GraphicsContextPropertiesInfo* ctxProperties =  (GraphicsContextPropertiesInfo* )ctxInfo;

    /* Get array values */
    values = (jfloat *)(*(table->GetPrimitiveArrayCritical))(env, vArray , NULL);

    /* Load attribute */
    ctxProperties->pfnglUniform1fvARB((GLint)location, length, values);

    /* Release array values */
    (*(table->ReleasePrimitiveArrayCritical))(env, vArray, values, 0);

    /* TODO : We need to handle ShaderError. */
    return NULL;
  
}

/*
 * Class:     javax_media_j3d_GLSLShaderProgramRetained
 * Method:    setUniform2iArray
 * Signature: (JJJI[I)Ljavax/media/j3d/ShaderError;
 */
JNIEXPORT jobject
JNICALL Java_javax_media_j3d_GLSLShaderProgramRetained_setUniform2iArray(
    JNIEnv *env,
    jobject obj,
    jlong ctxInfo,
    jlong shaderProgramId,
    jlong location,
    jint length,
    jintArray vArray)
{

    JNIEnv table = *env;
    jint *values;

    /* We do not need to use shaderProgramId because caller has already called
       useShaderProgram(). */

    GraphicsContextPropertiesInfo* ctxProperties =  (GraphicsContextPropertiesInfo* )ctxInfo;

    /* Get array values */
    values = (jint *)(*(table->GetPrimitiveArrayCritical))(env, vArray , NULL);

    /* Load attribute */
    ctxProperties->pfnglUniform2ivARB((GLint)location, length, values);

    /* Release array values */
    (*(table->ReleasePrimitiveArrayCritical))(env, vArray, values, 0);

    /* TODO : We need to handle ShaderError. */
    return NULL;

}

 

/*
 * Class:     javax_media_j3d_GLSLShaderProgramRetained
 * Method:    setUniform2fArray
 * Signature: (JJJI[F)Ljavax/media/j3d/ShaderError;
 */
JNIEXPORT jobject
JNICALL Java_javax_media_j3d_GLSLShaderProgramRetained_setUniform2fArray(
    JNIEnv *env,
    jobject obj,
    jlong ctxInfo,
    jlong shaderProgramId,
    jlong location,
    jint length,
    jfloatArray vArray)
{

    JNIEnv table = *env;
    jfloat *values;

    /* We do not need to use shaderProgramId because caller has already called
       useShaderProgram(). */

    GraphicsContextPropertiesInfo* ctxProperties =  (GraphicsContextPropertiesInfo* )ctxInfo;

    /* Get array values */
    values = (jfloat *)(*(table->GetPrimitiveArrayCritical))(env, vArray , NULL);

    /* Load attribute */
    ctxProperties->pfnglUniform2fvARB((GLint)location, length, values);

    /* Release array values */
    (*(table->ReleasePrimitiveArrayCritical))(env, vArray, values, 0);

    /* TODO : We need to handle ShaderError. */
    return NULL;

}


/*
 * Class:     javax_media_j3d_GLSLShaderProgramRetained
 * Method:    setUniform3iArray
 * Signature: (JJJI[I)Ljavax/media/j3d/ShaderError;
 */
JNIEXPORT jobject
JNICALL Java_javax_media_j3d_GLSLShaderProgramRetained_setUniform3iArray(
    JNIEnv *env,
    jobject obj,
    jlong ctxInfo,
    jlong shaderProgramId,
    jlong location,
    jint length,
    jintArray vArray)
{

    JNIEnv table = *env;
    jint *values;

    /* We do not need to use shaderProgramId because caller has already called
       useShaderProgram(). */

    GraphicsContextPropertiesInfo* ctxProperties =  (GraphicsContextPropertiesInfo* )ctxInfo;

    /* Get array values */
    values = (jint *)(*(table->GetPrimitiveArrayCritical))(env, vArray , NULL);

    /* Load attribute */
    ctxProperties->pfnglUniform3ivARB((GLint)location, length, values);

    /* Release array values */
    (*(table->ReleasePrimitiveArrayCritical))(env, vArray, values, 0);

    /* TODO : We need to handle ShaderError. */
    return NULL;

}


/*
 * Class:     javax_media_j3d_GLSLShaderProgramRetained
 * Method:    setUniform3fArray
 * Signature: (JJJI[F)Ljavax/media/j3d/ShaderError;
 */
JNIEXPORT jobject
JNICALL Java_javax_media_j3d_GLSLShaderProgramRetained_setUniform3fArray(
    JNIEnv *env,
    jobject obj,
    jlong ctxInfo,
    jlong shaderProgramId,
    jlong location,
    jint length,
    jfloatArray vArray)
{

    JNIEnv table = *env;
    jfloat *values;

    /* We do not need to use shaderProgramId because caller has already called
       useShaderProgram(). */

    GraphicsContextPropertiesInfo* ctxProperties =  (GraphicsContextPropertiesInfo* )ctxInfo;

    /* Get array values */
    values = (jfloat *)(*(table->GetPrimitiveArrayCritical))(env, vArray , NULL);

    /* Load attribute */
    ctxProperties->pfnglUniform3fvARB((GLint)location, length, values);

    /* Release array values */
    (*(table->ReleasePrimitiveArrayCritical))(env, vArray, values, 0);

    /* TODO : We need to handle ShaderError. */
    return NULL;

}


/*
 * Class:     javax_media_j3d_GLSLShaderProgramRetained
 * Method:    setUniform4iArray
 * Signature: (JJJI[I)Ljavax/media/j3d/ShaderError;
 */
JNIEXPORT jobject
JNICALL Java_javax_media_j3d_GLSLShaderProgramRetained_setUniform4iArray(
    JNIEnv *env,
    jobject obj,
    jlong ctxInfo,
    jlong shaderProgramId,
    jlong location,
    jint length,
    jintArray vArray)
{

    JNIEnv table = *env;
    jint *values;

    /* We do not need to use shaderProgramId because caller has already called
       useShaderProgram(). */

    GraphicsContextPropertiesInfo* ctxProperties =  (GraphicsContextPropertiesInfo* )ctxInfo;

    /* Get array values */
    values = (jint *)(*(table->GetPrimitiveArrayCritical))(env, vArray , NULL);

    /* Load attribute */
    ctxProperties->pfnglUniform4ivARB((GLint)location, length, values);

    /* Release array values */
    (*(table->ReleasePrimitiveArrayCritical))(env, vArray, values, 0);

    /* TODO : We need to handle ShaderError. */
    return NULL;

}


/*
 * Class:     javax_media_j3d_GLSLShaderProgramRetained
 * Method:    setUniform4fArray
 * Signature: (JJJI[F)Ljavax/media/j3d/ShaderError;
 */
JNIEXPORT jobject
JNICALL Java_javax_media_j3d_GLSLShaderProgramRetained_setUniform4fArray(
    JNIEnv *env,
    jobject obj,
    jlong ctxInfo,
    jlong shaderProgramId,
    jlong location,
    jint length,
    jfloatArray vArray)
{

    JNIEnv table = *env;
    jfloat *values;

    /* We do not need to use shaderProgramId because caller has already called
       useShaderProgram(). */

    GraphicsContextPropertiesInfo* ctxProperties =  (GraphicsContextPropertiesInfo* )ctxInfo;

    /* Get array values */
    values = (jfloat *)(*(table->GetPrimitiveArrayCritical))(env, vArray , NULL);

    /* Load attribute */
    ctxProperties->pfnglUniform4fvARB((GLint)location, length, values);

    /* Release array values */
    (*(table->ReleasePrimitiveArrayCritical))(env, vArray, values, 0);

    /* TODO : We need to handle ShaderError. */
    return NULL;

}


/*
 * Class:     javax_media_j3d_GLSLShaderProgramRetained
 * Method:    setUniformMatrix3fArray
 * Signature: (JJJI[F)Ljavax/media/j3d/ShaderError;
 */
JNIEXPORT jobject
JNICALL Java_javax_media_j3d_GLSLShaderProgramRetained_setUniformMatrix3fArray
(
    JNIEnv *env,
    jobject obj,
    jlong ctxInfo,
    jlong shaderProgramId,
    jlong location,
    jint length,
    jfloatArray vArray)
{

    JNIEnv table = *env;
    jfloat *values;

    /* We do not need to use shaderProgramId because caller has already called
       useShaderProgram(). */    

    GraphicsContextPropertiesInfo* ctxProperties =  (GraphicsContextPropertiesInfo* )ctxInfo;

    /* Get array values */
    values = (jfloat *)(*(table->GetPrimitiveArrayCritical))(env, vArray , NULL);

    /* Load attribute */
    /*  transpose is GL_TRUE : each matrix is supplied in row major order */
    ctxProperties->pfnglUniformMatrix3fvARB((GLint)location, length,
					    GL_TRUE, (GLfloat *)values);

    /* Release array values */
    (*(table->ReleasePrimitiveArrayCritical))(env, vArray, values, 0);

    /* TODO : We need to handle ShaderError. */
    return NULL;
}


/*
 * Class:     javax_media_j3d_GLSLShaderProgramRetained
 * Method:    setUniformMatrix4fArray
 * Signature: (JJJI[F)Ljavax/media/j3d/ShaderError;
 */
JNIEXPORT jobject
JNICALL Java_javax_media_j3d_GLSLShaderProgramRetained_setUniformMatrix4fArray
(
    JNIEnv *env,
    jobject obj,
    jlong ctxInfo,
    jlong shaderProgramId,
    jlong location,
    jint length,
    jfloatArray vArray)
{

    JNIEnv table = *env;
    jfloat *values;
    
    /* We do not need to use shaderProgramId because caller has already called
       useShaderProgram(). */    

    GraphicsContextPropertiesInfo* ctxProperties =  (GraphicsContextPropertiesInfo* )ctxInfo;

    /* Get array values */
    values = (jfloat *)(*(table->GetPrimitiveArrayCritical))(env, vArray , NULL);
    
    /* Load attribute */
    /*  transpose is GL_TRUE : each matrix is supplied in row major order */
    ctxProperties->pfnglUniformMatrix4fvARB((GLint)location, length,
					    GL_TRUE, (GLfloat *)values);

    /* Release array values */
    (*(table->ReleasePrimitiveArrayCritical))(env, vArray, values, 0);
    
    /* TODO : We need to handle ShaderError. */
    return NULL;
}
