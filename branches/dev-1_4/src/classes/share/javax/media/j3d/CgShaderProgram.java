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
 * The CgShaderProgram object is a concrete implementation of a ShaderProgram
 * node component for NVIDIA's Cg shader language.
 *
 * @see CgVertexShader
 * @see CgFragmentShader
 *
 * @since Java 3D 1.4
 */

public class CgShaderProgram extends ShaderProgram {
    private CgVertexShader vertexShader = null;
    private CgFragmentShader fragmentShader = null;

    public CgShaderProgram() {
    }

    public void setVertexShader(CgVertexShader vertexShader) {
	this.vertexShader = vertexShader;
    }

    public void setFragmentShader(CgFragmentShader fragmentShader) {
	this.fragmentShader = fragmentShader;
    }

    public CgVertexShader getVertexShader() {
	return vertexShader;
    }

    public CgFragmentShader getFragmentShader() {
	return fragmentShader;
    }

    private native void updateNative(long ctx,
				     String vtxShader,
				     String fragShader);

    void updateNative(long ctx) {
	/*
	System.err.println("CgShaderProgram.updateNative(ctx)");
	*/

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