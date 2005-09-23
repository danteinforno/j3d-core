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
#include "CgWrapper.h"

#if defined(UNIX)
#include <dlfcn.h>
#endif


extern char *strJavaToC(JNIEnv *env, jstring str);
extern void throwAssert(JNIEnv *env, char *str);
extern jobject createShaderError(JNIEnv *env,
				 int errorCode,
				 const char *errorMsg,
				 const char *detailMsg);

static void cgVertexAttrPointer(GraphicsContextPropertiesInfo *ctxProperties,
				int index, int size, int type, int stride,
				const void *pointer);
static void cgEnableVertexAttrArray(GraphicsContextPropertiesInfo *ctxProperties,
				    int index);
static void cgDisableVertexAttrArray(GraphicsContextPropertiesInfo *ctxProperties,
				     int index);
static void cgVertexAttr(GraphicsContextPropertiesInfo *ctxProperties,
			 int index, const float *v);


/* Global CG wrapper info struct, created by MasterControl during initialization */
static CgWrapperInfo *globalCgWrapperInfo = NULL;


/*
 * Class:     javax_media_j3d_MasterControl
 * Method:    loadNativeCgLibrary
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL
Java_javax_media_j3d_MasterControl_loadNativeCgLibrary(
    JNIEnv *env,
    jclass clazz,
    jobjectArray libpath)
{
    CgWrapperInfo *cgWrapperInfo;
    int i, pathLen;
    char *errName = NULL;

#ifdef WIN32
    DWORD err;
    LPTSTR errString;
    UINT origErrorMode;
#endif /* WIN32 */

    /*
     * This method is called exactly once to load and initialize the
     * CG wrapper library.
     */

    /* Assertion check that we don't get called more than once */
    if (globalCgWrapperInfo != NULL) {
	throwAssert(env, "MasterControl.loadNativeCgLibrary called more than once");
	return JNI_FALSE;
    }

    /* Allocate global Cg wrapper struct */
    cgWrapperInfo = (CgWrapperInfo*)malloc(sizeof(CgWrapperInfo));
    cgWrapperInfo->loaded = JNI_FALSE;
    cgWrapperInfo->cgLibraryHandle = NULL;

#ifdef COMPILE_CG_SHADERS

    /* Remove the following print statement when the native Cg code is done */
    fprintf(stderr, "*** JAVA 3D : loading experimental native Cg library\n");

    /* Get number of entries in libpath array */
    pathLen = (*env)->GetArrayLength(env, libpath);
    /*fprintf(stderr, "pathLen = %d\n", pathLen);*/

#ifdef UNIX

    for (i = 0; i < pathLen; i++) {
        jstring libname;
        char *libnameStr;

        libname = (*env)->GetObjectArrayElement(env, libpath, i);
        libnameStr = strJavaToC(env, libname);
        /*fprintf(stderr, "dlopen(%s)\n", libnameStr);*/
        cgWrapperInfo->cgLibraryHandle = dlopen(libnameStr, RTLD_LAZY);
        if ((cgWrapperInfo->cgLibraryHandle == NULL) && (i == pathLen-1)) {
            errName = strdup(libnameStr);
        }
        free(libnameStr);
        if (cgWrapperInfo->cgLibraryHandle != NULL) {
            break;
        }
    }

    if (cgWrapperInfo->cgLibraryHandle == NULL) {
        fprintf(stderr, "JAVA 3D ERROR : Unable to load library ");
        perror(errName);
        free(errName);
        free(cgWrapperInfo);
	return JNI_FALSE;
    }

    /* Get pointer to library function to setup function pointers */
    cgWrapperInfo->j3dLoadCgFunctionPointers =
	(PFNJ3DLOADCGFUNCTIONPOINTERS)dlsym(cgWrapperInfo->cgLibraryHandle,
					    "j3dLoadCgFunctionPointers");

#endif /* UNIX */

