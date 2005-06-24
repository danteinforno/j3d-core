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


/*
 * Class:     javax_media_j3d_GLSLShaderProgramRetained
 * Method:    lookupNativeShaderAttrName
 * Signature: (JJLjava/lang/String;[J)Ljavax/media/j3d/ShaderError;
 */
JNIEXPORT jobject JNICALL
Java_javax_media_j3d_GLSLShaderProgramRetained_lookupNativeShaderAttrName(
    JNIEnv *env,
    jobject obj,
    jlong ctxInfo,
    jlong shaderProgramId,
    jstring attrName,
    jlongArray locArr)
{
    GraphicsContextPropertiesInfo* ctxProperties =  (GraphicsContextPropertiesInfo* )ctxInfo;
    jobject shaderError = NULL;
    GLcharARB *attrNameString = (GLcharARB *)strJavaToC(env, attrName);
    jlong *locPtr;
    jlong loc;
    jclass oom;

    JNIEnv table = *env;

    if (attrNameString == NULL) {
	/* Just return, since strJavaToC will throw OOM if it returns NULL */
	return NULL;
    }
    
    locPtr = (*env)->GetLongArrayElements(env, locArr, NULL);

    /*
    fprintf(stderr,
	    "GLSLShaderProgramRetained.lookupNativeShaderAttrName: %s\n",
	    attrNameString);
    */

    /*
     * Get uniform attribute location
     */
    loc = ctxProperties->pfnglGetUniformLocationARB((GLhandleARB)shaderProgramId,
						    attrNameString);
    
    if (loc == -1) {
	char *msgStr = "Attribute name lookup failed: ";
	char *errMsg = (char*)malloc(strlen(msgStr) + strlen(attrNameString) + 1);
	if (errMsg == NULL) {
	    if ((oom = table->FindClass(env, "java/lang/OutOfMemoryError")) != NULL) {
		table->ThrowNew(env, oom, "malloc");
	    }
	    return NULL;
	}
	strcpy(errMsg, msgStr);
	strcat(errMsg, attrNameString);

	shaderError = createShaderError(env,
		javax_media_j3d_ShaderError_SHADER_ATTRIBUTE_LOOKUP_ERROR,
		errMsg,
		NULL);
	free(errMsg);
    }

    /*
    fprintf(stderr,
	    "str = %s, loc = %d\n",
	    attrNameString, loc);
    */

    locPtr[0] = loc;
    
    free(attrNameString);    

    (*env)->ReleaseLongArrayElements(env, locArr, locPtr, 0);

    return shaderError;

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

#if 0


/*
 * Class:     javax_media_j3d_GLSLShaderProgramRetained
 * Method:    setUniform1iArray
 * Signature: (JJJI[I)Ljavax/media/j3d/ShaderError;
 */
JNIEXPORT jobject JNICALL Java_javax_media_j3d_GLSLShaderProgramRetained_setUniform1iArray
  (JNIEnv *, jobject, jlong, jlong, jlong, jint, jintArray);


/*
 * Class:     javax_media_j3d_GLSLShaderProgramRetained
 * Method:    setUniform1fArray
 * Signature: (JJJI[F)Ljavax/media/j3d/ShaderError;
 */
JNIEXPORT jobject JNICALL Java_javax_media_j3d_GLSLShaderProgramRetained_setUniform1fArray
  (JNIEnv *, jobject, jlong, jlong, jlong, jint, jfloatArray);

/*
 * Class:     javax_media_j3d_GLSLShaderProgramRetained
 * Method:    setUniform2iArray
 * Signature: (JJJI[I)Ljavax/media/j3d/ShaderError;
 */
JNIEXPORT jobject JNICALL Java_javax_media_j3d_GLSLShaderProgramRetained_setUniform2iArray
  (JNIEnv *, jobject, jlong, jlong, jlong, jint, jintArray);

/*
 * Class:     javax_media_j3d_GLSLShaderProgramRetained
 * Method:    setUniform2fArray
 * Signature: (JJJI[F)Ljavax/media/j3d/ShaderError;
 */
JNIEXPORT jobject JNICALL Java_javax_media_j3d_GLSLShaderProgramRetained_setUniform2fArray
  (JNIEnv *, jobject, jlong, jlong, jlong, jint, jfloatArray);

/*
 * Class:     javax_media_j3d_GLSLShaderProgramRetained
 * Method:    setUniform3iArray
 * Signature: (JJJI[I)Ljavax/media/j3d/ShaderError;
 */
JNIEXPORT jobject JNICALL Java_javax_media_j3d_GLSLShaderProgramRetained_setUniform3iArray
  (JNIEnv *, jobject, jlong, jlong, jlong, jint, jintArray);

/*
 * Class:     javax_media_j3d_GLSLShaderProgramRetained
 * Method:    setUniform3fArray
 * Signature: (JJJI[F)Ljavax/media/j3d/ShaderError;
 */
JNIEXPORT jobject JNICALL Java_javax_media_j3d_GLSLShaderProgramRetained_setUniform3fArray
  (JNIEnv *, jobject, jlong, jlong, jlong, jint, jfloatArray);

/*
 * Class:     javax_media_j3d_GLSLShaderProgramRetained
 * Method:    setUniform4iArray
 * Signature: (JJJI[I)Ljavax/media/j3d/ShaderError;
 */
JNIEXPORT jobject JNICALL Java_javax_media_j3d_GLSLShaderProgramRetained_setUniform4iArray
  (JNIEnv *, jobject, jlong, jlong, jlong, jint, jintArray);

/*
 * Class:     javax_media_j3d_GLSLShaderProgramRetained
 * Method:    setUniform4fArray
 * Signature: (JJJI[F)Ljavax/media/j3d/ShaderError;
 */
JNIEXPORT jobject JNICALL Java_javax_media_j3d_GLSLShaderProgramRetained_setUniform4fArray
  (JNIEnv *, jobject, jlong, jlong, jlong, jint, jfloatArray);

/*
 * Class:     javax_media_j3d_GLSLShaderProgramRetained
 * Method:    setUniformMatrix3fArray
 * Signature: (JJJI[F)Ljavax/media/j3d/ShaderError;
 */
JNIEXPORT jobject JNICALL Java_javax_media_j3d_GLSLShaderProgramRetained_setUniformMatrix3fArray
  (JNIEnv *, jobject, jlong, jlong, jlong, jint, jfloatArray);

/*
 * Class:     javax_media_j3d_GLSLShaderProgramRetained
 * Method:    setUniformMatrix4fArray
 * Signature: (JJJI[F)Ljavax/media/j3d/ShaderError;
 */
JNIEXPORT jobject JNICALL Java_javax_media_j3d_GLSLShaderProgramRetained_setUniformMatrix4fArray
  (JNIEnv *, jobject, jlong, jlong, jlong, jint, jfloatArray);

#endif
