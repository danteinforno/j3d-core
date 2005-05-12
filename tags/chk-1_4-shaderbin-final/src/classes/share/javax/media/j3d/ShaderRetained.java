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
    protected int shadingLanguage;
    protected int shaderType;


    // shaderId use by native code. One per Canvas.
    protected long[] shaderIdPerCanvas;   
    
    // Each bit corresponds to a unique renderer if shared context
    // or a unique canvas otherwise.
    // This mask specifies which renderer/canvas has loaded the
    // shader. 0 means no renderer/canvas has loaded the shader.
    // 1 at the particular bit means that renderer/canvas has loaded the
    // shader. 0 means otherwise.
    int resourceCreationMask = 0x0;

    /*  Most like don't need this method.
    void createShader(int  shadingLanguage, int shaderType) {
	this.shadingLanguage = shadingLanguage;
	this.shaderType = shaderType;
    }
    */

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

     /**
      * Shader object doesn't really have mirror object.
      * But it's using the updateMirrorObject interface to propagate
      * the changes to the users
      */
     synchronized void updateMirrorObject(int component, Object value) {
	System.out.println("Shader.updateMirrorObject not implemented yet!");
     }

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

    void handleFrequencyChange(int bit) {
	System.out.println("Shader.handleFrequencyChange not implemented yet!");
    }

}

