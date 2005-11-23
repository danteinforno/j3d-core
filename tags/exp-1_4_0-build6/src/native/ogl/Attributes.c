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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <jni.h>

#include "gldefs.h"

#ifdef DEBUG
/* Uncomment the following for VERBOSE debug messages */
/* #define VERBOSE */
#endif /* DEBUG */


/*
 * Screen door transparency table.
 */
const unsigned int screen_door[17][32] = {
/* 0 / 16 */
    {
        0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000,
    },
/* 1 / 16 */
    {
        0x00000000, 0x22222222, 0x00000000, 0x00000000,
        0x00000000, 0x22222222, 0x00000000, 0x00000000,
        0x00000000, 0x22222222, 0x00000000, 0x00000000,
        0x00000000, 0x22222222, 0x00000000, 0x00000000,
        0x00000000, 0x22222222, 0x00000000, 0x00000000,
        0x00000000, 0x22222222, 0x00000000, 0x00000000,
        0x00000000, 0x22222222, 0x00000000, 0x00000000,
        0x00000000, 0x22222222, 0x00000000, 0x00000000,
    },
/* 2 / 16 */
    {
        0x00000000, 0x22222222, 0x00000000, 0x88888888,
        0x00000000, 0x22222222, 0x00000000, 0x88888888,
        0x00000000, 0x22222222, 0x00000000, 0x88888888,
        0x00000000, 0x22222222, 0x00000000, 0x88888888,
        0x00000000, 0x22222222, 0x00000000, 0x88888888,
        0x00000000, 0x22222222, 0x00000000, 0x88888888,
        0x00000000, 0x22222222, 0x00000000, 0x88888888,
        0x00000000, 0x22222222, 0x00000000, 0x88888888,
    },
/* 3 / 16 */
    {
        0x00000000, 0xaaaaaaaa, 0x00000000, 0x88888888,
        0x00000000, 0xaaaaaaaa, 0x00000000, 0x88888888,
        0x00000000, 0xaaaaaaaa, 0x00000000, 0x88888888,
        0x00000000, 0xaaaaaaaa, 0x00000000, 0x88888888,
        0x00000000, 0xaaaaaaaa, 0x00000000, 0x88888888,
        0x00000000, 0xaaaaaaaa, 0x00000000, 0x88888888,
        0x00000000, 0xaaaaaaaa, 0x00000000, 0x88888888,
        0x00000000, 0xaaaaaaaa, 0x00000000, 0x88888888,
    },
/* 4 / 16 */
    {
        0x00000000, 0xaaaaaaaa, 0x00000000, 0xaaaaaaaa,
        0x00000000, 0xaaaaaaaa, 0x00000000, 0xaaaaaaaa,
        0x00000000, 0xaaaaaaaa, 0x00000000, 0xaaaaaaaa,
        0x00000000, 0xaaaaaaaa, 0x00000000, 0xaaaaaaaa,
        0x00000000, 0xaaaaaaaa, 0x00000000, 0xaaaaaaaa,
        0x00000000, 0xaaaaaaaa, 0x00000000, 0xaaaaaaaa,
        0x00000000, 0xaaaaaaaa, 0x00000000, 0xaaaaaaaa,
        0x00000000, 0xaaaaaaaa, 0x00000000, 0xaaaaaaaa,
    },
/* 5 / 16 */
    {
        0x11111111, 0xaaaaaaaa, 0x00000000, 0xaaaaaaaa,
        0x11111111, 0xaaaaaaaa, 0x00000000, 0xaaaaaaaa,
        0x11111111, 0xaaaaaaaa, 0x00000000, 0xaaaaaaaa,
        0x11111111, 0xaaaaaaaa, 0x00000000, 0xaaaaaaaa,
        0x11111111, 0xaaaaaaaa, 0x00000000, 0xaaaaaaaa,
        0x11111111, 0xaaaaaaaa, 0x00000000, 0xaaaaaaaa,
        0x11111111, 0xaaaaaaaa, 0x00000000, 0xaaaaaaaa,
        0x11111111, 0xaaaaaaaa, 0x00000000, 0xaaaaaaaa,
    },
/* 6 / 16 */
    {
        0x11111111, 0xaaaaaaaa, 0x44444444, 0xaaaaaaaa,
        0x11111111, 0xaaaaaaaa, 0x44444444, 0xaaaaaaaa,
        0x11111111, 0xaaaaaaaa, 0x44444444, 0xaaaaaaaa,
        0x11111111, 0xaaaaaaaa, 0x44444444, 0xaaaaaaaa,
        0x11111111, 0xaaaaaaaa, 0x44444444, 0xaaaaaaaa,
        0x11111111, 0xaaaaaaaa, 0x44444444, 0xaaaaaaaa,
        0x11111111, 0xaaaaaaaa, 0x44444444, 0xaaaaaaaa,
        0x11111111, 0xaaaaaaaa, 0x44444444, 0xaaaaaaaa,
    },
/* 7 / 16 */
    {
        0x55555555, 0xaaaaaaaa, 0x44444444, 0xaaaaaaaa,
        0x55555555, 0xaaaaaaaa, 0x44444444, 0xaaaaaaaa,
        0x55555555, 0xaaaaaaaa, 0x44444444, 0xaaaaaaaa,
        0x55555555, 0xaaaaaaaa, 0x44444444, 0xaaaaaaaa,
        0x55555555, 0xaaaaaaaa, 0x44444444, 0xaaaaaaaa,
        0x55555555, 0xaaaaaaaa, 0x44444444, 0xaaaaaaaa,
        0x55555555, 0xaaaaaaaa, 0x44444444, 0xaaaaaaaa,
        0x55555555, 0xaaaaaaaa, 0x44444444, 0xaaaaaaaa,
    },
/* 8 / 16 */
    {
        0x55555555, 0xaaaaaaaa, 0x55555555, 0xaaaaaaaa,
        0x55555555, 0xaaaaaaaa, 0x55555555, 0xaaaaaaaa,
        0x55555555, 0xaaaaaaaa, 0x55555555, 0xaaaaaaaa,
        0x55555555, 0xaaaaaaaa, 0x55555555, 0xaaaaaaaa,
        0x55555555, 0xaaaaaaaa, 0x55555555, 0xaaaaaaaa,
        0x55555555, 0xaaaaaaaa, 0x55555555, 0xaaaaaaaa,
        0x55555555, 0xaaaaaaaa, 0x55555555, 0xaaaaaaaa,
        0x55555555, 0xaaaaaaaa, 0x55555555, 0xaaaaaaaa,
    },
/* 9 / 16 */
    {
        0x77777777, 0xaaaaaaaa, 0x55555555, 0xaaaaaaaa,
        0x77777777, 0xaaaaaaaa, 0x55555555, 0xaaaaaaaa,
        0x77777777, 0xaaaaaaaa, 0x55555555, 0xaaaaaaaa,
        0x77777777, 0xaaaaaaaa, 0x55555555, 0xaaaaaaaa,
        0x77777777, 0xaaaaaaaa, 0x55555555, 0xaaaaaaaa,
        0x77777777, 0xaaaaaaaa, 0x55555555, 0xaaaaaaaa,
        0x77777777, 0xaaaaaaaa, 0x55555555, 0xaaaaaaaa,
        0x77777777, 0xaaaaaaaa, 0x55555555, 0xaaaaaaaa,
    },
/* 10 / 16 */
    {
        0x77777777, 0xaaaaaaaa, 0xdddddddd, 0xaaaaaaaa,
        0x77777777, 0xaaaaaaaa, 0xdddddddd, 0xaaaaaaaa,
        0x77777777, 0xaaaaaaaa, 0xdddddddd, 0xaaaaaaaa,
        0x77777777, 0xaaaaaaaa, 0xdddddddd, 0xaaaaaaaa,
        0x77777777, 0xaaaaaaaa, 0xdddddddd, 0xaaaaaaaa,
        0x77777777, 0xaaaaaaaa, 0xdddddddd, 0xaaaaaaaa,
        0x77777777, 0xaaaaaaaa, 0xdddddddd, 0xaaaaaaaa,
        0x77777777, 0xaaaaaaaa, 0xdddddddd, 0xaaaaaaaa,
    },
/* 11 / 16 */
    {
        0xffffffff, 0xaaaaaaaa, 0xdddddddd, 0xaaaaaaaa,
        0xffffffff, 0xaaaaaaaa, 0xdddddddd, 0xaaaaaaaa,
        0xffffffff, 0xaaaaaaaa, 0xdddddddd, 0xaaaaaaaa,
        0xffffffff, 0xaaaaaaaa, 0xdddddddd, 0xaaaaaaaa,
        0xffffffff, 0xaaaaaaaa, 0xdddddddd, 0xaaaaaaaa,
        0xffffffff, 0xaaaaaaaa, 0xdddddddd, 0xaaaaaaaa,
        0xffffffff, 0xaaaaaaaa, 0xdddddddd, 0xaaaaaaaa,
        0xffffffff, 0xaaaaaaaa, 0xdddddddd, 0xaaaaaaaa,
    },
/* 12 / 16 */
    {
        0xffffffff, 0xaaaaaaaa, 0xffffffff, 0xaaaaaaaa,
        0xffffffff, 0xaaaaaaaa, 0xffffffff, 0xaaaaaaaa,
        0xffffffff, 0xaaaaaaaa, 0xffffffff, 0xaaaaaaaa,
        0xffffffff, 0xaaaaaaaa, 0xffffffff, 0xaaaaaaaa,
        0xffffffff, 0xaaaaaaaa, 0xffffffff, 0xaaaaaaaa,
        0xffffffff, 0xaaaaaaaa, 0xffffffff, 0xaaaaaaaa,
        0xffffffff, 0xaaaaaaaa, 0xffffffff, 0xaaaaaaaa,
        0xffffffff, 0xaaaaaaaa, 0xffffffff, 0xaaaaaaaa,
    },
/* 13 / 16 */
    {
        0xffffffff, 0xbbbbbbbb, 0xffffffff, 0xaaaaaaaa,
        0xffffffff, 0xbbbbbbbb, 0xffffffff, 0xaaaaaaaa,
        0xffffffff, 0xbbbbbbbb, 0xffffffff, 0xaaaaaaaa,
        0xffffffff, 0xbbbbbbbb, 0xffffffff, 0xaaaaaaaa,
        0xffffffff, 0xbbbbbbbb, 0xffffffff, 0xaaaaaaaa,
        0xffffffff, 0xbbbbbbbb, 0xffffffff, 0xaaaaaaaa,
        0xffffffff, 0xbbbbbbbb, 0xffffffff, 0xaaaaaaaa,
        0xffffffff, 0xbbbbbbbb, 0xffffffff, 0xaaaaaaaa,
    },
/* 14 / 16 */
    {
        0xffffffff, 0xbbbbbbbb, 0xffffffff, 0xeeeeeeee,
        0xffffffff, 0xbbbbbbbb, 0xffffffff, 0xeeeeeeee,
        0xffffffff, 0xbbbbbbbb, 0xffffffff, 0xeeeeeeee,
        0xffffffff, 0xbbbbbbbb, 0xffffffff, 0xeeeeeeee,
        0xffffffff, 0xbbbbbbbb, 0xffffffff, 0xeeeeeeee,
        0xffffffff, 0xbbbbbbbb, 0xffffffff, 0xeeeeeeee,
        0xffffffff, 0xbbbbbbbb, 0xffffffff, 0xeeeeeeee,
        0xffffffff, 0xbbbbbbbb, 0xffffffff, 0xeeeeeeee,
    },
/* 15 / 16 */
    {
        0xffffffff, 0xffffffff, 0xffffffff, 0xeeeeeeee,
        0xffffffff, 0xffffffff, 0xffffffff, 0xeeeeeeee,
        0xffffffff, 0xffffffff, 0xffffffff, 0xeeeeeeee,
        0xffffffff, 0xffffffff, 0xffffffff, 0xeeeeeeee,
        0xffffffff, 0xffffffff, 0xffffffff, 0xeeeeeeee,
        0xffffffff, 0xffffffff, 0xffffffff, 0xeeeeeeee,
        0xffffffff, 0xffffffff, 0xffffffff, 0xeeeeeeee,
        0xffffffff, 0xffffffff, 0xffffffff, 0xeeeeeeee,
    },
/* 16 / 16 */
    {
        0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
        0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
        0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
        0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
        0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
        0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
        0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
        0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
    },
};


	
JNIEXPORT
void JNICALL Java_javax_media_j3d_LinearFogRetained_update(
	JNIEnv *env,
	jobject fog,
	jlong ctxInfo,
	jfloat red,
	jfloat green,
	jfloat blue,
	jdouble fdist,
	jdouble bdist) 
{
    
    float color[3];
#ifdef VERBOSE
    fprintf(stderr, "LinearFog is on: %f %f %f %f %f\n", 
	    red, green, blue, fdist, bdist);
#endif

    color[0] = red;
    color[1] = green;
    color[2] = blue;
    glFogi(GL_FOG_MODE, GL_LINEAR);
    glFogfv(GL_FOG_COLOR, color);
    glFogf(GL_FOG_START, (float) fdist);
    glFogf(GL_FOG_END, (float) bdist);
    glEnable(GL_FOG);
}

JNIEXPORT
void JNICALL Java_javax_media_j3d_ExponentialFogRetained_update(
	JNIEnv *env, 
	jobject fog,
	jlong   ctxInfo,
	jfloat red,
	jfloat green,
	jfloat blue,
	jfloat density)
{

    float color[3];
#ifdef VERBOSE
    fprintf(stderr, "ExponentialFog is on: %f %f %f %f\n", 
	    red, green, blue, density);
#endif

    color[0] = red;
    color[1] = green;
    color[2] = blue;
    glFogi(GL_FOG_MODE, GL_EXP);
    glFogfv(GL_FOG_COLOR, color);
    glFogf(GL_FOG_DENSITY, density);
    glEnable(GL_FOG);
}

JNIEXPORT
void JNICALL Java_javax_media_j3d_ModelClipRetained_update(
	JNIEnv *env, 
	jobject modelClip,
	jlong ctxInfo,
	jint planeNum,
	jboolean enableFlag,
	jdouble A,
	jdouble B,
	jdouble C,
	jdouble D) 
{

    double equation[4];
    GLenum pl = GL_CLIP_PLANE0 + planeNum;

#ifdef VERBOSE
    fprintf(stderr, "ModelClip is on: %d %d %f %f %f %f\n", 
	    planeNum, enableFlag, A, B, C, D);
#endif

    /* OpenGL clip planes are opposite to J3d clip planes
     */
    if (enableFlag) {
        equation[0] = -A;
        equation[1] = -B;
        equation[2] = -C;
        equation[3] = -D;
        glClipPlane(pl, equation);
	glEnable(pl);
    } else
	glDisable(pl);
}

JNIEXPORT
void JNICALL Java_javax_media_j3d_Canvas3D_setModelViewMatrix(
    JNIEnv * env, 
    jobject obj,
    jlong ctxInfo,
    jdoubleArray viewMatrix, 
    jdoubleArray modelMatrix)
{
    jdouble *vmatrix_pointer;
    jdouble *mmatrix_pointer;
    JNIEnv table = *env;
    GraphicsContextPropertiesInfo *ctxProperties = (GraphicsContextPropertiesInfo *)ctxInfo;
    
    vmatrix_pointer = (jdouble *)(*(table->GetPrimitiveArrayCritical))(env,
                                  viewMatrix , NULL);
    mmatrix_pointer = (jdouble *)(*(table->GetPrimitiveArrayCritical))(env,
                                  modelMatrix , NULL);


    glMatrixMode(GL_MODELVIEW);
    

    if (ctxProperties->arb_transpose_matrix) {
        ctxProperties->glLoadTransposeMatrixdARB(vmatrix_pointer);
        ctxProperties->glMultTransposeMatrixdARB(mmatrix_pointer);
    } else {
        double v[16];
        double m[16];

        COPY_TRANSPOSE(vmatrix_pointer, v);
        COPY_TRANSPOSE(mmatrix_pointer, m);

        glLoadMatrixd(v);
        glMultMatrixd(m);
#ifdef VERBOSE
	fprintf(stderr, "\n");
	fprintf(stderr, "Canvas3D.setModelViewMatrix()\n");
	fprintf(stderr, "-----------------------------\n");
	fprintf(stderr, "VIEW  : %f %f %f %f\n", v[0], v[4], v[8], v[12]);
	fprintf(stderr, "      : %f %f %f %f\n", v[1], v[5], v[9], v[13]);
	fprintf(stderr, "      : %f %f %f %f\n", v[2], v[6], v[10], v[14]);
	fprintf(stderr, "      : %f %f %f %f\n", v[3], v[7], v[11], v[15]);
	fprintf(stderr, "\n");
	fprintf(stderr, "MODEL : %f %f %f %f\n", m[0], m[4], m[8], m[12]);
	fprintf(stderr, "      : %f %f %f %f\n", m[1], m[5], m[9], m[13]);
	fprintf(stderr, "      : %f %f %f %f\n", m[2], m[6], m[10], m[14]);
	fprintf(stderr, "      : %f %f %f %f\n", m[3], m[7], m[11], m[15]);
	fprintf(stderr, "\n\n");
#endif
    }
    (*(table->ReleasePrimitiveArrayCritical))(env, viewMatrix,
                                              vmatrix_pointer, 0);
    (*(table->ReleasePrimitiveArrayCritical))(env, modelMatrix,
                                              mmatrix_pointer, 0);
}

