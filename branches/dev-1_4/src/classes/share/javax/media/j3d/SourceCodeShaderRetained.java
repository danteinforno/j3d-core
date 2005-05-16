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

/**
 * The SourceCodeShaderRetained object is a shader that is defined using
 * text-based source code. It is used to define the source code for
 * both vertex and fragment shaders. The currently supported shading
 * languages are Cg and GLSL.
 */

class SourceCodeShaderRetained extends ShaderRetained {

    private String shaderSource = null;

    /**
     * Constructs a new shader retained object of the specified shading
     * language and shader type from the specified source string.
     */

    SourceCodeShaderRetained() {
    }
    
    final void set(int shadingLanguage, int shaderType, String shaderSource) {

	this.shadingLanguage = shadingLanguage;
	this.shaderType = shaderType;
	this.shaderSource = shaderSource;
    }

    /**
     * Retrieves the shader source string from this shader object.
     *
     * @return the shader source string.
     */
    final String getShaderSource() {
	return shaderSource;
    }

    final void setShaderSource(String sc) {
	this.shaderSource = shaderSource;
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

    synchronized void createMirrorObject() {
	// System.out.println("SourceCodeShaderRetained : createMirrorObject");

	if (mirror == null) {
	    SourceCodeShaderRetained  mirrorSCS = new SourceCodeShaderRetained();
	    mirror = mirrorSCS;
	}

	initMirrorObject();
    }
    
    /**
     * Initializes a mirror object.
     */
    synchronized void initMirrorObject() {
	mirror.source = source;

	((SourceCodeShaderRetained) mirror).set(shadingLanguage, shaderType, shaderSource);
	((SourceCodeShaderRetained) mirror).shaderIds = null;	
    }

    synchronized void updateMirrorObject(int component, Object value) {
	System.out.println("SourceCodeShader.updateMirrorObject not implemented yet!");
    }

}
