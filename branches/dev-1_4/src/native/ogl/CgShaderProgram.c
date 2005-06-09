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
 * TODO: Figure this out automatically.
 */
/*#define ENABLE_CG_SHADERS*/    /* Define to compile CG shaders */

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

#if defined(ENABLE_CG_SHADERS)
#define COMPILE_CG_SHADERS 1
#else
#undef COMPILE_CG_SHADERS
#endif


#ifdef COMPILE_CG_SHADERS
#include <Cg/cgGL.h>
#endif /* COMPILE_CG_SHADERS */


/* Structure used to hold CG context information; stored in ctxInfo */
struct CgCtxInfoRec {
#ifdef COMPILE_CG_SHADERS
    CGcontext cgCtx;
    CGprofile vProfile;
    CGprofile fProfile;
#else /* COMPILE_CG_SHADERS */
    int dummy;
#endif /* COMPILE_CG_SHADERS */
};


/* Structure used to hold CG shader information; passed back to Java as cgShaderId */
typedef struct CgShaderInfoRec CgShaderInfo;
struct CgShaderInfoRec {
#ifdef COMPILE_CG_SHADERS
    CGprogram cgShader;
    jint shaderType;
    CGprofile shaderProfile;
#else /* COMPILE_CG_SHADERS */
    int dummy;
#endif /* COMPILE_CG_SHADERS */
};

/*
 * Structure used to hold CG shader program information; passed back
 * to Java as cgShaderProgramId
 */
typedef struct CgShaderProgramInfoRec CgShaderProgramInfo;
struct CgShaderProgramInfoRec {
#ifdef COMPILE_CG_SHADERS
    int numShaders;
    CgShaderInfo **shaders;
#else /* COMPILE_CG_SHADERS */
    int dummy;
#endif /* COMPILE_CG_SHADERS */
};


#ifdef COMPILE_CG_SHADERS
#if 0
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
#endif /* 0 */


static char *
getErrorLog(
    GraphicsContextPropertiesInfo* ctxProperties,
    CGerror lastError)
{
    if (lastError != 0) {
	const char *errString = cgGetErrorString(lastError);
	const char *delimeter1 = "\n";
        const char *listing = cgGetLastListing(ctxProperties->cgCtxInfo->cgCtx);

	char *errMsg = (char *)
	    malloc(strlen(errString) + strlen(delimeter1) + strlen(listing) + 1);

	if (errMsg == NULL) {
	    perror("malloc");
	    return NULL;
	}

	strcpy(errMsg, errString);
	strcat(errMsg, delimeter1);
	strcat(errMsg, listing);

	return errMsg;
    }

    fprintf(stderr, "Assertion Error: assert(lastError != 0) failed\n");
    return NULL;
}


