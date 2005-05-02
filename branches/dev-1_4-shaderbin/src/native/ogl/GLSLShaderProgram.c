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

/* TODO: these need to be fields in the ctxInfo struct */
PFNGLATTACHOBJECTARBPROC pfnglAttachObjectARB = NULL;
PFNGLCOMPILESHADERARBPROC pfnglCompileShaderARB = NULL;
PFNGLCREATEPROGRAMOBJECTARBPROC pfnglCreateProgramObjectARB = NULL;
PFNGLCREATESHADEROBJECTARBPROC pfnglCreateShaderObjectARB = NULL;
PFNGLGETINFOLOGARBPROC pfnglGetInfoLogARB = NULL;
PFNGLGETOBJECTPARAMETERIVARBPROC pfnglGetObjectParameterivARB = NULL;
PFNGLLINKPROGRAMARBPROC pfnglLinkProgramARB = NULL;
PFNGLSHADERSOURCEARBPROC pfnglShaderSourceARB = NULL;
PFNGLUSEPROGRAMOBJECTARBPROC pfnglUseProgramObjectARB = NULL;
PFNGLGETUNIFORMLOCATIONARBPROC pfnglGetUniformLocationARB = NULL;
PFNGLUNIFORM1IARBPROC pfnglUniform1iARB = NULL;
PFNGLUNIFORM1FARBPROC pfnglUniform1fARB = NULL;
PFNGLUNIFORM2IARBPROC pfnglUniform2iARB = NULL;
PFNGLUNIFORM2FARBPROC pfnglUniform2fARB = NULL;
PFNGLUNIFORM3IARBPROC pfnglUniform3iARB = NULL;
PFNGLUNIFORM3FARBPROC pfnglUniform3fARB = NULL;
PFNGLUNIFORM4IARBPROC pfnglUniform4iARB = NULL;
PFNGLUNIFORM4FARBPROC pfnglUniform4fARB = NULL;

