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

/*
 * Comment out the following to disable GLSL shader compilation.
 */
#define ENABLE_GLSL_SHADERS  /* Define to compile GLSL shaders */


/* KCR: BEGIN SHADER HACK */
#if defined(LINUX)
#define _GNU_SOURCE 1
#endif
/* KCR: END SHADER HACK */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <jni.h>

#include "gldefs.h"


extern char *strJavaToC(JNIEnv *env, jstring str);

/* KCR: BEGIN GLSL SHADER HACK */
#if defined(ENABLE_GLSL_SHADERS) && defined(GL_ARB_shading_language_100)
#define COMPILE_GLSL_SHADERS 1
#else
#undef COMPILE_GLSL_SHADERS
#endif

#if defined(UNIX)
#include <dlfcn.h>
#endif
/* KCR: END GLSL SHADER HACK */


#ifdef DEBUG
/* Uncomment the following for VERBOSE debug messages */
/* #define VERBOSE */
#endif /* DEBUG */


#ifdef COMPILE_GLSL_SHADERS
/* KCR: BEGIN GLSL SHADER HACK */

static void
printInfoLog(GraphicsContextPropertiesInfo* ctxProperties, GLhandleARB obj) {
    int infoLogLength = 0;
    int len = 0;
    GLcharARB *infoLog;

    ctxProperties->pfnglGetObjectParameterivARB(obj,
						GL_OBJECT_INFO_LOG_LENGTH_ARB,
						&infoLogLength);
    if (infoLogLength > 0) {
	infoLog = (GLcharARB *)malloc(infoLogLength);
	if (infoLog == NULL) {
	    fprintf(stderr,
		    "ERROR: could not allocate infoLog buffer\n");
	    return;
	}

	ctxProperties->pfnglGetInfoLogARB(obj, infoLogLength, &len, infoLog);
	fprintf(stderr, "InfoLog: infoLogLength = %d, len = %d\n",
		infoLogLength, len);
	fprintf(stderr, "%s\n", infoLog);
    }
}
#endif /* !COMPILE_GLSL_SHADERS */

/*
 * Class:     javax_media_j3d_GLSLShaderProgramRetained
 * Method:    createShader
 * Signature: (JI[J)Ljavax/media/j3d/ShaderError;
 */
JNIEXPORT jobject JNICALL
Java_javax_media_j3d_GLSLShaderProgramRetained_createShader(
    JNIEnv *env,
    jobject obj,
    jlong ctxInfo,
    jint  shaderType,
    jlongArray shaderIdArray)
{

    jlong *shaderIdPtr;
    GLhandleARB shaderHandle;
    
    GraphicsContextPropertiesInfo* ctxProperties =  (GraphicsContextPropertiesInfo* )ctxInfo;
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
    
    shaderIdPtr[0] = (jlong) shaderHandle;
    (*env)->ReleaseLongArrayElements(env, shaderIdArray, shaderIdPtr, 0); 

    
    return NULL; /* Will handle error reporting later. Return null for now */
}

/*
 * Class:     javax_media_j3d_GLSLShaderProgramRetained
 * Method:    destroyShader
 * Signature: (JJ)Ljavax/media/j3d/ShaderError;
 */
JNIEXPORT jobject JNICALL
Java_javax_media_j3d_GLSLShaderProgramRetained_destroyShader(
    JNIEnv *env,
    jobject obj,
    jlong ctxInfo,
    jlong shaderId)
{
    GraphicsContextPropertiesInfo* ctxProperties =  (GraphicsContextPropertiesInfo* )ctxInfo;

    ctxProperties->pfnglglDeleteObjectARB(shaderId);
    
    return NULL; /* Will handle error reporting later. Return null for now */
}

/*
 * Class:     javax_media_j3d_GLSLShaderProgramRetained
 * Method:    compileShader
 * Signature: (JJLjava/lang/String;)Ljavax/media/j3d/ShaderError;
 */