JNIEXPORT
void JNICALL Java_javax_media_j3d_Canvas3D_setProjectionMatrix(
    JNIEnv * env,
    jobject obj,
    jlong ctxInfo,
    jdoubleArray projMatrix)
{
    jdouble *matrix_pointer;
    JNIEnv table = *env;
    GraphicsContextPropertiesInfo *ctxProperties = (GraphicsContextPropertiesInfo *)ctxInfo;

    matrix_pointer = (jdouble *)(*(table->GetPrimitiveArrayCritical))(env, 
				   projMatrix, NULL);

    glMatrixMode(GL_PROJECTION);

    if (ctxProperties->arb_transpose_matrix) {
	/*
	 * Invert the Z value in clipping coordinates because OpenGL uses
	 * left-handed clipping coordinates, while Java3D defines right-handed
	 * coordinates everywhere.
	 */
	matrix_pointer[8] *= -1.0;
	matrix_pointer[9] *= -1.0;
	matrix_pointer[10] *= -1.0;
	matrix_pointer[11] *= -1.0;
	ctxProperties->glLoadTransposeMatrixdARB(matrix_pointer);
	matrix_pointer[8] *= -1.0;
	matrix_pointer[9] *= -1.0;
	matrix_pointer[10] *= -1.0;
	matrix_pointer[11] *= -1.0;
    } else {
        double p[16];

        COPY_TRANSPOSE(matrix_pointer, p);
	/*
	 * Invert the Z value in clipping coordinates because OpenGL uses
	 * left-handed clipping coordinates, while Java3D defines right-handed
	 * coordinates everywhere.
	 */
        p[2] *= -1.0;
        p[6] *= -1.0;
        p[10] *= -1.0;
        p[14] *= -1.0;

        glLoadMatrixd(p);
#ifdef VERBOSE
	fprintf(stderr, "\n");
	fprintf(stderr, "Canvas3D.setProjectionMatrix()\n");
	fprintf(stderr, "------------------------------\n");
	fprintf(stderr, "PROJECTION : %f %f %f %f\n", p[0], p[4], p[8], p[12]);
	fprintf(stderr, "           : %f %f %f %f\n", p[1], p[5], p[9], p[13]);
	fprintf(stderr, "           : %f %f %f %f\n", p[2], p[6], p[10], p[14]);
	fprintf(stderr, "           : %f %f %f %f\n", p[3], p[7], p[11], p[15]);
	fprintf(stderr, "\n\n");
#endif

    }

    (*(table->ReleasePrimitiveArrayCritical))(env, projMatrix,
                                              matrix_pointer, 0);
}

JNIEXPORT
void JNICALL Java_javax_media_j3d_Canvas3D_setViewport(
    JNIEnv *env, 
    jobject obj,
    jlong ctxInfo,
    jint x, 
    jint y, 
    jint width, 
    jint height)
{

    glViewport(x, y, width, height);
}

#ifdef WIN32
#define M_PI            3.14159265358979323846
#endif

JNIEXPORT
void JNICALL Java_javax_media_j3d_Canvas3D_setSceneAmbient(
    JNIEnv *env, 
    jobject cv,
    jlong ctxInfo,
    jfloat red,
    jfloat green,
    jfloat blue)
{
    float color[4];
    
    color[0] = red;
    color[1] = green;
    color[2] = blue;
    color[3] = 1.0f;
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, color);
}

JNIEXPORT
void JNICALL Java_javax_media_j3d_Canvas3D_setLightEnables(
    JNIEnv *env, 
    jobject cv,
    jlong ctxInfo,
    jlong enable_mask, 
    jint nlights) 
{
    int i;

#ifdef VERBOSE
    fprintf(stderr, "Canvas3D.updateLightEnables: mask = 0x%x, 0x%x\n",
	    (int) ((enable_mask >> 32) & 0xffffffff),
	    (int) (enable_mask & 0xffffffff));
#endif

    for (i=0; i<nlights; i++) {
	if (enable_mask & (1<<i)) {
	    glEnable(GL_LIGHT0 + i);
	}
	else {
	    glDisable(GL_LIGHT0 + i);
	}
    }
}

JNIEXPORT
void JNICALL Java_javax_media_j3d_Canvas3D_setLightingEnable(
    JNIEnv *env,
    jobject cv,
    jlong ctxInfo,
    jboolean lightingOn)
{
    if (lightingOn) {
	glEnable(GL_LIGHTING);
#ifdef VERBOSE
	fprintf(stderr, "ENABLE LIGHTING\n\n");
#endif
    } else {
	glDisable(GL_LIGHTING);
#ifdef VERBOSE
	fprintf(stderr, "DISABLE LIGHTING\n\n");
#endif
    }
}

JNIEXPORT
void JNICALL Java_javax_media_j3d_Canvas3D_disableFog(
    JNIEnv *env, 
    jobject cv,
    jlong ctxInfo)
{
#ifdef VERBOSE
    fprintf(stderr, "Disable Fog\n");
#endif
    glDisable(GL_FOG);
}

JNIEXPORT
void JNICALL Java_javax_media_j3d_Canvas3D_disableModelClip(
        JNIEnv *env,
        jobject cv,
	jlong ctxInfo)
{
#ifdef VERBOSE
    fprintf(stderr, "Disable ModelClip\n");
#endif
    glDisable(GL_CLIP_PLANE0);
    glDisable(GL_CLIP_PLANE1);
    glDisable(GL_CLIP_PLANE2);
    glDisable(GL_CLIP_PLANE3);
    glDisable(GL_CLIP_PLANE4);
    glDisable(GL_CLIP_PLANE5);
}



JNIEXPORT
void JNICALL Java_javax_media_j3d_Canvas3D_resetRenderingAttributes(
    JNIEnv *env, 
    jobject cv,
    jlong ctxInfo,    
    jboolean db_write_enable_override,
    jboolean db_enable_override)
{
    if (db_write_enable_override == JNI_FALSE) {
        glDepthMask(GL_TRUE);
    }
    if (db_enable_override == JNI_FALSE) {
        glEnable(GL_DEPTH_TEST);
    }
    glAlphaFunc(GL_ALWAYS, 0.0f);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_COLOR_MATERIAL);
    glDisable(GL_COLOR_LOGIC_OP);

}

JNIEXPORT
void JNICALL Java_javax_media_j3d_RenderingAttributesRetained_updateNative(
    JNIEnv *env, 
    jobject cv,
    jlong ctxInfo,    
    jboolean db_write_enable_override,
    jboolean db_enable_override,
    jboolean db_enable,
    jboolean db_write_enable,
    jint db_func,
    jfloat at_value,
    jint at_func,
    jboolean ignoreVertexColors,
    jboolean rasterOpEnable,
    jint rasterOp)
{
    if (db_enable_override == JNI_FALSE) {
        if (db_enable == JNI_TRUE) {
            int func = GL_LEQUAL;
            glEnable(GL_DEPTH_TEST);
            switch (db_func) {
            case javax_media_j3d_RenderingAttributes_ALWAYS:
                func = GL_ALWAYS;
                break;
            case javax_media_j3d_RenderingAttributes_NEVER:
                func = GL_NEVER;
                break;
            case javax_media_j3d_RenderingAttributes_EQUAL:
                func = GL_EQUAL;
                break;
            case javax_media_j3d_RenderingAttributes_NOT_EQUAL:
                func = GL_NOTEQUAL;
                break;
            case javax_media_j3d_RenderingAttributes_LESS:
                func = GL_LESS;
                break;
            case javax_media_j3d_RenderingAttributes_LESS_OR_EQUAL:
                func = GL_LEQUAL;
                break;
            case javax_media_j3d_RenderingAttributes_GREATER:
                func = GL_GREATER;
                break;
            case javax_media_j3d_RenderingAttributes_GREATER_OR_EQUAL:
                func = GL_GEQUAL;
                break;
        }
        glDepthFunc( func );

      } else {
            glDisable(GL_DEPTH_TEST);
        }
    } 

    if (db_write_enable_override == JNI_FALSE) {
        if (db_write_enable == JNI_TRUE ) {
            glDepthMask(GL_TRUE);
        } else {
            glDepthMask(GL_FALSE);
        }
    } 

    if (at_func == javax_media_j3d_RenderingAttributes_ALWAYS) {
        glDisable(GL_ALPHA_TEST);
    } else {
        glEnable(GL_ALPHA_TEST);
    }

    if (ignoreVertexColors == JNI_TRUE) {
	glDisable(GL_COLOR_MATERIAL);
    }
    else {
	glEnable(GL_COLOR_MATERIAL);
    }	

    /*
     * [PEPE] NOTE: shouldn't this switch be moved in 'enable' part of
     * the at_func test above, just like i did for db_func?
     */
    switch (at_func) {
	case javax_media_j3d_RenderingAttributes_ALWAYS:
	    glAlphaFunc(GL_ALWAYS, at_value);
	    break;
	case javax_media_j3d_RenderingAttributes_NEVER:
	    glAlphaFunc(GL_NEVER, at_value);
	    break;
	case javax_media_j3d_RenderingAttributes_EQUAL:
	    glAlphaFunc(GL_EQUAL, at_value);
	    break;
	case javax_media_j3d_RenderingAttributes_NOT_EQUAL:
	    glAlphaFunc(GL_NOTEQUAL, at_value);
	    break;
	case javax_media_j3d_RenderingAttributes_LESS:
	    glAlphaFunc(GL_LESS, at_value);
	    break;
	case javax_media_j3d_RenderingAttributes_LESS_OR_EQUAL:
	    glAlphaFunc(GL_LEQUAL, at_value);
	    break;
	case javax_media_j3d_RenderingAttributes_GREATER:
	    glAlphaFunc(GL_GREATER, at_value);
	    break;
	case javax_media_j3d_RenderingAttributes_GREATER_OR_EQUAL:
	    glAlphaFunc(GL_GEQUAL, at_value);
	    break;
    }

    if (rasterOpEnable == JNI_TRUE) {
	glEnable(GL_COLOR_LOGIC_OP);
	switch (rasterOp) {
	case javax_media_j3d_RenderingAttributes_ROP_CLEAR:
	    glLogicOp(GL_CLEAR);
	    break;
	case javax_media_j3d_RenderingAttributes_ROP_AND:
	    glLogicOp(GL_AND);
	    break;
	case javax_media_j3d_RenderingAttributes_ROP_AND_REVERSE:
	    glLogicOp(GL_AND_REVERSE);
	    break;
	case javax_media_j3d_RenderingAttributes_ROP_COPY:
	    glLogicOp(GL_COPY);
	    break;
	case javax_media_j3d_RenderingAttributes_ROP_AND_INVERTED:
	    glLogicOp(GL_AND_INVERTED);
	    break;
	case javax_media_j3d_RenderingAttributes_ROP_NOOP:
	    glLogicOp(GL_NOOP);
	    break;
	case javax_media_j3d_RenderingAttributes_ROP_XOR:
	    glLogicOp(GL_XOR);
	    break;
	case javax_media_j3d_RenderingAttributes_ROP_OR:
	    glLogicOp(GL_OR);
	    break;
	case javax_media_j3d_RenderingAttributes_ROP_NOR:
	    glLogicOp(GL_NOR);
	    break;
	case javax_media_j3d_RenderingAttributes_ROP_EQUIV:
	    glLogicOp(GL_EQUIV);
	    break;
	case javax_media_j3d_RenderingAttributes_ROP_INVERT:
	    glLogicOp(GL_INVERT);
	    break;
	case javax_media_j3d_RenderingAttributes_ROP_OR_REVERSE:
	    glLogicOp(GL_OR_REVERSE);
	    break;
	case javax_media_j3d_RenderingAttributes_ROP_COPY_INVERTED:
	    glLogicOp(GL_COPY_INVERTED);
	    break;
	case javax_media_j3d_RenderingAttributes_ROP_OR_INVERTED:
	    glLogicOp(GL_OR_INVERTED);
	    break;
	case javax_media_j3d_RenderingAttributes_ROP_NAND:
	    glLogicOp(GL_NAND);
	    break;
	case javax_media_j3d_RenderingAttributes_ROP_SET:
	    glLogicOp(GL_SET);
	    break;
	}
    } else
	glDisable(GL_COLOR_LOGIC_OP);
}

JNIEXPORT
void JNICALL Java_javax_media_j3d_Canvas3D_resetPolygonAttributes(
    JNIEnv *env, 
    jobject cv,
    jlong ctxInfo)
{
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glPolygonOffset(0.0f, 0.0f);
    glDisable(GL_POLYGON_OFFSET_POINT);
    glDisable(GL_POLYGON_OFFSET_LINE);
    glDisable(GL_POLYGON_OFFSET_FILL);
}

JNIEXPORT
void JNICALL Java_javax_media_j3d_PolygonAttributesRetained_updateNative(
    JNIEnv *env, 
    jobject cv,
    jlong ctxInfo,
    jint polygonMode,
    jint cullFace,
    jboolean backFaceNormalFlip,
    jfloat polygonOffset,
    jfloat polygonOffsetFactor)
{
    if (cullFace == javax_media_j3d_PolygonAttributes_CULL_NONE) {
	glDisable(GL_CULL_FACE);
    } else {
        if (cullFace == javax_media_j3d_PolygonAttributes_CULL_BACK) {
       	    glCullFace(GL_BACK);
        } else {
	    glCullFace(GL_FRONT);
	}
        glEnable(GL_CULL_FACE);
    }

    if (backFaceNormalFlip == JNI_TRUE && (cullFace != javax_media_j3d_PolygonAttributes_CULL_BACK)) {
        glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    } else {
        glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
    }

    if (polygonMode == javax_media_j3d_PolygonAttributes_POLYGON_POINT) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
    } else if (polygonMode == javax_media_j3d_PolygonAttributes_POLYGON_LINE) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    
    glPolygonOffset(polygonOffsetFactor, polygonOffset);
    
    if((polygonOffsetFactor != 0.0) || (polygonOffset != 0.0)) {
	/* fprintf(stderr, "set polygonOffSet\n"); */
	switch (polygonMode) {
	case javax_media_j3d_PolygonAttributes_POLYGON_POINT:
	    glEnable(GL_POLYGON_OFFSET_POINT);
	    glDisable(GL_POLYGON_OFFSET_LINE);
	    glDisable(GL_POLYGON_OFFSET_FILL);
	    break;
        case javax_media_j3d_PolygonAttributes_POLYGON_LINE:
	    glEnable(GL_POLYGON_OFFSET_LINE);
	    glDisable(GL_POLYGON_OFFSET_POINT);
	    glDisable(GL_POLYGON_OFFSET_FILL);
	    break;
        case javax_media_j3d_PolygonAttributes_POLYGON_FILL:
	    glEnable(GL_POLYGON_OFFSET_FILL); 
	    glDisable(GL_POLYGON_OFFSET_POINT);
	    glDisable(GL_POLYGON_OFFSET_LINE);
	    break;
	}
    }
    else {
	glDisable(GL_POLYGON_OFFSET_POINT);
	glDisable(GL_POLYGON_OFFSET_LINE);
	glDisable(GL_POLYGON_OFFSET_FILL);
    }
}

JNIEXPORT
void JNICALL Java_javax_media_j3d_Canvas3D_resetLineAttributes(
    JNIEnv *env, 
    jobject cv,
    jlong ctxInfo)
/* comment out until called in java code
    jfloat lineWidth,
    jint linePattern,
    jboolean lineAntialiasing)
*/
{
    glLineWidth(1.0f);
    glDisable(GL_LINE_STIPPLE);

    /* XXXX: Polygon Mode check, blend enable */
    glDisable (GL_LINE_SMOOTH);
}

JNIEXPORT
void JNICALL Java_javax_media_j3d_LineAttributesRetained_updateNative(
    JNIEnv *env, 
    jobject cv,
    jlong ctxInfo,
    jfloat lineWidth,
    jint linePattern,
    jint linePatternMask,
    jint linePatternScaleFactor,
    jboolean lineAntialiasing)
{
    glLineWidth(lineWidth);

    if (linePattern == javax_media_j3d_LineAttributes_PATTERN_SOLID) {
        glDisable(GL_LINE_STIPPLE);
    } else {
        if (linePattern == javax_media_j3d_LineAttributes_PATTERN_DASH) { /* dashed lines */
	    glLineStipple(1, 0x00ff);
        } else if (linePattern == javax_media_j3d_LineAttributes_PATTERN_DOT) { /* dotted lines */
	    glLineStipple(1, 0x0101);
        } else if (linePattern == javax_media_j3d_LineAttributes_PATTERN_DASH_DOT) { /* dash-dotted lines */
	    glLineStipple(1, 0x087f);
	} else if (linePattern == javax_media_j3d_LineAttributes_PATTERN_USER_DEFINED) { /* user-defined mask */
	    glLineStipple(linePatternScaleFactor, (GLushort) linePatternMask);
	}
	glEnable(GL_LINE_STIPPLE);
    }

    /* XXXX: Polygon Mode check, blend enable */
    if (lineAntialiasing == JNI_TRUE) {
        glEnable (GL_LINE_SMOOTH);
    } else {
        glDisable (GL_LINE_SMOOTH);
    }
}