static void
printInfoLog(GLhandleARB obj) {
    int infoLogLength = 0;
    int len = 0;
    GLcharARB *infoLog;

    pfnglGetObjectParameterivARB(obj,
				 GL_OBJECT_INFO_LOG_LENGTH_ARB,
				 &infoLogLength);
    if (infoLogLength > 0) {
	infoLog = (GLcharARB *)malloc(infoLogLength);
	if (infoLog == NULL) {
	    fprintf(stderr,
		    "ERROR: could not allocate infoLog buffer\n");
	    return;
	}

	pfnglGetInfoLogARB(obj, infoLogLength, &len, infoLog);
	fprintf(stderr, "InfoLog: infoLogLength = %d, len = %d\n",
		infoLogLength, len);
	fprintf(stderr, "%s\n", infoLog);
    }
}
#endif /* !COMPILE_GLSL_SHADERS */


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

    if (firstTime) {
#if defined(UNIX)
	pfnglAttachObjectARB =
	    (PFNGLATTACHOBJECTARBPROC)dlsym(RTLD_DEFAULT, "glAttachObjectARB");
	pfnglCompileShaderARB =
	    (PFNGLCOMPILESHADERARBPROC)dlsym(RTLD_DEFAULT, "glCompileShaderARB");
	pfnglCreateProgramObjectARB =
	    (PFNGLCREATEPROGRAMOBJECTARBPROC)dlsym(RTLD_DEFAULT, "glCreateProgramObjectARB");
	pfnglCreateShaderObjectARB =
	    (PFNGLCREATESHADEROBJECTARBPROC)dlsym(RTLD_DEFAULT, "glCreateShaderObjectARB");
	pfnglGetInfoLogARB =
	    (PFNGLGETINFOLOGARBPROC)dlsym(RTLD_DEFAULT, "glGetInfoLogARB");
	pfnglGetObjectParameterivARB =
	    (PFNGLGETOBJECTPARAMETERIVARBPROC)dlsym(RTLD_DEFAULT, "glGetObjectParameterivARB");
	pfnglLinkProgramARB =
	    (PFNGLLINKPROGRAMARBPROC)dlsym(RTLD_DEFAULT, "glLinkProgramARB");
	pfnglShaderSourceARB =
	    (PFNGLSHADERSOURCEARBPROC)dlsym(RTLD_DEFAULT, "glShaderSourceARB");
	pfnglUseProgramObjectARB =
	    (PFNGLUSEPROGRAMOBJECTARBPROC)dlsym(RTLD_DEFAULT, "glUseProgramObjectARB");
	pfnglGetUniformLocationARB =
	    (PFNGLGETUNIFORMLOCATIONARBPROC)dlsym(RTLD_DEFAULT, "glGetUniformLocationARB");
	pfnglUniform1iARB =
	    (PFNGLUNIFORM1IARBPROC)dlsym(RTLD_DEFAULT, "glUniform1iARB");
	pfnglUniform1fARB =
	    (PFNGLUNIFORM1FARBPROC)dlsym(RTLD_DEFAULT, "glUniform1fARB");
	pfnglUniform2iARB =
	    (PFNGLUNIFORM2IARBPROC)dlsym(RTLD_DEFAULT, "glUniform2iARB");
	pfnglUniform2fARB =
	    (PFNGLUNIFORM2FARBPROC)dlsym(RTLD_DEFAULT, "glUniform2fARB");
	pfnglUniform3iARB =
	    (PFNGLUNIFORM3IARBPROC)dlsym(RTLD_DEFAULT, "glUniform3iARB");
	pfnglUniform3fARB =
	    (PFNGLUNIFORM3FARBPROC)dlsym(RTLD_DEFAULT, "glUniform3fARB");
	pfnglUniform4iARB =
	    (PFNGLUNIFORM4IARBPROC)dlsym(RTLD_DEFAULT, "glUniform4iARB");
	pfnglUniform4fARB =
	    (PFNGLUNIFORM4FARBPROC)dlsym(RTLD_DEFAULT, "glUniform4fARB");
#endif
#ifdef WIN32
	pfnglAttachObjectARB =
	    (PFNGLATTACHOBJECTARBPROC)wglGetProcAddress("glAttachObjectARB");
	pfnglCompileShaderARB =
	    (PFNGLCOMPILESHADERARBPROC)wglGetProcAddress("glCompileShaderARB");
	pfnglCreateProgramObjectARB =
	    (PFNGLCREATEPROGRAMOBJECTARBPROC)wglGetProcAddress("glCreateProgramObjectARB");
	pfnglCreateShaderObjectARB =
	    (PFNGLCREATESHADEROBJECTARBPROC)wglGetProcAddress("glCreateShaderObjectARB");
	pfnglGetInfoLogARB =
	    (PFNGLGETINFOLOGARBPROC)wglGetProcAddress("glGetInfoLogARB");
	pfnglGetObjectParameterivARB =
	    (PFNGLGETOBJECTPARAMETERIVARBPROC)wglGetProcAddress("glGetObjectParameterivARB");
	pfnglLinkProgramARB =
	    (PFNGLLINKPROGRAMARBPROC)wglGetProcAddress("glLinkProgramARB");
	pfnglShaderSourceARB =
	    (PFNGLSHADERSOURCEARBPROC)wglGetProcAddress("glShaderSourceARB");
	pfnglUseProgramObjectARB =
	    (PFNGLUSEPROGRAMOBJECTARBPROC)wglGetProcAddress("glUseProgramObjectARB");
	pfnglGetUniformLocationARB =
	    (PFNGLGETUNIFORMLOCATIONARBPROC)wglGetProcAddress("glGetUniformLocationARB");
	pfnglUniform1iARB =
	    (PFNGLUNIFORM1IARBPROC)wglGetProcAddress("glUniform1iARB");
	pfnglUniform1fARB =
	    (PFNGLUNIFORM1FARBPROC)wglGetProcAddress("glUniform1fARB");
	pfnglUniform2iARB =
	    (PFNGLUNIFORM2IARBPROC)wglGetProcAddress("glUniform2iARB");
	pfnglUniform2fARB =
	    (PFNGLUNIFORM2FARBPROC)wglGetProcAddress("glUniform2fARB");
	pfnglUniform3iARB =
	    (PFNGLUNIFORM3IARBPROC)wglGetProcAddress("glUniform3iARB");
	pfnglUniform3fARB =
	    (PFNGLUNIFORM3FARBPROC)wglGetProcAddress("glUniform3fARB");
	pfnglUniform4iARB =
	    (PFNGLUNIFORM4IARBPROC)wglGetProcAddress("glUniform4iARB");
	pfnglUniform4fARB =
	    (PFNGLUNIFORM4FARBPROC)wglGetProcAddress("glUniform4fARB");
#endif
	if (pfnglCreateShaderObjectARB == NULL) {
	    fprintf(stderr, "Java 3D ERROR : GLSLShader extension not available\n");
	}

	firstTime = GL_FALSE;
    }

    if (pfnglCreateShaderObjectARB == NULL) {
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
	pfnglUseProgramObjectARB(0);
	return 0;
    }

    /* If already created, just bind the shader program */
    if (glShaderProgram != 0) {
	/*
	fprintf(stderr, "    glUseProgramObject(%d)\n", glShaderProgram);
	*/
	pfnglUseProgramObjectARB(glShaderProgram);
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
	    glVertexShader = pfnglCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
	    pfnglShaderSourceARB(glVertexShader, 1, &vertexShaderString, NULL);
	    pfnglCompileShaderARB(glVertexShader);
	    pfnglGetObjectParameterivARB(glVertexShader,
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
		printInfoLog(glVertexShader);
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
	    glFragmentShader = pfnglCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
	    pfnglShaderSourceARB(glFragmentShader, 1, &fragmentShaderString, NULL);
	    pfnglCompileShaderARB(glFragmentShader);
	    pfnglGetObjectParameterivARB(glFragmentShader,
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
		printInfoLog(glFragmentShader);
	    }
	}
	free(fragmentShaderString);
    }

    /* Link the shader (if first time) */
    glShaderProgram = pfnglCreateProgramObjectARB();
    if (vertexShader != 0) {
	pfnglAttachObjectARB(glShaderProgram, glVertexShader);
    }
    if (fragmentShader != 0) {
	pfnglAttachObjectARB(glShaderProgram, glFragmentShader);
    }
    pfnglLinkProgramARB(glShaderProgram);
    
    pfnglGetObjectParameterivARB(glShaderProgram,
				 GL_OBJECT_LINK_STATUS_ARB,
				 &status);
    fprintf(stderr, "GLSLShaderProgram LINK : glShaderProgram = %d -- ", glShaderProgram);
    if (status) {
	fprintf(stderr, "SUCCESSFUL\n");
    }
    else {
	fprintf(stderr, "FAILED\n");
	printInfoLog(glShaderProgram);
	pfnglUseProgramObjectARB(0);
	return 0;
    }

    pfnglUseProgramObjectARB(glShaderProgram);
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
    loc = pfnglGetUniformLocationARB(glShaderProgram, attrNameString);
    /*
    fprintf(stderr,
	    "str = %s, loc = %d, val = %d\n",
	    attrNameString, loc, value);
    */
    free(attrNameString);

    /* Load attribute */
    pfnglUniform1iARB(loc, value);
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
    loc = pfnglGetUniformLocationARB(glShaderProgram, attrNameString);
    /*
    fprintf(stderr,
	    "str = %s, loc = %d, val = %f\n",
	    attrNameString, loc, value);
    */
    free(attrNameString);

    /* Load attribute */
    pfnglUniform1fARB(loc, value);
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
    loc = pfnglGetUniformLocationARB(glShaderProgram, attrNameString);

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
    pfnglUniform2iARB(loc, values[0], values[1]);

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
    loc = pfnglGetUniformLocationARB(glShaderProgram, attrNameString);

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
    pfnglUniform2fARB(loc, values[0], values[1]);

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
    loc = pfnglGetUniformLocationARB(glShaderProgram, attrNameString);

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
    pfnglUniform3iARB(loc, values[0], values[1], values[2]);

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
    loc = pfnglGetUniformLocationARB(glShaderProgram, attrNameString);

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
    pfnglUniform3fARB(loc, values[0], values[1], values[2]);

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
    loc = pfnglGetUniformLocationARB(glShaderProgram, attrNameString);

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
    pfnglUniform4iARB(loc, values[0], values[1], values[2], values[3]);

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
    loc = pfnglGetUniformLocationARB(glShaderProgram, attrNameString);

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
    pfnglUniform4fARB(loc, values[0], values[1], values[2], values[3]);

    /* Release array values */
    table->ReleasePrimitiveArrayCritical(env,
					 varray,
					 values,
					 JNI_ABORT);
#endif /* COMPILE_GLSL_SHADERS */
}
/* KCR: END GLSL SHADER HACK */
