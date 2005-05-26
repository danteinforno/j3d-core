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

    protected ShaderProgramData shaderProgramData[];

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

    // an array of vertex attribute names
    protected String[] vertexAttrNames;

    // an array of (uniform) shader attribute names
    protected String[] shaderAttrNames;

    // need to synchronize access from multiple rendering threads 
    Object resourceLock = new Object();

    /**
     * Sets the vertex attribute names array for this ShaderProgram
     * object. Each element in the array specifies the shader
     * attribute name that is bound to the corresponding numbered
     * vertex attribute within a GeometryArray object that uses this
     * shader program. Array element 0 specifies the name of
     * GeometryArray vertex attribute 0, array element 1 specifies the
     * name of GeometryArray vertex attribute 1, and so forth.
     *
     * @param vertexAttrNames array of vertex attribute names for this
     * shader program. A copy of this array is made.
     */
    void setVertexAttrNames(String[] vertexAttrNames) {
        if (vertexAttrNames == null) {
            this.vertexAttrNames = null;
        }
        else {
            this.vertexAttrNames = (String[])vertexAttrNames.clone();
        }
    }

    /**
     * Sets the shader attribute names array for this ShaderProgram
     * object. Each element in the array specifies a shader
     * attribute name that may be set via a ShaderAttribute object.
     * Only those attributes whose names that appear in the shader
     * attribute names array can be set for a given shader program.
     *
     * @param shaderAttrNames array of shader attribute names for this
     * shader program. A copy of this array is made.
     */
    void setShaderAttrNames(String[] shaderAttrNames) {
        if (shaderAttrNames == null) {
            this.shaderAttrNames = null;
        }
        else {
            this.shaderAttrNames = (String[])shaderAttrNames.clone();
        }
    }

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
    void setShaders(Shader[] shaders) {

	if (shaders == null) {
	    this.shaders = null;
	    return;
	}
	
	this.shaders = new ShaderRetained[shaders.length];

	// Copy vertex and fragment shader
	for (int i = 0; i < shaders.length; i++) {
	    this.shaders[i] = (ShaderRetained)shaders[i].retained;
	}

    }

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
    Shader[] getShaders() {

	if (shaders == null) {
	    return null;
	} else {
	    Shader shads[] = 
		new Shader[shaders.length];
	    for (int i = 0; i < shaders.length; i++) {
		if (shaders[i] != null) {
		    shads[i] = (Shader) shaders[i].source;
		} else {
		    shads[i] = null;
		}
	    }
	    return shads;
	}
    }    
    
    /**
     * Method to create the native shader.
     */
    abstract ShaderError createShader(long ctx, ShaderRetained shader, long[] shaderIdArr); 

    /**
     * Method to destroy the native shader.
     */
    abstract ShaderError destroyShader(long ctx, long shaderId);

    /**
     * Method to compile the native shader.
     */
    abstract ShaderError compileShader(long ctx, long shaderId, String source);

    /**
     * Method to create the native shader program.
     */
    abstract ShaderError createShaderProgram(long ctx, long[] shaderProgramIdArr);

    /**
     * Method to destroy the native shader program.
     */
    abstract ShaderError destroyShaderProgram(long ctx, long shaderProgramId);

    /**
     * Method to link the native shader program.
     */
    abstract ShaderError linkShaderProgram(long ctx, long shaderProgramId, long[] shaderIds);

    /**
     * Method to bind a vertex attribute name to the specified index.
     */
    abstract ShaderError bindVertexAttrName(long ctx, long shaderProgramId, String attrName, int attrIndex);

    /**
     * Method to bind a vertex attribute name to the specified index.
     */
    abstract ShaderError lookupShaderAttrName(long ctx, long shaderProgramId, String attrName, long[] locArr);

    /**
     * Method to use the native shader program.
     */
    abstract ShaderError enableShaderProgram(long ctx, long shaderProgramId);
    
    /**
     * Method to disable the native shader program.
     */
    abstract ShaderError disableShaderProgram(long ctx);
    
    abstract ShaderError setUniform1i(long ctx,
					    long shaderProgramId,
					    long uniformLocation,
					    int value);
    
    abstract ShaderError setUniform1f(long ctx,
					    long shaderProgramId,
					    long uniformLocation,
					    float value);
    
    abstract ShaderError setUniform2i(long ctx,
					    long shaderProgramId,
					    long uniformLocation,
					    int[] value);
    
    abstract ShaderError setUniform2f(long ctx,
					    long shaderProgramId,
					    long uniformLocation,
					    float[] value);
    
    abstract ShaderError setUniform3i(long ctx,
					    long shaderProgramId,
					    long uniformLocation,
					    int[] value);
    
    abstract ShaderError setUniform3f(long ctx,
					    long shaderProgramId,
					    long uniformLocation,
					    float[] value);    
    
    abstract ShaderError setUniform4i(long ctx,
					    long shaderProgramId,
					    long uniformLocation,
					    int[] value);
    
    abstract ShaderError setUniform4f(long ctx,
					    long shaderProgramId,
					    long uniformLocation,
					    float[] value);    
    
    abstract ShaderError setUniformMatrix3f(long ctx,
					   long shaderProgramId,
				           long uniformLocation,
					   float[] value);

    abstract ShaderError setUniformMatrix4f(long ctx,
					   long shaderProgramId,
			         	   long uniformLocation,
					   float[] value);
    

    
    
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
            return enableShaderProgram(cv.ctx, 
				       shaderProgramData[cvRdrIndex].getShaderProgramId());
	}

    }

    /**
     * Method to disable the native shader program.
     */
    private ShaderError disableShaderProgram(Canvas3D cv) {
        return disableShaderProgram(cv.ctx);
    }
    
    /**
     * Initializes a mirror object.
     */
    synchronized void initMirrorObject() {
        mirror.source = source;
        
        // Create mirror copy of shaders
        if (this.shaders == null) {
            ((ShaderProgramRetained)mirror).shaders = null;
        }
        else {
            ((ShaderProgramRetained)mirror).shaders = new ShaderRetained[this.shaders.length];
            // Copy vertex and fragment shader
            for (int i = 0; i < this.shaders.length; i++) {
                ((ShaderProgramRetained)mirror).shaders[i] =
                        (ShaderRetained)this.shaders[i].mirror;
            }
        }
        ((ShaderProgramRetained)mirror).resourceCreationMask = 0x0;
        ((ShaderProgramRetained)mirror).shaderProgramData = null;
        
        // Create mirror copy of vertex attribute names
        if (this.vertexAttrNames == null) {
            ((ShaderProgramRetained)mirror).vertexAttrNames = null;
        }
        else {
            ((ShaderProgramRetained)mirror).vertexAttrNames = (String[])this.vertexAttrNames.clone();
        }
        
        // Create mirror copy of shader attribute names
        if (this.shaderAttrNames == null) {
            ((ShaderProgramRetained)mirror).shaderAttrNames = null;
        }
        else {
            ((ShaderProgramRetained)mirror).shaderAttrNames = (String[])this.shaderAttrNames.clone();
        }
    }
    
    /**
     * Update the "component" field of the mirror object with the  given "value"
     */
    synchronized void updateMirrorObject(int component, Object value) {

	// System.out.println("ShaderProgramRetained : updateMirrorObject");

	ShaderProgramRetained mirrorSp = (ShaderProgramRetained)mirror;

	if ((component & SHADER_PROGRAM_CREATE) != 0) {
	    // Note: update from the mirror object only
	    mirrorSp.resourceCreationMask = 0x0;
	}
    } 

    /**
     * Method to create the native shader program.
     */
    private ShaderError createShaderProgram(Canvas3D cv, int cvRdrIndex) {
        // Create shaderProgram resources if it has not been done.
        synchronized(resourceLock) {
	    if(shaderProgramData == null) {
                // We rely on Java to initial the array elements to null.
                shaderProgramData = new ShaderProgramData[cvRdrIndex+1];
	    }
	    else if(shaderProgramData.length <= cvRdrIndex) {
                // We rely on Java to initial the array elements to null.
		ShaderProgramData[] tempSPData = new ShaderProgramData[cvRdrIndex+1];
                System.arraycopy(shaderProgramData, 0,
                        tempSPData, 0,
                        shaderProgramData.length);
                shaderProgramData = tempSPData;
	    }

            if(shaderProgramData[cvRdrIndex] != null) {
                // We have already created the shaderProgramId for this Canvas.
                return null;
            }
            
            long[] spIdArr = new long[1];
            ShaderError err = createShaderProgram(cv.ctx, spIdArr);
            if(err != null) {
                return err;
            }
            shaderProgramData[cvRdrIndex] = new ShaderProgramData(spIdArr[0]);
            resourceCreationMask |= (1 << cvRdrIndex);
        }
        
        return null;
    }

    /**
     * Method to link the native shader program.
     */
    private ShaderError linkShaderProgram(Canvas3D cv, int cvRdrIndex, 
					  ShaderRetained[] shaders) {
	synchronized(resourceLock) {
            long[] shaderIds = new long[shaders.length];
	    for(int i=0; i<shaders.length; i++) {
                synchronized(shaders[i]) {
                    shaderIds[i] = shaders[i].shaderIds[cvRdrIndex];
                }
	    }
	    ShaderError err = 
		linkShaderProgram(cv.ctx, 
				  shaderProgramData[cvRdrIndex].getShaderProgramId(),
				  shaderIds);
            if(err != null) {
                return err;
            }
	    shaderProgramData[cvRdrIndex].setLinked(true);
	}

	return null;
    }

    
    private ShaderError bindVertexAttrNames(Canvas3D cv, int cvRdrIndex, String[] attrNames) {
        synchronized(resourceLock) {
            long shaderProgramId = shaderProgramData[cvRdrIndex].getShaderProgramId();
//            System.err.println("bindVertexAttrNames: attrNames.length = " + attrNames.length);
            for (int i = 0; i < attrNames.length; i++) {
//                System.err.println("attrNames[" + i + "] = " + attrNames[i]);
                ShaderError err = bindVertexAttrName(cv.ctx, shaderProgramId, attrNames[i], i);
                if (err != null) {
                    return err;
                }
            }
        }
        return null;
    }


    private ShaderError lookupShaderAttrNames(Canvas3D cv, int cvRdrIndex, String[] attrNames) {
        // TODO Chien: Finish and test this...
        synchronized(resourceLock) {
            long shaderProgramId = shaderProgramData[cvRdrIndex].getShaderProgramId();
            System.err.println("lookupShaderAttrNames: attrNames.length = " + attrNames.length);
            for (int i = 0; i < attrNames.length; i++) {
                long[] locArr = new long[1];
                ShaderError err = lookupShaderAttrName(cv.ctx, shaderProgramId, attrNames[i], locArr);
                if (err != null) {
                    return err;
                }
                System.err.println(attrNames[i] + " = " + locArr[0]);
                shaderProgramData[cvRdrIndex].setLocation(attrNames[i], new Long(locArr[0]));
            }
        }
        return null;
    }


    /**
     * Method to return the shaderProgram data for the specified canvas or renderer
     */
    private ShaderProgramData getShaderProgramData(int cvRdrIndex) {
        synchronized(resourceLock) {
            return shaderProgramData[cvRdrIndex];
        }
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
            
	    String source = ((SourceCodeShaderRetained)shader).getShaderSource();
            ShaderError err = compileShader(cv.ctx, shader.shaderIds[cvRdrIndex], source);
            if(err != null) {
                return err;
            }
            shader.compiled[cvRdrIndex] = true;
        }
        
        return null;
    }
    
    /**
     * Send a message to the notification thread, which will call the
     * shader error listeners.
     */
    void notifyErrorListeners(Canvas3D cv, ShaderError err) {
        J3dNotification notification = new J3dNotification();
        notification.type = J3dNotification.SHADER_ERROR;
        notification.universe = cv.view.universe;
        notification.args[0] = err;
        VirtualUniverse.mc.sendNotification(notification);
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
            destroyShader(cv.ctx, shader.shaderIds[cvRdrIndex]);
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
            // Check whether an entry in the shaderProgramData array has been allocated
            if (shaderProgramData == null ||
                    shaderProgramData.length <= cvRdrIndex ||
                    shaderProgramData[cvRdrIndex] == null) {
                return;
            }
            
	    long shaderProgramId = shaderProgramData[cvRdrIndex].getShaderProgramId(); 
            // Nothing to do if the shaderProgramId is 0
            if (shaderProgramId == 0) {
                return;
            }

            // Destroy the native resource, set the ID to 0 for this canvas/renderer,
            // and clear the bit in the resourceCreationMask
            // Ignore any possible shader error, because there is no meaningful way to report it
            destroyShaderProgram(cv.ctx, shaderProgramId);
            // Free this ShaderProgramData object.
	    shaderProgramData[cvRdrIndex] = null;
            resourceCreationMask &= ~(1 << cvRdrIndex);
        }
    } 


    /**
     * updateNative is called while traversing the RenderBin to 
     * update the shader program state
     */
    void updateNative(Canvas3D cv, boolean enable) {
	// System.out.println("ShaderProgramRetained.updateNative : ");

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
	    // Or (shaderProgramData[cv.canvasId] == null)
            if ((resourceCreationMask & cv.canvasBit) == 0) {
		loadShaderProgram = true;               
	    }
            cvRdrIndex = cv.canvasId;
	}

	//System.out.println(".... loadShaderProgram = " + loadShaderProgram);
	//System.out.println(".... resourceCreationMask= " + resourceCreationMask);
 
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
            
            // Create shader program
            if (!errorOccurred) {
                err = createShaderProgram(cv, cvRdrIndex);
                if (err != null) {
                    err.setShaderProgram((ShaderProgram)this.source);
                    err.setCanvas3D(cv);
                    notifyErrorListeners(cv, err);
                    errorOccurred = true;
                }
            }
            
            boolean linked = getShaderProgramData(cvRdrIndex).isLinked();
            if (!linked) {
                // Bind vertex attribute names
                if (!errorOccurred) {
                    if (vertexAttrNames != null) {
                        err = bindVertexAttrNames(cv, cvRdrIndex, vertexAttrNames);
                        if (err != null) {
                            err.setShaderProgram((ShaderProgram)this.source);
                            err.setCanvas3D(cv);
                            notifyErrorListeners(cv, err);
                            linkErrorOccurred = true;
                            errorOccurred = true;
                        }
                    }
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
                    if (shaderAttrNames != null) {
                        err = lookupShaderAttrNames(cv, cvRdrIndex, shaderAttrNames);
                        if (err != null) {
                            err.setShaderProgram((ShaderProgram)this.source);
                            err.setCanvas3D(cv);
                            notifyErrorListeners(cv, err);
                            destroyShaderProgram(cv, cvRdrIndex);
                            linkErrorOccurred = true;
                            errorOccurred = true;
                        }
                    }
                }
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
 
    /**
     * Update native value for ShaderAttributeValue class
     */
    ShaderError setUniformAttrValue(long ctx, long shaderProgramId, long loc, ShaderAttributeValue sav) {
        
        switch (sav.classType) {           
        case ShaderAttributeObject.TYPE_INTEGER:
            
            return setUniform1i(ctx, shaderProgramId, loc,
                    ((int[])sav.attrWrapper.getRef())[0]);

            

        case ShaderAttributeObject.TYPE_FLOAT:

            return setUniform1f(ctx, shaderProgramId, loc,
                    ((float[])sav.attrWrapper.getRef())[0]);
            

        case ShaderAttributeObject.TYPE_DOUBLE:
            throw new RuntimeException("not implemented");

        case ShaderAttributeObject.TYPE_TUPLE2I:
            return setUniform2i(ctx, shaderProgramId, loc,
                    (int[])sav.attrWrapper.getRef());
            

        case ShaderAttributeObject.TYPE_TUPLE2F:
            return setUniform2f(ctx, shaderProgramId, loc,
                    (float[])sav.attrWrapper.getRef());
            

        case ShaderAttributeObject.TYPE_TUPLE2D:
            throw new RuntimeException("not implemented");

        case ShaderAttributeObject.TYPE_TUPLE3I:
            return setUniform3i(ctx, shaderProgramId, loc,
                    (int[])sav.attrWrapper.getRef());
            

        case ShaderAttributeObject.TYPE_TUPLE3F:
            return setUniform3f(ctx, shaderProgramId, loc,
                    (float[])sav.attrWrapper.getRef());
            

        case ShaderAttributeObject.TYPE_TUPLE3D:
            throw new RuntimeException("not implemented");

        case ShaderAttributeObject.TYPE_TUPLE4I:
            return setUniform4i(ctx, shaderProgramId, loc,
                    (int[])sav.attrWrapper.getRef());
            

        case ShaderAttributeObject.TYPE_TUPLE4F:
            return setUniform4f(ctx, shaderProgramId, loc,
                    (float[])sav.attrWrapper.getRef());
            

        case ShaderAttributeObject.TYPE_TUPLE4D:
            throw new RuntimeException("not implemented");

        case ShaderAttributeObject.TYPE_MATRIX3F:
            throw new RuntimeException("not implemented");
        /*
        return setUniformMatrix3f(ctx, shaderProgramId, loc,
                           (float[])sav.attrWrapper.getRef());
        
         */
        case ShaderAttributeObject.TYPE_MATRIX3D:
            throw new RuntimeException("not implemented");
        case ShaderAttributeObject.TYPE_MATRIX4F:
            throw new RuntimeException("not implemented");
        /*
        return setUniformMatrix4f(ctx, shaderProgramId, loc,
                           (float[])sav.attrWrapper.getRef());
        
         */
        case ShaderAttributeObject.TYPE_MATRIX4D:
            throw new RuntimeException("not implemented");

        default:
            // Should never get here
            assert(false);
            return null;
        }
    }
    
    void setShaderAttributes(Canvas3D cv, ShaderAttributeSet attributeSet) {
        int cvRdrIndex;
        if (cv.useSharedCtx && cv.screen.renderer.sharedCtx != 0) {
            // TODO : Need to test useSharedCtx case. ** Untested case **
            cvRdrIndex = cv.screen.renderer.rendererId;
        } else {
            cvRdrIndex = cv.canvasId;
        }
        
        ShaderError err = null;
        ShaderProgramData spData = getShaderProgramData(cvRdrIndex);
        long shaderProgramId = spData.getShaderProgramId();
 
        Iterator attrs = attributeSet.getAttrs().values().iterator();
        while (attrs.hasNext()) {
            ShaderAttribute sa = (ShaderAttribute)attrs.next();
            
            Long attrLocation = spData.getLocation(sa.getName());
            if(attrLocation == null) {
                // TODO : Need to generate a ShaderError.
                System.err.println("ShaderProgramRetained : attrLocation (" + sa.getName() + ") is null.");
            } else {
                long loc = attrLocation.longValue();
                if (sa instanceof ShaderAttributeValue) {
                    err = setUniformAttrValue(cv.ctx, shaderProgramId, loc, (ShaderAttributeValue)sa);
                } else if (sa instanceof ShaderAttributeArray) {
                    throw new RuntimeException("not implemented");
                } else if (sa instanceof ShaderAttributeBinding) {
                    throw new RuntimeException("not implemented");
                } else {
                    assert(false);
                }
                
                if (err != null) {
                    err.setShaderProgram((ShaderProgram)this.source);
                    err.setShaderAttributeSet(attributeSet);
                    err.setShaderAttribute(sa);
                    err.setCanvas3D(cv);
                    
                    notifyErrorListeners(cv, err);
                }                    
            }
        }
    }
    
    class ShaderProgramData extends Object {
	
	// shaderProgramId use by native code. 
	private long shaderProgramId;
	
	// linked flag for native.
	private boolean linked;
	
	// A map of locations for ShaderAttributes.
	private HashMap locationMap = new HashMap();

	/** ShaderProgramData Constructor */
	ShaderProgramData(long shaderProgramId) {
	    this.shaderProgramId = shaderProgramId;
	}

	long getShaderProgramId() {
	    return this.shaderProgramId;
	}

	void setLinked(boolean linked) {
	    this.linked = linked;
	}

	boolean isLinked() {
	    return linked;
	}

	void setLocation(String shaderAttribute, Long locObj) {
	    assert(shaderAttribute != null);
	    locationMap.put(shaderAttribute, locObj);
	}

	Long getLocation(String shaderAttribute) {
	    return  (Long) locationMap.get(shaderAttribute);
	}


    }


}
