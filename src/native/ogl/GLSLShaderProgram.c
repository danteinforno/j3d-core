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


extern char *strJavaToC(JNIEnv *env, jstring str);
extern jobject createShaderError(JNIEnv *env,
				 int errorCode,
				 const char *errorMsg,
				 const char *detailMsg);


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

    shaderString = (GLcharARB *)strJavaToC(env, program);
    if (shaderString == NULL) {	
	/* Just return, since strJavaToC will throw OOM if it returns NULL */
	return NULL;
    }

    ctxProperties->pfnglShaderSourceARB((GLhandleARB)shaderId, 1, &shaderString, NULL);
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

    fprintf(stderr,
	    "GLSLShaderProgramRetained.bindNativeVertexAttrName: %s\n",
	    attrNameString);

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

    fprintf(stderr,
	    "GLSLShaderProgramRetained.lookupNativeShaderAttrName: %s\n",
	    attrNameString);

    /* TODO Chien: implement this */

    shaderError = createShaderError(env,
				    javax_media_j3d_ShaderError_SHADER_ATTRIBUTE_ERROR,
				    "lookupNativeShaderAttrName is not implemented",
				    NULL);

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


/* KCR: BEGIN GLSL SHADER HACK */
JNIEXPORT void JNICALL
Java_javax_media_j3d_GLSLShaderProgramRetained_setUniform1i(
    JNIEnv *env,
    jobject obj,
    jlong ctxInfo,
    jint shaderProgram,
    jstring attrName,
    jint value)
{
    JNIEnv table = *env;
    
    GLcharARB *attrNameString = NULL; /* Null-terminated "C" string */
    GLint loc = -1;
    GLhandleARB glShaderProgram = (GLhandleARB)shaderProgram;

    GraphicsContextPropertiesInfo* ctxProperties =  (GraphicsContextPropertiesInfo* )ctxInfo;

    attrNameString = (GLcharARB *)strJavaToC(env, attrName);
    if (attrNameString == NULL) {
	return;
    }

    /*
     * Get uniform attribute location
     *
     * TODO: we need to separate the string lookup from the setting of
     * the value
     */
    loc = ctxProperties->pfnglGetUniformLocationARB(glShaderProgram, attrNameString);
    /*
    fprintf(stderr,
	    "str = %s, loc = %d, val = %d\n",
	    attrNameString, loc, value);
    */
    free(attrNameString);

    /* Load attribute */
    ctxProperties->pfnglUniform1iARB(loc, value);
}


JNIEXPORT void JNICALL
Java_javax_media_j3d_GLSLShaderProgramRetained_setUniform1f(
    JNIEnv *env,
    jobject obj,
    jlong ctxInfo,
    jint shaderProgram,
    jstring attrName,
    jfloat value)
{
    JNIEnv table = *env;

    GLcharARB *attrNameString = NULL; /* Null-terminated "C" string */
    GLint loc = -1;
    GLhandleARB glShaderProgram = (GLhandleARB)shaderProgram;

    GraphicsContextPropertiesInfo* ctxProperties =  (GraphicsContextPropertiesInfo* )ctxInfo;

    attrNameString = (GLcharARB *)strJavaToC(env, attrName);
    if (attrNameString == NULL) {
	return;
    }

    /*
     * Get uniform attribute location
     *
     * TODO: we need to separate the string lookup from the setting of
     * the value
     */
    loc = ctxProperties->pfnglGetUniformLocationARB(glShaderProgram, attrNameString);
    /*
    fprintf(stderr,
	    "str = %s, loc = %d, val = %f\n",
	    attrNameString, loc, value);
    */
    free(attrNameString);

    /* Load attribute */
    ctxProperties->pfnglUniform1fARB(loc, value);
}