JNIEXPORT jobject JNICALL
Java_javax_media_j3d_GLSLShaderProgramRetained_compileShader(
    JNIEnv *env,
    jobject obj,
    jlong ctxInfo,
    jlong shaderId,
    jstring program)
{    
    GLint status;
    jlong *shaderIdPtr;
    GLhandleARB shaderHandle;
    
    GraphicsContextPropertiesInfo* ctxProperties =  (GraphicsContextPropertiesInfo* )ctxInfo;
    
    /* Null-terminated "C" strings */
    GLcharARB *shaderString = NULL;

    shaderString = (GLcharARB *)strJavaToC(env, program);
    if (shaderString == NULL) {	
	fprintf(stderr, "Error in compileShader (1)\n");
	return NULL;
    }

    ctxProperties->pfnglShaderSourceARB(shaderId, 1, &shaderString, NULL);
    ctxProperties->pfnglCompileShaderARB(shaderId);
    ctxProperties->pfnglGetObjectParameterivARB(shaderId,
						GL_OBJECT_COMPILE_STATUS_ARB,
						&status);
    fprintf(stderr,
	    "GLSLShaderProgram COMPILE : shaderId = %d -- ", shaderId);
    if (status) {
	fprintf(stderr, "SUCCESSFUL\n");
    }
    else {
	fprintf(stderr, "FAILED\n");
	printInfoLog(ctxProperties, shaderId); /* TODO - Replace witht detail message in
						  the return ShaderError. */
    }
    
    free(shaderString);    
    return NULL; /* Will handle error reporting later. Return null for now */
    
}

/*
 * Class:     javax_media_j3d_GLSLShaderProgramRetained
 * Method:    createShaderProgram
 * Signature: (J[J)Ljavax/media/j3d/ShaderError;
 */
JNIEXPORT jobject JNICALL
Java_javax_media_j3d_GLSLShaderProgramRetained_createShaderProgram(
    JNIEnv *env,
    jobject obj,
    jlong ctxInfo,
    jlongArray shaderProgramIdArray)    
{

    jlong *shaderProgramIdPtr;
    GLhandleARB shaderProgramHandle;

    GraphicsContextPropertiesInfo* ctxProperties =  (GraphicsContextPropertiesInfo* )ctxInfo;
    shaderProgramIdPtr = (*env)->GetLongArrayElements(env, shaderProgramIdArray, NULL);

    shaderProgramHandle = ctxProperties->pfnglCreateProgramObjectARB();

    shaderProgramIdPtr[0] = (jlong) shaderProgramHandle;
    (*env)->ReleaseLongArrayElements(env, shaderProgramIdArray, shaderProgramIdPtr, 0);
    
    return NULL; /* Will handle error reporting later. Return null for now */
}

/*
 * Class:     javax_media_j3d_GLSLShaderProgramRetained
 * Method:    destroyShaderProgram
 * Signature: (JJ)Ljavax/media/j3d/ShaderError;
 */
JNIEXPORT jobject JNICALL
Java_javax_media_j3d_GLSLShaderProgramRetained_destroyShaderProgram(
    JNIEnv *env,
    jobject obj,
    jlong ctxInfo,
    jlong shaderProgramId)
{
    GraphicsContextPropertiesInfo* ctxProperties =  (GraphicsContextPropertiesInfo* )ctxInfo;

    ctxProperties->pfnglglDeleteObjectARB(shaderProgramId);

    return NULL; /* Will handle error reporting later. Return null for now */
}

/*
 * Class:     javax_media_j3d_GLSLShaderProgramRetained
 * Method:    linkShaderProgram
 * Signature: (JJ[J)Ljavax/media/j3d/ShaderError;
 */
