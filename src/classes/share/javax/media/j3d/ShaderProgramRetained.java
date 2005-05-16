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

package javax.media.j3d;

import java.util.*;
import javax.vecmath.*;

/**
 * The ShaderProgram object is a component object of an Appearance object
 * that defines the shader properties used when programmable shader is
 * enabled. ShaderProgram object is an abstract class. All shader program 
 * objects must be created as either a GLSLShaderProgram object or a
 * CgShaderProgram object.
 */
abstract class ShaderProgramRetained extends NodeComponentRetained {
    
    // Each bit corresponds to a unique renderer if shared context
    // or a unique canvas otherwise.
    // This mask specifies which renderer/canvas has loaded the
    // shader program. 0 means no renderer/canvas has loaded the shader
    // program 1 at the particular bit means that renderer/canvas has 
    // loaded the shader program. 0 means otherwise.
    int resourceCreationMask = 0x0;

    // shaderProgramId use by native code. One per Canvas.
    protected long[] shaderProgramIds;   

    // an array of shaders used by this shader program
    protected ShaderRetained[] shaders;

    // need to synchronize access from multiple rendering threads 
    protected Object resourceLock = new Object();

    /**
     * Copies the specified array of shaders into this shader
     * program. This method makes a shallow copy of the array. The
     * array of shaders may be null or empty (0 length), but the
     * elements of the array must be non-null. The shading
     * language of each shader in the array must match the
     * subclass. Subclasses may impose additional restrictions.
     *
     * @param shaders array of Shader objects to be copied into this
     * ShaderProgram
     *
     * @exception CapabilityNotSetException if appropriate capability is
     * not set and this object is part of live or compiled scene graph
     *
     * @exception IllegalArgumentException if the shading language of
     * any shader in the shaders array doesn't match the type of the
     * subclass.
     */
    abstract void setShaders(Shader[] shaders);

    /**
     * Retrieves the array of shaders from this shader program. A
     * shallow copy of the array is returned. The return value may
     * be null.
     *
     * @return a copy of this ShaderProgram's array of Shader objects
     *
     * @exception CapabilityNotSetException if appropriate capability is
     * not set and this object is part of live or compiled scene graph
     */
    abstract Shader[] getShaders();

    /**
     * Method to update the native shader program.
     */
    // abstract void updateNative(Canvas3D cv); 

    /**
     * Method to disable the native shader program.
     */
    abstract void disableNative(Canvas3D cv);

    /**
     * Method to update the native shader attributes
     */
    abstract void setUniformAttrValue(long ctx, ShaderAttributeValue sav);



    /**
     * Method to create the native shader.
     */
    abstract ShaderError createShader(long ctx, int cvRdrIndex, ShaderRetained shader);

    /**
     * Method to destroy the native shader.
     */
    abstract ShaderError destroyShader(long ctx, int cvRdrIndex, ShaderRetained shader);

    /**
     * Method to compile the native shader.
     */
    abstract ShaderError compileShader(long ctx, int cvRdrIndex, ShaderRetained shader);


    /**
     * Method to create the native shader program.
     */
    abstract ShaderError createShaderProgram(long ctx, int cvRdrIndex);

    /**
     * Method to destroy the native shader program.
     */
    abstract ShaderError destroyShaderProgram(long ctx, int cvRdrIndex);

    /**
     * Method to link the native shader program.
     */
    abstract ShaderError linkShaderProgram(long ctx, int cvRdrIndex, ShaderRetained[] shaders);


    /**
     * Method to use the native shader program.
     */
    abstract ShaderError useShaderProgram(long ctx, int cvRdrIndex);

    /**
     * updateNative is called while traversing the RenderBin to 
     * update the shader program state
     */
    void updateNative(Canvas3D cv) {
	boolean loadShaderProgram = false; // true - reload all shaderProgram states

        // System.out.println("GLSLShaderProgramRetained.updateNative : ");

        if (cv.useSharedCtx && cv.screen.renderer.sharedCtx != 0) {
	    // TODO : Need to test useSharedCtx case. ** Untested case **
            if ((resourceCreationMask & cv.screen.renderer.rendererBit) == 0) {
		loadShaderProgram = true;
		cv.makeCtxCurrent(cv.screen.renderer.sharedCtx);
	    }
	} else {
	    // Or (shaderProgramIds[cv.canvasId] == null)
            if ((resourceCreationMask & cv.canvasBit) == 0) {
		loadShaderProgram = true;
	    }
	}

	// System.out.println(".... loadShaderProgram = " + loadShaderProgram);
	// System.out.println(".... resourceCreationMask= " + resourceCreationMask);
 
	if (loadShaderProgram) {
	    
	    if(shaders == null) {
		// System.out.println("GLSLShaderProgramRetained : shaders is ** null **.");
		return;
	    }

	    // Create shaders resources if it has not been done.
	    for(int i=0; i < shaders.length; i++) {

		// Need to handle the returned ShaderError.
		createShader(cv.ctx, cv.canvasId, shaders[i]);

		// Need to handle the returned ShaderError.
		compileShader(cv.ctx, cv.canvasId, shaders[i]);
	    }

	    if (cv.useSharedCtx) {
		System.out.println("GLSLShaderProgramRetained.updateNative");
		System.out.println("    sharedCtx is not implemented yet.");
		// cv.makeCtxCurrent(cv.ctx);

	    }
	    else {

		// Need to handle the returned ShaderError.
		createShaderProgram(cv.ctx, cv.canvasId);
		
		// Need to handle the returned ShaderError.
		linkShaderProgram(cv.ctx, cv.canvasId, shaders);
		
		
	    }

	}
 
	useShaderProgram(cv.ctx, cv.canvasId);
	
    }




}
