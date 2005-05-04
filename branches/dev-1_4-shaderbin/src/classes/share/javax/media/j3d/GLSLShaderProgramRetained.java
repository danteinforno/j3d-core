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
 * @see SourceCodeShader
 *
 * @since Java 3D 1.4
 */

class GLSLShaderProgramRetained extends ShaderProgramRetained {

    // TODO : Use the members in ShaderProgramRetained -- Chien.
    private SourceCodeShader vertexShader = null; // TODO: make this an array
    private SourceCodeShader fragmentShader = null; // TODO: make this an array
    private int shaderProgramId = 0;


    /**
     * Constructs a GLSL shader program node component.
     *
     * <br>
     * TODO: ADD MORE DOCUMENTATION HERE.
     */
    GLSLShaderProgramRetained() {
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
     */
    void setShaders(Shader[] shaders) {
	
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
    Shader[] getShaders() {

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

    /* New native interfaces */
    private native ShaderError createShader(long ctx, int shaderType, long[] shaderId);
    private native ShaderError destroyShader(long ctx, long shaderId);
    private native ShaderError compileShader(long ctx, long shaderId, String program);

    private native ShaderError createShaderProgram(long ctx, long[] shaderProgramId);
    private native ShaderError destroyShaderProgram(long ctx, long shaderProgramId);
    private native ShaderError linkShaderProgram(long ctx, long shaderProgramId,
						 long[] shaderId);

    /*
    private native ShaderError createUniformLocation(long ctx,
						     long shaderProgramId,
						     String attrName,
						     long[] uniformLocation);

    private native ShaderError setUniform1i(long ctx,
					    long shaderProgramId,
					    long uniformLocation,
					    int value);

    private native ShaderError setUniform3f(long ctx,
					    long shaderProgramId,
					    long uniformLocation,
					    float[] value);    
    */

    /**
     * Method to create the native shader.
     */
    ShaderError createShader(long ctx, int cvRdrIndex, ShaderRetained shader) {
	System.out.println("GLSLShaderProgram : createShader not implemented yet!");
	return null;
    }

    /**
     * Method to destroy the native shader.
     */
    ShaderError destroyShader(long ctx, int cvRdrIndex, ShaderRetained shader) {
	System.out.println("GLSLShaderProgram : destroyShader not implemented yet!");
	return null;
    }
    
    /**
     * Method to compile the native shader.
     */
    ShaderError compileShader(long ctx, int cvRdrIndex, ShaderRetained shader) {
	System.out.println("GLSLShaderProgram : compileShader not implemented yet!");
	return null;
    }

    /**
     * Method to create the native shader program.
     */
    ShaderError createShaderProgram(long ctx, int cvRdrIndex) {
	System.out.println("GLSLShaderProgram : createShaderProgram not implemented yet!");
	return null;
    }

    /**
     * Method to destroy the native shader program.
     */
    ShaderError destroyShaderProgram(long ctx, int cvRdrIndex) {
	System.out.println("GLSLShaderProgram : destroyShaderProgram not implemented yet!");
	return null;
    }

    /**
     * Method to link the native shader program.
     */
    ShaderError linkShaderProgram(long ctx, int cvRdrIndex, ShaderRetained[] shaders) {
	System.out.println("GLSLShaderProgram : linkShaderProgram not implemented yet!");
	return null;
    }

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