static CgCtxInfo *
createCgShaderContext(
    JNIEnv *env,
    GraphicsContextPropertiesInfo *ctxInfo)
{
    jclass oom;
    CGerror lastError;
    CgCtxInfo *cgCtxInfo = NULL;

    cgCtxInfo = (CgCtxInfo*)malloc(sizeof(CgCtxInfo));
    if (cgCtxInfo == NULL) {
	if ((oom = (*env)->FindClass(env, "java/lang/OutOfMemoryError")) != NULL) {
	    (*env)->ThrowNew(env, oom, "malloc");
	}
	return NULL;
    }

    /* Create CG context */
    cgCtxInfo->cgCtx = cgCreateContext();

    if ((lastError = cgGetError()) != 0) {
	fprintf(stderr, "Fatal error in creating Cg context:\n");
	fprintf(stderr, "\t%s\n", cgGetErrorString(lastError));
	free(cgCtxInfo);
	return NULL;
    }

    if (cgCtxInfo->cgCtx == 0) {
	fprintf(stderr, "Invalid NULL Cg context\n");
	free(cgCtxInfo);
	return NULL;
    }

    /* Use GL_ARB_vertex_program extension if supported by video card */
    if (cgGLIsProfileSupported(CG_PROFILE_ARBVP1)) {
	fprintf(stderr, "Using CG_PROFILE_ARBVP1\n");
	cgCtxInfo->vProfile = CG_PROFILE_ARBVP1;
    }
    else if (cgGLIsProfileSupported(CG_PROFILE_VP20)) {
	fprintf(stderr, "Using CG_PROFILE_VP20\n");
	cgCtxInfo->vProfile = CG_PROFILE_VP20;
    }
    else {
	fprintf(stderr,
		"ERROR: Vertex programming extensions (GL_ARB_vertex_program or\n"
		"GL_NV_vertex_program) not supported, exiting...\n");
	free(cgCtxInfo);
	return NULL;
    }

    if ((lastError = cgGetError()) != 0) {
	fprintf(stderr, "FATAL ERROR IN CREATING VERTEX SHADER PROFILE:\n");
	fprintf(stderr, "\t%s\n", cgGetErrorString(lastError));
	free(cgCtxInfo);
	return NULL;
    }

    /* Use GL_ARB_fragment_program extension if supported by video card */
    if (cgGLIsProfileSupported(CG_PROFILE_ARBFP1)) {
	fprintf(stderr, "Using CG_PROFILE_ARBFP1\n");
	cgCtxInfo->fProfile = CG_PROFILE_ARBFP1;
    }
    else if (cgGLIsProfileSupported(CG_PROFILE_FP20)) {
	fprintf(stderr, "Using CG_PROFILE_FP20\n");
	cgCtxInfo->fProfile = CG_PROFILE_FP20;
    }
    else {
	fprintf(stderr,
		"Fragment programming extensions (GL_ARB_fragment_program or\n"
		"GL_NV_fragment_program) not supported, exiting...\n");
	free(cgCtxInfo);
	return NULL;
    }

    if ((lastError = cgGetError()) != 0) {
	fprintf(stderr, "FATAL ERROR IN CREATING FRAGMENT SHADER PROFILE:\n");
	fprintf(stderr, "\t%s\n", cgGetErrorString(lastError));
	free(cgCtxInfo);
	return NULL;
    }

    fprintf(stderr, "createCgShaderContext: SUCCESS\n");
    fprintf(stderr, "    cgCtx = 0x%x\n", cgCtxInfo->cgCtx);
    fprintf(stderr, "    vProfile = 0x%x\n", cgCtxInfo->vProfile);
    fprintf(stderr, "    fProfile = 0x%x\n", cgCtxInfo->fProfile);

    return cgCtxInfo;
}

#endif /* COMPILE_CG_SHADERS */


/*
 * Called by getPropertiesFromCurrentContext to initialize the Cg
 * shader function pointers and set the flag indicating whether Cg
 * shaders are available.
 */
void
checkCgShaderExtensions(
    JNIEnv *env,
    jobject obj,
    char *tmpExtensionStr,
    GraphicsContextPropertiesInfo *ctxInfo,
    jboolean cgLibraryAvailable)
{
    ctxInfo->shadingLanguageCg = JNI_FALSE;
    ctxInfo->cgCtxInfo = NULL;

#ifdef COMPILE_CG_SHADERS
    if (cgLibraryAvailable) {
	ctxInfo->cgCtxInfo = createCgShaderContext(env, ctxInfo);
	if (ctxInfo->cgCtxInfo != NULL) {
	    ctxInfo->shadingLanguageCg = JNI_TRUE;
	    fprintf(stderr, "Cg ctx is available\n");
	}
	else {
	    fprintf(stderr, "ERROR: Cg ctx *not* available\n");
	}
    }
#endif /* COMPILE_CG_SHADERS */

}


/*
 * Class:     javax_media_j3d_CgShaderProgramRetained
 * Method:    createNativeShader
 * Signature: (JI[J)Ljavax/media/j3d/ShaderError;
 */
