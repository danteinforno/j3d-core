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
 * The GLSLShaderProgram object is a concrete implementation of a ShaderProgram
 * node component for the OpenGL GLSL shading language.
 *
 * @see GLSLVertexShader
 * @see GLSLFragmentShader
 *
 * @since Java 3D 1.4
 */

public class GLSLShaderProgram extends ShaderProgram {
    private GLSLVertexShader vertexShader = null;
    private GLSLFragmentShader fragmentShader = null;
    private int shaderProgramId = 0;


    public GLSLShaderProgram() {
    }

    public void setVertexShader(GLSLVertexShader vertexShader) {
	this.vertexShader = vertexShader;
    }

    public void setFragmentShader(GLSLFragmentShader fragmentShader) {
	this.fragmentShader = fragmentShader;
    }

    public GLSLVertexShader getVertexShader() {
	return vertexShader;
    }

    public GLSLFragmentShader getFragmentShader() {
	return fragmentShader;
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