JNIEXPORT
void JNICALL Java_javax_media_j3d_Canvas3D_resetPointAttributes(
    JNIEnv *env, 
    jobject cv,
    jlong ctxInfo)
{
    glPointSize(1.0f);

    /* XXXX: Polygon Mode check, blend enable */
    glDisable (GL_POINT_SMOOTH);
}

JNIEXPORT
void JNICALL Java_javax_media_j3d_PointAttributesRetained_updateNative(
    JNIEnv *env, 
    jobject cv,
    jlong ctxInfo,
    jfloat pointSize,
    jboolean pointAntialiasing)
{
    glPointSize(pointSize);

    /* XXXX: Polygon Mode check, blend enable */
    if (pointAntialiasing == JNI_TRUE) {
        glEnable (GL_POINT_SMOOTH);
    } else {
        glDisable (GL_POINT_SMOOTH);
    }
}

JNIEXPORT
void JNICALL Java_javax_media_j3d_Canvas3D_resetTexCoordGeneration(
    JNIEnv *env, 
    jobject cv,
    jlong ctxInfo)
{
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    glDisable(GL_TEXTURE_GEN_R);
    glDisable(GL_TEXTURE_GEN_Q);
}

JNIEXPORT
void JNICALL Java_javax_media_j3d_TexCoordGenerationRetained_updateNative(
    JNIEnv *env, 
    jobject cv,
    jlong ctxInfo,
    jboolean enable,
    jint genMode,
    jint format,
    jfloat planeSx,
    jfloat planeSy,
    jfloat planeSz,
    jfloat planeSw,
    jfloat planeTx,
    jfloat planeTy,
    jfloat planeTz,
    jfloat planeTw,
    jfloat planeRx,
    jfloat planeRy,
    jfloat planeRz,
    jfloat planeRw,
    jfloat planeQx,
    jfloat planeQy,
    jfloat planeQz,
    jfloat planeQw,
    jdoubleArray vworldToEc)
{
    float planeS[4], planeT[4], planeR[4], planeQ[4];
    JNIEnv table = *env;
    jdouble *mat;

    GraphicsContextPropertiesInfo *ctxProperties = (GraphicsContextPropertiesInfo *)ctxInfo;

    if (enable == JNI_TRUE) {
        glEnable(GL_TEXTURE_GEN_S);
        glEnable(GL_TEXTURE_GEN_T);
	if (format == javax_media_j3d_TexCoordGeneration_TEXTURE_COORDINATE_3) {
            glEnable(GL_TEXTURE_GEN_R);
            glDisable(GL_TEXTURE_GEN_Q);
        } else if (format == javax_media_j3d_TexCoordGeneration_TEXTURE_COORDINATE_4) {
            glEnable(GL_TEXTURE_GEN_R);
	    glEnable(GL_TEXTURE_GEN_Q);
	} else {
            glDisable(GL_TEXTURE_GEN_R);
            glDisable(GL_TEXTURE_GEN_Q);
	}

	if (genMode != javax_media_j3d_TexCoordGeneration_SPHERE_MAP) {
	    planeS[0] = planeSx; planeS[1] = planeSy; 
	    planeS[2] = planeSz; planeS[3] = planeSw;
	    planeT[0] = planeTx; planeT[1] = planeTy; 
	    planeT[2] = planeTz; planeT[3] = planeTw;
	    if (format == javax_media_j3d_TexCoordGeneration_TEXTURE_COORDINATE_3) {
	        planeR[0] = planeRx; planeR[1] = planeRy; 
	        planeR[2] = planeRz; planeR[3] = planeRw;
	    } else if (format == javax_media_j3d_TexCoordGeneration_TEXTURE_COORDINATE_4) {
	        planeR[0] = planeRx; planeR[1] = planeRy; 
	        planeR[2] = planeRz; planeR[3] = planeRw;
	        planeQ[0] = planeQx; planeQ[1] = planeQy; 
	        planeQ[2] = planeQz; planeQ[3] = planeQw;
	    }
	}

        switch (genMode) {
	    case javax_media_j3d_TexCoordGeneration_OBJECT_LINEAR:
        	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
        	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
        	glTexGenfv(GL_S, GL_OBJECT_PLANE, planeS);
        	glTexGenfv(GL_T, GL_OBJECT_PLANE, planeT);

	    	if (format == javax_media_j3d_TexCoordGeneration_TEXTURE_COORDINATE_3) {
        	    glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
        	    glTexGenfv(GL_R, GL_OBJECT_PLANE, planeR);
		} else if (format == javax_media_j3d_TexCoordGeneration_TEXTURE_COORDINATE_4) {
        	    glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
        	    glTexGenfv(GL_R, GL_OBJECT_PLANE, planeR);
        	    glTexGeni(GL_Q, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
        	    glTexGenfv(GL_Q, GL_OBJECT_PLANE, planeQ);
		}
	        break;
	    case javax_media_j3d_TexCoordGeneration_EYE_LINEAR:

    		mat = (jdouble *)(*(table->GetPrimitiveArrayCritical))(env,
                                vworldToEc, NULL);

    		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

                if (ctxProperties->arb_transpose_matrix) {
                    ctxProperties->glLoadTransposeMatrixdARB(mat);
                } else {
                    jdouble v[16];
                    COPY_TRANSPOSE(mat, v);
                    glLoadMatrixd(v);
                }

                (*(table->ReleasePrimitiveArrayCritical))(env, vworldToEc,
                                              mat, 0);

        	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
        	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
		glTexGenfv(GL_S, GL_EYE_PLANE, planeS);
		glTexGenfv(GL_T, GL_EYE_PLANE, planeT);

	    	if (format == javax_media_j3d_TexCoordGeneration_TEXTURE_COORDINATE_3) {
        	    glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
		    glTexGenfv(GL_R, GL_EYE_PLANE, planeR);
		} else if (format == javax_media_j3d_TexCoordGeneration_TEXTURE_COORDINATE_4) {
        	    glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
		    glTexGenfv(GL_R, GL_EYE_PLANE, planeR);
        	    glTexGeni(GL_Q, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
		    glTexGenfv(GL_Q, GL_EYE_PLANE, planeQ);
		}
		glPopMatrix();
	        break;
	    case javax_media_j3d_TexCoordGeneration_SPHERE_MAP:
        	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
        	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
		if (format == javax_media_j3d_TexCoordGeneration_TEXTURE_COORDINATE_3) {
        	    glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
		} else if (format == javax_media_j3d_TexCoordGeneration_TEXTURE_COORDINATE_4) {
        	    glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
        	    glTexGeni(GL_Q, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
		}

	        break;
	    case javax_media_j3d_TexCoordGeneration_NORMAL_MAP:
        	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_NORMAL_MAP_EXT);
        	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_NORMAL_MAP_EXT);
		if (format == javax_media_j3d_TexCoordGeneration_TEXTURE_COORDINATE_3) {
        	    glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_NORMAL_MAP_EXT);
		} else if (format == javax_media_j3d_TexCoordGeneration_TEXTURE_COORDINATE_4) {
        	    glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_NORMAL_MAP_EXT);
        	    glTexGeni(GL_Q, GL_TEXTURE_GEN_MODE, GL_NORMAL_MAP_EXT);
		}
	        break;
	    case javax_media_j3d_TexCoordGeneration_REFLECTION_MAP:
        	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP_EXT);
        	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP_EXT);
		if (format == javax_media_j3d_TexCoordGeneration_TEXTURE_COORDINATE_3) {
        	    glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP_EXT);
		} else if (format == javax_media_j3d_TexCoordGeneration_TEXTURE_COORDINATE_4) {
        	    glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP_EXT);
        	    glTexGeni(GL_Q, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP_EXT);
		}
	        break;
        }
    } else {
        glDisable(GL_TEXTURE_GEN_S);
    	glDisable(GL_TEXTURE_GEN_T);
    	glDisable(GL_TEXTURE_GEN_R);
    	glDisable(GL_TEXTURE_GEN_Q);
    }
}

JNIEXPORT
void JNICALL Java_javax_media_j3d_Canvas3D_resetTextureAttributes(
    JNIEnv *env, 
    jobject cv,
    jlong ctxInfo)
{
    float color[] = {0.0, 0.0, 0.0, 0.0};

    GraphicsContextPropertiesInfo *ctxProperties = (GraphicsContextPropertiesInfo *)ctxInfo;

    glPushAttrib(GL_MATRIX_MODE);
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glPopAttrib();
    glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, color);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    if(ctxProperties->textureRegisterCombinersAvailable)
        glDisable(GL_REGISTER_COMBINERS_NV);

    if(ctxProperties->textureColorTableAvailable)
	glDisable(GL_TEXTURE_COLOR_TABLE_SGI);
    /* GL_SGI_texture_color_table */
}

JNIEXPORT
void JNICALL Java_javax_media_j3d_TextureAttributesRetained_updateNative(
    JNIEnv *env, 
    jobject cv,
    jlong ctxInfo,
    jdoubleArray transform,
    jboolean isIdentity,
    jint textureMode,
    jint perspCorrectionMode,
    jfloat textureBlendColorRed,
    jfloat textureBlendColorGreen,
    jfloat textureBlendColorBlue,
    jfloat textureBlendColorAlpha,
    jint textureFormat)
{
    jdouble *mx_ptr;
    float color[4];
    JNIEnv table = *env;

    GraphicsContextPropertiesInfo *ctxProperties = (GraphicsContextPropertiesInfo *)ctxInfo;

    if (perspCorrectionMode == javax_media_j3d_TextureAttributes_NICEST) {
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    } else {
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
    }

    /* set OGL texture matrix */
    glPushAttrib(GL_MATRIX_MODE);
    glMatrixMode(GL_TEXTURE);

    mx_ptr = (jdouble *)(*(table->GetPrimitiveArrayCritical))(env, transform, 
							      NULL);
    if (isIdentity) {
	glLoadIdentity();
    } else if (ctxProperties->arb_transpose_matrix) {
        ctxProperties->glLoadTransposeMatrixdARB(mx_ptr);
    } else {
        double mx[16];
        COPY_TRANSPOSE(mx_ptr, mx);
        glLoadMatrixd(mx);
    }

    (*(table->ReleasePrimitiveArrayCritical))(env, transform, mx_ptr, 0);

    glPopAttrib();

    /* set texture color */
    color[0] = textureBlendColorRed;
    color[1] = textureBlendColorGreen;
    color[2] = textureBlendColorBlue;
    color[3] = textureBlendColorAlpha;
    glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, color);

    /* set texture environment mode */

    switch (textureMode) {
        case javax_media_j3d_TextureAttributes_MODULATE:
            glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
            break;
        case javax_media_j3d_TextureAttributes_DECAL:
            glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
            break;
        case javax_media_j3d_TextureAttributes_BLEND:
            glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
            break;
        case javax_media_j3d_TextureAttributes_REPLACE:
            glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	    break;
        case javax_media_j3d_TextureAttributes_COMBINE:
            glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, 
		ctxProperties->combine_enum);
	    break;
    }

    if(ctxProperties->textureColorTableAvailable)
	glDisable(GL_TEXTURE_COLOR_TABLE_SGI);
 /* GL_SGI_texture_color_table */
}

GLenum getCombinerArg(jint arg, GLenum textureUnit, GLenum combUnit) {
    GLenum comb;

    switch (arg) {
    case javax_media_j3d_TextureAttributes_COMBINE_OBJECT_COLOR:
        if (combUnit == GL_COMBINER0_NV) {
	    comb = GL_PRIMARY_COLOR_NV;
	} else {
	    comb = GL_SPARE0_NV;
	}
	break;
    case javax_media_j3d_TextureAttributes_COMBINE_TEXTURE_COLOR:
        comb = textureUnit;
	break;
    case javax_media_j3d_TextureAttributes_COMBINE_CONSTANT_COLOR:
        comb = GL_CONSTANT_COLOR0_NV;
	break;
    case javax_media_j3d_TextureAttributes_COMBINE_PREVIOUS_TEXTURE_UNIT_STATE:
        comb = textureUnit -1;
	break;
    }

    return (comb);
}

