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
 * Comment out the following to disable CG shader compilation.
 */
/* #define ENABLE_CG_SHADERS */    /* Define to compile CG shaders */


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

/* KCR: BEGIN CG SHADER HACK */
#if defined(ENABLE_CG_SHADERS)
#define COMPILE_CG_SHADERS 1
#else
#undef COMPILE_CG_SHADERS
#endif

#ifdef COMPILE_CG_SHADERS
#include <Cg/cgGL.h>
#endif /* COMPILE_CG_SHADERS */
/* KCR: END CG SHADER HACK */


#ifdef DEBUG
/* Uncomment the following for VERBOSE debug messages */
/* #define VERBOSE */
#endif /* DEBUG */


#ifdef COMPILE_CG_SHADERS
/* KCR: BEGIN CG SHADER HACK */
/* TODO: these need to be instance variables in the Java class */
static CGcontext vContext = 0;
static CGprogram vShader = 0;
static CGprofile vProfile = 0;
static CGcontext fContext = 0;
static CGprogram fShader = 0;
static CGprofile fProfile = 0;


static void
cgErrorCallback(void)
{
    CGerror LastError = cgGetError();

    if(LastError) {
        const char *Listing = cgGetLastListing(vContext);
	fprintf(stderr, "\n---------------------------------------------------\n");
        fprintf(stderr, "%s\n\n", cgGetErrorString(LastError));
        fprintf(stderr, "%s\n", Listing);
        fprintf(stderr, "---------------------------------------------------\n");
        fprintf(stderr, "Cg error, exiting...\n");
        exit(1);
    }
}
#endif /* COMPILE_CG_SHADERS */


/*
 * Class:     javax_media_j3d_CgShaderProgram
 * Method:    updateNative
 * Signature: (JLjava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_javax_media_j3d_CgShaderProgram_updateNative(
    JNIEnv *env,
    jobject obj,
    jlong ctxInfo,
    jstring vertexShader,
    jstring fragmentShader)
{
#ifndef COMPILE_CG_SHADERS
    static GLboolean firstTime = GL_TRUE;

    if (firstTime) {
	fprintf(stderr, "Java 3D ERROR : CgShader code not compiled\n");
	firstTime = GL_FALSE;
    }
    return;
#endif /* !COMPILE_CG_SHADERS */

#ifdef COMPILE_CG_SHADERS
    /* Null-terminated "C" strings */
    char *vertexShaderString = NULL;
    char *fragmentShaderString = NULL;

    /* Process vertex shader */
    /*
    fprintf(stderr, "    vertexShader == 0x%x\n", vertexShader);
    */
    if (vertexShader != 0) {
	vertexShaderString = strJavaToC(env, vertexShader);
	if (vertexShaderString == NULL) {
	    return;
	}

	/*
	 * TODO: need to check whether the shader has changed and free up the
	 * old shader before allocating a new one (like we do for texture)
	 */
	if (vContext == 0) {
	    /* Use GL_ARB_vertex_program extension if supported by video card */
	    if (cgGLIsProfileSupported(CG_PROFILE_ARBVP1)) {
		fprintf(stderr, "Using CG_PROFILE_ARBVP1\n");
		vProfile = CG_PROFILE_ARBVP1;
	    }
	    else if (cgGLIsProfileSupported(CG_PROFILE_VP20)) {
		fprintf(stderr, "Using CG_PROFILE_VP20\n");
		vProfile = CG_PROFILE_VP20;
	    }
	    else {
		fprintf(stderr,
			"ERROR: Vertex programming extensions (GL_ARB_vertex_program or\n"
			"GL_NV_vertex_program) not supported, exiting...\n");
		return;
	    }

	    cgSetErrorCallback(cgErrorCallback);

	    vContext = cgCreateContext();

	    /* create the vertex shader */
	    fprintf(stderr,
		    "CgShaderProgram_updateNative: create vertex shader program\n");
	    vShader = cgCreateProgram(vContext,
				      CG_SOURCE, vertexShaderString,
				      vProfile, NULL, NULL);
	}
	free(vertexShaderString);

	/*
	fprintf(stderr,
		"CgShaderProgram_updateNative: load/bind/enable vertex shader program\n");
	*/
	cgGLLoadProgram(vShader);
	cgGLBindProgram(vShader);
	cgGLEnableProfile(vProfile);
    }
    else {
	if (vProfile != 0) {
	    cgGLDisableProfile(vProfile);
	}
    }

    /* Process fragment shader */
    /*
    fprintf(stderr, "    fragmentShader == 0x%x\n", fragmentShader);
    */
    if (fragmentShader != 0) {
	fragmentShaderString = strJavaToC(env, fragmentShader);
	if (fragmentShaderString == NULL) {
	    return;
	}

	/*
	 * TODO: need to check whether the shader has changed and free up the
	 * old shader before allocating a new one (like we do for texture)
	 */
	if (fContext == 0) {
	    /* Use GL_ARB_fragment_program extension if supported by video card */
	    if (cgGLIsProfileSupported(CG_PROFILE_ARBFP1)) {
		fprintf(stderr, "Using CG_PROFILE_ARBFP1\n");
		fProfile = CG_PROFILE_ARBFP1;
	    }
	    else if (cgGLIsProfileSupported(CG_PROFILE_FP20)) {
		fprintf(stderr, "Using CG_PROFILE_FP20\n");
		fProfile = CG_PROFILE_FP20;
	    }
	    else {
		fprintf(stderr,
			"Fragment programming extensions (GL_ARB_fragment_program or\n"
			"GL_NV_fragment_program) not supported, exiting...\n");
		return;
	    }

	    cgSetErrorCallback(cgErrorCallback);

	    fContext = cgCreateContext();

	    /* create the fragment shader */
	    fprintf(stderr,
		    "CgShaderProgram_updateNative: create fragment shader program\n");
	    fShader = cgCreateProgram(fContext,
				      CG_SOURCE, fragmentShaderString,
				      fProfile, NULL, NULL);
	}
	free(fragmentShaderString);

	cgGLLoadProgram(fShader);
	cgGLBindProgram(fShader);
	/*
	fprintf(stderr,
		"CgShaderProgram_updateNative: load/bind/enable fragment shader program\n");
	*/
	cgGLEnableProfile(fProfile);
    }
    else {
	if (fProfile != 0) {
	    cgGLDisableProfile(fProfile);
	}
    }
#endif /* COMPILE_CG_SHADERS */

}
/* KCR: END CG SHADER HACK */
