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
 * The GLSLShaderProgram object is a concrete implementation of a
 * ShaderProgram node component for the OpenGL GLSL shading language.
 *
 * @see Shader
 *
 * @since Java 3D 1.4
 */

public class GLSLShaderProgram extends ShaderProgram {
    private SourceCodeShader vertexShader = null; // TODO: make this an array
    private SourceCodeShader fragmentShader = null; // TODO: make this an array
    private int shaderProgramId = 0;


    /**
     * Constructs a GLSL shader program node component.
     *
     * <br>
     * TODO: ADD MORE DOCUMENTATION HERE.
     */
    public GLSLShaderProgram() {
    }

    /**
     * Copies the specified array of shaders into this shader
     * program. This method makes a shallow copy of the array. The
     * array of shaders may be null or empty (0 length), but the
     * elements of the array must be non-null. The shading language of
     * each shader in the array must be
     * <code>SHADING_LANGUAGE_GLSL</code>. Each shader in the array must
     * be a SourceCodeShader.
     *
     * @param shaders array of Shader objects to be copied into this
     * ShaderProgram
     *
     * @exception CapabilityNotSetException if appropriate capability is
     * not set and this object is part of live or compiled scene graph
     *
     * @exception IllegalArgumentException if the shading language of
     * any shader in the shaders array is <em>not</em>
     * <code>SHADING_LANGUAGE_GLSL</code>.
     *
     * @exception ClassCastException if any shader in the shaders
     * array is <em>not</em> a SourceCodeShader.
     */
    public void setShaders(Shader[] shaders) {
	if (isLiveOrCompiled()) {
	    if(!this.getCapability(ALLOW_SHADERS_WRITE)) {
		throw new CapabilityNotSetException(J3dI18N.getString("GLSLShaderProgram0"));
	    }
	}

	// TODO: move the rest of this into a GLSLShaderProgramRetained class

	// TODO: create an array of shaders rather than one of each

	if (shaders == null) {
	    vertexShader = fragmentShader = null;
	    return;
	}

	// Check shaders for valid shading language and class type
	for (int i = 0; i < shaders.length; i++) {
	    if (shaders[i].getShadingLanguage() != Shader.SHADING_LANGUAGE_GLSL) {
		throw new IllegalArgumentException(J3dI18N.getString("GLSLShaderProgram2"));
	    }

	    // Try to cast shader to SourceCodeShader; it will throw
	    // ClassCastException if it isn't.
	    SourceCodeShader shad = (SourceCodeShader)shaders[i];
	}

	vertexShader = fragmentShader = null;
	// Copy vertex and fragment shader
	// TODO: handle array of shaders
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
		throw new CapabilityNotSetException(J3dI18N.getString("GLSLShaderProgram1"));
	    }
	}

	throw new RuntimeException("not implemented");
    }


    private native int updateNative(long ctx,
				    int shaderProgram,
				    String vtxShader,
				    String fragShader);

    private native void setUniform1i(long ctx,
				     int shaderProgram,
				     String attrName,
				     int value);
    private native void setUniform1f(long ctx,
				     int shaderProgram,
				     String attrName,
				     float value);
    private native void setUniform2i(long ctx,
				     int shaderProgram,
				     String attrName,
				     int[] value);
    private native void setUniform2f(long ctx,
				     int shaderProgram,
				     String attrName,
				     float[] value);
    private native void setUniform3i(long ctx,
				     int shaderProgram,
				     String attrName,
				     int[] value);
    private native void setUniform3f(long ctx,
				     int shaderProgram,
				     String attrName,
				     float[] value);
    private native void setUniform4i(long ctx,
				     int shaderProgram,
				     String attrName,
				     int[] value);
    private native void setUniform4f(long ctx,
				     int shaderProgram,
				     String attrName,
				     float[] value);
    private native void setUniformMatrix3f(long ctx,
					   int shaderProgram,
					   String attrName,
					   float[] value);
    private native void setUniformMatrix4f(long ctx,
					   int shaderProgram,
					   String attrName,
					   float[] value);

    void updateNative(long ctx) {
	String vertexShaderStr = null;
	String fragmentShaderStr = null;

	if (vertexShader != null) {
	    vertexShaderStr = vertexShader.getShaderSource();
	}
	if (fragmentShader != null) {
	    fragmentShaderStr = fragmentShader.getShaderSource();
	}

	shaderProgramId = updateNative(ctx,
				       shaderProgramId,
				       vertexShaderStr,
				       fragmentShaderStr);
    }

    void disableNative(long ctx) {
	updateNative(ctx, 0, null, null);
    }


    /**
     * Update native value for ShaderAttributeValue class
     */
    void setUniformAttrValue(long ctx, ShaderAttributeValue sav) {
	switch (sav.classType) {
	case ShaderAttributeObject.TYPE_INTEGER:
	    setUniform1i(ctx,
			 shaderProgramId,
			 sav.getAttributeName(),
			 ((int[])sav.attrWrapper.getRef())[0]);
	    break;

	case ShaderAttributeObject.TYPE_FLOAT:
	    setUniform1f(ctx,
			 shaderProgramId,
			 sav.getAttributeName(),
			 ((float[])sav.attrWrapper.getRef())[0]);
	    break;

	case ShaderAttributeObject.TYPE_DOUBLE:
	    throw new RuntimeException("not implemented");

	case ShaderAttributeObject.TYPE_TUPLE2I:
	    setUniform2i(ctx,
			 shaderProgramId,
			 sav.getAttributeName(),
			 (int[])sav.attrWrapper.getRef());
	    break;

	case ShaderAttributeObject.TYPE_TUPLE2F:
	    setUniform2f(ctx,
			 shaderProgramId,
			 sav.getAttributeName(),
			 (float[])sav.attrWrapper.getRef());
	    break;

	case ShaderAttributeObject.TYPE_TUPLE2D:
	    throw new RuntimeException("not implemented");

	case ShaderAttributeObject.TYPE_TUPLE3I:
	    setUniform3i(ctx,
			 shaderProgramId,
			 sav.getAttributeName(),
			 (int[])sav.attrWrapper.getRef());
	    break;

	case ShaderAttributeObject.TYPE_TUPLE3F:
	    setUniform3f(ctx,
			 shaderProgramId,
			 sav.getAttributeName(),
			 (float[])sav.attrWrapper.getRef());
	    break;

	case ShaderAttributeObject.TYPE_TUPLE3D:
	    throw new RuntimeException("not implemented");

	case ShaderAttributeObject.TYPE_TUPLE4I:
	    setUniform4i(ctx,
			 shaderProgramId,
			 sav.getAttributeName(),
			 (int[])sav.attrWrapper.getRef());
	    break;

	case ShaderAttributeObject.TYPE_TUPLE4F:
	    setUniform4f(ctx,
			 shaderProgramId,
			 sav.getAttributeName(),
			 (float[])sav.attrWrapper.getRef());
	    break;

	case ShaderAttributeObject.TYPE_TUPLE4D:
	    throw new RuntimeException("not implemented");

	case ShaderAttributeObject.TYPE_MATRIX3F:
	    throw new RuntimeException("not implemented");
	    /*
	    setUniformMatrix3f(ctx,
			 shaderProgramId,
			       sav.getAttributeName(),
			       (float[])sav.attrWrapper.getRef());
	    break;
	    */
	case ShaderAttributeObject.TYPE_MATRIX3D:
	    throw new RuntimeException("not implemented");
	case ShaderAttributeObject.TYPE_MATRIX4F:
	    throw new RuntimeException("not implemented");
	    /*
	    setUniformMatrix4f(ctx,
			 shaderProgramId,
			       sav.getAttributeName(),
			       (float[])sav.attrWrapper.getRef());
	    break;
	    */
	case ShaderAttributeObject.TYPE_MATRIX4D:
	    throw new RuntimeException("not implemented");

	default:
	    // Should never get here
	    assert(false);
	    return;
	}
    }

}