JNIEXPORT
void JNICALL Java_javax_media_j3d_TextureAttributesRetained_updateNativeRegisterCombiners(
    JNIEnv *env, 
    jobject cv,
    jlong ctxInfo,
    jdoubleArray transform,
    jboolean isIdentity,
    jint textureMode,
    jint perspCorrectionMode,
    jfloat textureBlendColorRed,
    jfloat textureBlendColorGreen,
    jfloat textureBlendColorBlue,
    jfloat textureBlendColorAlpha,
    jint textureFormat,
    jint combineRgbMode,
    jint combineAlphaMode,
    jintArray combineRgbSrc,
    jintArray combineAlphaSrc,
    jintArray combineRgbFcn,
    jintArray combineAlphaFcn,
    jint combineRgbScale,
    jint combineAlphaScale)
{
    jdouble *mx_ptr;
    float color[4];
    JNIEnv table = *env;
    GLenum textureUnit;
    GLenum combinerUnit;

    GraphicsContextPropertiesInfo *ctxProperties = (GraphicsContextPropertiesInfo *)ctxInfo;
    jint * rgbSrc;
    GLenum color1, color2;
    GLenum fragment;

    if (perspCorrectionMode == javax_media_j3d_TextureAttributes_NICEST) {
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    } else {
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
    }

    /* set OGL texture matrix */
    glPushAttrib(GL_MATRIX_MODE);
    glMatrixMode(GL_TEXTURE);

    mx_ptr = (jdouble *)(*(table->GetPrimitiveArrayCritical))(env, transform, 
							      NULL);
    if (isIdentity) {
	glLoadIdentity();
    } else if (ctxProperties->arb_transpose_matrix) {
        ctxProperties->glLoadTransposeMatrixdARB(mx_ptr);
    } else {
        double mx[16];
        COPY_TRANSPOSE(mx_ptr, mx);
        glLoadMatrixd(mx);
    }

    (*(table->ReleasePrimitiveArrayCritical))(env, transform, mx_ptr, 0);

    glPopAttrib();

    /* set texture color */
    color[0] = textureBlendColorRed;
    color[1] = textureBlendColorGreen;
    color[2] = textureBlendColorBlue;
    color[3] = textureBlendColorAlpha;
    glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, color);

    /* set texture environment mode */
    glEnable(GL_REGISTER_COMBINERS_NV);
    textureUnit = ctxProperties->currentTextureUnit;
    combinerUnit = ctxProperties->currentCombinerUnit;
    if (combinerUnit == GL_COMBINER0_NV) {
        fragment = GL_PRIMARY_COLOR_NV;
    } else {
        fragment = GL_SPARE0_NV;
    }

    switch (textureMode) {
        case javax_media_j3d_TextureAttributes_MODULATE:

 	    ctxProperties->glCombinerInputNV(combinerUnit, GL_RGB, 
			      GL_VARIABLE_A_NV, fragment,
			      GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	    ctxProperties->glCombinerInputNV(combinerUnit, GL_RGB, 
 			      GL_VARIABLE_B_NV, textureUnit,
	 		      GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	    ctxProperties->glCombinerInputNV(combinerUnit, GL_ALPHA, 
			      GL_VARIABLE_A_NV, fragment,
			      GL_UNSIGNED_IDENTITY_NV, GL_ALPHA);
	    ctxProperties->glCombinerInputNV(combinerUnit, GL_ALPHA, 
			      GL_VARIABLE_B_NV, textureUnit,
			      GL_UNSIGNED_IDENTITY_NV, GL_ALPHA);

	    ctxProperties->glCombinerOutputNV(combinerUnit, GL_RGB, 
			      GL_SPARE0_NV, GL_DISCARD_NV, GL_DISCARD_NV, 
			      GL_NONE, GL_NONE, GL_FALSE, GL_FALSE, GL_FALSE);
	    ctxProperties->glCombinerOutputNV(combinerUnit, GL_ALPHA, 
			      GL_SPARE0_NV, GL_DISCARD_NV, GL_DISCARD_NV, 
			      GL_NONE, GL_NONE, GL_FALSE, GL_FALSE, GL_FALSE);
            break;

        case javax_media_j3d_TextureAttributes_DECAL:

            ctxProperties->glCombinerInputNV(combinerUnit, GL_RGB, 
			      GL_VARIABLE_A_NV, fragment,
			      GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	    ctxProperties->glCombinerInputNV(combinerUnit, GL_RGB, 
			      GL_VARIABLE_B_NV, textureUnit,
			      GL_UNSIGNED_INVERT_NV, GL_ALPHA);
	    ctxProperties->glCombinerInputNV(combinerUnit, GL_RGB, 
			      GL_VARIABLE_C_NV, textureUnit,
			      GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	    ctxProperties->glCombinerInputNV(combinerUnit, GL_RGB, 
			      GL_VARIABLE_D_NV, textureUnit,
			      GL_UNSIGNED_IDENTITY_NV, GL_ALPHA);

	    ctxProperties->glCombinerInputNV(combinerUnit, GL_ALPHA, 
			      GL_VARIABLE_A_NV, fragment,
			      GL_UNSIGNED_IDENTITY_NV, GL_ALPHA);
	    ctxProperties->glCombinerInputNV(combinerUnit, GL_ALPHA, 
			      GL_VARIABLE_B_NV, GL_ZERO,
			      GL_UNSIGNED_INVERT_NV, GL_ALPHA);

	    ctxProperties->glCombinerOutputNV(combinerUnit, GL_RGB, 
			      GL_DISCARD_NV, GL_DISCARD_NV, GL_SPARE0_NV, 
			      GL_NONE, GL_NONE, GL_FALSE, GL_FALSE, GL_FALSE);
	    ctxProperties->glCombinerOutputNV(combinerUnit, GL_ALPHA, 
			      GL_SPARE0_NV, GL_DISCARD_NV, GL_DISCARD_NV, 
			      GL_NONE, GL_NONE, GL_FALSE, GL_FALSE, GL_FALSE);
            break;

        case javax_media_j3d_TextureAttributes_BLEND:

	    ctxProperties->glCombinerParameterfvNV(GL_CONSTANT_COLOR0_NV, color);

	    ctxProperties->glCombinerInputNV(combinerUnit, GL_RGB, 
			      GL_VARIABLE_A_NV, fragment,
			      GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	    ctxProperties->glCombinerInputNV(combinerUnit, GL_RGB, 
			      GL_VARIABLE_B_NV, textureUnit,
			      GL_UNSIGNED_INVERT_NV, GL_RGB);
	    ctxProperties->glCombinerInputNV(combinerUnit, GL_RGB, 
			      GL_VARIABLE_C_NV, GL_CONSTANT_COLOR0_NV,
			      GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	    ctxProperties->glCombinerInputNV(combinerUnit, GL_RGB, 
			      GL_VARIABLE_D_NV, textureUnit,
			      GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	    
	    ctxProperties->glCombinerInputNV(combinerUnit, GL_ALPHA, 
			      GL_VARIABLE_A_NV, fragment,
			      GL_UNSIGNED_IDENTITY_NV, GL_ALPHA);
	    ctxProperties->glCombinerInputNV(combinerUnit, GL_ALPHA, 
			      GL_VARIABLE_B_NV, textureUnit,
			      GL_UNSIGNED_IDENTITY_NV, GL_ALPHA);
	    
	    ctxProperties->glCombinerOutputNV(combinerUnit, GL_RGB, 
			      GL_DISCARD_NV, GL_DISCARD_NV, GL_SPARE0_NV, 
			      GL_NONE, GL_NONE, GL_FALSE, GL_FALSE, GL_FALSE);
	    ctxProperties->glCombinerOutputNV(combinerUnit, GL_ALPHA, 
			      GL_SPARE0_NV, GL_DISCARD_NV, GL_DISCARD_NV, 
			      GL_NONE, GL_NONE, GL_FALSE, GL_FALSE, GL_FALSE);
            break;

        case javax_media_j3d_TextureAttributes_REPLACE:

  	    ctxProperties->glCombinerInputNV(combinerUnit, GL_RGB, 
			      GL_VARIABLE_A_NV, textureUnit,
			      GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	    ctxProperties->glCombinerInputNV(combinerUnit, GL_RGB, 
			      GL_VARIABLE_B_NV, GL_ZERO,
			      GL_UNSIGNED_INVERT_NV, GL_RGB);
	    ctxProperties->glCombinerInputNV(combinerUnit, GL_ALPHA, 
			      GL_VARIABLE_A_NV, textureUnit,
			      GL_UNSIGNED_IDENTITY_NV, GL_ALPHA);
	    ctxProperties->glCombinerInputNV(combinerUnit, GL_ALPHA, 
			      GL_VARIABLE_B_NV, GL_ZERO,
			      GL_UNSIGNED_INVERT_NV, GL_ALPHA);

	    ctxProperties->glCombinerOutputNV(combinerUnit, GL_RGB, 
			      GL_SPARE0_NV, GL_DISCARD_NV, GL_DISCARD_NV, 
			      GL_NONE, GL_NONE, GL_FALSE, GL_FALSE, GL_FALSE);
	    ctxProperties->glCombinerOutputNV(combinerUnit, GL_ALPHA, 
			      GL_SPARE0_NV, GL_DISCARD_NV, GL_DISCARD_NV, 
			      GL_NONE, GL_NONE, GL_FALSE, GL_FALSE, GL_FALSE);
	    break;

        case javax_media_j3d_TextureAttributes_COMBINE:
	    if (combineRgbMode == 
		javax_media_j3d_TextureAttributes_COMBINE_DOT3) {
	        rgbSrc = (jint *)(*(table->GetPrimitiveArrayCritical))(
				           env, combineRgbSrc, NULL);
		color1 = getCombinerArg(rgbSrc[0], textureUnit, combinerUnit);
	        ctxProperties->glCombinerInputNV(combinerUnit, GL_RGB, 
			      GL_VARIABLE_A_NV, color1,
			      GL_EXPAND_NORMAL_NV, GL_RGB);
		color2 = getCombinerArg(rgbSrc[1], textureUnit, combinerUnit);
	        ctxProperties->glCombinerInputNV(combinerUnit, GL_RGB, 
			      GL_VARIABLE_B_NV, color2,
			      GL_EXPAND_NORMAL_NV, GL_RGB);
		(*(table->ReleasePrimitiveArrayCritical))(env, combineRgbSrc, 
							  rgbSrc, 0);
		ctxProperties->glCombinerInputNV(combinerUnit, GL_ALPHA, 
		 	          GL_VARIABLE_A_NV, GL_ZERO,
			          GL_UNSIGNED_INVERT_NV, GL_ALPHA);
		ctxProperties->glCombinerInputNV(combinerUnit, GL_ALPHA, 
			          GL_VARIABLE_B_NV, GL_ZERO,
			          GL_UNSIGNED_INVERT_NV, GL_ALPHA);
	    
		ctxProperties->glCombinerOutputNV(combinerUnit, GL_RGB, 
				  GL_SPARE0_NV, GL_DISCARD_NV, GL_DISCARD_NV, 
						  GL_NONE/*SCALE_BY_FOUR_NV*/, GL_NONE, GL_TRUE, 
				  GL_FALSE, GL_FALSE);
		ctxProperties->glCombinerOutputNV(combinerUnit, GL_ALPHA, 
			          GL_SPARE0_NV, GL_DISCARD_NV, GL_DISCARD_NV, 
			          GL_NONE, GL_NONE, GL_FALSE, 
				  GL_FALSE, GL_FALSE);
	    }
	    break;
    }


    ctxProperties->glFinalCombinerInputNV(GL_VARIABLE_A_NV, 
			   GL_SPARE0_NV, GL_UNSIGNED_IDENTITY_NV, GL_RGB);
    ctxProperties->glFinalCombinerInputNV(GL_VARIABLE_B_NV, 
			   GL_ZERO, GL_UNSIGNED_INVERT_NV, GL_RGB);
    ctxProperties->glFinalCombinerInputNV(GL_VARIABLE_C_NV, 
			   GL_ZERO, GL_UNSIGNED_IDENTITY_NV, GL_RGB);
    ctxProperties->glFinalCombinerInputNV(GL_VARIABLE_D_NV, 
			   GL_ZERO, GL_UNSIGNED_IDENTITY_NV, GL_RGB);
    ctxProperties->glFinalCombinerInputNV(GL_VARIABLE_E_NV, 
			   GL_ZERO, GL_UNSIGNED_IDENTITY_NV, GL_RGB);
    ctxProperties->glFinalCombinerInputNV(GL_VARIABLE_F_NV, 
			   GL_ZERO, GL_UNSIGNED_IDENTITY_NV, GL_RGB);
    ctxProperties->glFinalCombinerInputNV(GL_VARIABLE_G_NV, 
			   GL_SPARE0_NV, GL_UNSIGNED_IDENTITY_NV, GL_ALPHA);

    if(ctxProperties->textureColorTableAvailable)
	glDisable(GL_TEXTURE_COLOR_TABLE_SGI);
 /* GL_SGI_texture_color_table */
}

void getGLCombineMode(GraphicsContextPropertiesInfo *ctxInfo, 
	jint combineRgbMode, jint combineAlphaMode,
	jint *GLrgbMode, jint *GLalphaMode) {

    switch (combineRgbMode) {
    case javax_media_j3d_TextureAttributes_COMBINE_REPLACE:
	*GLrgbMode = GL_REPLACE;
	break;
    case javax_media_j3d_TextureAttributes_COMBINE_MODULATE:
	*GLrgbMode = GL_MODULATE;
	break;
    case javax_media_j3d_TextureAttributes_COMBINE_ADD:
	*GLrgbMode = GL_ADD;
	break;
    case javax_media_j3d_TextureAttributes_COMBINE_ADD_SIGNED:
	*GLrgbMode = ctxInfo->combine_add_signed_enum;
	break;
    case javax_media_j3d_TextureAttributes_COMBINE_SUBTRACT:
	*GLrgbMode = ctxInfo->combine_subtract_enum;
	break;
    case javax_media_j3d_TextureAttributes_COMBINE_INTERPOLATE:
	*GLrgbMode = ctxInfo->combine_interpolate_enum;
	break;
    case javax_media_j3d_TextureAttributes_COMBINE_DOT3:
	*GLrgbMode = ctxInfo->combine_dot3_rgb_enum;
	break;
    default:
	break;
    }

    switch (combineAlphaMode) {
    case javax_media_j3d_TextureAttributes_COMBINE_REPLACE:
	*GLalphaMode = GL_REPLACE;
	break;
    case javax_media_j3d_TextureAttributes_COMBINE_MODULATE:
	*GLalphaMode = GL_MODULATE;
	break;
    case javax_media_j3d_TextureAttributes_COMBINE_ADD:
	*GLalphaMode = GL_ADD;
	break;
    case javax_media_j3d_TextureAttributes_COMBINE_ADD_SIGNED:
	*GLalphaMode = ctxInfo->combine_add_signed_enum;
	break;
    case javax_media_j3d_TextureAttributes_COMBINE_SUBTRACT:
	*GLalphaMode = ctxInfo->combine_subtract_enum;
	break;
    case javax_media_j3d_TextureAttributes_COMBINE_INTERPOLATE:
	*GLalphaMode = ctxInfo->combine_interpolate_enum;
	break;
    case javax_media_j3d_TextureAttributes_COMBINE_DOT3:
	/* dot3 will only make sense for alpha if rgb is also
           doing dot3. So if rgb is not doing dot3, fallback to replace
	 */
	if (combineRgbMode == javax_media_j3d_TextureAttributes_COMBINE_DOT3) {
	    *GLrgbMode = ctxInfo->combine_dot3_rgba_enum;
	} else {
	    *GLalphaMode = GL_REPLACE;
        }
	break;
    default:
	break;
    }

    return;
}

/* mapping from java enum to gl enum
 */

jint _gl_combineRgbSrcIndex[] = {
	GL_SOURCE0_RGB_ARB,
	GL_SOURCE1_RGB_ARB,
	GL_SOURCE2_RGB_ARB,
};

jint _gl_combineAlphaSrcIndex[] = {
	GL_SOURCE0_ALPHA_ARB,
	GL_SOURCE1_ALPHA_ARB,
	GL_SOURCE2_ALPHA_ARB,
};
	
jint _gl_combineRgbOpIndex[] = {
	GL_OPERAND0_RGB_ARB,
	GL_OPERAND1_RGB_ARB,
	GL_OPERAND2_RGB_ARB,
};

jint _gl_combineAlphaOpIndex[] = {
	GL_OPERAND0_ALPHA_ARB,
	GL_OPERAND1_ALPHA_ARB,
	GL_OPERAND2_ALPHA_ARB,
};
	
jint _gl_combineSrc[] = {
	GL_PRIMARY_COLOR_ARB,	/* TextureAttributes.COMBINE_OBJECT_COLOR */
	GL_TEXTURE,		/* TextureAttributes.COMBINE_TEXTURE */
	GL_CONSTANT_ARB,	/* TextureAttributes.COMBINE_CONSTANT_COLOR */
	GL_PREVIOUS_ARB,	/* TextureAttributes.COMBINE_PREVIOUS_TEXTURE_UNIT_STATE */
};
	
jint _gl_combineFcn[] = {
	GL_SRC_COLOR,		/* TextureAttributes.COMBINE_SRC_COLOR */
	GL_ONE_MINUS_SRC_COLOR,	/* TextureAttributes.COMBINE_ONE_MINUS_SRC_COLOR */
	GL_SRC_ALPHA,		/* TextureAttributes.COMBINE_SRC_ALPHA */
	GL_ONE_MINUS_SRC_ALPHA,	/* TextureAttributes.COMBINE_ONE_MINUS_SRC_ALPHA */
};


JNIEXPORT
void JNICALL Java_javax_media_j3d_TextureAttributesRetained_updateCombinerNative(
    JNIEnv *env, 
    jobject cv,
    jlong ctxProperties,
    jint combineRgbMode,
    jint combineAlphaMode,
    jintArray combineRgbSrc,
    jintArray combineAlphaSrc,
    jintArray combineRgbFcn,
    jintArray combineAlphaFcn,
    jint combineRgbScale,
    jint combineAlphaScale)  {


    JNIEnv table = *env;
    GraphicsContextPropertiesInfo *ctxInfo = 
		(GraphicsContextPropertiesInfo *)ctxProperties;
    jint *rgbSrc, *alphaSrc, *rgbFcn, *alphaFcn;
    jint GLrgbMode, GLalphaMode;
    jint nargs, i;

    rgbSrc = (jint *)(*(table->GetPrimitiveArrayCritical))(
					env, combineRgbSrc, NULL);
    alphaSrc = (jint *)(*(table->GetPrimitiveArrayCritical))(
					env, combineAlphaSrc, NULL);
    rgbFcn = (jint *)(*(table->GetPrimitiveArrayCritical))(
					env, combineRgbFcn, NULL);
    alphaFcn = (jint *)(*(table->GetPrimitiveArrayCritical))(
					env, combineAlphaFcn, NULL);

    getGLCombineMode(ctxInfo, combineRgbMode, combineAlphaMode, 
				&GLrgbMode, &GLalphaMode);

    glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GLrgbMode);
    glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_ARB, GLalphaMode);
    
    if (combineRgbMode == javax_media_j3d_TextureAttributes_COMBINE_REPLACE) {
	nargs = 1;
    } else if (combineRgbMode == javax_media_j3d_TextureAttributes_COMBINE_INTERPOLATE) {
	nargs = 3;
    } else {
	nargs = 2;
    }

    for (i = 0; i < nargs; i++) {
	glTexEnvi(GL_TEXTURE_ENV, _gl_combineRgbSrcIndex[i], 
					_gl_combineSrc[rgbSrc[i]]);
	glTexEnvi(GL_TEXTURE_ENV, _gl_combineRgbOpIndex[i], 
					_gl_combineFcn[rgbFcn[i]]);
    }

    if (combineAlphaMode == javax_media_j3d_TextureAttributes_COMBINE_REPLACE) {
	nargs = 1;
    } else if (combineAlphaMode == javax_media_j3d_TextureAttributes_COMBINE_INTERPOLATE) {
	nargs = 3;
    } else {
	nargs = 2;
    }

    for (i = 0; i < nargs; i++) {
	glTexEnvi(GL_TEXTURE_ENV, _gl_combineAlphaSrcIndex[i], 
					_gl_combineSrc[alphaSrc[i]]);
	glTexEnvi(GL_TEXTURE_ENV, _gl_combineAlphaOpIndex[i], 
					_gl_combineFcn[alphaFcn[i]]);
    }

    glTexEnvi(GL_TEXTURE_ENV, GL_RGB_SCALE_ARB, combineRgbScale);
    glTexEnvi(GL_TEXTURE_ENV, GL_ALPHA_SCALE, combineAlphaScale);

    (*(table->ReleasePrimitiveArrayCritical))(env, combineRgbSrc, rgbSrc, 0);
    (*(table->ReleasePrimitiveArrayCritical))(env, combineAlphaSrc, alphaSrc, 0);
    (*(table->ReleasePrimitiveArrayCritical))(env, combineRgbFcn, rgbFcn, 0);
    (*(table->ReleasePrimitiveArrayCritical))(env, combineAlphaFcn, alphaFcn, 0);

}


JNIEXPORT
void JNICALL Java_javax_media_j3d_TextureAttributesRetained_updateTextureColorTableNative(
	JNIEnv *env, 
	jobject cv,
	jlong ctxInfo,
	jint numComponents,
	jint colorTableSize,
	jintArray textureColorTable)
{
    JNIEnv table = *env;
    jint *ctable;

    GraphicsContextPropertiesInfo *ctxProperties = (GraphicsContextPropertiesInfo *)ctxInfo;

    if(ctxProperties->textureColorTableAvailable) {
	ctable = (jint *)(*(table->GetPrimitiveArrayCritical))(env, 
							       textureColorTable, NULL);
	if (numComponents == 3) {
	    ctxProperties->glColorTable(GL_TEXTURE_COLOR_TABLE_SGI, GL_RGB, 
			    colorTableSize, GL_RGB, GL_INT, ctable);
	} else {
	    ctxProperties->glColorTable(GL_TEXTURE_COLOR_TABLE_SGI, GL_RGBA, 
			    colorTableSize, GL_RGBA, GL_INT, ctable);
	}
    
	(*(table->ReleasePrimitiveArrayCritical))(env, textureColorTable, ctable, 0);
	glEnable(GL_TEXTURE_COLOR_TABLE_SGI);
    }
}

JNIEXPORT
void JNICALL Java_javax_media_j3d_Canvas3D_updateMaterial(
	JNIEnv *env, 
	jobject cv,
	jlong ctxInfo,	
	jfloat colorRed,
	jfloat colorGreen,
	jfloat colorBlue,
	jfloat transparency)
{
    float color[4];

    color[0] = colorRed;
    color[1] = colorGreen;
    color[2] = colorBlue;
    color[3] = transparency;
    glColor4fv(color);
    glDisable(GL_LIGHTING);
}

JNIEXPORT
void JNICALL Java_javax_media_j3d_MaterialRetained_updateNative(
	JNIEnv *env, 
	jobject cv,
	jlong ctxInfo,	
	jfloat colorRed,
	jfloat colorGreen,
	jfloat colorBlue,
	jfloat transparency,
	jfloat aRed,
	jfloat aGreen,
	jfloat aBlue,
	jfloat eRed,
	jfloat eGreen,
	jfloat eBlue,
	jfloat dRed,
	jfloat dGreen,
	jfloat dBlue,
	jfloat sRed,
	jfloat sGreen,
	jfloat sBlue,
	jfloat shininess,
	jint colorTarget,
	jboolean lightEnable)
{
    float color[4];

    color[3] = 1.0f;
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &shininess);
    if (colorTarget == javax_media_j3d_Material_DIFFUSE) {
	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
    }
    else if (colorTarget == javax_media_j3d_Material_AMBIENT) {
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT);
    }
    else if (colorTarget == javax_media_j3d_Material_EMISSIVE) {
	glColorMaterial(GL_FRONT_AND_BACK, GL_EMISSION);
    }
    else if (colorTarget == javax_media_j3d_Material_SPECULAR) {
	glColorMaterial(GL_FRONT_AND_BACK, GL_SPECULAR);
    }
    else if (colorTarget == javax_media_j3d_Material_AMBIENT_AND_DIFFUSE) {
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    }

    color[0] = eRed; color[1] = eGreen; color[2] = eBlue;
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, color);
 
    color[0] = aRed; color[1] = aGreen; color[2] = aBlue;
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color);

    color[0] = sRed; color[1] = sGreen; color[2] = sBlue;
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color);
  
    if (lightEnable == JNI_TRUE) {
        color[0] = dRed; color[1] = dGreen; color[2] = dBlue;
    } else {
        color[0] = colorRed; color[1] = colorGreen; color[2] = colorBlue;
    }
    color[3] = transparency;
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color); 
    glColor4fv(color);

    if (lightEnable) {
        glEnable(GL_LIGHTING);
    } else {
        glDisable(GL_LIGHTING);
    }
}