JNIEXPORT void JNICALL
Java_javax_media_j3d_GLSLShaderProgramRetained_setUniform2i(
    JNIEnv *env,
    jobject obj,
    jlong ctxInfo,
    jint shaderProgram,
    jstring attrName,
    jintArray varray)
{
    JNIEnv table = *env;

    GLcharARB *attrNameString = NULL; /* Null-terminated "C" string */
    jint *values;
    GLint loc = -1;
    GLhandleARB glShaderProgram = (GLhandleARB)shaderProgram;
    
    GraphicsContextPropertiesInfo* ctxProperties =  (GraphicsContextPropertiesInfo* )ctxInfo;

    attrNameString = (GLcharARB *)strJavaToC(env, attrName);
    if (attrNameString == NULL) {
	return;
    }

    /*
     * Get uniform attribute location
     *
     * TODO: we need to separate the string lookup from the setting of
     * the value
     */
    loc = ctxProperties->pfnglGetUniformLocationARB(glShaderProgram, attrNameString);

    /* Get array values */
    values = (jint *)table->GetPrimitiveArrayCritical(env, varray , NULL);
    /*
    fprintf(stderr,
	    "str = %s, loc = %d, val = (%d, %d)\n",
	    attrNameString, loc,
	    values[0], values[1]);
    */
    free(attrNameString);

    /* Load attribute */
    ctxProperties->pfnglUniform2iARB(loc, values[0], values[1]);

    /* Release array values */
    table->ReleasePrimitiveArrayCritical(env,
					 varray,
					 values,
					 JNI_ABORT);
}


JNIEXPORT void JNICALL
Java_javax_media_j3d_GLSLShaderProgramRetained_setUniform2f(
    JNIEnv *env,
    jobject obj,
    jlong ctxInfo,
    jint shaderProgram,
    jstring attrName,
    jfloatArray varray)
{
    JNIEnv table = *env;

    GLcharARB *attrNameString = NULL; /* Null-terminated "C" string */
    jfloat *values;
    GLint loc = -1;
    GLhandleARB glShaderProgram = (GLhandleARB)shaderProgram;

    GraphicsContextPropertiesInfo* ctxProperties =  (GraphicsContextPropertiesInfo* )ctxInfo;

    attrNameString = (GLcharARB *)strJavaToC(env, attrName);
    if (attrNameString == NULL) {
	return;
    }

    /*
     * Get uniform attribute location
     *
     * TODO: we need to separate the string lookup from the setting of
     * the value
     */
    loc = ctxProperties->pfnglGetUniformLocationARB(glShaderProgram, attrNameString);

    /* Get array values */
    values = (jfloat *)table->GetPrimitiveArrayCritical(env, varray , NULL);
    /*
    fprintf(stderr,
	    "str = %s, loc = %d, val = (%f, %f)\n",
	    attrNameString, loc,
	    values[0], values[1]);
    */
    free(attrNameString);

    /* Load attribute */
    ctxProperties->pfnglUniform2fARB(loc, values[0], values[1]);

    /* Release array values */
    table->ReleasePrimitiveArrayCritical(env,
					 varray,
					 values,
					 JNI_ABORT);
}


JNIEXPORT void JNICALL
Java_javax_media_j3d_GLSLShaderProgramRetained_setUniform3i(
    JNIEnv *env,
    jobject obj,
    jlong ctxInfo,
    jint shaderProgram,
    jstring attrName,
    jintArray varray)
{
    JNIEnv table = *env;

    GLcharARB *attrNameString = NULL; /* Null-terminated "C" string */
    jint *values;
    GLint loc = -1;
    GLhandleARB glShaderProgram = (GLhandleARB)shaderProgram;

    GraphicsContextPropertiesInfo* ctxProperties =  (GraphicsContextPropertiesInfo* )ctxInfo;

    attrNameString = (GLcharARB *)strJavaToC(env, attrName);
    if (attrNameString == NULL) {
	return;
    }

    /*
     * Get uniform attribute location
     *
     * TODO: we need to separate the string lookup from the setting of
     * the value
     */
    loc = ctxProperties->pfnglGetUniformLocationARB(glShaderProgram, attrNameString);

    /* Get array values */
    values = (jint *)table->GetPrimitiveArrayCritical(env, varray , NULL);
    /*
    fprintf(stderr,
	    "str = %s, loc = %d, val = (%d, %d, %d)\n",
	    attrNameString, loc,
	    values[0], values[1], values[2]);
    */
    free(attrNameString);

    /* Load attribute */
    ctxProperties->pfnglUniform3iARB(loc, values[0], values[1], values[2]);

    /* Release array values */
    table->ReleasePrimitiveArrayCritical(env,
					 varray,
					 values,
					 JNI_ABORT);
}


