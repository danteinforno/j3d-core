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
    
    // linked flag native for one per Canvas.
    protected boolean[] linked;     

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
     * Method to update the native shader attributes
     */
    abstract void setUniformAttrValue(long ctx, ShaderAttributeValue sav);

    /**
     * Method to create the native shader.
     */
    abstract ShaderError createShader(long ctx, ShaderRetained shader, long[] shaderIdArr); 

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
    abstract ShaderError createShaderProgram(long ctx, int cvRdrIndex, long[] shaderProgramIdArr);

    /**
     * Method to destroy the native shader program.
     */
    abstract ShaderError destroyShaderProgram(long ctx, int cvRdrIndex);

    /**
     * Method to link the native shader program.
     */
    abstract ShaderError linkShaderProgram(long ctx, int cvRdrIndex, long[] shaderIds);

    /**
     * Method to use the native shader program.
     */
    abstract ShaderError enableShaderProgram(Canvas3D cv, int cvRdrIndex);
    
    /**
     * Method to disable the native shader program.
     */
    abstract void disableShaderProgram(Canvas3D cv);
   
    /**
     * Method to link the native shader program.
     */
    ShaderError linkShaderProgram(Canvas3D cv, int cvRdrIndex, ShaderRetained[] shaders) {

	synchronized(resourceLock) {
            if(linked[cvRdrIndex] == true) {
                // We have already linked the shaderProgramId for this Canvas. 
                return null;
            }
            
            long[] shaderIds = new long[shaders.length];
	    for(int i=0; i<shaders.length; i++) {
                synchronized(shaders[i]) {
                    shaderIds[i] = shaders[i].shaderIds[cvRdrIndex];
                }
	    }
	    ShaderError err = linkShaderProgram(cv.ctx, cvRdrIndex, shaderIds);
            if(err != null) {
                return err;
            }
            linked[cvRdrIndex] = true;
	}

	return null;
    }
 

    
    /**
     * Method to create the native shader program.
     */
    ShaderError createShaderProgram(Canvas3D cv, int cvRdrIndex) {
        
        // Create shaderProgram resources if it has not been done.
        synchronized(resourceLock) {
            if(shaderProgramIds == null){
                // We rely on Java to initial the array elements to 0 or false;
                shaderProgramIds = new long[cvRdrIndex+1];
                linked = new boolean[cvRdrIndex+1];
            } else if( shaderProgramIds.length <= cvRdrIndex) {
                // We rely on Java to initial the array elements to 0 or false;
                long[] tempSpIds = new long[cvRdrIndex+1];
                boolean[] tempLinked = new boolean[cvRdrIndex+1];
                
                System.arraycopy(shaderProgramIds, 0,
                        tempSpIds, 0,
                        shaderProgramIds.length);
                shaderProgramIds = tempSpIds;
                
                System.arraycopy(linked, 0,
                        tempLinked, 0,
                        linked.length);
                linked = tempLinked;
            }
            if(shaderProgramIds[cvRdrIndex] != 0) {
                // We have already created the shaderProgramId for this Canvas.
                return null;
            }
            
            long[] spIdArr = new long[1];
            ShaderError err = createShaderProgram(cv.ctx, cvRdrIndex, spIdArr);
            if(err != null) {
                return err;
            }
            shaderProgramIds[cvRdrIndex] = spIdArr[0];
            resourceCreationMask |= (1 << cvRdrIndex);
        }
        
        return null;
    }

    /**
     * Method to destroy the native shader program.
     */
    ShaderError destroyShaderProgram(Canvas3D cv, int cvRdrIndex) {
	System.out.println("ShaderProgramRetained : destroyShaderProgram not implemented yet!");
	return null;
    } 
    
    /**
     * Method to compile the native shader.
     */
    ShaderError compileShader(Canvas3D cv, int cvRdrIndex, ShaderRetained shader) {
        
        synchronized(shader.resourceLock) {
            
            if(shader.compiled[cvRdrIndex] == true) {
                // We have already compiled the shaderId for this Canvas.
                return null;
            }
            
            ShaderError err = compileShader(cv.ctx, cvRdrIndex, shader);
            if(err != null) {
                return err;
            }
            shader.compiled[cvRdrIndex] = true;
        }
        
        return null;
    }
    
    /**
     * Method to create the native shader.
     */
    ShaderError createShader(Canvas3D cv, int cvRdrIndex, ShaderRetained shader) {
        
        // Create shaderProgram resources if it has not been done.
        synchronized(shader.resourceLock) {
            if(shader.shaderIds == null){
                // We rely on Java to initial the array elements to 0 or false;
                shader.shaderIds = new long[cvRdrIndex+1];
                shader.compiled = new boolean[cvRdrIndex+1];
            } else if( shader.shaderIds.length <= cvRdrIndex) {
                // We rely on Java to initial the array elements to 0 or false;
                long[] tempSIds = new long[cvRdrIndex+1];
                boolean[] tempCompiled = new boolean[cvRdrIndex+1];
                
                System.arraycopy(shader.shaderIds, 0,
                        tempSIds, 0,
                        shader.shaderIds.length);
                shader.shaderIds = tempSIds;
                
                System.arraycopy(shader.compiled, 0,
                        tempCompiled, 0,
                        shader.compiled.length);
                shader.compiled = tempCompiled;
            }
            
            if(shader.shaderIds[cvRdrIndex] != 0) {
                // We have already created the shaderId for this Canvas.
                return null;
            }
            
            long[] shaderIdArr = new long[1];
            ShaderError err = createShader(cv.ctx, shader, shaderIdArr);
            if(err != null) {
                return err;
            }
            shader.shaderIds[cvRdrIndex] = shaderIdArr[0];
        }
        return null;
    }
    
    /**
     * Method to destroy the native shader.
     */
    ShaderError destroyShader(Canvas3D cv, int cvRdrIndex, ShaderRetained shader) {
	System.out.println("ShaderProgramRetained : destroyShader not implemented yet!");
	return null;
    }
     
    
    /**
     * updateNative is called while traversing the RenderBin to 
     * update the shader program state
     */
    void updateNative(Canvas3D cv) {
	boolean loadShaderProgram = false; // true - reload all shaderProgram states
        int cvRdrIndex = -1;
        
	// System.out.println("GLSLShaderProgramRetained.updateNative : ");

        if (cv.useSharedCtx && cv.screen.renderer.sharedCtx != 0) {
	    // TODO : Need to test useSharedCtx case. ** Untested case **
            if ((resourceCreationMask & cv.screen.renderer.rendererBit) == 0) {
		loadShaderProgram = true;
		cv.makeCtxCurrent(cv.screen.renderer.sharedCtx);
	    }
            cvRdrIndex = cv.screen.renderer.rendererId;
	} else {
	    // Or (shaderProgramIds[cv.canvasId] == null)
            if ((resourceCreationMask & cv.canvasBit) == 0) {
		loadShaderProgram = true;               
	    }
            cvRdrIndex = cv.canvasId;
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
                createShader(cv, cvRdrIndex, shaders[i]);
                
                // Need to handle the returned ShaderError.
                compileShader(cv, cvRdrIndex, shaders[i]);
            }
            
            // Need to handle the returned ShaderError.
            createShaderProgram(cv, cvRdrIndex);
            
            // Need to handle the returned ShaderError.
            linkShaderProgram(cv, cvRdrIndex, shaders);
            
            if (cv.useSharedCtx) {
                cv.makeCtxCurrent(cv.ctx);
            }
	   
	}
 
	enableShaderProgram(cv, cvRdrIndex);
        
	
    }




}