JNIEXPORT
void JNICALL Java_javax_media_j3d_Canvas3D_resetTransparency(
    JNIEnv *env, 
    jobject cv,
    jlong ctxInfo,
    jint geometryType,
    jint polygonMode,
    jboolean lineAA,
    jboolean pointAA) 
{
    if (((((geometryType & javax_media_j3d_RenderMolecule_LINE) != 0) ||
	  (polygonMode == javax_media_j3d_PolygonAttributes_POLYGON_LINE)) 
		&& lineAA == JNI_TRUE) ||
        ((((geometryType & javax_media_j3d_RenderMolecule_POINT) != 0) ||
	  (polygonMode == javax_media_j3d_PolygonAttributes_POLYGON_POINT)) 
		&& pointAA == JNI_TRUE)) {
        glEnable (GL_BLEND);
        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    } else {
        glDisable (GL_BLEND);
    }
    glDisable(GL_POLYGON_STIPPLE);

}

JNIEXPORT
void JNICALL Java_javax_media_j3d_TransparencyAttributesRetained_updateNative(
    JNIEnv *env, 
    jobject tr,
    jlong ctxInfo,    
    jfloat transparency,
    jint geometryType,
    jint polygonMode,
    jboolean lineAA,
    jboolean pointAA,
    jint transparencyMode,
    jint srcBlendFunction,
    jint dstBlendFunction) 
{
    GraphicsContextPropertiesInfo *ctxProperties = (GraphicsContextPropertiesInfo *)ctxInfo;

    if (transparencyMode != javax_media_j3d_TransparencyAttributes_SCREEN_DOOR) {
        glDisable(GL_POLYGON_STIPPLE);
    } else  {
        glEnable(GL_POLYGON_STIPPLE);
	glPolygonStipple((GLubyte *)(screen_door[(int)((transparency)*16)]));
    }

    if ((transparencyMode < javax_media_j3d_TransparencyAttributes_SCREEN_DOOR) ||
	((((geometryType & javax_media_j3d_RenderMolecule_LINE) != 0) ||
	  (polygonMode == javax_media_j3d_PolygonAttributes_POLYGON_LINE))
		 && lineAA == JNI_TRUE) ||
        ((((geometryType & javax_media_j3d_RenderMolecule_POINT) != 0) ||
	  (polygonMode == javax_media_j3d_PolygonAttributes_POLYGON_POINT)) 
		 && pointAA == JNI_TRUE)) {
        glEnable (GL_BLEND);
	/* valid range of blendFunction 0..3 is already verify in Java code. */
	glBlendFunc(ctxProperties->blendFunctionTable[srcBlendFunction],
		    ctxProperties->blendFunctionTable[dstBlendFunction]);
    } else {
        glDisable (GL_BLEND);
    }
}

JNIEXPORT
void JNICALL Java_javax_media_j3d_Canvas3D_resetColoringAttributes(
    JNIEnv *env, 
    jobject cv,
    jlong ctxInfo,    
    jfloat colorRed,
    jfloat colorGreen,
    jfloat colorBlue,
    jfloat transparency,
    jboolean lightEnable) 
{

    float color[4];

    if (lightEnable != JNI_TRUE) {
        color[0] = colorRed; color[1] = colorGreen; color[2] = colorBlue;
	color[3] = transparency;
	glColor4fv(color);

    }
    glShadeModel(GL_SMOOTH);
}

JNIEXPORT
void JNICALL Java_javax_media_j3d_ColoringAttributesRetained_updateNative(
    JNIEnv *env, 
    jobject cv,
    jlong ctxInfo,
    jfloat dRed,
    jfloat dGreen,
    jfloat dBlue,
    jfloat colorRed,
    jfloat colorGreen,
    jfloat colorBlue,
    jfloat transparency,
    jboolean lightEnable,    
    jint shadeModel) 
{

    float color[4];

    if (lightEnable == JNI_TRUE) {
        color[0] = dRed; color[1] = dGreen; color[2] = dBlue;
    } else {
        color[0] = colorRed; color[1] = colorGreen; color[2] = colorBlue;
    }
    color[3] = transparency;

	glColor4fv(color);
    if (shadeModel == javax_media_j3d_ColoringAttributes_SHADE_FLAT) {
        glShadeModel(GL_FLAT);
    } else {
        glShadeModel(GL_SMOOTH);
    }
}


JNIEXPORT
void JNICALL Java_javax_media_j3d_Canvas3D_resetTextureNative(
    JNIEnv *env, 
    jobject texture,
    jlong ctxInfo,
    jint texUnitIndex)
{
    GraphicsContextPropertiesInfo *ctxProperties = (GraphicsContextPropertiesInfo *)ctxInfo;

    if(ctxProperties->arb_multitexture) {

      if (texUnitIndex >= 0) {
	    ctxProperties->glActiveTextureARB(texUnitIndex + GL_TEXTURE0_ARB);
	    ctxProperties->glClientActiveTextureARB(texUnitIndex + GL_TEXTURE0_ARB);
      }
    }

    glDisable(GL_TEXTURE_1D);
    glDisable(GL_TEXTURE_2D);
    
    if(ctxProperties->texture3DAvailable)  {
	glDisable(ctxProperties->texture_3D_ext_enum);
    }

    if(ctxProperties->textureCubeMapAvailable) {
	glDisable(ctxProperties->texture_cube_map_ext_enum);
    }
}


/*
 * A set of common updateTexture functions shared among Texture2D, Texture3D,
 * and TextureCubeMap for setting texture parameters
 */
void updateTextureFilterModes(
    GraphicsContextPropertiesInfo *ctxProperties,
    jint target,
    jint minFilter,
    jint magFilter) {
 
    /* set texture min filter */
    switch(minFilter) {
        case javax_media_j3d_Texture_FASTEST:
        case javax_media_j3d_Texture_BASE_LEVEL_POINT:
            glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	    break;
        case javax_media_j3d_Texture_BASE_LEVEL_LINEAR:
            glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	    break;
        case javax_media_j3d_Texture_MULTI_LEVEL_POINT:
            glTexParameteri(target, GL_TEXTURE_MIN_FILTER, 
			    GL_NEAREST_MIPMAP_NEAREST);
	    break;
        case javax_media_j3d_Texture_NICEST:
        case javax_media_j3d_Texture_MULTI_LEVEL_LINEAR:
            glTexParameteri(target, GL_TEXTURE_MIN_FILTER, 
			    GL_LINEAR_MIPMAP_LINEAR);
	    break;
	case javax_media_j3d_Texture_FILTER4:
            glTexParameteri(target, GL_TEXTURE_MIN_FILTER, 
			    	ctxProperties->filter4_enum);
	    break;

    }

    /* set texture mag filter */
    switch(magFilter){
        case javax_media_j3d_Texture_FASTEST:
        case javax_media_j3d_Texture_BASE_LEVEL_POINT:
            glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	    break;
        case javax_media_j3d_Texture_NICEST:
        case javax_media_j3d_Texture_BASE_LEVEL_LINEAR:
            glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	    break;
	case javax_media_j3d_Texture_LINEAR_SHARPEN:
	    glTexParameteri(target, GL_TEXTURE_MAG_FILTER,
				ctxProperties->linear_sharpen_enum);
	    break;
	case javax_media_j3d_Texture_LINEAR_SHARPEN_RGB:
	    glTexParameteri(target, GL_TEXTURE_MAG_FILTER,
				ctxProperties->linear_sharpen_rgb_enum);
	    break;
	case javax_media_j3d_Texture_LINEAR_SHARPEN_ALPHA:
	    glTexParameteri(target, GL_TEXTURE_MAG_FILTER,
				ctxProperties->linear_sharpen_alpha_enum);
	    break;
	case javax_media_j3d_Texture2D_LINEAR_DETAIL:
	    glTexParameteri(target, GL_TEXTURE_MAG_FILTER,
				ctxProperties->linear_detail_enum);
	    break;
	case javax_media_j3d_Texture2D_LINEAR_DETAIL_RGB:
	    glTexParameteri(target, GL_TEXTURE_MAG_FILTER,
				ctxProperties->linear_detail_rgb_enum);
	    break;
	case javax_media_j3d_Texture2D_LINEAR_DETAIL_ALPHA:
	    glTexParameteri(target, GL_TEXTURE_MAG_FILTER,
				ctxProperties->linear_detail_alpha_enum);
	    break;
	case javax_media_j3d_Texture_FILTER4:
            glTexParameteri(target, GL_TEXTURE_MAG_FILTER, 
			    	ctxProperties->filter4_enum);
	    break;
    }
}


void updateTextureLodRange(
    GraphicsContextPropertiesInfo *ctxProperties,
    jint target,
    jint baseLevel,
    jint maximumLevel,
    float minimumLod, float maximumLod) {

    /*
     * checking of the availability of the extension is already done
     * in the java side
     */
    glTexParameteri(target, ctxProperties->texture_base_level_enum,
					baseLevel);
    glTexParameteri(target, ctxProperties->texture_max_level_enum,
					maximumLevel);
    glTexParameterf(target, ctxProperties->texture_min_lod_enum,
					minimumLod);
    glTexParameterf(target, ctxProperties->texture_max_lod_enum,
					maximumLod);
}

void updateTextureLodOffset(
    GraphicsContextPropertiesInfo *ctxProperties,
    jint target,
    float lodOffsetS, float lodOffsetT, float lodOffsetR) {

    /*
     * checking of the availability of the extension is already done
     * in the java side
     */
    glTexParameterf(target, GL_TEXTURE_LOD_BIAS_S_SGIX, lodOffsetS);
    glTexParameterf(target, GL_TEXTURE_LOD_BIAS_T_SGIX, lodOffsetT);
    glTexParameterf(target, GL_TEXTURE_LOD_BIAS_R_SGIX, lodOffsetR);
}


void updateTextureBoundary(
    GraphicsContextPropertiesInfo *ctxProperties,
    jint target,
    jint boundaryModeS, 
    jint boundaryModeT, 
    jint boundaryModeR, 
    jfloat boundaryRed, 
    jfloat boundaryGreen, 
    jfloat boundaryBlue, 
    jfloat boundaryAlpha)
{
    float color[4];
    
    /* set texture wrap parameter */
    switch (boundaryModeS){
        case javax_media_j3d_Texture_WRAP: 
	    glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
	    break;
        case javax_media_j3d_Texture_CLAMP:
	    glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP);
	    break;
        case javax_media_j3d_Texture_CLAMP_TO_EDGE:
	    glTexParameteri(target, GL_TEXTURE_WRAP_S, 
				ctxProperties->texture_clamp_to_edge_enum);
	    break;
        case javax_media_j3d_Texture_CLAMP_TO_BOUNDARY:
	    glTexParameteri(target, GL_TEXTURE_WRAP_S, 
				ctxProperties->texture_clamp_to_border_enum);
	    break;
    }

    switch (boundaryModeT) {
        case javax_media_j3d_Texture_WRAP: 
	    glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT);
	    break;
        case javax_media_j3d_Texture_CLAMP:
	    glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP);
	    break;
        case javax_media_j3d_Texture_CLAMP_TO_EDGE:
	    glTexParameteri(target, GL_TEXTURE_WRAP_T, 
				ctxProperties->texture_clamp_to_edge_enum);
	    break;
        case javax_media_j3d_Texture_CLAMP_TO_BOUNDARY:
	    glTexParameteri(target, GL_TEXTURE_WRAP_T, 
				ctxProperties->texture_clamp_to_border_enum);
	    break;
    }

    /* applies to Texture3D only */
    if (boundaryModeR != -1) {
        switch (boundaryModeR) {
            case javax_media_j3d_Texture_WRAP:
            glTexParameteri(target,
                            ctxProperties->texture_wrap_r_ext_enum, GL_REPEAT);
            break;

            case javax_media_j3d_Texture_CLAMP:
            glTexParameteri(target,
                            ctxProperties->texture_wrap_r_ext_enum, GL_CLAMP);
            break;
            case javax_media_j3d_Texture_CLAMP_TO_EDGE:
	    glTexParameteri(target, 
                            ctxProperties->texture_wrap_r_ext_enum, 
				ctxProperties->texture_clamp_to_edge_enum);
	    break;
            case javax_media_j3d_Texture_CLAMP_TO_BOUNDARY:
	    glTexParameteri(target, 
                            ctxProperties->texture_wrap_r_ext_enum,
				ctxProperties->texture_clamp_to_border_enum);
	    break;
        }
    }

    if (boundaryModeS == javax_media_j3d_Texture_CLAMP || 
	boundaryModeT == javax_media_j3d_Texture_CLAMP ||
	boundaryModeR == javax_media_j3d_Texture_CLAMP) {
        /* set texture border color */
        color[0] = boundaryRed;
        color[1] = boundaryGreen;
        color[2] = boundaryBlue;
        color[3] = boundaryAlpha;
        glTexParameterfv(target, GL_TEXTURE_BORDER_COLOR, color);
    }
}

void updateTextureSharpenFunc(
    JNIEnv *env, 
    GraphicsContextPropertiesInfo *ctxProperties,
    jint target,
    jint numPts,
    jfloatArray pts)
{
    /*
     * checking of the availability of sharpen texture functionality
     * is already done in the java side
     */

    JNIEnv table = *env;
    jfloat *funcPts = NULL;
	
    if (numPts > 0) {
        funcPts = (jfloat *)(*(table->GetPrimitiveArrayCritical))(
					env, pts, NULL);
    }

    ctxProperties->glSharpenTexFuncSGIS(target, numPts, funcPts);

    if (funcPts != NULL) {
        (*(table->ReleasePrimitiveArrayCritical))(env, pts, funcPts, 0);	
    }
}


