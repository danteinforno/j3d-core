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
    private int shaderProgramId = 0;

    // For debugging only
    static int GLSLCounter = 0;
    int spId;
    
    /**
     * Constructs a GLSL shader program node component.
     *
     * <br>
     * TODO: ADD MORE DOCUMENTATION HERE.
     */
    GLSLShaderProgramRetained() {
       	// For debugging only
	// spId = GLSLCounter++;
        // System.out.println("GLSLShaderProgramRetained : creation " + spId);
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

	if (shaders == null) {
	    this.shaders = null;
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

	this.shaders = new ShaderRetained[shaders.length];

	// Copy vertex and fragment shader
	for (int i = 0; i < shaders.length; i++) {
	    this.shaders[i] = (ShaderRetained)shaders[i].retained;
	}

    }

    // Implement abstract getShaders method (inherit javadoc from parent class)
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

    synchronized void createMirrorObject() {
	// System.out.println("GLSLShaderProgramRetained : createMirrorObject");
        // This method should only call by setLive().
	if (mirror == null) {
	    GLSLShaderProgramRetained  mirrorGLSLSP = new GLSLShaderProgramRetained();	    
	    mirror = mirrorGLSLSP;
	}
	initMirrorObject();
    }

    
    private native ShaderError createUniformLocation(long ctx,
						     long shaderProgramId,
						     String attrName,
						     long[] uniformLocation);

    private native ShaderError setUniform1i(long ctx,
					    long shaderProgramId,
					    long uniformLocation,
					    int value);
    
    private native ShaderError setUniform1f(long ctx,
					    long shaderProgramId,
					    long uniformLocation,
					    float value);
    
    private native ShaderError setUniform2i(long ctx,
					    long shaderProgramId,
					    long uniformLocation,
					    int[] value);
    
    private native ShaderError setUniform2f(long ctx,
					    long shaderProgramId,
					    long uniformLocation,
					    float[] value);
    
    private native ShaderError setUniform3i(long ctx,
					    long shaderProgramId,
					    long uniformLocation,
					    int[] value);
    
    private native ShaderError setUniform3f(long ctx,
					    long shaderProgramId,
					    long uniformLocation,
					    float[] value);    
    
    private native ShaderError setUniform4i(long ctx,
					    long shaderProgramId,
					    long uniformLocation,
					    int[] value);
    
    private native ShaderError setUniform4f(long ctx,
					    long shaderProgramId,
					    long uniformLocation,
					    float[] value);    
    
    private native void setUniformMatrix3f(long ctx,
					   long shaderProgram,
				           long uniformLocation,
					   float[] value);

    private native void setUniformMatrix4f(long ctx,
					   long shaderProgram,
			         	   long uniformLocation,
					   float[] value);
    
    
    // TODO : REMOVE Old native interfaces once ShaderAttribute implementation is done.
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
    private native ShaderError createNativeShader(long ctx, int shaderType, long[] shaderId);
    private native ShaderError destroyNativeShader(long ctx, long shaderId);
    private native ShaderError compileNativeShader(long ctx, long shaderId, String program);

    private native ShaderError createNativeShaderProgram(long ctx, long[] shaderProgramId);
    private native ShaderError destroyNativeShaderProgram(long ctx, long shaderProgramId);
    private native ShaderError linkNativeShaderProgram(long ctx, long shaderProgramId,
						       long[] shaderId);
    private native ShaderError bindNativeVertexAttrName(long ctx, long shaderProgramId,
                                                        String attrName, int attrIndex);
    private native ShaderError lookupNativeShaderAttrName(long ctx, long shaderProgramId,
                                                          String attrName, long[] locArr);
    
    private native ShaderError useShaderProgram(long ctx, long shaderProgramId);
 
    /**
     * Method to return a flag indicating whether this
     * ShaderProgram is supported on the specified Canvas.
     */
    boolean isSupported(Canvas3D cv) {
        return cv.shadingLanguageGLSL;
    }

    /**
     * Method to create the native shader.
     */
    ShaderError createShader(long ctx, ShaderRetained shader, long[] shaderIdArr) {	
	  return  createNativeShader(ctx, shader.shaderType, shaderIdArr);
    }
    
    /**
     * Method to destroy the native shader.
     */
    ShaderError destroyShader(long ctx, long shaderId) {
	return destroyNativeShader(ctx, shaderId);
    }
    
    /**
     * Method to compile the native shader.
     */
    ShaderError compileShader(long ctx, long shaderId, String source) {
        return compileNativeShader(ctx, shaderId, source );
    }

    /**
     * Method to create the native shader program.
     */
    ShaderError createShaderProgram(long ctx, long[] shaderProgramIdArr) {
	    return createNativeShaderProgram(ctx, shaderProgramIdArr);  
    }

    /**
     * Method to destroy the native shader program.
     */
    ShaderError destroyShaderProgram(long ctx, long shaderProgramId) {
        return destroyNativeShaderProgram(ctx, shaderProgramId);
    }

    /**
     * Method to link the native shader program.
     */
    ShaderError linkShaderProgram(long ctx, long shaderProgramId, long[] shaderIds) {
        return linkNativeShaderProgram(ctx, shaderProgramId, shaderIds);
    }
 
    ShaderError bindVertexAttrName(long ctx, long shaderProgramId, String attrName, int attrIndex) {
        return bindNativeVertexAttrName(ctx, shaderProgramId, attrName, attrIndex);
    }

    ShaderError lookupShaderAttrName(long ctx, long shaderProgramId, String attrName, long[] locArr) {
        return lookupNativeShaderAttrName(ctx, shaderProgramId, attrName, locArr);
    }

    /**
     * Method to enable the native shader program.
     */
    ShaderError enableShaderProgram(long ctx, long shaderProgramId) {
	return useShaderProgram(ctx, shaderProgramId);
    }
	
    /**
     * Method to disable the native shader program.
     */
    ShaderError disableShaderProgram(long ctx) {
	return useShaderProgram(ctx, 0);
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