JNIEXPORT jobject JNICALL
Java_javax_media_j3d_CgShaderProgramRetained_createNativeShader(
    JNIEnv *env,
    jobject obj,
    jlong ctxInfo,
    jint  shaderType,
    jlongArray shaderIdArray)
{
    jobject shaderError = NULL;

#ifdef COMPILE_CG_SHADERS
    GraphicsContextPropertiesInfo* ctxProperties =  (GraphicsContextPropertiesInfo* )ctxInfo;
    CgCtxInfo *cgCtxInfo = ctxProperties->cgCtxInfo;

    jlong *shaderIdPtr;
    CGprogram shaderId = 0;
    jclass oom;
    CgShaderInfo *cgShaderInfo;

    fprintf(stderr, "CgShaderProgramRetained.createNativeShader\n");

    cgShaderInfo = (CgShaderInfo*)malloc(sizeof(CgShaderInfo));
    if (cgShaderInfo == NULL) {
	if ((oom = (*env)->FindClass(env, "java/lang/OutOfMemoryError")) != NULL) {
	    (*env)->ThrowNew(env, oom, "malloc");
	}
	return NULL;
    }
    cgShaderInfo->cgShader = 0;
    cgShaderInfo->shaderType = shaderType;
    if (shaderType == javax_media_j3d_Shader_SHADER_TYPE_VERTEX) {
	cgShaderInfo->shaderProfile = cgCtxInfo->vProfile;
    }
    else if (shaderType == javax_media_j3d_Shader_SHADER_TYPE_FRAGMENT) {
	cgShaderInfo->shaderProfile = cgCtxInfo->fProfile;
    }
    else {
	cgShaderInfo->shaderProfile = 0;
	fprintf(stderr,
		"Assertion error: unrecognized shaderType (%d)\n",
		shaderType);
    }

    shaderIdPtr = (*env)->GetLongArrayElements(env, shaderIdArray, NULL);
    shaderIdPtr[0] = (jlong) cgShaderInfo;
    (*env)->ReleaseLongArrayElements(env, shaderIdArray, shaderIdPtr, 0); 

#else /* COMPILE_CG_SHADERS */

    shaderError = createShaderError(env,
				    javax_media_j3d_ShaderError_UNSUPPORTED_LANGUAGE_ERROR,
				    "CgShaderProgram support not compiled",
				    NULL);

#endif /* !COMPILE_CG_SHADERS */

    return shaderError;
}


/*
 * Class:     javax_media_j3d_CgShaderProgramRetained
 * Method:    destroyNativeShader
 * Signature: (JJ)Ljavax/media/j3d/ShaderError;
 */
JNIEXPORT jobject JNICALL
Java_javax_media_j3d_CgShaderProgramRetained_destroyNativeShader(
    JNIEnv *env,
    jobject obj,
    jlong ctxInfo,
    jlong shaderId)
{
    jobject shaderError = NULL;

#ifdef COMPILE_CG_SHADERS

    CgShaderInfo *cgShaderInfo = (CgShaderInfo *)shaderId;

    fprintf(stderr, "CgShaderProgramRetained.destroyNativeShader\n");

    if (cgShaderInfo != NULL) {
	if (cgShaderInfo->cgShader != 0) {
	    cgDestroyProgram(cgShaderInfo->cgShader);
	}

	free(cgShaderInfo);
    }

#else /* COMPILE_CG_SHADERS */

    shaderError = createShaderError(env,
				    javax_media_j3d_ShaderError_UNSUPPORTED_LANGUAGE_ERROR,
				    "CgShaderProgram support not compiled",
				    NULL);

#endif /* !COMPILE_CG_SHADERS */

    return shaderError;
}


/*
 * Class:     javax_media_j3d_CgShaderProgramRetained
 * Method:    compileNativeShader
 * Signature: (JJLjava/lang/String;)Ljavax/media/j3d/ShaderError;
 */
