/*
 * $RCSfile$
 *
 * Copyright (c) 2004 Sun Microsystems, Inc. All rights reserved.
 *
 * Use is subject to license terms.
 *
 * $Revision$
 * $Date$
 * $State$
 */

package javax.media.j3d;

/**
 * The GL2ShaderProgram object is a concrete implementation of a ShaderProgram
 * node component for the OpenGL GL2 shading language.
 *
 * @see GL2VertexShader
 * @see GL2FragmentShader
 *
 * @since Java 3D 1.4
 */

public class GL2ShaderProgram extends ShaderProgram {
    private GL2VertexShader vertexShader = null;
    private GL2FragmentShader fragmentShader = null;

    public GL2ShaderProgram() {
    }

    public void setVertexShader(GL2VertexShader vertexShader) {
	this.vertexShader = vertexShader;
    }

    public void setFragmentShader(GL2FragmentShader fragmentShader) {
	this.fragmentShader = fragmentShader;
    }

    public GL2VertexShader getVertexShader() {
	return vertexShader;
    }

    public GL2FragmentShader getFragmentShader() {
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