void updateTextureFilter4Func(
    JNIEnv *env,
    GraphicsContextPropertiesInfo *ctxProperties,
    jint target,
    jint numPts,
    jfloatArray pts)
{
    /*
     * checking of the availability of filter4 functionality
     * is already done in the java side
     */

    JNIEnv table = *env;
    jfloat *funcPts = NULL;
	
    if (numPts > 0) {
        funcPts = (jfloat *)(*(table->GetPrimitiveArrayCritical))(
					env, pts, NULL);
    }

    ctxProperties->glTexFilterFuncSGIS(target, ctxProperties->filter4_enum,
						numPts, funcPts);

    if (funcPts != NULL) {
        (*(table->ReleasePrimitiveArrayCritical))(env, pts, funcPts, 0);	
    }
}

void updateTextureAnisotropicFilter(
    JNIEnv *env,
    GraphicsContextPropertiesInfo *ctxProperties,
    jint target,
    jfloat degree)
{
    /*
     * checking of the availability of anisotropic filter functionality
     * is already done in the java side
     */
    glTexParameterf(target, 
			ctxProperties->texture_filter_anisotropic_ext_enum, 
			degree);
}


/*
 * common function to define 2D texture image for different target
 */
void updateTexture2DImage(
    JNIEnv *env, 
    GraphicsContextPropertiesInfo *ctxProperties,
    jint target,
    jint numLevels,
    jint level,
    jint internalFormat, 
    jint format, 
    jint width, 
    jint height, 
    jint boundaryWidth,
    jbyteArray imageYup)
{
    GLenum oglFormat = 0, oglInternalFormat=0;
    JNIEnv table = *env;
    jbyte *byteData;
    jshort *shortData;
    
    switch (internalFormat) {
        case INTENSITY:
	    oglInternalFormat = GL_INTENSITY;
	    break;
        case LUMINANCE:
	    oglInternalFormat = GL_LUMINANCE;
	    break;
        case ALPHA:
	    oglInternalFormat = GL_ALPHA;
	    break;
        case LUMINANCE_ALPHA:
	    oglInternalFormat = GL_LUMINANCE_ALPHA;
	    break;
        case J3D_RGB: 
	    oglInternalFormat = GL_RGB;
	    break;
        case J3D_RGBA:
	    oglInternalFormat = GL_RGBA;
	    break;
    }
    switch (format) {
        case FORMAT_BYTE_RGBA:         
	    /* all RGB types are stored as RGBA */
	    oglFormat = GL_RGBA;
	    break;
        case FORMAT_BYTE_RGB:         
	    oglFormat = GL_RGB;
	    break;

        case FORMAT_BYTE_ABGR:         
	    if (ctxProperties->abgr_ext) { /* If its zero, should never come here! */
		oglFormat = GL_ABGR_EXT;
	    }
	    break;
	    
        case FORMAT_BYTE_BGR:         
	    if (ctxProperties->bgr_ext) { /* If its zero, should never come here! */
		oglFormat = ctxProperties->bgr_ext_enum;
	    }
	    break;

        case FORMAT_BYTE_LA: 
	    /* all LA types are stored as LA8 */
	    oglFormat = GL_LUMINANCE_ALPHA;
	    break;
        case FORMAT_BYTE_GRAY:
        case FORMAT_USHORT_GRAY:	    
            if (oglInternalFormat == GL_ALPHA) {
	        oglFormat = GL_ALPHA;
            } else  {
	        oglFormat = GL_LUMINANCE;
	    }
            break;
    }
    /*
    fprintf(stderr,"native updateTextureImage\n");
    fprintf(stderr,"internalFormat = %x\n",internalFormat);
    fprintf(stderr,"format = %x\n",format);
    fprintf(stderr,"oglFormat = %x\n",oglFormat);
    fprintf(stderr,"oglInternalFormat = %x\n",oglInternalFormat);
    fprintf(stderr,"boundaryWidth= %d\n", boundaryWidth);
    */
    if (imageYup != NULL) {
	if (format != FORMAT_USHORT_GRAY) {
	    byteData = (jbyte *)(*(table->GetPrimitiveArrayCritical))(env,
								      imageYup, 
								      NULL);
    /*
    {
        jbyte *c = byteData;
	int i, j;
	for (i = 0; i < 1; i++) {
	    for (j = 0; j < 8; j++, c++) {
		fprintf(stderr, "%x ",*c);
	    }
	    fprintf(stderr, "\n");
	}
    }
    */
	}
	else { /* unsigned short */
	    shortData = (jshort *)(*(table->GetPrimitiveArrayCritical))(env,
									imageYup, 
									NULL);
	}
    }
    else {
	byteData = NULL;
	shortData = NULL;
    }
    if (format != FORMAT_USHORT_GRAY) {
	glTexImage2D(target, level, oglInternalFormat, 
		     width, height, boundaryWidth, 
		     oglFormat, GL_UNSIGNED_BYTE, (GLvoid *)byteData);
    }
    else {
	glTexImage2D(target, level, oglInternalFormat, 
		     width, height, boundaryWidth, 
		     oglFormat, GL_UNSIGNED_SHORT, (GLvoid *)shortData);
    }

    if (imageYup != NULL) {
	if (format != FORMAT_USHORT_GRAY) {
	    (*(table->ReleasePrimitiveArrayCritical))(env, imageYup, byteData, 0);
	}
	else {
	    (*(table->ReleasePrimitiveArrayCritical))(env, imageYup, shortData, 0);
	}
    }

    /* No idea why we need following call. */
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}


/* 
 * common function to update 2D texture image for different target
 */

void updateTexture2DSubImage(
    JNIEnv *env, 
    GraphicsContextPropertiesInfo *ctxProperties,
    jint target,
    jint level,
    jint xoffset,
    jint yoffset,
    jint internalFormat, 
    jint format,
    jint imgXOffset,
    jint imgYOffset,
    jint tilew,
    jint width, 
    jint height,
    jbyteArray image) {
 
    GLenum oglFormat = 0, oglInternalFormat=0;
    JNIEnv table = *env;
    jbyte *byteData, *tmpByte;
    jshort *shortData, *tmpShort;
    jint numBytes = 0;
    jboolean pixelStore = JNI_FALSE;


    switch (internalFormat) {
        case INTENSITY:
	    oglInternalFormat = GL_INTENSITY;
	    break;
        case LUMINANCE:
	    oglInternalFormat = GL_LUMINANCE;
	    break;
        case ALPHA:
	    oglInternalFormat = GL_ALPHA;
	    break;
        case LUMINANCE_ALPHA:
	    oglInternalFormat = GL_LUMINANCE_ALPHA;
	    break;
        case J3D_RGB: 
	    oglInternalFormat = GL_RGB;
	    break;
        case J3D_RGBA:
	    oglInternalFormat = GL_RGBA;
	    break;
    }

    switch (format) {
        case FORMAT_BYTE_RGBA:         
	    /* all RGB types are stored as RGBA */
	    oglFormat = GL_RGBA;
	    numBytes = 4;
	    break;
        case FORMAT_BYTE_RGB:         
	    oglFormat = GL_RGB;
	    numBytes = 3;
	    break;

        case FORMAT_BYTE_ABGR:         
	    if (ctxProperties->abgr_ext) { /* If its zero, should never come here! */
		oglFormat = GL_ABGR_EXT;
		numBytes = 4;
	    }
	    break;
        case FORMAT_BYTE_BGR:         
	    if (ctxProperties->bgr_ext) { /* If its zero, should never come here! */
		oglFormat = ctxProperties->bgr_ext_enum;
		numBytes = 3;
	    }
	    break;

        case FORMAT_BYTE_LA: 
	    /* all LA types are stored as LA8 */
	    oglFormat = GL_LUMINANCE_ALPHA;
	    numBytes = 2;
	    break;
        case FORMAT_BYTE_GRAY:
            if (oglInternalFormat == GL_ALPHA) {
	        oglFormat = GL_ALPHA;
            } else  {
	        oglFormat = GL_LUMINANCE;
	    }
	    numBytes = 1;
        case FORMAT_USHORT_GRAY:	    
           if (oglInternalFormat == GL_ALPHA) {
	        oglFormat = GL_ALPHA;
            } else  {
	        oglFormat = GL_LUMINANCE;
	    }
	    numBytes = 2;
            break;
    }
    /*
    fprintf(stderr,"format = %x\n",format);
    fprintf(stderr,"oglFormat = %x\n",oglFormat);
    fprintf(stderr, "imgXOffset = %d\n",imgXOffset);
    fprintf(stderr, "imgYOffset = %d\n",imgYOffset);
    fprintf(stderr, "xoffset = %d\n",xoffset);
    fprintf(stderr, "yoffset = %d\n",yoffset);
    fprintf(stderr, "tilew = %d\n",tilew);
    fprintf(stderr, "numBytes = %d\n",numBytes);
    fprintf(stderr, "width = %d\n",width);
    fprintf(stderr, "height = %d\n",height);
    */
    if (imgXOffset > 0 || (width < tilew)) {
	pixelStore = JNI_TRUE;
	glPixelStorei(GL_UNPACK_ROW_LENGTH, tilew);
    }
	
    if (format != FORMAT_USHORT_GRAY) {
	int off = 0;
	byteData = (jbyte *)(*(table->GetPrimitiveArrayCritical))(env,
								  image, 
								  NULL);
	/* offset by the imageOffset */
	off = (tilew * imgYOffset + imgXOffset) * numBytes;
	tmpByte = byteData+(off);

/*
printf("tmpByte: %x %x %x %x\n", *(tmpByte), *(tmpByte+1),
					*(tmpByte+2), *(tmpByte+3));
*/

	glTexSubImage2D(target, level, xoffset, yoffset, width, height, 
		     oglFormat, GL_UNSIGNED_BYTE, (GLvoid *)tmpByte);
	(*(table->ReleasePrimitiveArrayCritical))(env, image, byteData, 0);	
    } else { /* unsigned short */
	shortData = (jshort *)(*(table->GetPrimitiveArrayCritical))(env,
								    image, 
								    NULL);
	tmpShort = (jshort*)((jbyte*)shortData+
			     (tilew * imgYOffset + imgXOffset)*numBytes);
	glTexSubImage2D(target, level, xoffset, yoffset, width, height, 
		     oglFormat, GL_UNSIGNED_SHORT, (GLvoid *)tmpShort);
	(*(table->ReleasePrimitiveArrayCritical))(env, image, shortData, 0);
    }
    if (pixelStore) {
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    }
}

JNIEXPORT
void JNICALL Java_javax_media_j3d_TextureRetained_bindTexture(
    JNIEnv *env, 
    jobject texture,
    jlong ctxInfo,    
    jint objectId,
    jboolean enable) 
{
    GraphicsContextPropertiesInfo *ctxProperties = (GraphicsContextPropertiesInfo *)ctxInfo;

    if (ctxProperties->textureCubeMapAvailable) {
	glDisable(ctxProperties->texture_cube_map_ext_enum);
    }
    if (ctxProperties->texture3DAvailable) {
	glDisable(ctxProperties->texture_3D_ext_enum);	
    }
    
    if (enable == JNI_FALSE) {
        glDisable(GL_TEXTURE_2D);

    } else {
        glBindTexture(GL_TEXTURE_2D, objectId);
        glEnable(GL_TEXTURE_2D);
    }
}

JNIEXPORT
void JNICALL Java_javax_media_j3d_TextureRetained_updateTextureFilterModes(
    JNIEnv *env, 
    jobject texture,
    jlong ctxInfo,    
    jint minFilter,
    jint magFilter)
{
    GraphicsContextPropertiesInfo *ctxProperties = (GraphicsContextPropertiesInfo *)ctxInfo;

    updateTextureFilterModes(ctxProperties, GL_TEXTURE_2D, 
				minFilter, magFilter);
}

JNIEXPORT
void JNICALL Java_javax_media_j3d_TextureRetained_updateTextureLodRange(
    JNIEnv *env, 
    jobject texture,
    jlong ctxInfo,    
    jint baseLevel, 
    jint maximumLevel,
    jfloat minimumLOD,
    jfloat maximumLOD) 
{

    GraphicsContextPropertiesInfo *ctxProperties = (GraphicsContextPropertiesInfo *)ctxInfo;

    updateTextureLodRange(ctxProperties, GL_TEXTURE_2D, 
			baseLevel, maximumLevel,
			minimumLOD, maximumLOD);
}

JNIEXPORT
void JNICALL Java_javax_media_j3d_TextureRetained_updateTextureLodOffset(
    JNIEnv *env,
    jobject texture,
    jlong ctxInfo,
    jfloat lodOffsetS,
    jfloat lodOffsetT,
    jfloat lodOffsetR)
{
    GraphicsContextPropertiesInfo *ctxProperties = (GraphicsContextPropertiesInfo *)ctxInfo;

    updateTextureLodOffset(ctxProperties, GL_TEXTURE_2D, 
			lodOffsetS, lodOffsetT, lodOffsetR);
}


JNIEXPORT
void JNICALL Java_javax_media_j3d_TextureRetained_updateTextureBoundary(
    JNIEnv *env, 
    jobject texture,
    jlong ctxInfo,    
    jint boundaryModeS, 
    jint boundaryModeT, 
    jfloat boundaryRed, 
    jfloat boundaryGreen, 
    jfloat boundaryBlue, 
    jfloat boundaryAlpha)
{
    GraphicsContextPropertiesInfo *ctxProperties = (GraphicsContextPropertiesInfo *)ctxInfo;

    updateTextureBoundary(ctxProperties, GL_TEXTURE_2D, 
			boundaryModeS, boundaryModeT, -1,
			boundaryRed, boundaryGreen, 
			boundaryBlue, boundaryAlpha);
}

JNIEXPORT
void JNICALL Java_javax_media_j3d_TextureRetained_updateTextureSharpenFunc(
    JNIEnv *env, 
    jobject texture,
    jlong ctxInfo,    
    jint numPts,
    jfloatArray pts)
{
    GraphicsContextPropertiesInfo *ctxProperties = (GraphicsContextPropertiesInfo *)ctxInfo;

    updateTextureSharpenFunc(env, ctxProperties, GL_TEXTURE_2D, numPts, pts);
}

JNIEXPORT
void JNICALL Java_javax_media_j3d_TextureRetained_updateTextureFilter4Func(
    JNIEnv *env, 
    jobject texture,
    jlong ctxInfo,    
    jint numPts,
    jfloatArray pts)
{
    GraphicsContextPropertiesInfo *ctxProperties = (GraphicsContextPropertiesInfo *)ctxInfo;

    updateTextureFilter4Func(env, ctxProperties, GL_TEXTURE_2D, numPts, pts);
}

JNIEXPORT
void JNICALL Java_javax_media_j3d_TextureRetained_updateTextureAnisotropicFilter(
    JNIEnv *env, 
    jobject texture,
    jlong ctxInfo,    
    jfloat degree)
{
    GraphicsContextPropertiesInfo *ctxProperties = (GraphicsContextPropertiesInfo *)ctxInfo;

    updateTextureAnisotropicFilter(env, ctxProperties, GL_TEXTURE_2D, degree);
}

JNIEXPORT
void JNICALL Java_javax_media_j3d_Texture2DRetained_updateTextureSubImage(
    JNIEnv *env, 
    jobject texture,
    jlong ctxInfo,
    jint level,
    jint xoffset,
    jint yoffset,
    jint internalFormat, 
    jint format,
    jint imgXOffset,
    jint imgYOffset,
    jint tilew,
    jint width, 
    jint height,
    jbyteArray image) {
 
    GraphicsContextPropertiesInfo *ctxProperties = (GraphicsContextPropertiesInfo *)ctxInfo;
    updateTexture2DSubImage(env, ctxProperties, GL_TEXTURE_2D,
				level, xoffset, yoffset,
				internalFormat, format,
				imgXOffset, imgYOffset, tilew, width, height,
				image);
}


JNIEXPORT
void JNICALL Java_javax_media_j3d_Texture2DRetained_updateTextureImage(
    JNIEnv *env, 
    jobject texture,
    jlong ctxInfo,    
    jint numLevels,
    jint level,
    jint internalFormat, 
    jint format, 
    jint width, 
    jint height, 
    jint boundaryWidth,
    jbyteArray imageYup)
{
    GraphicsContextPropertiesInfo *ctxProperties = (GraphicsContextPropertiesInfo *)ctxInfo;

    updateTexture2DImage(env, ctxProperties, GL_TEXTURE_2D,
			numLevels, level, internalFormat, format,
			width, height, boundaryWidth, imageYup);
}

JNIEXPORT
void JNICALL Java_javax_media_j3d_Texture2DRetained_updateDetailTextureParameters(
    JNIEnv *env, 
    jobject texture,
    jlong ctxInfo,    
    jint mode,
    jint level,
    jint nPts,
    jfloatArray funcPts)
{
    GraphicsContextPropertiesInfo *ctxProperties = (GraphicsContextPropertiesInfo *)ctxInfo;
    float *pts = NULL;
    JNIEnv table = *env;

    if (ctxProperties->textureDetailAvailable) {
	switch (mode) {
        case javax_media_j3d_Texture2D_DETAIL_ADD:
	    glTexParameterf(GL_TEXTURE_2D, 
			ctxProperties->texture_detail_mode_enum, GL_ADD);
	    break;
        case javax_media_j3d_Texture2D_DETAIL_MODULATE:
	    glTexParameterf(GL_TEXTURE_2D, 
			ctxProperties->texture_detail_mode_enum, GL_MODULATE);
	    break;
	}

	glTexParameteri(GL_TEXTURE_2D, 
			ctxProperties->texture_detail_level_enum, -level);

	if (nPts > 0) {
	    pts = (jfloat *)(*(table->GetPrimitiveArrayCritical))(env, 
				funcPts, NULL);
	}
        ctxProperties->glDetailTexFuncSGIS(GL_TEXTURE_2D, nPts, pts);

	if (pts != NULL) {
	    (*(table->ReleasePrimitiveArrayCritical))(env, funcPts, pts, 0);
	}
    }
}

