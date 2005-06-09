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

class CgShaderProgramRetained extends ShaderProgramRetained {
    
    /**
     * Constructs a Cg shader program node component.
     *
     * <br>
     * TODO: ADD MORE DOCUMENTATION HERE.
     */
    CgShaderProgramRetained() {
    }

    synchronized void createMirrorObject() {
	// System.out.println("CgShaderProgramRetained : createMirrorObject");
        // This method should only call by setLive().
	if (mirror == null) {
	    CgShaderProgramRetained  mirrorCgSP = new CgShaderProgramRetained();	    
	    mirror = mirrorCgSP;
	}
	initMirrorObject();
    }

    

    native ShaderError setUniform1i(long ctx,
					    long shaderProgramId,
					    long uniformLocation,
					    int value);
    
    native ShaderError setUniform1f(long ctx,
					    long shaderProgramId,
					    long uniformLocation,
					    float value);
    
    native ShaderError setUniform2i(long ctx,
					    long shaderProgramId,
					    long uniformLocation,
					    int[] value);
    
    native ShaderError setUniform2f(long ctx,
					    long shaderProgramId,
					    long uniformLocation,
					    float[] value);
    
    native ShaderError setUniform3i(long ctx,
					    long shaderProgramId,
					    long uniformLocation,
					    int[] value);
    
    native ShaderError setUniform3f(long ctx,
					    long shaderProgramId,
					    long uniformLocation,
					    float[] value);    
    
    native ShaderError setUniform4i(long ctx,
					    long shaderProgramId,
					    long uniformLocation,
					    int[] value);
    
    native ShaderError setUniform4f(long ctx,
					    long shaderProgramId,
					    long uniformLocation,
					    float[] value);    
    
    native ShaderError setUniformMatrix3f(long ctx,
					   long shaderProgramId,
				           long uniformLocation,
					   float[] value);

    native ShaderError setUniformMatrix4f(long ctx,
					   long shaderProgramId,
			         	   long uniformLocation,
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
        return cv.shadingLanguageCg;
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


}