JNIEXPORT jobject JNICALL
Java_javax_media_j3d_CgShaderProgramRetained_compileNativeShader(
    JNIEnv *env,
    jobject obj,
    jlong ctxInfo,
    jlong shaderId,
    jstring program)
{
    jobject shaderError = NULL;

#ifdef COMPILE_CG_SHADERS

    /* TODO: implement this */

    GraphicsContextPropertiesInfo* ctxProperties =  (GraphicsContextPropertiesInfo* )ctxInfo;
    CgCtxInfo *cgCtxInfo = ctxProperties->cgCtxInfo;
    CgShaderInfo *cgShaderInfo = (CgShaderInfo *)shaderId;
    CGerror lastError;
    GLcharARB *shaderString = NULL;

    fprintf(stderr, "CgShaderProgramRetained.compileNativeShader\n");

    /* Assertion check the cgShaderInfo pointer */
    if (cgShaderInfo == NULL) {
	shaderError = createShaderError(env,
					javax_media_j3d_ShaderError_COMPILE_ERROR,
					"Assertion error: cgShaderInfo is NULL",
					NULL);
	return shaderError;
    }

    /* Assertion check the program string */
    if (program == NULL) {
	shaderError = createShaderError(env,
					javax_media_j3d_ShaderError_COMPILE_ERROR,
					"Assertion error: program string is NULL",
					NULL);
	return shaderError;
    }

    shaderString = strJavaToC(env, program);
    if (shaderString == NULL) {	
	/* Just return, since strJavaToC will throw OOM if it returns NULL */
	return NULL;
    }

    /* create the shader */
    if (cgShaderInfo->shaderType == javax_media_j3d_Shader_SHADER_TYPE_VERTEX) { 
	fprintf(stderr, "Create vertex shader\n");
    }
    else if (cgShaderInfo->shaderType == javax_media_j3d_Shader_SHADER_TYPE_FRAGMENT) { 
	fprintf(stderr, "Create fragment shader\n");
    }
    fprintf(stderr, "cgCtx = 0x%x\n", cgCtxInfo->cgCtx);
    fprintf(stderr, "shaderProfile = 0x%x\n", cgShaderInfo->shaderProfile);
    cgShaderInfo->cgShader = cgCreateProgram(cgCtxInfo->cgCtx,
					     CG_SOURCE, shaderString,
					     cgShaderInfo->shaderProfile, NULL, NULL);
    fprintf(stderr, "    cgShader = 0x%x\n", cgShaderInfo->cgShader);

    free(shaderString);

#ifdef OUT__XXX__OUT
    cgShaderInfo->cgShader = 0;
    shaderError = createShaderError(env,
				    javax_media_j3d_ShaderError_COMPILE_ERROR,
				    "Cg shader compile error",
				    "NOT YET IMPLEMENTED...");
#endif /* OUT__XXX__OUT */

    if ((lastError = cgGetError()) != 0) {
	char *detailMsg = getErrorLog(ctxProperties, lastError);
	shaderError = createShaderError(env,
					javax_media_j3d_ShaderError_COMPILE_ERROR,
					"Cg shader compile error",
					detailMsg);
	if (detailMsg != NULL) {
	    free(detailMsg);
	}
    }

#else /* COMPILE_CG_SHADERS */

    shaderError = createShaderError(env,
				    javax_media_j3d_ShaderError_UNSUPPORTED_LANGUAGE_ERROR,
				    "CgShaderProgram support not compiled",
				    NULL);

#endif /* !COMPILE_CG_SHADERS */

    return shaderError;
}


/*
 * Class:     javax_media_j3d_CgShaderProgramRetained
 * Method:    createNativeShaderProgram
 * Signature: (J[J)Ljavax/media/j3d/ShaderError;
 */
JNIEXPORT jobject JNICALL
Java_javax_media_j3d_CgShaderProgramRetained_createNativeShaderProgram(
    JNIEnv *env,
    jobject obj,
    jlong ctxInfo,
    jlongArray shaderProgramIdArray)    
{
    jobject shaderError = NULL;

#ifdef COMPILE_CG_SHADERS

    jlong *shaderProgramIdPtr;

    CgShaderProgramInfo *shaderProgramInfo =
	(CgShaderProgramInfo*)malloc(sizeof(CgShaderProgramInfo));

    fprintf(stderr, "CgShaderProgramRetained.createNativeShaderProgram\n");

    shaderProgramInfo->numShaders = 0;
    shaderProgramInfo->shaders = NULL;

    shaderProgramIdPtr = (*env)->GetLongArrayElements(env, shaderProgramIdArray, NULL);
    shaderProgramIdPtr[0] = (jlong)shaderProgramInfo;
    (*env)->ReleaseLongArrayElements(env, shaderProgramIdArray, shaderProgramIdPtr, 0); 

#else /* COMPILE_CG_SHADERS */

    shaderError = createShaderError(env,
				    javax_media_j3d_ShaderError_UNSUPPORTED_LANGUAGE_ERROR,
				    "CgShaderProgram support not compiled",
				    NULL);

#endif /* !COMPILE_CG_SHADERS */

    return shaderError;
}