JNIEXPORT jobject JNICALL
Java_javax_media_j3d_GLSLShaderProgramRetained_linkShaderProgram(
    JNIEnv *env,
    jobject obj,
    jlong ctxInfo,
    jlong shaderProgramId,
    jlongArray shaderIdArray)
{
    GLint status;
    int i;
    GLhandleARB shaderHandle;
    
    GraphicsContextPropertiesInfo* ctxProperties =  (GraphicsContextPropertiesInfo* )ctxInfo;
    jlong *shaderIdPtr = (*env)->GetLongArrayElements(env, shaderIdArray, NULL);
    jsize shaderIdArrayLength = (*env)->GetArrayLength(env,  shaderIdArray);

    
    fprintf(stderr, "shaderIdArrayLength %d\n", shaderIdArrayLength);
    
    for(i=0; i<shaderIdArrayLength; i++) {
	ctxProperties->pfnglAttachObjectARB(shaderProgramId, shaderIdPtr[i]);
    }

    ctxProperties->pfnglLinkProgramARB(shaderProgramId);
    ctxProperties->pfnglGetObjectParameterivARB(shaderProgramId,
						GL_OBJECT_LINK_STATUS_ARB,
						&status);
    fprintf(stderr, "GLSLShaderProgram LINK : shaderProgramId = %d -- ", shaderProgramId);
    if (status) {
	fprintf(stderr, "SUCCESSFUL\n");
    }
    else {
	fprintf(stderr, "FAILED\n");
	printInfoLog(ctxProperties, shaderProgramId);/* TODO - Replace witht detail message in
							the return ShaderError. */
	
	ctxProperties->pfnglUseProgramObjectARB(0);
    }

    (*env)->ReleaseLongArrayElements(env, shaderIdArray, shaderIdPtr, JNI_ABORT); 

    return NULL; /* Will handle error reporting later. Return null for now */
}

/*
 * TODO: pass in an array of shaders (no need to distinguish
 * vertex from fragment in the native code)
 */

/*
 * Class:     javax_media_j3d_GLSLShaderProgramRetained
 * Method:    updateNative
 * Signature: (JLjava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT jint JNICALL Java_javax_media_j3d_GLSLShaderProgramRetained_updateNative(
    JNIEnv *env,
    jobject obj,
    jlong ctxInfo,
    jint shaderProgramId,
    jstring vertexShader,
    jstring fragmentShader)
{
#ifndef COMPILE_GLSL_SHADERS
    static GLboolean firstTime = GL_TRUE;
    if (firstTime) {
	fprintf(stderr, "Java 3D ERROR : GLSLShader code not compiled\n");
	firstTime = GL_FALSE;
    }
    return 0;
#endif /* !COMPILE_GLSL_SHADERS */