JNIEXPORT
void JNICALL Java_javax_media_j3d_DetailTextureImage_bindTexture(
    JNIEnv *env, 
    jobject texture,
    jlong ctxInfo,    
    jint objectId)
{
    GraphicsContextPropertiesInfo *ctxProperties = (GraphicsContextPropertiesInfo *)ctxInfo;

    if (ctxProperties->textureDetailAvailable) {
        glBindTexture(ctxProperties->texture_detail_ext_enum, objectId);
    }
}

JNIEXPORT
void JNICALL Java_javax_media_j3d_DetailTextureImage_updateTextureSubImage(
    JNIEnv *env, 
    jobject texture,
    jlong ctxInfo,
    jint level,
    jint xoffset,
    jint yoffset,
    jint internalFormat, 
    jint format,
    jint imgXOffset,
    jint imgYOffset,
    jint tilew,
    jint width, 
    jint height,
    jbyteArray image) {
 
    GraphicsContextPropertiesInfo *ctxProperties = (GraphicsContextPropertiesInfo *)ctxInfo;

    if (ctxProperties->textureDetailAvailable) {
        updateTexture2DSubImage(env, ctxProperties, 
				ctxProperties->texture_detail_ext_enum,
				level, xoffset, yoffset,
				internalFormat, format,
				imgXOffset, imgYOffset, tilew, width, height,
				image);
    }
}

JNIEXPORT
void JNICALL Java_javax_media_j3d_DetailTextureImage_updateTextureImage(
    JNIEnv *env, 
    jobject texture,
    jlong ctxInfo,    
    jint numLevels,
    jint level,
    jint internalFormat, 
    jint format, 
    jint width, 
    jint height, 
    jint boundaryWidth,
    jbyteArray imageYup)
{
    GraphicsContextPropertiesInfo *ctxProperties = (GraphicsContextPropertiesInfo *)ctxInfo;

    if (ctxProperties->textureDetailAvailable) {
        updateTexture2DImage(env, ctxProperties, 
			ctxProperties->texture_detail_ext_enum,
			numLevels, level, internalFormat, format,
			width, height, boundaryWidth, imageYup);
    }
}

JNIEXPORT
void JNICALL Java_javax_media_j3d_Texture3DRetained_bindTexture(
    JNIEnv *env, 
    jobject texture,
    jlong ctxInfo,    
    jint objectId,
    jboolean enable) 
{
    GraphicsContextPropertiesInfo *ctxProperties = (GraphicsContextPropertiesInfo *)ctxInfo;
 
    if (ctxProperties->textureCubeMapAvailable) {
	/* textureCubeMap will take precedure over 3D Texture */
	glDisable(ctxProperties->texture_cube_map_ext_enum);
    }
    
    if (enable == JNI_FALSE) {
	if(ctxProperties->texture3DAvailable)  {
	    glDisable(ctxProperties->texture_3D_ext_enum);
	}
	
    } else {
	if(ctxProperties->texture3DAvailable){
	    glBindTexture(ctxProperties->texture_3D_ext_enum, objectId);
	    glEnable(ctxProperties->texture_3D_ext_enum);
	}
    }
}


JNIEXPORT
void JNICALL Java_javax_media_j3d_Texture3DRetained_updateTextureFilterModes(
    JNIEnv *env,
    jobject texture,
    jlong ctxInfo,
    jint minFilter,
    jint magFilter)
{
    GraphicsContextPropertiesInfo *ctxProperties = (GraphicsContextPropertiesInfo *)ctxInfo;

    updateTextureFilterModes(ctxProperties, GL_TEXTURE_3D,
                                minFilter, magFilter);
}



JNIEXPORT
void JNICALL Java_javax_media_j3d_Texture3DRetained_updateTextureLodRange(
    JNIEnv *env, 
    jobject texture,
    jlong ctxInfo,    
    jint baseLevel, 
    jint maximumLevel,
    jfloat minimumLOD,
    jfloat maximumLOD) 
{

    GraphicsContextPropertiesInfo *ctxProperties = (GraphicsContextPropertiesInfo *)ctxInfo;

    updateTextureLodRange(ctxProperties, GL_TEXTURE_3D, 
			baseLevel, maximumLevel,
			minimumLOD, maximumLOD);
}

JNIEXPORT
void JNICALL Java_javax_media_j3d_Texture3DRetained_updateTextureLodOffset(
    JNIEnv *env,
    jobject texture,
    jlong ctxInfo,
    jfloat lodOffsetS,
    jfloat lodOffsetT,
    jfloat lodOffsetR)
{
    GraphicsContextPropertiesInfo *ctxProperties = (GraphicsContextPropertiesInfo *)ctxInfo;

    updateTextureLodOffset(ctxProperties, GL_TEXTURE_3D, 
			lodOffsetS, lodOffsetT, lodOffsetR);
}


JNIEXPORT
void JNICALL Java_javax_media_j3d_Texture3DRetained_updateTextureBoundary(
    JNIEnv *env,
    jobject texture,
    jlong ctxInfo,
    jint boundaryModeS,
    jint boundaryModeT,
    jint boundaryModeR,
    jfloat boundaryRed,
    jfloat boundaryGreen,
    jfloat boundaryBlue,
    jfloat boundaryAlpha)
{
    GraphicsContextPropertiesInfo *ctxProperties = (GraphicsContextPropertiesInfo *)ctxInfo;

    updateTextureBoundary(ctxProperties, GL_TEXTURE_3D,
                        boundaryModeS, boundaryModeT, boundaryModeR,
                        boundaryRed, boundaryGreen,
                        boundaryBlue, boundaryAlpha);
}

JNIEXPORT
void JNICALL Java_javax_media_j3d_Texture3DRetained_updateTextureSharpenFunc(
    JNIEnv *env,
    jobject texture,
    jlong ctxInfo,
    jint numPts,
    jfloatArray pts)
{
    GraphicsContextPropertiesInfo *ctxProperties = (GraphicsContextPropertiesInfo *)ctxInfo;

    updateTextureSharpenFunc(env, ctxProperties, GL_TEXTURE_3D, numPts, pts);
}

JNIEXPORT
void JNICALL Java_javax_media_j3d_Texture3DRetained_updateTextureFilter4Func(
    JNIEnv *env, 
    jobject texture,
    jlong ctxInfo,    
    jint numPts,
    jfloatArray pts)
{
    GraphicsContextPropertiesInfo *ctxProperties = (GraphicsContextPropertiesInfo *)ctxInfo;

    updateTextureFilter4Func(env, ctxProperties, GL_TEXTURE_3D, numPts, pts);
}

JNIEXPORT
void JNICALL Java_javax_media_j3d_Texture3DRetained_updateTextureAnisotropicFilter(
    JNIEnv *env, 
    jobject texture,
    jlong ctxInfo,    
    jfloat degree)
{
    GraphicsContextPropertiesInfo *ctxProperties = (GraphicsContextPropertiesInfo *)ctxInfo;

    updateTextureAnisotropicFilter(env, ctxProperties, GL_TEXTURE_3D, degree);
}

								       
JNIEXPORT
void JNICALL Java_javax_media_j3d_Texture3DRetained_updateTextureImage(
    JNIEnv *env, 
    jobject texture,
    jlong ctxInfo,
    jint numLevels,
    jint level,
    jint internalFormat, 
    jint format, 
    jint width, 
    jint height, 
    jint depth,
    jint boundaryWidth,
    jbyteArray imageYup)
{
    GraphicsContextPropertiesInfo *ctxProperties = (GraphicsContextPropertiesInfo *)ctxInfo;

    if(ctxProperties->texture3DAvailable) {

	GLenum oglFormat = 0, oglInternalFormat=0;
	JNIEnv table = *env;
	jbyte *byteData;
	jshort *shortData;

	switch (internalFormat) {
        case INTENSITY:
	    oglInternalFormat = GL_INTENSITY;
	    break;
        case LUMINANCE:
	    oglInternalFormat = GL_LUMINANCE;
	    break;
        case ALPHA:
	    oglInternalFormat = GL_ALPHA;
	    break;
        case LUMINANCE_ALPHA:
	    oglInternalFormat = GL_LUMINANCE_ALPHA;
	    break;
        case J3D_RGB: 
	    oglInternalFormat = GL_RGB;
	    break;
        case J3D_RGBA:
	    oglInternalFormat = GL_RGBA;
	    break;
	}

	switch (format) {
        case FORMAT_BYTE_RGBA:         
	    /* all RGB types are stored as RGBA */
	    oglFormat = GL_RGBA;
	    break;
        case FORMAT_BYTE_RGB:         
	    oglFormat = GL_RGB;
	    break;

        case FORMAT_BYTE_ABGR:         
	    if (ctxProperties->abgr_ext) { /* If its zero, should never come here! */
		oglFormat = GL_ABGR_EXT;
	    }
	    break;
        case FORMAT_BYTE_BGR:         
	    if (ctxProperties->bgr_ext) { /* If its zero, should never come here! */
		oglFormat = ctxProperties->bgr_ext_enum;
	    }
	    break;
        case FORMAT_BYTE_LA: 
	    /* all LA types are stored as LA8 */
	    oglFormat = GL_LUMINANCE_ALPHA;
	    break;
        case FORMAT_BYTE_GRAY:
        case FORMAT_USHORT_GRAY:	    
            if (oglInternalFormat == GL_ALPHA) {
	        oglFormat = GL_ALPHA;
            } else  {
	        oglFormat = GL_LUMINANCE;
	    }
            break;
	}

	/*
	  fprintf(stderr,"internalFormat = %x\n",internalFormat);
	  fprintf(stderr,"format = %x\n",format);
	  fprintf(stderr,"oglFormat = %x\n",oglFormat);
	  fprintf(stderr,"oglInternalFormat = %x\n",oglInternalFormat);
	  */
	if (imageYup != NULL) {
	    if (format != FORMAT_USHORT_GRAY) {
		byteData = (jbyte *)(*(table->GetPrimitiveArrayCritical))(env,
									  imageYup, 
									  NULL);
	    }
	    else { /* unsigned short */
		shortData = (jshort *)(*(table->GetPrimitiveArrayCritical))(env,
									    imageYup, 
									    NULL);
	
	    }
	} else {
	    byteData = NULL;
	    shortData = NULL;
	}

	if (format != FORMAT_USHORT_GRAY) {

	    ctxProperties->glTexImage3DEXT(ctxProperties->texture_3D_ext_enum, 
			level, oglInternalFormat, 
			width, height, depth, boundaryWidth, 
			oglFormat, GL_UNSIGNED_BYTE, 
			(GLvoid *)byteData);
	}
	else {
	    ctxProperties->glTexImage3DEXT(ctxProperties->texture_3D_ext_enum, 
			level, oglInternalFormat, 
			width, height, depth, boundaryWidth, 
			oglFormat, GL_UNSIGNED_SHORT, 
			(GLvoid *)shortData);
	}
	if (imageYup != NULL) {
	    if (format != FORMAT_USHORT_GRAY) {
		(*(table->ReleasePrimitiveArrayCritical))(env, imageYup, byteData, 0);
	    } else { /* unsigned short */
		(*(table->ReleasePrimitiveArrayCritical))(env, imageYup, shortData, 0);
	    
	    }
	}
    
	/* No idea why we need following call. */
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    }

}

JNIEXPORT
void JNICALL Java_javax_media_j3d_Texture3DRetained_updateTextureSubImage(
    JNIEnv *env, 
    jobject texture,
    jlong ctxInfo,
    jint level,
    jint xoffset,
    jint yoffset,
    jint zoffset,
    jint internalFormat, 
    jint format,
    jint imgXOffset,
    jint imgYOffset,
    jint imgZOffset,
    jint tilew,
    jint tileh,
    jint width, 
    jint height,
    jint depth,
    jbyteArray image) {
 
    GraphicsContextPropertiesInfo *ctxProperties = (GraphicsContextPropertiesInfo *)ctxInfo;
 
    if (ctxProperties->texture3DAvailable) {
        GLenum oglFormat = 0, oglInternalFormat=0;
        JNIEnv table = *env;
        jbyte *byteData, *tmpByte;
        jshort *shortData, *tmpShort;
        jint numBytes = 0;
        jboolean pixelStore = JNI_FALSE;

        switch (internalFormat) {
            case INTENSITY:
	        oglInternalFormat = GL_INTENSITY;
	        break;
            case LUMINANCE:
	        oglInternalFormat = GL_LUMINANCE;
	        break;
            case ALPHA:
	        oglInternalFormat = GL_ALPHA;
	        break;
            case LUMINANCE_ALPHA:
	        oglInternalFormat = GL_LUMINANCE_ALPHA;
	        break;
            case J3D_RGB: 
	        oglInternalFormat = GL_RGB;
	        break;
            case J3D_RGBA:
	        oglInternalFormat = GL_RGBA;
	        break;
        }
    
        switch (format) {
            case FORMAT_BYTE_RGBA:         
	        /* all RGB types are stored as RGBA */
	        oglFormat = GL_RGBA;
	        numBytes = 4;
	        break;
            case FORMAT_BYTE_RGB:         
	        oglFormat = GL_RGB;
	        numBytes = 3;
	        break;
    
            case FORMAT_BYTE_ABGR:         
	        if (ctxProperties->abgr_ext) { /* If its zero, should never come here! */
		    oglFormat = GL_ABGR_EXT;
		    numBytes = 4;
	        }
	        break;
            case FORMAT_BYTE_BGR:         
	        if (ctxProperties->bgr_ext) { /* If its zero, should never come here! */
		    oglFormat = ctxProperties->bgr_ext_enum;
		    numBytes = 3;
	        }
	        break;
    
            case FORMAT_BYTE_LA: 
	        /* all LA types are stored as LA8 */
	        oglFormat = GL_LUMINANCE_ALPHA;
	        numBytes = 2;
	        break;
            case FORMAT_BYTE_GRAY:
                if (oglInternalFormat == GL_ALPHA) {
	            oglFormat = GL_ALPHA;
                } else  {
	            oglFormat = GL_LUMINANCE;
	        }
	        numBytes = 1;
            case FORMAT_USHORT_GRAY:	    
               if (oglInternalFormat == GL_ALPHA) {
	            oglFormat = GL_ALPHA;
                } else  {
	            oglFormat = GL_LUMINANCE;
	        }
	        numBytes = 2;
                break;
        }
        /*
        fprintf(stderr,"format = %x\n",format);
        fprintf(stderr,"oglFormat = %x\n",oglFormat);
        fprintf(stderr, "imgXOffset = %d\n",imgXOffset);
        fprintf(stderr, "imgYOffset = %d\n",imgYOffset);
        fprintf(stderr, "imgZOffset = %d\n",imgZOffset);
        fprintf(stderr, "xoffset = %d\n",xoffset);
        fprintf(stderr, "yoffset = %d\n",yoffset);
        fprintf(stderr, "zoffset = %d\n",zoffset);
        fprintf(stderr, "tilew = %d\n",tilew);
        fprintf(stderr, "tileh = %d\n",tilew);
        fprintf(stderr, "numBytes = %d\n",numBytes);
        fprintf(stderr, "width = %d\n",width);
        fprintf(stderr, "height = %d\n",height);
        fprintf(stderr, "depth = %d\n",depth);
        */
        if (imgXOffset > 0 || (width < tilew)) {
	    pixelStore = JNI_TRUE;
	    glPixelStorei(GL_UNPACK_ROW_LENGTH, tilew);
        }
    	
        if (format != FORMAT_USHORT_GRAY) {
	    byteData = (jbyte *)(*(table->GetPrimitiveArrayCritical))(env,
				    image, NULL);
    
            tmpByte = byteData +
                           (tilew * tileh * imgZOffset +
                            tilew * imgYOffset + imgXOffset) * numBytes;
    
            ctxProperties->glTexSubImage3DEXT(
                                    ctxProperties->texture_3D_ext_enum,
                                    level, xoffset, yoffset, zoffset,
                                    width, height, depth,
                                    oglFormat, GL_UNSIGNED_BYTE,
                                    (GLvoid *)tmpByte);
    
	    (*(table->ReleasePrimitiveArrayCritical))(env, image, byteData, 0);	
        } else { /* unsigned short */
	    shortData = (jshort *)(*(table->GetPrimitiveArrayCritical))(env,
				    image, NULL);
	    tmpShort = (jshort*)((jbyte*)shortData+
			         (tilew * tileh * imgZOffset +
			          tilew * imgYOffset + imgXOffset)*numBytes);
    
            ctxProperties->glTexSubImage3DEXT(
                                    ctxProperties->texture_3D_ext_enum,
                                    level, xoffset, yoffset, zoffset,
                                    width, height, depth,
                                    oglFormat, GL_UNSIGNED_SHORT,
                                    (GLvoid *)tmpShort);
	    (*(table->ReleasePrimitiveArrayCritical))(env, image, shortData, 0);
        }
        if (pixelStore) {
	    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
        }
    }
}
    

