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

    private native void updateNative(long ctx,
				     byte[] vtxShader,
				     byte[] fragShader);

    void updateNative(long ctx) {
	byte[] vertexShaderBytes = null;
	byte[] fragmentShaderBytes = null;

	if (vertexShader != null && vertexShader.getShaderSource() != null) {
	    vertexShaderBytes = vertexShader.getShaderSource().getBytes();
	}
	if (fragmentShader != null && fragmentShader.getShaderSource() != null) {
	    fragmentShaderBytes = fragmentShader.getShaderSource().getBytes();
	}

	updateNative(ctx, vertexShaderBytes, fragmentShaderBytes);
    }
}