#ifdef WIN32

    /* Load the library, suppressing any dialog boxes that may occur */
    origErrorMode = SetErrorMode(SEM_NOOPENFILEERRORBOX |
				 SEM_FAILCRITICALERRORS);

    for (i = 0; i < pathLen; i++) {
        jstring libname;
        char *libnameStr;

        libname = (*env)->GetObjectArrayElement(env, libpath, i);
        libnameStr = strJavaToC(env, libname);
        /*fprintf(stderr, "LoadLibrary(%s)\n", libnameStr);*/
        cgWrapperInfo->cgLibraryHandle = LoadLibrary(libnameStr);
        if ((cgWrapperInfo->cgLibraryHandle == NULL) && (i == pathLen-1)) {
            errName = strdup(libnameStr);
        }
        free(libnameStr);
        if (cgWrapperInfo->cgLibraryHandle != NULL) {
            break;
        }
    }

    SetErrorMode(origErrorMode);

    if (cgWrapperInfo->cgLibraryHandle == NULL) {
	err = GetLastError();
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
		      FORMAT_MESSAGE_FROM_SYSTEM,
		      NULL, err, 0, (LPTSTR)&errString, 0, NULL);

	fprintf(stderr,
		"JAVA 3D ERROR : Unable to load library %s: %s\n",
		errName, errString);
        free(errName);
	return JNI_FALSE;
    }

    cgWrapperInfo->j3dLoadCgFunctionPointers =
	(PFNJ3DLOADCGFUNCTIONPOINTERS)GetProcAddress(
		(HMODULE)cgWrapperInfo->cgLibraryHandle,
		"j3dLoadCgFunctionPointers");

    if (cgWrapperInfo->j3dLoadCgFunctionPointers == NULL) {
	err = GetLastError();
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
		      FORMAT_MESSAGE_FROM_SYSTEM,
		      NULL, err, 0, (LPTSTR)&errString, 0, NULL);

	fprintf(stderr,
		"JAVA 3D ERROR : Unable to find: j3dLoadCgFunctionPointers: %s\n",
		errString);
	return JNI_FALSE;
    }

#endif /* WIN32 */

    if (cgWrapperInfo->j3dLoadCgFunctionPointers) {
	cgWrapperInfo->j3dLoadCgFunctionPointers(cgWrapperInfo);
	cgWrapperInfo->loaded = JNI_TRUE;
    }

#else /* COMPILE_CG_SHADERS */

    fprintf(stderr, "Java 3D: CgShaderProgram code not compiled\n");

#endif /* COMPILE_CG_SHADERS */

    /* Save pointer in global variable */
    globalCgWrapperInfo = cgWrapperInfo;

    return cgWrapperInfo->loaded;
}


#ifdef COMPILE_CG_SHADERS