/*
 * mapping from java enum to gl enum
 */

jint _gl_textureCubeMapFace[] = {
	GL_TEXTURE_CUBE_MAP_POSITIVE_X_EXT,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_X_EXT,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Y_EXT,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_EXT,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Z_EXT,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_EXT,
};


JNIEXPORT
void JNICALL Java_javax_media_j3d_TextureCubeMapRetained_bindTexture(
    JNIEnv *env, 
    jobject texture,
    jlong ctxInfo,    
    jint objectId,
    jboolean enable) 
{
    GraphicsContextPropertiesInfo *ctxProperties = (GraphicsContextPropertiesInfo *)ctxInfo;

    /*
     * TextureCubeMap will take precedure over 3D Texture so
     * there is no need to disable 3D Texture here.
     */
    if (ctxProperties->textureCubeMapAvailable) {
        if (enable == JNI_FALSE) {
	  glDisable(ctxProperties->texture_cube_map_ext_enum);
        } else {
	    glBindTexture(ctxProperties->texture_cube_map_ext_enum, objectId);
	    glEnable(ctxProperties->texture_cube_map_ext_enum);
	}
    }
}

JNIEXPORT
void JNICALL Java_javax_media_j3d_TextureCubeMapRetained_updateTextureFilterModes(
    JNIEnv *env,
    jobject texture,
    jlong ctxInfo,
    jint minFilter,
    jint magFilter)
{
    GraphicsContextPropertiesInfo *ctxProperties = (GraphicsContextPropertiesInfo *)ctxInfo;

    if (ctxProperties->textureCubeMapAvailable) {
        updateTextureFilterModes(ctxProperties, 
				ctxProperties->texture_cube_map_ext_enum,
                                minFilter, magFilter);
    }
}



JNIEXPORT
void JNICALL Java_javax_media_j3d_TextureCubeMapRetained_updateTextureLodRange(
    JNIEnv *env, 
    jobject texture,
    jlong ctxInfo,    
    jint baseLevel, 
    jint maximumLevel,
    jfloat minimumLOD,
    jfloat maximumLOD) 
{

    GraphicsContextPropertiesInfo *ctxProperties = (GraphicsContextPropertiesInfo *)ctxInfo;

    updateTextureLodRange(ctxProperties, 
			ctxProperties->texture_cube_map_ext_enum,
			baseLevel, maximumLevel,
			minimumLOD, maximumLOD);
}

JNIEXPORT
void JNICALL Java_javax_media_j3d_TextureCubeMapRetained_updateTextureLodOffset(
    JNIEnv *env,
    jobject texture,
    jlong ctxInfo,
    jfloat lodOffsetS,
    jfloat lodOffsetT,
    jfloat lodOffsetR)
{
    GraphicsContextPropertiesInfo *ctxProperties = (GraphicsContextPropertiesInfo *)ctxInfo;

    updateTextureLodOffset(ctxProperties, 
			ctxProperties->texture_cube_map_ext_enum,
			lodOffsetS, lodOffsetT, lodOffsetR);
}


JNIEXPORT
void JNICALL Java_javax_media_j3d_TextureCubeMapRetained_updateTextureBoundary(
    JNIEnv *env, 
    jobject texture,
    jlong ctxInfo,    
    jint boundaryModeS, 
    jint boundaryModeT, 
    jfloat boundaryRed, 
    jfloat boundaryGreen, 
    jfloat boundaryBlue, 
    jfloat boundaryAlpha)
{
    GraphicsContextPropertiesInfo *ctxProperties = (GraphicsContextPropertiesInfo *)ctxInfo;

    if (ctxProperties->textureCubeMapAvailable) {
	updateTextureBoundary(ctxProperties, 
				ctxProperties->texture_cube_map_ext_enum,
				boundaryModeS, boundaryModeT, -1,
				boundaryRed, boundaryGreen,
				boundaryBlue, boundaryAlpha);
    }
}

JNIEXPORT
void JNICALL Java_javax_media_j3d_TextureCubeMapRetained_updateTextureSharpenFunc(
    JNIEnv *env,
    jobject texture,
    jlong ctxInfo,
    jint numPts,
    jfloatArray pts)
{
    GraphicsContextPropertiesInfo *ctxProperties = (GraphicsContextPropertiesInfo *)ctxInfo;

    if (ctxProperties->textureCubeMapAvailable) {
        updateTextureSharpenFunc(env, ctxProperties, 
				ctxProperties->texture_cube_map_ext_enum, 
				numPts, pts);
    }
}


JNIEXPORT
void JNICALL Java_javax_media_j3d_TextureCubeMapRetained_updateTextureFilter4Func(
    JNIEnv *env, 
    jobject texture,
    jlong ctxInfo,    
    jint numPts,
    jfloatArray pts)
{
    GraphicsContextPropertiesInfo *ctxProperties = (GraphicsContextPropertiesInfo *)ctxInfo;

    if (ctxProperties->textureCubeMapAvailable) {
        updateTextureFilter4Func(env, ctxProperties, 
				ctxProperties->texture_cube_map_ext_enum, 
				numPts, pts);
    }
}

JNIEXPORT
void JNICALL Java_javax_media_j3d_TextureCubeMapRetained_updateTextureAnisotropicFilter(
    JNIEnv *env, 
    jobject texture,
    jlong ctxInfo,    
    jfloat degree)
{
    GraphicsContextPropertiesInfo *ctxProperties = (GraphicsContextPropertiesInfo *)ctxInfo;

    if (ctxProperties->textureCubeMapAvailable) {
        updateTextureAnisotropicFilter(env, ctxProperties, 
				ctxProperties->texture_cube_map_ext_enum, 
				degree);
    }
}

JNIEXPORT
void JNICALL Java_javax_media_j3d_TextureCubeMapRetained_updateTextureSubImage(
    JNIEnv *env, 
    jobject texture,
    jlong ctxInfo,
    jint face,
    jint level,
    jint xoffset,
    jint yoffset,
    jint internalFormat, 
    jint format,
    jint imgXOffset,
    jint imgYOffset,
    jint tilew,
    jint width, 
    jint height,
    jbyteArray image) {
 
    GraphicsContextPropertiesInfo *ctxProperties = (GraphicsContextPropertiesInfo *)ctxInfo;
    updateTexture2DSubImage(env, ctxProperties, _gl_textureCubeMapFace[face],
				level, xoffset, yoffset, internalFormat,
				format, imgXOffset, imgYOffset, tilew,
				width, height, image);
}



JNIEXPORT
void JNICALL Java_javax_media_j3d_TextureCubeMapRetained_updateTextureImage(
    JNIEnv *env, 
    jobject texture,
    jlong ctxInfo,    
    jint face,
    jint numLevels,
    jint level,
    jint internalFormat, 
    jint format, 
    jint width, 
    jint height, 
    jint boundaryWidth,
    jbyteArray imageYup)
{
    GraphicsContextPropertiesInfo *ctxProperties = (GraphicsContextPropertiesInfo *)ctxInfo;
    updateTexture2DImage(env, ctxProperties, _gl_textureCubeMapFace[face],
				numLevels, level, internalFormat, format,
				width, height, boundaryWidth, imageYup);
}


JNIEXPORT
jboolean JNICALL Java_javax_media_j3d_Canvas3D_decal1stChildSetup(
    JNIEnv *env,
    jobject obj,
    jlong ctxInfo)
{
    glEnable(GL_STENCIL_TEST);
    glClearStencil(0x0);
    glClear(GL_STENCIL_BUFFER_BIT);
    glStencilFunc (GL_ALWAYS, 0x1, 0x1);
    glStencilOp (GL_KEEP, GL_KEEP, GL_REPLACE);
    if (glIsEnabled(GL_DEPTH_TEST))
	return JNI_TRUE;
    else
	return JNI_FALSE;
}

JNIEXPORT
void JNICALL Java_javax_media_j3d_Canvas3D_decalNthChildSetup(
    JNIEnv *env,
    jobject obj,
    jlong ctxInfo)
{
    glDisable(GL_DEPTH_TEST);
    glStencilFunc (GL_EQUAL, 0x1, 0x1);
    glStencilOp (GL_KEEP, GL_KEEP, GL_KEEP);
}

JNIEXPORT
void JNICALL Java_javax_media_j3d_Canvas3D_decalReset(
    JNIEnv *env,
    jobject obj,
    jlong ctxInfo,    
    jboolean depthBufferEnable)
{
    glDisable(GL_STENCIL_TEST);
    if (depthBufferEnable == JNI_TRUE)
        glEnable(GL_DEPTH_TEST);
}

JNIEXPORT
void JNICALL Java_javax_media_j3d_Canvas3D_ctxUpdateEyeLightingEnable(
    JNIEnv *env,
    jobject obj,
    jlong ctxInfo,    
    jboolean localEyeLightingEnable)
{
    if (localEyeLightingEnable == JNI_TRUE) {
        glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    } else {
        glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);
    }
}

JNIEXPORT
void JNICALL Java_javax_media_j3d_Canvas3D_updateSeparateSpecularColorEnable(
    JNIEnv *env,
    jobject obj,
    jlong ctxInfo,    
    jboolean enable)
{
    /*
     * This method will not be called if the rendering layer does not support
     * separate specular color control. The checking of the availability
     * of the functionality is done in Renderer at rendering time
     */

    /*  1.2 feature only */
    GraphicsContextPropertiesInfo *ctxProperties = (GraphicsContextPropertiesInfo *)ctxInfo;
    
    if(ctxProperties->seperate_specular_color) {
	if (enable == JNI_TRUE) {
	    glLightModeli(ctxProperties->light_model_color_control_enum, 
			  ctxProperties->seperate_specular_color_enum);
	} else {
	    glLightModeli(ctxProperties->light_model_color_control_enum, ctxProperties->single_color_enum);
	}
    }
}

JNIEXPORT
void JNICALL Java_javax_media_j3d_TextureUnitStateRetained_updateTextureUnitState(
    JNIEnv *env, 
    jobject cv,
    jlong ctxInfo,
    jint index,
    jboolean enable)
{

    GraphicsContextPropertiesInfo *ctxProperties = (GraphicsContextPropertiesInfo *)ctxInfo;
    
    if(ctxProperties->arb_multitexture) {
	if (index >= 0) {
	    ctxProperties->glActiveTextureARB(index + GL_TEXTURE0_ARB);
	    ctxProperties->glClientActiveTextureARB(GL_TEXTURE0_ARB + index);
	    if (ctxProperties->textureRegisterCombinersAvailable) {
	        ctxProperties->currentTextureUnit = index + GL_TEXTURE0_ARB;
	        ctxProperties->currentCombinerUnit = index + GL_COMBINER0_NV;
		if (ctxProperties->glCombinerParameteriNV!=NULL)
	            ctxProperties->glCombinerParameteriNV(
	                         GL_NUM_GENERAL_COMBINERS_NV, index + 1);

	    }
	}
    } /* GL_ARB_multitexture */

    if (enable == JNI_FALSE) {
        /* if not enabled, then don't enable any tex mapping */

        glDisable(GL_TEXTURE_1D);
        glDisable(GL_TEXTURE_2D);

	if(ctxProperties->texture3DAvailable) 
	    glDisable(ctxProperties->texture_3D_ext_enum);

	if(ctxProperties->textureCubeMapAvailable) 
	    glDisable(ctxProperties->texture_cube_map_ext_enum);
    } 

    /*
     * if it is enabled, the enable flag will be taken care of
     * in the bindTexture call
     */
}


JNIEXPORT
void JNICALL Java_javax_media_j3d_Canvas3D_setBlendColor(
    JNIEnv *env, 
    jobject obj,
    jlong ctxInfo,    
    jfloat colorRed,
    jfloat colorGreen,
    jfloat colorBlue,
    jfloat colorAlpha) 
{

    GraphicsContextPropertiesInfo *ctxProperties = (GraphicsContextPropertiesInfo *)ctxInfo;
    
    if (ctxProperties->blend_color_ext) {
	/*
	fprintf(stderr, "setBlendColor is on: %f %f %f %f\n",
		colorRed, colorGreen, colorBlue, colorAlpha);
	*/

    	ctxProperties->glBlendColor(colorRed, colorGreen, colorBlue, colorAlpha);
    }
}


JNIEXPORT
void JNICALL Java_javax_media_j3d_Canvas3D_setBlendFunc(
    JNIEnv * env, 
    jobject obj,
    jlong ctxInfo,    
    jint srcBlendFunction,
    jint dstBlendFunction)
{
    GraphicsContextPropertiesInfo *ctxProperties = (GraphicsContextPropertiesInfo *)ctxInfo;
    glEnable(GL_BLEND);
    glBlendFunc(ctxProperties->blendFunctionTable[srcBlendFunction],
		    ctxProperties->blendFunctionTable[dstBlendFunction]);
}

JNIEXPORT
void JNICALL Java_javax_media_j3d_Canvas3D_setFogEnableFlag(
    JNIEnv * env, 
    jobject obj,
    jlong ctxInfo,
    jboolean enable)
{
    if (enable == JNI_TRUE)
        glEnable(GL_FOG);
    else
	glDisable(GL_FOG);
}

JNIEXPORT
void JNICALL Java_javax_media_j3d_Canvas3D_activeTextureUnit(
    JNIEnv *env,
    jobject obj,
    jlong ctxInfo,
    jint index)
{
    GraphicsContextPropertiesInfo *ctxProperties = (GraphicsContextPropertiesInfo *)ctxInfo;

    if(ctxProperties->arb_multitexture){
	ctxProperties->glActiveTextureARB(GL_TEXTURE0_ARB + index);
	ctxProperties->glClientActiveTextureARB(GL_TEXTURE0_ARB + index);
	/* GL_ARB_multitexture  */
    }
}

JNIEXPORT
void JNICALL Java_javax_media_j3d_Canvas3D_updateTexUnitStateMap(
    JNIEnv *env,
    jobject obj,
    jlong ctxInfo,
    jint numActiveTexUnit,
    jintArray texUnitStateMapArray)
{
    /*
     * texture unit state map is explicitly handled in
     * execute; for display list, texture unit has to match
     * texture unit state.
     */ 
}


/*
 * strJavaToC
 *
 * Returns a copy of the specified Java String object as a new,
 * null-terminated "C" string. The caller must free this string.
 */
char *
strJavaToC(JNIEnv *env, jstring str)
{
    JNIEnv table = *env;
    jclass oom;

    const char *strUTFBytes;	/* Array of UTF-8 bytes */
    char *cString = NULL;	/* Null-terminated "C" string */

    if (str == NULL) {
	return NULL;
    }

    strUTFBytes = table->GetStringUTFChars(env, str, NULL);
    if (strUTFBytes == NULL) {
	/* Just return, since GetStringUTFChars will throw OOM if it returns NULL */
	return NULL;
    }

    cString = strdup(strUTFBytes);
    table->ReleaseStringUTFChars(env, str, strUTFBytes);
    if (cString == NULL) {
	if ((oom = table->FindClass(env, "java/lang/OutOfMemoryError")) != NULL) {
	    table->ThrowNew(env, oom, "strdup");
	}
	return NULL;
    }

    return cString;
}


/*
 * createShaderError
 *
 * Constructs a new ShaderError object from the given error code,
 * error message, and detail message.
 */
jobject
createShaderError(
    JNIEnv *env,
    int errorCode,
    const char *errorMsg,
    const char *detailMsg)
{
    JNIEnv table = *env;
    jclass shaderErrorClass;
    jobject shaderError;
    jmethodID methodID;
    jstring errorMsgString = NULL;
    jstring detailMsgString = NULL;

    if (errorMsg != NULL) {
	if ((errorMsgString = table->NewStringUTF(env, errorMsg)) == NULL) {
	    return NULL;
	}
    }

    if (detailMsg != NULL) {
	if ((detailMsgString = table->NewStringUTF(env, detailMsg)) == NULL) {
	    return NULL;
	}
    }

    shaderErrorClass = (*(table->FindClass))(env, "javax/media/j3d/ShaderError");
    if (shaderErrorClass == NULL) {
	return NULL;
    }

    methodID = table->GetMethodID(env, shaderErrorClass,
				  "<init>",
				  "(ILjava/lang/String;)V");
    if (methodID == NULL) {
	return NULL;
    }

    shaderError = table->NewObject(env, shaderErrorClass, methodID,
				   errorCode, errorMsgString);
    if (shaderError == NULL) {
	return NULL;
    }

    methodID = table->GetMethodID(env, shaderErrorClass,
				  "setDetailMessage",
				  "(Ljava/lang/String;)V");
    if (methodID == NULL) {
	return NULL;
    }

    table->CallVoidMethod(env, shaderError, methodID,
			  detailMsgString);

    return shaderError;
}


void
throwAssert(JNIEnv *env, char *str)
{
    jclass rte;
    if ((rte = (*env)->FindClass(env, "java/lang/AssertionError")) != NULL) {
	(*env)->ThrowNew(env, rte, str);
    }
}