/*
 * Class:     javax_media_j3d_CgShaderProgramRetained
 * Method:    destroyNativeShaderProgram
 * Signature: (JJ)Ljavax/media/j3d/ShaderError;
 */
JNIEXPORT jobject JNICALL
Java_javax_media_j3d_CgShaderProgramRetained_destroyNativeShaderProgram(
    JNIEnv *env,
    jobject obj,
    jlong ctxInfo,
    jlong shaderProgramId)
{
    jobject shaderError = NULL;

#ifdef COMPILE_CG_SHADERS

    fprintf(stderr, "CgShaderProgramRetained.destroyNativeShaderProgram\n");

    /* TODO: implement this */

#else /* COMPILE_CG_SHADERS */

    shaderError = createShaderError(env,
				    javax_media_j3d_ShaderError_UNSUPPORTED_LANGUAGE_ERROR,
				    "CgShaderProgram support not compiled",
				    NULL);

#endif /* !COMPILE_CG_SHADERS */

    return shaderError;
}


/*
 * Class:     javax_media_j3d_CgShaderProgramRetained
 * Method:    linkNativeShaderProgram
 * Signature: (JJ[J)Ljavax/media/j3d/ShaderError;
 */
JNIEXPORT jobject JNICALL
Java_javax_media_j3d_CgShaderProgramRetained_linkNativeShaderProgram(
    JNIEnv *env,
    jobject obj,
    jlong ctxInfo,
    jlong shaderProgramId,
    jlongArray shaderIdArray)
{
    jobject shaderError = NULL;

#ifdef COMPILE_CG_SHADERS

    GraphicsContextPropertiesInfo* ctxProperties =  (GraphicsContextPropertiesInfo* )ctxInfo;
    CgCtxInfo *cgCtxInfo = ctxProperties->cgCtxInfo;

    jsize shaderIdArrayLength = (*env)->GetArrayLength(env,  shaderIdArray);
    jlong *shaderIdPtr = (*env)->GetLongArrayElements(env, shaderIdArray, NULL);
    CGerror lastError;

    int i;

    CgShaderProgramInfo *shaderProgramInfo = (CgShaderProgramInfo*)shaderProgramId;

    shaderProgramInfo->numShaders = shaderIdArrayLength;
    shaderProgramInfo->shaders =
	(CgShaderInfo**)malloc(shaderIdArrayLength * sizeof(CgShaderInfo*));

    fprintf(stderr, "CgShaderProgramRetained.linkNativeShaderProgram\n");

    for (i = 0; i < shaderIdArrayLength; i++) {
	shaderProgramInfo->shaders[i] = (CgShaderInfo*)shaderIdPtr[i];

	cgGLLoadProgram(shaderProgramInfo->shaders[i]->cgShader);

	if ((lastError = cgGetError()) != 0) {
	    char *detailMsg = getErrorLog(ctxProperties, lastError);
	    shaderError = createShaderError(env,
					    javax_media_j3d_ShaderError_LINK_ERROR,
					    "Cg shader link/load error",
					    detailMsg);
	    if (detailMsg != NULL) {
		free(detailMsg);
	    }
	}

	cgGLBindProgram(shaderProgramInfo->shaders[i]->cgShader);

	if ((lastError = cgGetError()) != 0) {
	    char *detailMsg = getErrorLog(ctxProperties, lastError);
	    shaderError = createShaderError(env,
					    javax_media_j3d_ShaderError_LINK_ERROR,
					    "Cg shader link/bind error",
					    detailMsg);
	    if (detailMsg != NULL) {
		free(detailMsg);
	    }
	}
    }

    (*env)->ReleaseLongArrayElements(env, shaderIdArray, shaderIdPtr, JNI_ABORT); 

#else /* COMPILE_CG_SHADERS */

    shaderError = createShaderError(env,
				    javax_media_j3d_ShaderError_UNSUPPORTED_LANGUAGE_ERROR,
				    "CgShaderProgram support not compiled",
				    NULL);

#endif /* !COMPILE_CG_SHADERS */

    return shaderError;
}


/*
 * Class:     javax_media_j3d_CgShaderProgramRetained
 * Method:    bindNativeVertexAttrName
 * Signature: (JJLjava/lang/String;I)Ljavax/media/j3d/ShaderError;
 */