static char *
getErrorLog(
    GraphicsContextPropertiesInfo* ctxProperties,
    CGerror lastError)
{
    CgCtxInfo *cgCtxInfo = ctxProperties->cgCtxInfo;
    CgWrapperInfo *cgWrapperInfo = cgCtxInfo->cgWrapperInfo;

    if (lastError != 0) {
	const char *errString = cgWrapperInfo->cgGetErrorString(lastError);
	const char *delimeter1 = "\n";
        const char *listing = cgWrapperInfo->cgGetLastListing(cgCtxInfo->cgCtx);

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

    fprintf(stderr, "Assertion error: assert(lastError != 0) failed\n");
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
    CgWrapperInfo *cgWrapperInfo;

    /* Assertion check that we don't get here unless the library is loaded */
    if (globalCgWrapperInfo == NULL) {
	throwAssert(env, "createCgShaderContext: cgWrapperInfo is NULL");
	return NULL;
    }

    if (!globalCgWrapperInfo->loaded) {
	throwAssert(env, "createCgShaderContext: cgWrapper library not loaded");
	return NULL;
    }

    cgCtxInfo = (CgCtxInfo*)malloc(sizeof(CgCtxInfo));
    if (cgCtxInfo == NULL) {
	if ((oom = (*env)->FindClass(env, "java/lang/OutOfMemoryError")) != NULL) {
	    (*env)->ThrowNew(env, oom, "malloc");
	}
	return NULL;
    }

    /* Point to the global CG wrapper info */
    cgWrapperInfo = cgCtxInfo->cgWrapperInfo = globalCgWrapperInfo;

    /* Create CG context */
    cgCtxInfo->cgCtx = cgWrapperInfo->cgCreateContext();

    if ((lastError = cgWrapperInfo->cgGetError()) != 0) {
	fprintf(stderr, "Fatal error in creating Cg context:\n");
	fprintf(stderr, "\t%s\n", cgWrapperInfo->cgGetErrorString(lastError));
	free(cgCtxInfo);
	return NULL;
    }

    if (cgCtxInfo->cgCtx == 0) {
	fprintf(stderr, "Invalid NULL Cg context\n");
	free(cgCtxInfo);
	return NULL;
    }

    /* Use GL_ARB_vertex_program extension if supported by video card */
    if (cgWrapperInfo->cgGLIsProfileSupported(CG_PROFILE_ARBVP1)) {
	fprintf(stderr, "Using CG_PROFILE_ARBVP1\n");
	cgCtxInfo->vProfile = CG_PROFILE_ARBVP1;
    }
    else if (cgWrapperInfo->cgGLIsProfileSupported(CG_PROFILE_VP20)) {
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

    if ((lastError = cgWrapperInfo->cgGetError()) != 0) {
	fprintf(stderr, "FATAL ERROR IN CREATING VERTEX SHADER PROFILE:\n");
	fprintf(stderr, "\t%s\n", cgWrapperInfo->cgGetErrorString(lastError));
	free(cgCtxInfo);
	return NULL;
    }

    /* Use GL_ARB_fragment_program extension if supported by video card */
    if (cgWrapperInfo->cgGLIsProfileSupported(CG_PROFILE_ARBFP1)) {
	fprintf(stderr, "Using CG_PROFILE_ARBFP1\n");
	cgCtxInfo->fProfile = CG_PROFILE_ARBFP1;
    }
    else if (cgWrapperInfo->cgGLIsProfileSupported(CG_PROFILE_FP20)) {
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

    if ((lastError = cgWrapperInfo->cgGetError()) != 0) {
	fprintf(stderr, "FATAL ERROR IN CREATING FRAGMENT SHADER PROFILE:\n");
	fprintf(stderr, "\t%s\n", cgWrapperInfo->cgGetErrorString(lastError));
	free(cgCtxInfo);
	return NULL;
    }

    /*
    fprintf(stderr, "createCgShaderContext: SUCCESS\n");
    fprintf(stderr, "    cgCtx = 0x%x\n", cgCtxInfo->cgCtx);
    fprintf(stderr, "    vProfile = 0x%x\n", cgCtxInfo->vProfile);
    fprintf(stderr, "    fProfile = 0x%x\n", cgCtxInfo->fProfile);
    */

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
	/* TODO: need to free ctxInfo->cgCtxInfo when ctxInfo is freed */
	ctxInfo->cgCtxInfo = createCgShaderContext(env, ctxInfo);
	if (ctxInfo->cgCtxInfo != NULL) {
	    /*
	    fprintf(stderr, "Cg ctx is available\n");
	    */
	    ctxInfo->shadingLanguageCg = JNI_TRUE;

	    /* Initialize shader vertex attribute function pointers */
	    ctxInfo->vertexAttrPointer = cgVertexAttrPointer;
	    ctxInfo->enableVertexAttrArray = cgEnableVertexAttrArray;
	    ctxInfo->disableVertexAttrArray = cgDisableVertexAttrArray;
	    ctxInfo->vertexAttr1fv = cgVertexAttr;
	    ctxInfo->vertexAttr2fv = cgVertexAttr;
	    ctxInfo->vertexAttr3fv = cgVertexAttr;
	    ctxInfo->vertexAttr4fv = cgVertexAttr;
	}
	/*
	else {
	    fprintf(stderr, "ERROR: Cg ctx *not* available\n");
	}
	*/
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
	fprintf(stderr, "shaderType = %d\n", shaderType);
	throwAssert(env, "unrecognized shaderType");
	return NULL;
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

    GraphicsContextPropertiesInfo* ctxProperties =  (GraphicsContextPropertiesInfo* )ctxInfo;
    CgCtxInfo *cgCtxInfo = ctxProperties->cgCtxInfo;
    CgWrapperInfo *cgWrapperInfo = cgCtxInfo->cgWrapperInfo;

    CgShaderInfo *cgShaderInfo = (CgShaderInfo *)shaderId;

    fprintf(stderr, "CgShaderProgramRetained.destroyNativeShader\n");

    if (cgShaderInfo != NULL) {
	if (cgShaderInfo->cgShader != 0) {
	    cgWrapperInfo->cgDestroyProgram(cgShaderInfo->cgShader);
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
    CgWrapperInfo *cgWrapperInfo = cgCtxInfo->cgWrapperInfo;

    CgShaderInfo *cgShaderInfo = (CgShaderInfo *)shaderId;
    CGerror lastError;
    GLcharARB *shaderString = NULL;

    fprintf(stderr, "CgShaderProgramRetained.compileNativeShader\n");

    /* Assertion check the cgShaderInfo pointer */
    if (cgShaderInfo == NULL) {
	throwAssert(env, "cgShaderInfo is NULL");
	return NULL;
    }

    /* Assertion check the program string */
    if (program == NULL) {
	throwAssert(env, "shader program string is NULL");
	return NULL;
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
    cgShaderInfo->cgShader = cgWrapperInfo->cgCreateProgram(cgCtxInfo->cgCtx,
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

    if ((lastError = cgWrapperInfo->cgGetError()) != 0) {
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

    shaderProgramInfo->vShader = NULL;
    shaderProgramInfo->fShader = NULL;
    shaderProgramInfo->numVtxAttrs = 0;
    shaderProgramInfo->vtxAttrs = NULL;

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

    CgShaderProgramInfo *shaderProgramInfo = (CgShaderProgramInfo*)shaderProgramId;

    fprintf(stderr, "CgShaderProgramRetained.destroyNativeShaderProgram\n");

    if (shaderProgramInfo != NULL) {
	if (shaderProgramInfo->vtxAttrs != NULL) {
	    free(shaderProgramInfo->vtxAttrs);
	    shaderProgramInfo->vtxAttrs = NULL;
	}
	free(shaderProgramInfo);
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
    CgWrapperInfo *cgWrapperInfo = cgCtxInfo->cgWrapperInfo;

    jsize shaderIdArrayLength = (*env)->GetArrayLength(env,  shaderIdArray);
    jlong *shaderIdPtr = (*env)->GetLongArrayElements(env, shaderIdArray, NULL);
    CGerror lastError;

    int i;

    CgShaderProgramInfo *shaderProgramInfo = (CgShaderProgramInfo*)shaderProgramId;

    fprintf(stderr, "CgShaderProgramRetained.linkNativeShaderProgram\n");

    /*
     * NOTE: we assume that the caller has already verified that there
     * is at most one vertex program and one fragment program
     */
    shaderProgramInfo->vShader = NULL;
    shaderProgramInfo->fShader = NULL;
    for (i = 0; i < shaderIdArrayLength; i++) {
	CgShaderInfo *shader = (CgShaderInfo*)shaderIdPtr[i];
	if (shader->shaderType == javax_media_j3d_Shader_SHADER_TYPE_VERTEX) {
	    shaderProgramInfo->vShader = shader;
	} else {
	    shaderProgramInfo->fShader = shader;
	}

	cgWrapperInfo->cgGLLoadProgram(shader->cgShader);

	if ((lastError = cgWrapperInfo->cgGetError()) != 0) {
	    char *detailMsg = getErrorLog(ctxProperties, lastError);
	    shaderError = createShaderError(env,
					    javax_media_j3d_ShaderError_LINK_ERROR,
					    "Cg shader link/load error",
					    detailMsg);
	    if (detailMsg != NULL) {
		free(detailMsg);
	    }
	}

	cgWrapperInfo->cgGLBindProgram(shader->cgShader);

	if ((lastError = cgWrapperInfo->cgGetError()) != 0) {
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
 * Method:    lookupNativeVertexAttrNames
 * Signature: (JJI[Ljava/lang/String;[Z)V
 */
JNIEXPORT void JNICALL
Java_javax_media_j3d_CgShaderProgramRetained_lookupNativeVertexAttrNames(
    JNIEnv *env,
    jobject obj,
    jlong ctxInfo,
    jlong shaderProgramId,
    jint numAttrNames,
    jobjectArray attrNames,
    jbooleanArray errArr)
{
#ifdef COMPILE_CG_SHADERS

    GraphicsContextPropertiesInfo* ctxProperties =  (GraphicsContextPropertiesInfo* )ctxInfo;
    CgCtxInfo *cgCtxInfo = ctxProperties->cgCtxInfo;
    CgWrapperInfo *cgWrapperInfo = cgCtxInfo->cgWrapperInfo;

    CgShaderProgramInfo *shaderProgramInfo = (CgShaderProgramInfo*)shaderProgramId;

    int i;
    jstring attrName;
    char *attrNameString;
    jboolean *errPtr;

    errPtr = (*env)->GetBooleanArrayElements(env, errArr, NULL);

    if (shaderProgramInfo->vShader == NULL) {
	/* If there if no vertex shader, no attributes can be looked up, so all fail */
	for (i = 0; i < numAttrNames; i++) {
	    errPtr[i] = JNI_TRUE;
	}
	(*env)->ReleaseBooleanArrayElements(env, errArr, errPtr, 0);
	return;
    }

    shaderProgramInfo->numVtxAttrs = numAttrNames;
    shaderProgramInfo->vtxAttrs = (CGparameter *)malloc(numAttrNames * sizeof(CGparameter));

    fprintf(stderr, "CgShaderProgramRetained.lookupNativeVertexAttrNames()\n");
    for (i = 0; i < numAttrNames; i++) {
        attrName = (*env)->GetObjectArrayElement(env, attrNames, i);
        attrNameString = strJavaToC(env, attrName);

	shaderProgramInfo->vtxAttrs[i] =
	    cgWrapperInfo->cgGetNamedParameter(shaderProgramInfo->vShader->cgShader,
					       attrNameString);
	fprintf(stderr, "    %s : 0x%x\n", attrNameString, shaderProgramInfo->vtxAttrs[i]);
	if (shaderProgramInfo->vtxAttrs[i] == NULL) {
	    errPtr[i] = JNI_TRUE;
	}

        free(attrNameString);
    }

    (*env)->ReleaseBooleanArrayElements(env, errArr, errPtr, 0);

#endif /* COMPILE_CG_SHADERS */
}


#ifdef COMPILE_CG_SHADERS

static jint
cgToJ3dType(CGtype type)
{
    switch (type) {
    case CG_BOOL:
    case CG_BOOL1:
    case CG_FIXED:
    case CG_FIXED1:
    case CG_HALF:
    case CG_HALF1:
    case CG_INT:
    case CG_INT1:
	return TYPE_INTEGER;

    /*
     * TODO: add ShaderAttribute support for setting samplers. In the
     * mean time, the binding between sampler and texture unit will
     * need to be specified in the shader itself (which it already is
     * in most example shaders).
     *
     * case CG_SAMPLER2D:
     * case CG_SAMPLER3D:
     * case CG_SAMPLERCUBE:
     *
     */

    case CG_BOOL2:
    case CG_FIXED2:
    case CG_HALF2:
    case CG_INT2:
	/* TODO: return TYPE_TUPLE2I; */
	return -1;

    case CG_BOOL3:
    case CG_FIXED3:
    case CG_HALF3:
    case CG_INT3:
	/*TODO: return TYPE_TUPLE3I; */
	return -1;

    case CG_BOOL4:
    case CG_FIXED4:
    case CG_HALF4:
    case CG_INT4:
	/*TODO: return TYPE_TUPLE4I; */
	return -1;

    case CG_FLOAT:
    case CG_FLOAT1:
	return TYPE_FLOAT;

    case CG_FLOAT2:
	/*TODO: return TYPE_TUPLE2F; */
	return -1;

    case CG_FLOAT3:
	/*TODO: return TYPE_TUPLE3F; */
	return -1;

    case CG_FLOAT4:
	/*TODO: return TYPE_TUPLE4F; */
	return -1;

    case CG_FLOAT3x3:
	/*TODO: return TYPE_MATRIX3F; */
	return -1;

    case CG_FLOAT4x4:
	/*TODO: return TYPE_MATRIX4F; */
	return -1;

    /*
     * Java 3D does not support the following sampler types:
     *
     * case CG_SAMPLER1D:
     * case CG_SAMPLERRECT:
     */
    }

    return -1;
}

static CGparameter
lookupParams(
    CgWrapperInfo *cgWrapperInfo,
    CgShaderInfo *shader,
    char *attrNameString,
    CGtype *type,
    int *size,
    jboolean *isArray)
{
    CGparameter loc;
    CGparameter firstElem;

    loc = cgWrapperInfo->cgGetNamedParameter(shader->cgShader,
					     attrNameString);
    if (loc != NULL) {
	*type = cgWrapperInfo->cgGetParameterType(loc);
	if (*type == CG_ARRAY) {
	    *isArray = JNI_TRUE;
	    *size = cgWrapperInfo->cgGetArraySize(loc, 0);
	    /**type = cgWrapperInfo->cgGetArrayType(loc);*/
	    firstElem = cgWrapperInfo->cgGetArrayParameter(loc, 0);
	    *type = cgWrapperInfo->cgGetParameterType(firstElem);
	    /*
	      fprintf(stderr,
	      "firstElem = %d, *type = %d\n",
	      firstElem, *type);
	    */
	}
	else {
	    *isArray = JNI_FALSE;
	    *size = 1;
	}
    }

    return loc;
}

#endif /* COMPILE_CG_SHADERS */


/*
 * Class:     javax_media_j3d_CgShaderProgramRetained
 * Method:    lookupNativeShaderAttrNames
 * Signature: (JJI[Ljava/lang/String;[J[I[I[Z)V
 */
JNIEXPORT void JNICALL
Java_javax_media_j3d_CgShaderProgramRetained_lookupNativeShaderAttrNames(
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

#ifdef COMPILE_CG_SHADERS

    GraphicsContextPropertiesInfo* ctxProperties =  (GraphicsContextPropertiesInfo* )ctxInfo;
    CgCtxInfo *cgCtxInfo = ctxProperties->cgCtxInfo;
    CgWrapperInfo *cgWrapperInfo = cgCtxInfo->cgWrapperInfo;
    CgShaderProgramInfo *shaderProgramInfo = (CgShaderProgramInfo*)shaderProgramId;

    jstring attrName;
    char *attrNameString;
    jlong *locPtr;
    jint *typePtr;
    jint *sizePtr;
    jboolean *isArrayPtr;
    CGparameter vLoc, fLoc;
    CGtype vType, fType;
    int vSize, fSize;
    jboolean vIsArray, fIsArray;
    int i;

    locPtr = (*env)->GetLongArrayElements(env, locArr, NULL);
    typePtr = (*env)->GetIntArrayElements(env, typeArr, NULL);
    sizePtr = (*env)->GetIntArrayElements(env, sizeArr, NULL);
    isArrayPtr = (*env)->GetBooleanArrayElements(env, isArrayArr, NULL);

    /*
     * Set the loc, type, and size arrays to out-of-band values
     */
    for (i = 0; i < numAttrNames; i++) {
	locPtr[i] = -1;
	typePtr[i] = -1;
	sizePtr[i] = -1;
    }

    /* Now lookup the location of each name in the attrNames array */
    for (i = 0; i < numAttrNames; i++) {
        int j;
	jboolean err;
	CgParameterInfo *cgParamInfo;

        attrName = (*env)->GetObjectArrayElement(env, attrNames, i);
        attrNameString = (GLcharARB *)strJavaToC(env, attrName);

	fprintf(stderr, "lookup %s\n", attrNameString);

	/*
	 * Get uniform attribute location -- note that we need to
	 * lookup the name in both the vertex and fragment shader
	 * (although we will generalize it to look at the list of "N"
	 * shaders). If all parameter locations are NULL, then no
	 * struct will be allocated and -1 will be stored for this
	 * attribute. If there is more than one non-NULL parameter,
	 * then all must be of the same type and dimensionality,
	 * otherwise an error will be generated and -1 will be stored
	 * for this attribute.  If all non-NULL parameters are of the
	 * same type and dimensionality, then a struct is allocated
	 * containing the list of parameters.
	 *
	 * When any of the setUniform methods are called, the attribute
	 * will be set for each parameter in the list.
	 */
	cgParamInfo = (CgParameterInfo *)malloc(sizeof(CgParameterInfo));
	cgParamInfo->vParam = NULL;
	cgParamInfo->fParam = NULL;
	err = JNI_FALSE;

	vLoc = NULL;
	if (shaderProgramInfo->vShader != NULL) {
	    vLoc = lookupParams(cgWrapperInfo, shaderProgramInfo->vShader,
				attrNameString, &vType, &vSize, &vIsArray);
	    cgParamInfo->vParam = vLoc;
	    if (vLoc != NULL) {
		sizePtr[i] = (jint)vSize;
		isArrayPtr[i] = vIsArray;
		typePtr[i] = cgToJ3dType(vType);

		fprintf(stderr, "    vLoc = %d, vType = %d, vSize = %d, vIsArray = %d\n",
			vLoc, vType, vSize, vIsArray);
	    }
	}

	fLoc = NULL;
	if (shaderProgramInfo->fShader != NULL) {
	    fLoc = lookupParams(cgWrapperInfo, shaderProgramInfo->fShader,
				attrNameString, &fType, &fSize, &fIsArray);
	    cgParamInfo->fParam = fLoc;
	    if (fLoc != NULL) {
		sizePtr[i] = (jint)fSize;
		isArrayPtr[i] = fIsArray;
		typePtr[i] = cgToJ3dType(fType);

		fprintf(stderr, "    fLoc = %d, fType = %d, fSize = %d, fIsArray = %d\n",
			fLoc, fType, fSize, fIsArray);
	    }
	}

	/*
	 * If the name lookup found an entry in both vertex and
	 * fragment program, verify that the type and size are the
	 * same.
	 */
	if (cgParamInfo->vParam != NULL && cgParamInfo->fParam != NULL) {
	    if (vType != fType || vSize != fSize || vIsArray != fIsArray) {
		/* TODO: the following needs to be propagated to ShaderError */
		fprintf(stderr,
			"JAVA 3D : error shader attribute type mismatch: %s\n",
			attrNameString);
		fprintf(stderr,
			"    1 : type = %d, size = %d, isArray = %d\n",
			vType, vSize, vIsArray);
		fprintf(stderr,
			"    0 : type = %d, size = %d, isArray = %d\n",
			fType, fSize, fIsArray);
		err = JNI_TRUE;
	    }
	}

	/*
	 * Report an error if we got a mismatch or if the attribute
	 * was not found in either the vertex or the fragment program
	 */
        if (err || (cgParamInfo->vParam == NULL && cgParamInfo->fParam == NULL)) {
	    /* TODO: distinguish between (err) and (vParam and fParam both NULL) */
	    free(cgParamInfo);
	    locPtr[i] = (jlong)-1;
        }
	else {
	    /*
	     * TODO: need to store the cgParamInfo pointers in the
	     * shader program so we can free them later.
	     *
	     * NOTE: WE CURRENTLY HAVE A MEMORY LEAK.
	     */
	    locPtr[i] = (jlong)cgParamInfo;
	}

        free(attrNameString);
    }

    /* Release JNI arrays */
    (*env)->ReleaseLongArrayElements(env, locArr, locPtr, 0);
    (*env)->ReleaseIntArrayElements(env, typeArr, typePtr, 0);
    (*env)->ReleaseIntArrayElements(env, sizeArr, sizePtr, 0);
    (*env)->ReleaseBooleanArrayElements(env, isArrayArr, isArrayPtr, 0);

#endif /* COMPILE_CG_SHADERS */

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
    CgWrapperInfo *cgWrapperInfo = cgCtxInfo->cgWrapperInfo;

    CgShaderProgramInfo *shaderProgramInfo = (CgShaderProgramInfo*)shaderProgramId;

    cgWrapperInfo->cgGLDisableProfile(cgCtxInfo->vProfile);
    cgWrapperInfo->cgGLDisableProfile(cgCtxInfo->fProfile);

    if (shaderProgramId != 0) {
	if (shaderProgramInfo->vShader != NULL) {
	    cgWrapperInfo->cgGLBindProgram(shaderProgramInfo->vShader->cgShader);
	    cgWrapperInfo->cgGLEnableProfile(shaderProgramInfo->vShader->shaderProfile);
	}

	if (shaderProgramInfo->fShader != NULL) {
	    cgWrapperInfo->cgGLBindProgram(shaderProgramInfo->fShader->cgShader);
	    cgWrapperInfo->cgGLEnableProfile(shaderProgramInfo->fShader->shaderProfile);
	}
    }
    else {
	/* TODO: Unbind old shader program */
    }

    ctxProperties->shaderProgramId = shaderProgramId;

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

    GraphicsContextPropertiesInfo* ctxProperties =  (GraphicsContextPropertiesInfo* )ctxInfo;
    CgCtxInfo *cgCtxInfo = ctxProperties->cgCtxInfo;
    CgWrapperInfo *cgWrapperInfo = cgCtxInfo->cgWrapperInfo;
    CgShaderProgramInfo *shaderProgramInfo = (CgShaderProgramInfo*)shaderProgramId;
    CgParameterInfo *cgParamInfo = (CgParameterInfo *)location;

    if (cgParamInfo->vParam != NULL) {
	cgWrapperInfo->cgSetParameter1f(cgParamInfo->vParam, value);
    }

    if (cgParamInfo->fParam != NULL) {
	cgWrapperInfo->cgSetParameter1f(cgParamInfo->fParam, value);
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


/*
 * Cg vertex attribute functions
 */

static void
cgVertexAttrPointer(
    GraphicsContextPropertiesInfo *ctxProperties,
    int index, int size, int type, int stride,
    const void *pointer)
{
    /* TODO: implement this */
    fprintf(stderr, "cgVertexAttrPointer: not implemented\n");
}

static void
cgEnableVertexAttrArray(
    GraphicsContextPropertiesInfo *ctxProperties,
    int index)
{
    /* TODO: implement this */
    fprintf(stderr, "cgEnableVertexAttrArray: not implemented\n");
}

static void
cgDisableVertexAttrArray(
    GraphicsContextPropertiesInfo *ctxProperties,
    int index)
{
    /* TODO: implement this */
    fprintf(stderr, "cgDisableVertexAttrArray: not implemented\n");
}

static void
cgVertexAttr(
    GraphicsContextPropertiesInfo *ctxProperties,
    int index, const float *v)
{
    /*
     * NOTE: we should never get here. This function is only called
     * when building display lists for geometry arrays with vertex
     * attributes, and such display lists are disabled in Cg mode.
     */
    fprintf(stderr,
	    "Java 3D ERROR : Assertion failed: invalid call to cgVertexAttr*f\n");
}


#if 0


/*
 * Class:     javax_media_j3d_CgShaderProgramRetained
 * Method:    setUniform1iArray
 * Signature: (JJJI[I)Ljavax/media/j3d/ShaderError;
 */
JNIEXPORT jobject JNICALL Java_javax_media_j3d_CgShaderProgramRetained_setUniform1iArray
  (JNIEnv *, jobject, jlong, jlong, jlong, jint, jintArray);

/*
 * Class:     javax_media_j3d_CgShaderProgramRetained
 * Method:    setUniform1fArray
 * Signature: (JJJI[F)Ljavax/media/j3d/ShaderError;
 */
JNIEXPORT jobject JNICALL Java_javax_media_j3d_CgShaderProgramRetained_setUniform1fArray
  (JNIEnv *, jobject, jlong, jlong, jlong, jint, jfloatArray);

/*
 * Class:     javax_media_j3d_CgShaderProgramRetained
 * Method:    setUniform2iArray
 * Signature: (JJJI[I)Ljavax/media/j3d/ShaderError;
 */
JNIEXPORT jobject JNICALL Java_javax_media_j3d_CgShaderProgramRetained_setUniform2iArray
  (JNIEnv *, jobject, jlong, jlong, jlong, jint, jintArray);

/*
 * Class:     javax_media_j3d_CgShaderProgramRetained
 * Method:    setUniform2fArray
 * Signature: (JJJI[F)Ljavax/media/j3d/ShaderError;
 */
JNIEXPORT jobject JNICALL Java_javax_media_j3d_CgShaderProgramRetained_setUniform2fArray
  (JNIEnv *, jobject, jlong, jlong, jlong, jint, jfloatArray);

/*
 * Class:     javax_media_j3d_CgShaderProgramRetained
 * Method:    setUniform3iArray
 * Signature: (JJJI[I)Ljavax/media/j3d/ShaderError;
 */
JNIEXPORT jobject JNICALL Java_javax_media_j3d_CgShaderProgramRetained_setUniform3iArray
  (JNIEnv *, jobject, jlong, jlong, jlong, jint, jintArray);

/*
 * Class:     javax_media_j3d_CgShaderProgramRetained
 * Method:    setUniform3fArray
 * Signature: (JJJI[F)Ljavax/media/j3d/ShaderError;
 */
JNIEXPORT jobject JNICALL Java_javax_media_j3d_CgShaderProgramRetained_setUniform3fArray
  (JNIEnv *, jobject, jlong, jlong, jlong, jint, jfloatArray);

/*
 * Class:     javax_media_j3d_CgShaderProgramRetained
 * Method:    setUniform4iArray
 * Signature: (JJJI[I)Ljavax/media/j3d/ShaderError;
 */
JNIEXPORT jobject JNICALL Java_javax_media_j3d_CgShaderProgramRetained_setUniform4iArray
  (JNIEnv *, jobject, jlong, jlong, jlong, jint, jintArray);

/*
 * Class:     javax_media_j3d_CgShaderProgramRetained
 * Method:    setUniform4fArray
 * Signature: (JJJI[F)Ljavax/media/j3d/ShaderError;
 */
JNIEXPORT jobject JNICALL Java_javax_media_j3d_CgShaderProgramRetained_setUniform4fArray
  (JNIEnv *, jobject, jlong, jlong, jlong, jint, jfloatArray);

/*
 * Class:     javax_media_j3d_CgShaderProgramRetained
 * Method:    setUniformMatrix3fArray
 * Signature: (JJJI[F)Ljavax/media/j3d/ShaderError;
 */
JNIEXPORT jobject JNICALL Java_javax_media_j3d_CgShaderProgramRetained_setUniformMatrix3fArray
  (JNIEnv *, jobject, jlong, jlong, jlong, jint, jfloatArray);

/*
 * Class:     javax_media_j3d_CgShaderProgramRetained
 * Method:    setUniformMatrix4fArray
 * Signature: (JJJI[F)Ljavax/media/j3d/ShaderError;
 */
JNIEXPORT jobject JNICALL Java_javax_media_j3d_CgShaderProgramRetained_setUniformMatrix4fArray
  (JNIEnv *, jobject, jlong, jlong, jlong, jint, jfloatArray);



#endif





#if 0
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