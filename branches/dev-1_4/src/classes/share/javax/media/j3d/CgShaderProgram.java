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
 * The CgShaderProgram object is a concrete implementation of a
 * ShaderProgram node component for NVIDIA's Cg shader language.
 *
 * @see SourceCodeShader
 *
 * @since Java 3D 1.4
 */

public class CgShaderProgram extends ShaderProgram {
    private SourceCodeShader vertexShader = null;
    private SourceCodeShader fragmentShader = null;


    /**
     * Constructs a Cg shader program node component.
     *
     * <br>
     * TODO: ADD MORE DOCUMENTATION HERE.
     */
    public CgShaderProgram() {
    }

    // Implement abstract setVertexAttrNames method (inherit javadoc from parent class)
    public void setVertexAttrNames(String[] vertexAttrNames) {
	checkForLiveOrCompiled();

	throw new RuntimeException("not implemented");
    }

    // Implement abstract getVertexAttrNames method (inherit javadoc from parent class)
    public String[] getVertexAttrNames() {
	throw new RuntimeException("not implemented");
    }

    // Implement abstract setShaderAttrNames method (inherit javadoc from parent class)
    public void setShaderAttrNames(String[] shaderAttrNames) {
	checkForLiveOrCompiled();

	throw new RuntimeException("not implemented");
    }

    // Implement abstract getShaderAttrNames method (inherit javadoc from parent class)
    public String[] getShaderAttrNames() {
	throw new RuntimeException("not implemented");
    }

    /**
     * Copies the specified array of shaders into this shader
     * program. This method makes a shallow copy of the array. The
     * array of shaders may be null or empty (0 length), but the
     * elements of the array must be non-null. The shading language of
     * each shader in the array must be
     * <code>SHADING_LANGUAGE_CG</code>. Each shader in the array must
     * be a SourceCodeShader. There must be no more than one vertex shader
     * and one fragment shader in the array.
     *
     * @param shaders array of Shader objects to be copied into this
     * ShaderProgram
     *
     * @exception CapabilityNotSetException if appropriate capability is
     * not set and this object is part of live or compiled scene graph
     *
     * @exception IllegalArgumentException if the shading language of
     * any shader in the shaders array is <em>not</em>
     * <code>SHADING_LANGUAGE_CG</code>.
     *
     * @exception IllegalArgumentException if there are more than one
     * vertex shader or more than one fragment shader in the shaders
     * array.
     *
     * @exception ClassCastException if any shader in the shaders
     * array is <em>not</em> a SourceCodeShader.
     */
    public void setShaders(Shader[] shaders) {
	checkForLiveOrCompiled();

	// TODO: move the rest of this into a CgShaderProgramRetained class

	if (shaders == null) {
	    vertexShader = fragmentShader = null;
	    return;
	}

	// Check shaders for valid shading language, class type, etc.
	for (int i = 0; i < shaders.length; i++) {
	    boolean hasVertexShader = false;
	    boolean hasFragmentShader = false;

	    // Check shading language
	    if (shaders[i].getShadingLanguage() != Shader.SHADING_LANGUAGE_CG) {
		throw new IllegalArgumentException(J3dI18N.getString("CgShaderProgram2"));
	    }

	    // Check for more than one vertex shader or fragment shader
	    if (shaders[i].getShaderType() == Shader.SHADER_TYPE_VERTEX) {
		if (hasVertexShader) {
		    throw new IllegalArgumentException(J3dI18N.getString("CgShaderProgram3"));
		}
		hasVertexShader = true;
	    }
	    else { // Shader.SHADER_TYPE_FRAGMENT
		if (hasFragmentShader) {
		    throw new IllegalArgumentException(J3dI18N.getString("CgShaderProgram4"));
		}
		hasFragmentShader = true;
	    }

	    // Try to cast shader to SourceCodeShader; it will throw
	    // ClassCastException if it isn't.
	    SourceCodeShader shad = (SourceCodeShader)shaders[i];
	}

	// Copy vertex and fragment shader
	vertexShader = fragmentShader = null;
	for (int i = 0; i < shaders.length; i++) {
	    if (shaders[i].getShaderType() == Shader.SHADER_TYPE_VERTEX) {
		vertexShader = (SourceCodeShader)shaders[i];
	    }
	    else { // Shader.SHADER_TYPE_FRAGMENT
		fragmentShader = (SourceCodeShader)shaders[i];
	    }
	}
    }

    // Implement abstract getShaders method (inherit javadoc from parent class)
    public Shader[] getShaders() {
	if (isLiveOrCompiled()) {
	    if(!this.getCapability(ALLOW_SHADERS_READ)) {
		throw new CapabilityNotSetException(J3dI18N.getString("CgShaderProgram1"));
	    }
	}

	throw new RuntimeException("not implemented");
    }


    private native void updateNative(long ctx,
				     String vtxShader,
				     String fragShader);

    void disableNative(Canvas3D cv) {
	updateNative(cv.ctx, null, null);
    }

    void updateNative(Canvas3D cv) {
	/*
	System.err.println("CgShaderProgram.updateNative(ctx)");
	*/
	long ctx = cv.ctx;

	String vertexShaderStr = null;
	String fragmentShaderStr = null;

	if (vertexShader != null) {
	    vertexShaderStr = vertexShader.getShaderSource();
	}
	if (fragmentShader != null) {
	    fragmentShaderStr = fragmentShader.getShaderSource();
	}

	updateNative(ctx, vertexShaderStr, fragmentShaderStr);
    }

    void setUniformAttrValue(long ctx, ShaderAttributeValue sav) {
	throw new RuntimeException("not implemented");
    }
}
