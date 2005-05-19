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
    
    // A list of pre-defined bits to indicate which component
    // in this ShaderProgram object changed.
    static final int SHADER_PROGRAM_CREATE              = 0x001;
    static final int SHADER_UPDATE                      = 0x002;
    static final int VERTEX_ATTRIBUTE_NAME_UPDATE       = 0x004;
    static final int SHADER_ATTRIBUTE_UPDATE            = 0x008;
    static final int SHADER_PROGRAM_DESTROY             = 0x010;

    // Each bit corresponds to a unique renderer if shared context
    // or a unique canvas otherwise.
    // This mask specifies which renderer/canvas has loaded the
    // shader program. 0 means no renderer/canvas has loaded the shader
    // program 1 at the particular bit means that renderer/canvas has 
    // loaded the shader program. 0 means otherwise.
    protected int resourceCreationMask = 0x0;

    // shaderProgramId use by native code. One per Canvas.
    protected long[] shaderProgramIds;
    
    // linked flag native for one per Canvas.
    protected boolean[] linked;
    
    // Flag indicating whether an UNSUPPORTED_LANGUAGE_ERROR has
    // already been reported for this shader program object.  It is
    // set in verifyShaderProgram and cleared in setLive or clearLive.
    // TODO KCR: Add code to clear this in setLive or clearLive
    private boolean unsupportedErrorReported = false;

    // Flag indicating whether a LINK_ERROR has occurred for this shader program
    // object.  It is set in updateNative to indicate that the linkShaderProgram
    // operation failed. It is cleared in setLive or clearLive.
    // TODO KCR: Add code to clear this in setLive or clearLive
    private boolean linkErrorOccurred = false;

    // an array of shaders used by this shader program
    protected ShaderRetained[] shaders;

    // need to synchronize access from multiple rendering threads 
    Object resourceLock = new Object();

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
    abstract ShaderError enableShaderProgram(long ctx, int cvRdrIndex);
    
    /**
     * Method to disable the native shader program.
     */
    abstract ShaderError disableShaderProgram(long ctx);
    
    /**
     * Method to return a flag indicating whether this
     * ShaderProgram is supported on the specified Canvas.
     */
    abstract boolean isSupported(Canvas3D cv);


    void setLive(boolean backgroundGroup, int refCount) {
	
	// System.out.println("ShaderProgramRetained.setLive()");

	if (shaders != null) {
	    for (int i = 0; i < shaders.length; i++){
		shaders[i].setLive(backgroundGroup, refCount);
	    }
	}
	
	super.doSetLive(backgroundGroup, refCount);

        // Send a message to Rendering Attr stucture to update the resourceMask
	// via updateMirrorObject().
	J3dMessage createMessage = VirtualUniverse.mc.getMessage();
	createMessage.threads = J3dThread.UPDATE_RENDERING_ATTRIBUTES;
	createMessage.type = J3dMessage.SHADER_PROGRAM_CHANGED;
	createMessage.args[0] = this;
	createMessage.args[1]= new Integer(SHADER_PROGRAM_CREATE);
 	createMessage.args[2] = null;
 	createMessage.args[3] = new Integer(changedFrequent);
	VirtualUniverse.mc.processMessage(createMessage);
	
	super.markAsLive();
    }

    void clearLive(int refCount) {

        // System.out.println("ShaderProgramRetained.clearLive()");

	super.clearLive(refCount);

	if (shaders != null) {
	    for (int i = 0; i < shaders.length; i++) {
		shaders[i].clearLive(refCount);
	    }
	}
    }

    /**
     * Method to enable the native shader program.
     */
    private ShaderError enableShaderProgram(Canvas3D cv, int cvRdrIndex) {
        assert(cvRdrIndex >= 0);
	synchronized(resourceLock) {
            return enableShaderProgram(cv.ctx, cvRdrIndex);
	}

    }

    /**
     * Method to disable the native shader program.
     */
    private ShaderError disableShaderProgram(Canvas3D cv) {
        return disableShaderProgram(cv.ctx);
    }
    
   
    /**
     * Method to create the native shader program.
     */
    private ShaderError createShaderProgram(Canvas3D cv, int cvRdrIndex) {
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
     * Method to link the native shader program.
     */
    private ShaderError linkShaderProgram(Canvas3D cv, int cvRdrIndex, ShaderRetained[] shaders) {
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
     * Method to create the native shader.
     */
    private ShaderError createShader(Canvas3D cv, int cvRdrIndex, ShaderRetained shader) {
        
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
     * Method to compile the native shader.
     */
    private ShaderError compileShader(Canvas3D cv, int cvRdrIndex, ShaderRetained shader) {
        
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
     */
    void notifyErrorListeners(Canvas3D cv, ShaderError err) {
        // TODO KCR: send a messge to NotificationThread
        cv.view.universe.notifyShaderErrorListeners(err);
    }
    
    
    /**
     * This method checks whether this ShaderProgram is supported on
     * the specified Canvas. If it isn't supported, it will report a
     * ShaderError unless an error has already been reported for this
     * shader program.
     */
    private boolean verifyShaderProgramSupported(Canvas3D cv) {
        boolean supported = isSupported(cv);
        if (!supported && !unsupportedErrorReported) {
            String errorMsg = J3dI18N.getString("ShaderProgramRetained0");
            ShaderError err = new ShaderError(ShaderError.UNSUPPORTED_LANGUAGE_ERROR, errorMsg);
            err.setShaderProgram((ShaderProgram)this.source);
            err.setCanvas3D(cv);
            notifyErrorListeners(cv, err);
            unsupportedErrorReported = true;
        }
        return supported;
    }

    /**
     * Method to destroy the native shader.
     */
    void destroyShader(Canvas3D cv, int cvRdrIndex, ShaderRetained shader) {
        if (!verifyShaderProgramSupported(cv)) {
            return;
        }

        // Destroy shader resource if it exists
        synchronized(shader.resourceLock) {
            // Check whether an entry in the shaderIds array has been allocated
            if (shader.shaderIds == null || shader.shaderIds.length <= cvRdrIndex) {
                return;
            }
            
            // Nothing to do if the shaderId is 0
            if (shader.shaderIds[cvRdrIndex] == 0) {
                return;
            }

            // Destroy the native resource and set the ID to 0 for this canvas/renderer
            // Ignore any possible shader error, because there is no meaningful way to report it
            destroyShader(cv.ctx, cvRdrIndex, shader);
            shader.shaderIds[cvRdrIndex] = 0;
        }
    }


    /**
     * Method to destroy the native shader program.
     */
    void destroyShaderProgram(Canvas3D cv, int cvRdrIndex) {
        if (!verifyShaderProgramSupported(cv)) {
            return;
        }
        
        // Destroy shaderProgram resource if it exists
        synchronized(resourceLock) {
            // Check whether an entry in the shaderProgramIds array has been allocated
            if (shaderProgramIds == null || shaderProgramIds.length <= cvRdrIndex) {
                return;
            }
            
            // Nothing to do if the shaderProgramId is 0
            if (shaderProgramIds[cvRdrIndex] == 0) {
                return;
            }

            // Destroy the native resource, set the ID to 0 for this canvas/renderer,
            // and clear the bit in the resourceCreationMask
            // Ignore any possible shader error, because there is no meaningful way to report it
            destroyShaderProgram(cv.ctx, cvRdrIndex);
            shaderProgramIds[cvRdrIndex] = 0;
            resourceCreationMask &= ~(1 << cvRdrIndex);
        }
    } 


    /**
     * updateNative is called while traversing the RenderBin to 
     * update the shader program state
     */
    void updateNative(Canvas3D cv, boolean enable) {
	// System.out.println("GLSLShaderProgramRetained.updateNative : ");

        if (!verifyShaderProgramSupported(cv)) {
            return;
        }
        
        if (!enable) {
            // Given the current design, disableShaderProgram cannot return a non-null value,
            // so no need to check it
            disableShaderProgram(cv);
            return;
        }

        // Just disable shader program and return if array of shaders is empty,
        // or if a previous attempt to link resulted in an error
        if (shaders == null || shaders.length == 0 || linkErrorOccurred) {
            disableShaderProgram(cv);
            return;
        }

	boolean loadShaderProgram = false; // true - reload all shaderProgram states
        int cvRdrIndex;        
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
 
        ShaderError err;
        boolean errorOccurred = false;
	if (loadShaderProgram) {
            // Create shader resources if not already done
            for(int i=0; i < shaders.length; i++) {
                if (shaders[i].compileErrorOccurred) {
                    errorOccurred = true;
                }
                else {
                    err = createShader(cv, cvRdrIndex, shaders[i]);
                    if (err != null) {
                        err.setShaderProgram((ShaderProgram)this.source);
                        err.setShader((Shader)shaders[i].source);
                        err.setCanvas3D(cv);
                        notifyErrorListeners(cv, err);
                        errorOccurred = true;
                    }
                    else {
                        err = compileShader(cv, cvRdrIndex, shaders[i]);
                        if (err != null) {
                            err.setShaderProgram((ShaderProgram)this.source);
                            err.setShader((Shader)shaders[i].source);
                            err.setCanvas3D(cv);
                            notifyErrorListeners(cv, err);
                            destroyShader(cv, cvRdrIndex, shaders[i]);
                            shaders[i].compileErrorOccurred = true;
                            errorOccurred = true;
                        }
                    }
                }
            }
            
            // Create and link shader program
            if (!errorOccurred) {
                err = createShaderProgram(cv, cvRdrIndex);
                if (err != null) {
                    err.setShaderProgram((ShaderProgram)this.source);
                    err.setCanvas3D(cv);
                    notifyErrorListeners(cv, err);
                    errorOccurred = true;
                }
            }
            
            // Bind vertex attribute names
            if (!errorOccurred) {
                // TODO KCR: implement this
            }
             
            // Link shader program
            if (!errorOccurred) {
                err = linkShaderProgram(cv, cvRdrIndex, shaders);
                if (err != null) {
                    err.setShaderProgram((ShaderProgram)this.source);
                    err.setCanvas3D(cv);
                    notifyErrorListeners(cv, err);
                    destroyShaderProgram(cv, cvRdrIndex);
                    linkErrorOccurred = true;
                    errorOccurred = true;
                }
            }

            // Lookup shader attribute names
            if (!errorOccurred) {
                // TODO KCR: implement this
            }
            
            // Restore current context if we changed it to the shareCtx
            if (cv.useSharedCtx) {
                cv.makeCtxCurrent(cv.ctx);
            }
	   
            // If compilation or link error occured, disable shader program and return
            if (errorOccurred) {
                disableShaderProgram(cv);
                return;
            }
        }
 
        // Now we can enable the shader program
	enableShaderProgram(cv, cvRdrIndex);
    }

}