JNIEXPORT jobject JNICALL
Java_javax_media_j3d_CgShaderProgramRetained_bindNativeVertexAttrName(
    JNIEnv * env,
    jobject obj,
    jlong ctxInfo,
    jlong shaderProgramId,
    jstring attrName,
    jint attrIndex)
{
    jobject shaderError = NULL;

#ifdef COMPILE_CG_SHADERS

    fprintf(stderr, "CgShaderProgramRetained.bindNativeVertexAttrName\n");

    /* TODO: implement this */

#else /* COMPILE_CG_SHADERS */

    shaderError = createShaderError(env,
				    javax_media_j3d_ShaderError_UNSUPPORTED_LANGUAGE_ERROR,
				    "CgShaderProgram support not compiled",
				    NULL);

#endif /* !COMPILE_CG_SHADERS */

    return shaderError;
}


/*
 * Class:     javax_media_j3d_CgShaderProgramRetained
 * Method:    lookupNativeShaderAttrName
 * Signature: (JJLjava/lang/String;[J)Ljavax/media/j3d/ShaderError;
 */
JNIEXPORT jobject JNICALL
Java_javax_media_j3d_CgShaderProgramRetained_lookupNativeShaderAttrName(
    JNIEnv *env,
    jobject obj,
    jlong ctxInfo,
    jlong shaderProgramId,
    jstring attrName,
    jlongArray locArr)
{
    jobject shaderError = NULL;

#ifdef COMPILE_CG_SHADERS

    fprintf(stderr, "CgShaderProgramRetained.lookupNativeShaderAttrName\n");

    /* TODO: implement this */

#else /* COMPILE_CG_SHADERS */

    shaderError = createShaderError(env,
				    javax_media_j3d_ShaderError_UNSUPPORTED_LANGUAGE_ERROR,
				    "CgShaderProgram support not compiled",
				    NULL);

#endif /* !COMPILE_CG_SHADERS */

    return shaderError;
}


/*
 * Class:     javax_media_j3d_CgShaderProgramRetained
 * Method:    useShaderProgram
 * Signature: (JJ)Ljavax/media/j3d/ShaderError;
 */
JNIEXPORT jobject JNICALL
Java_javax_media_j3d_CgShaderProgramRetained_useShaderProgram(
    JNIEnv *env,
    jobject obj,
    jlong ctxInfo,
    jlong shaderProgramId)
{
#ifdef COMPILE_CG_SHADERS

    GraphicsContextPropertiesInfo* ctxProperties =  (GraphicsContextPropertiesInfo* )ctxInfo;
    CgCtxInfo *cgCtxInfo = ctxProperties->cgCtxInfo;
    CGerror lastError;

    int i;

    CgShaderProgramInfo *shaderProgramInfo = (CgShaderProgramInfo*)shaderProgramId;

    cgGLDisableProfile(cgCtxInfo->vProfile);
    cgGLDisableProfile(cgCtxInfo->fProfile);

    if (shaderProgramId != 0) {
	for (i = 0; i < shaderProgramInfo->numShaders; i++) {
	    cgGLBindProgram(shaderProgramInfo->shaders[i]->cgShader);
	    cgGLEnableProfile(shaderProgramInfo->shaders[i]->shaderProfile);
	}
    }

#endif /* COMPILE_CG_SHADERS */

    return NULL;
}


/*
 * Class:     javax_media_j3d_CgShaderProgramRetained
 * Method:    setUniform1i
 * Signature: (JJJI)Ljavax/media/j3d/ShaderError;
 */
JNIEXPORT jobject JNICALL
Java_javax_media_j3d_CgShaderProgramRetained_setUniform1i(
    JNIEnv *env,
    jobject obj,
    jlong ctxInfo,
    jlong shaderProgramId,
    jlong location,
    jint value)
{
    jobject shaderError = NULL;

#ifdef COMPILE_CG_SHADERS

    /* TODO: implement this */

#else /* COMPILE_CG_SHADERS */

    shaderError = createShaderError(env,
				    javax_media_j3d_ShaderError_UNSUPPORTED_LANGUAGE_ERROR,
				    "CgShaderProgram support not compiled",
				    NULL);

#endif /* !COMPILE_CG_SHADERS */

    return shaderError;
}


