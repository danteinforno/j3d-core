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

/**
 * The ShaderRetained object is the abstract base class for programmable
 * shader code. Currently, only text-based source code shaders are
 * supported, so the only subclass of Shader is SourceCodeShader. We
 * leave open the possibility for binary (object code) shaders in the
 * future.
 */
abstract class ShaderRetained extends NodeComponentRetained {
    int shadingLanguage;
    int shaderType;

    // shaderId use by native code. One per Canvas.
    long[] shaderIds;
    boolean[] compiled;

    // Flag indicating whether a COMPILE_ERROR has occurred for this shader
    // object.  It is set in updateNative to indicate that the compileShader
    // operation failed. It is cleared in setLive or clearLive.
    // TODO KCR: Add code to clear this in setLive or clearLive
    boolean compileErrorOccurred = false;

    // need to synchronize access from multiple rendering threads 
    Object resourceLock = new Object();

    void set(int shadingLanguage, int shaderType) {
	this.shadingLanguage = shadingLanguage;
	this.shaderType = shaderType;
    }

    int getShadingLanguage() {
	return shadingLanguage;
    }

    int getShaderType() {
	return shaderType;
    }
 
    void setLive(boolean inBackgroundGroup, int refCount) {
	// System.out.println("SourceCodeShaderRetained.setLive()");
	super.setLive(inBackgroundGroup, refCount);
    }

    void clearLive(int refCount) {
	// System.out.println("SourceCodeShaderRetained.clearLive()");

	super.clearLive(refCount);
	if (this.refCount <= 0) {
	    // Should this be done here ? In user thread ? --- Chien
	    // freeShader();
	}
    }

     /**
      * Shader object doesn't really have mirror object.
      * But it's using the updateMirrorObject interface to propagate
      * the changes to the users
      */
     synchronized void updateMirrorObject(int component, Object value) {
	System.out.println("Shader.updateMirrorObject not implemented yet!");
     }

    /*
     final void sendMessage(int attrMask, Object attr) {
	System.out.println("Shader.sendMessage not implemented yet!");
	 

        J3dMessage createMessage = VirtualUniverse.mc.getMessage();
        createMessage.threads = J3dThread.UPDATE_RENDERING_ATTRIBUTES |
				J3dThread.UPDATE_RENDER;
        createMessage.type = J3dMessage.SHADER_CHANGED;
        createMessage.universe = null;
        createMessage.args[0] = this;
        createMessage.args[1]= new Integer(attrMask);
        createMessage.args[2] = attr;
	createMessage.args[3] = new Integer(changedFrequent);
        VirtualUniverse.mc.processMessage(createMessage);
     }

    */

    void handleFrequencyChange(int bit) {
	System.out.println("Shader.handleFrequencyChange not implemented yet!");
    }

}