JNIEXPORT void JNICALL
Java_javax_media_j3d_GLSLShaderProgramRetained_setUniform3f(
    JNIEnv *env,
    jobject obj,
    jlong ctxInfo,
    jint shaderProgram,
    jstring attrName,
    jfloatArray varray)
{
    JNIEnv table = *env;

    GLcharARB *attrNameString = NULL; /* Null-terminated "C" string */
    jfloat *values;
    GLint loc = -1;
    GLhandleARB glShaderProgram = (GLhandleARB)shaderProgram;

    GraphicsContextPropertiesInfo* ctxProperties =  (GraphicsContextPropertiesInfo* )ctxInfo;

    attrNameString = (GLcharARB *)strJavaToC(env, attrName);
    if (attrNameString == NULL) {
	return;
    }

    /*
     * Get uniform attribute location
     *
     * TODO: we need to separate the string lookup from the setting of
     * the value
     */
    loc = ctxProperties->pfnglGetUniformLocationARB(glShaderProgram, attrNameString);

    /* Get array values */
    values = (jfloat *)table->GetPrimitiveArrayCritical(env, varray , NULL);
    /*
    fprintf(stderr,
	    "str = %s, loc = %d, val = (%f, %f, %f)\n",
	    attrNameString, loc,
	    values[0], values[1], values[2]);
    */
    free(attrNameString);

    /* Load attribute */
    ctxProperties->pfnglUniform3fARB(loc, values[0], values[1], values[2]);

    /* Release array values */
    table->ReleasePrimitiveArrayCritical(env,
					 varray,
					 values,
					 JNI_ABORT);
}


JNIEXPORT void JNICALL
Java_javax_media_j3d_GLSLShaderProgramRetained_setUniform4i(
    JNIEnv *env,
    jobject obj,
    jlong ctxInfo,
    jint shaderProgram,
    jstring attrName,
    jintArray varray)
{
    JNIEnv table = *env;

    GLcharARB *attrNameString = NULL; /* Null-terminated "C" string */
    jint *values;
    GLint loc = -1;
    GLhandleARB glShaderProgram = (GLhandleARB)shaderProgram;

    GraphicsContextPropertiesInfo* ctxProperties =  (GraphicsContextPropertiesInfo* )ctxInfo;

    attrNameString = (GLcharARB *)strJavaToC(env, attrName);
    if (attrNameString == NULL) {
	return;
    }

    /*
     * Get uniform attribute location
     *
     * TODO: we need to separate the string lookup from the setting of
     * the value
     */
    loc = ctxProperties->pfnglGetUniformLocationARB(glShaderProgram, attrNameString);

    /* Get array values */
    values = (jint *)table->GetPrimitiveArrayCritical(env, varray , NULL);
    /*
    fprintf(stderr,
	    "str = %s, loc = %d, val = (%d, %d, %d, %d)\n",
	    attrNameString, loc,
	    values[0], values[1], values[2], values[3]);
    */
    free(attrNameString);

    /* Load attribute */
    ctxProperties->pfnglUniform4iARB(loc, values[0], values[1], values[2], values[3]);

    /* Release array values */
    table->ReleasePrimitiveArrayCritical(env,
					 varray,
					 values,
					 JNI_ABORT);
}


JNIEXPORT void JNICALL
Java_javax_media_j3d_GLSLShaderProgramRetained_setUniform4f(
    JNIEnv *env,
    jobject obj,
    jlong ctxInfo,
    jint shaderProgram,
    jstring attrName,
    jfloatArray varray)
{
    JNIEnv table = *env;

    GLcharARB *attrNameString = NULL; /* Null-terminated "C" string */
    jfloat *values;
    GLint loc = -1;
    GLhandleARB glShaderProgram = (GLhandleARB)shaderProgram;

    GraphicsContextPropertiesInfo* ctxProperties =  (GraphicsContextPropertiesInfo* )ctxInfo;

    attrNameString = (GLcharARB *)strJavaToC(env, attrName);
    if (attrNameString == NULL) {
	return;
    }

    /*
     * Get uniform attribute location
     *
     * TODO: we need to separate the string lookup from the setting of
     * the value
     */
    loc = ctxProperties->pfnglGetUniformLocationARB(glShaderProgram, attrNameString);

    /* Get array values */
    values = (jfloat *)table->GetPrimitiveArrayCritical(env, varray , NULL);
    /*
    fprintf(stderr,
	    "str = %s, loc = %d, val = (%f, %f, %f, %f)\n",
	    attrNameString, loc,
	    values[0], values[1], values[2], values[3]);
    */
    free(attrNameString);

    /* Load attribute */
    ctxProperties->pfnglUniform4fARB(loc, values[0], values[1], values[2], values[3]);

    /* Release array values */
    table->ReleasePrimitiveArrayCritical(env,
					 varray,
					 values,
					 JNI_ABORT);
}
/* KCR: END GLSL SHADER HACK */