/*
 * Class:     javax_media_j3d_CgShaderProgramRetained
 * Method:    setUniform1f
 * Signature: (JJJF)Ljavax/media/j3d/ShaderError;
 */
JNIEXPORT jobject JNICALL
Java_javax_media_j3d_CgShaderProgramRetained_setUniform1f(
    JNIEnv *env,
    jobject obj,
    jlong ctxInfo,
    jlong shaderProgramId,
    jlong location,
    jfloat value)
{
    jobject shaderError = NULL;

#ifdef COMPILE_CG_SHADERS

    /* TODO: implement this */

#else /* COMPILE_CG_SHADERS */

    shaderError = createShaderError(env,
				    javax_media_j3d_ShaderError_UNSUPPORTED_LANGUAGE_ERROR,
				    "CgShaderProgram support not compiled",
				    NULL);

#endif /* !COMPILE_CG_SHADERS */

    return shaderError;
}


/*
 * Class:     javax_media_j3d_CgShaderProgramRetained
 * Method:    setUniform2i
 * Signature: (JJJ[I)Ljavax/media/j3d/ShaderError;
 */
JNIEXPORT jobject JNICALL
Java_javax_media_j3d_CgShaderProgramRetained_setUniform2i(
    JNIEnv *env,
    jobject obj,
    jlong ctxInfo,
    jlong shaderProgramId,
    jlong location,
    jintArray varray)
{
    jobject shaderError = NULL;

#ifdef COMPILE_CG_SHADERS

    /* TODO: implement this */

#else /* COMPILE_CG_SHADERS */

    shaderError = createShaderError(env,
				    javax_media_j3d_ShaderError_UNSUPPORTED_LANGUAGE_ERROR,
				    "CgShaderProgram support not compiled",
				    NULL);

#endif /* !COMPILE_CG_SHADERS */

    return shaderError;
}


/*
 * Class:     javax_media_j3d_CgShaderProgramRetained
 * Method:    setUniform2f
 * Signature: (JJJ[F)Ljavax/media/j3d/ShaderError;
 */
JNIEXPORT jobject JNICALL
Java_javax_media_j3d_CgShaderProgramRetained_setUniform2f(
    JNIEnv *env,
    jobject obj,
    jlong ctxInfo,
    jlong shaderProgramId,
    jlong location,
    jfloatArray varray)
{
    jobject shaderError = NULL;

#ifdef COMPILE_CG_SHADERS

    /* TODO: implement this */

#else /* COMPILE_CG_SHADERS */

    shaderError = createShaderError(env,
				    javax_media_j3d_ShaderError_UNSUPPORTED_LANGUAGE_ERROR,
				    "CgShaderProgram support not compiled",
				    NULL);

#endif /* !COMPILE_CG_SHADERS */

    return shaderError;
}


/*
 * Class:     javax_media_j3d_CgShaderProgramRetained
 * Method:    setUniform3i
 * Signature: (JJJ[I)Ljavax/media/j3d/ShaderError;
 */
JNIEXPORT jobject JNICALL
Java_javax_media_j3d_CgShaderProgramRetained_setUniform3i(
    JNIEnv *env,
    jobject obj,
    jlong ctxInfo,
    jlong shaderProgramId,
    jlong location,
    jintArray varray)
{
    jobject shaderError = NULL;

#ifdef COMPILE_CG_SHADERS

    /* TODO: implement this */

#else /* COMPILE_CG_SHADERS */

    shaderError = createShaderError(env,
				    javax_media_j3d_ShaderError_UNSUPPORTED_LANGUAGE_ERROR,
				    "CgShaderProgram support not compiled",
				    NULL);

#endif /* !COMPILE_CG_SHADERS */

    return shaderError;
}


/*
 * Class:     javax_media_j3d_CgShaderProgramRetained
 * Method:    setUniform3f
 * Signature: (JJJ[F)Ljavax/media/j3d/ShaderError;
 */