#ifdef COMPILE_GLSL_SHADERS
    GLint status;

    GLhandleARB glVertexShader = 0;
    GLhandleARB glFragmentShader = 0;
    GLhandleARB glShaderProgram = (GLhandleARB)shaderProgramId;

    /* Null-terminated "C" strings */
    GLcharARB *vertexShaderString = NULL;
    GLcharARB *fragmentShaderString = NULL;

    static GLboolean firstTime = GL_TRUE;
    GraphicsContextPropertiesInfo* ctxProperties =  (GraphicsContextPropertiesInfo* )ctxInfo;


    if (ctxProperties->pfnglCreateShaderObjectARB == NULL) {
	return 0;
    }

    /*
     * If the vertexProgram and fragment program are both NULL, then
     * disable shading by binding to program 0
     */
    if (vertexShader == 0 && fragmentShader == 0) {
	/*
	fprintf(stderr, "    vertexShader and fragmentShader are NULL\n");
	*/
	ctxProperties->pfnglUseProgramObjectARB(0);
	return 0;
    }

    /* If already created, just bind the shader program */
    if (glShaderProgram != 0) {
	/*
	fprintf(stderr, "    glUseProgramObject(%d)\n", glShaderProgram);
	*/
	ctxProperties->pfnglUseProgramObjectARB(glShaderProgram);
	return glShaderProgram;
    }

    /* Process vertex shader */
    /*
    fprintf(stderr, "    vertexShader == 0x%x\n", vertexShader);
    */
    if (vertexShader != 0) {
	vertexShaderString = (GLcharARB *)strJavaToC(env, vertexShader);
	if (vertexShaderString == NULL) {
	    return 0;
	}

	/*
	 * TODO: need to check whether the shader has changed and free up the
	 * old shader before allocating a new one (like we do for texture)
	 */
	if (glVertexShader == 0) {
	    /* create the vertex shader */
	    glVertexShader = ctxProperties->pfnglCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
	    ctxProperties->pfnglShaderSourceARB(glVertexShader, 1, &vertexShaderString, NULL);
	    ctxProperties->pfnglCompileShaderARB(glVertexShader);
	    ctxProperties->pfnglGetObjectParameterivARB(glVertexShader,
				      GL_OBJECT_COMPILE_STATUS_ARB,
				      &status);
	    fprintf(stderr,
		    "GLSLShaderProgram COMPILE : glVertexShader = %d -- ",
		    glVertexShader);
	    if (status) {
		fprintf(stderr, "SUCCESSFUL\n");
	    }
	    else {
		fprintf(stderr, "FAILED\n");
		printInfoLog(ctxProperties, glVertexShader);
	    }
	}
	free(vertexShaderString);
    }

    /* Process fragment shader */
    /*
    fprintf(stderr, "    fragmentShader == 0x%x\n", fragmentShader);
    */
    if (fragmentShader != 0) {
	fragmentShaderString = (GLcharARB *)strJavaToC(env, fragmentShader);
	if (fragmentShaderString == NULL) {
	    return 0;
	}

	/*
	 * TODO: need to check whether the shader has changed and free up the
	 * old shader before allocating a new one (like we do for texture)
	 */
	if (glFragmentShader == 0) {
	    /* create the fragment shader */
	    glFragmentShader = ctxProperties->pfnglCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
	    ctxProperties->pfnglShaderSourceARB(glFragmentShader, 1, &fragmentShaderString, NULL);
	    ctxProperties->pfnglCompileShaderARB(glFragmentShader);
	    ctxProperties->pfnglGetObjectParameterivARB(glFragmentShader,
				      GL_OBJECT_COMPILE_STATUS_ARB,
				      &status);
	    fprintf(stderr,
		    "GLSLShaderProgram COMPILE : glFragmentShader = %d -- ",
		    glFragmentShader);
	    if (status) {
		fprintf(stderr, "SUCCESSFUL\n");
	    }
	    else {
		fprintf(stderr, "FAILED\n");
		printInfoLog(ctxProperties, glFragmentShader);
	    }
	}
	free(fragmentShaderString);
    }

    /* Link the shader (if first time) */
    glShaderProgram = ctxProperties->pfnglCreateProgramObjectARB();
    if (vertexShader != 0) {
	ctxProperties->pfnglAttachObjectARB(glShaderProgram, glVertexShader);
    }
    if (fragmentShader != 0) {
	ctxProperties->pfnglAttachObjectARB(glShaderProgram, glFragmentShader);
    }
    ctxProperties->pfnglLinkProgramARB(glShaderProgram);
    
    ctxProperties->pfnglGetObjectParameterivARB(glShaderProgram,
				 GL_OBJECT_LINK_STATUS_ARB,
				 &status);
    fprintf(stderr, "GLSLShaderProgram LINK : glShaderProgram = %d -- ", glShaderProgram);
    if (status) {
	fprintf(stderr, "SUCCESSFUL\n");
    }
    else {
	fprintf(stderr, "FAILED\n");
	printInfoLog(ctxProperties, glShaderProgram);
	ctxProperties->pfnglUseProgramObjectARB(0);
	return 0;
    }

    ctxProperties->pfnglUseProgramObjectARB(glShaderProgram);
    return glShaderProgram;
#endif /* !COMPILE_GLSL_SHADERS */
}


JNIEXPORT void JNICALL
Java_javax_media_j3d_GLSLShaderProgramRetained_setUniform1i(
    JNIEnv *env,
    jobject obj,
    jlong ctxInfo,
    jint shaderProgram,
    jstring attrName,
    jint value)
{
#ifdef COMPILE_GLSL_SHADERS
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
#endif /* COMPILE_GLSL_SHADERS */
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
#ifdef COMPILE_GLSL_SHADERS
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
#endif /* COMPILE_GLSL_SHADERS */
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
#ifdef COMPILE_GLSL_SHADERS
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
#endif /* COMPILE_GLSL_SHADERS */
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
#ifdef COMPILE_GLSL_SHADERS
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
#endif /* COMPILE_GLSL_SHADERS */
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
#ifdef COMPILE_GLSL_SHADERS
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
#endif /* COMPILE_GLSL_SHADERS */
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
#ifdef COMPILE_GLSL_SHADERS
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
#endif /* COMPILE_GLSL_SHADERS */
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
#ifdef COMPILE_GLSL_SHADERS
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
#endif /* COMPILE_GLSL_SHADERS */
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
#ifdef COMPILE_GLSL_SHADERS
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
#endif /* COMPILE_GLSL_SHADERS */
}
/* KCR: END GLSL SHADER HACK */