JNIEXPORT jobject JNICALL
Java_javax_media_j3d_CgShaderProgramRetained_setUniform3f(
    JNIEnv *env,
    jobject obj,
    jlong ctxInfo,
    jlong shaderProgramId,
    jlong location,
    jfloatArray varray)
{
    jobject shaderError = NULL;

#ifdef COMPILE_CG_SHADERS

    /* TODO: implement this */

#else /* COMPILE_CG_SHADERS */

    shaderError = createShaderError(env,
				    javax_media_j3d_ShaderError_UNSUPPORTED_LANGUAGE_ERROR,
				    "CgShaderProgram support not compiled",
				    NULL);

#endif /* !COMPILE_CG_SHADERS */

    return shaderError;
}


/*
 * Class:     javax_media_j3d_CgShaderProgramRetained
 * Method:    setUniform4i
 * Signature: (JJJ[I)Ljavax/media/j3d/ShaderError;
 */
JNIEXPORT jobject JNICALL
Java_javax_media_j3d_CgShaderProgramRetained_setUniform4i(
    JNIEnv *env,
    jobject obj,
    jlong ctxInfo,
    jlong shaderProgramId,
    jlong location,
    jintArray varray)
{
    jobject shaderError = NULL;

#ifdef COMPILE_CG_SHADERS

    /* TODO: implement this */

#else /* COMPILE_CG_SHADERS */

    shaderError = createShaderError(env,
				    javax_media_j3d_ShaderError_UNSUPPORTED_LANGUAGE_ERROR,
				    "CgShaderProgram support not compiled",
				    NULL);

#endif /* !COMPILE_CG_SHADERS */

    return shaderError;
}


/*
 * Class:     javax_media_j3d_CgShaderProgramRetained
 * Method:    setUniform4f
 * Signature: (JJJ[F)Ljavax/media/j3d/ShaderError;
 */
JNIEXPORT jobject JNICALL
Java_javax_media_j3d_CgShaderProgramRetained_setUniform4f(
    JNIEnv *env,
    jobject obj,
    jlong ctxInfo,
    jlong shaderProgramId,
    jlong location,
    jfloatArray varray)
{
    jobject shaderError = NULL;

#ifdef COMPILE_CG_SHADERS

    /* TODO: implement this */

#else /* COMPILE_CG_SHADERS */

    shaderError = createShaderError(env,
				    javax_media_j3d_ShaderError_UNSUPPORTED_LANGUAGE_ERROR,
				    "CgShaderProgram support not compiled",
				    NULL);

#endif /* !COMPILE_CG_SHADERS */

    return shaderError;
}


/*
 * Class:     javax_media_j3d_CgShaderProgramRetained
 * Method:    setUniformMatrix3f
 * Signature: (JJJ[F)Ljavax/media/j3d/ShaderError;
 */
JNIEXPORT jobject JNICALL
Java_javax_media_j3d_CgShaderProgramRetained_setUniformMatrix3f(
    JNIEnv *env,
    jobject obj,
    jlong ctxInfo,
    jlong shaderProgramId,
    jlong location,
    jfloatArray varray)
{
    jobject shaderError = NULL;

#ifdef COMPILE_CG_SHADERS

    /* TODO: implement this */

#else /* COMPILE_CG_SHADERS */

    shaderError = createShaderError(env,
				    javax_media_j3d_ShaderError_UNSUPPORTED_LANGUAGE_ERROR,
				    "CgShaderProgram support not compiled",
				    NULL);

#endif /* !COMPILE_CG_SHADERS */

    return shaderError;
}


/*
 * Class:     javax_media_j3d_CgShaderProgramRetained
 * Method:    setUniformMatrix4f
 * Signature: (JJJ[F)Ljavax/media/j3d/ShaderError;
 */
JNIEXPORT jobject JNICALL
Java_javax_media_j3d_CgShaderProgramRetained_setUniformMatrix4f(
    JNIEnv *env,
    jobject obj,
    jlong ctxInfo,
    jlong shaderProgramId,
    jlong location,
    jfloatArray varray)
{
    jobject shaderError = NULL;

#ifdef COMPILE_CG_SHADERS

    /* TODO: implement this */

#else /* COMPILE_CG_SHADERS */

    shaderError = createShaderError(env,
				    javax_media_j3d_ShaderError_UNSUPPORTED_LANGUAGE_ERROR,
				    "CgShaderProgram support not compiled",
				    NULL);

#endif /* !COMPILE_CG_SHADERS */

    return shaderError;
}








#if 0
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

#endif /* 0 */
