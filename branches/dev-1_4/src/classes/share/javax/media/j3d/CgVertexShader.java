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
 * The Cg Vertex Shader object...
 *
 * @see CgShaderProgram
 * @see CgFragmentShader
 *
 * @since Java 3D 1.4
 */

public class CgVertexShader extends CgShader {
    public CgVertexShader() {
    }

    public CgVertexShader(String shaderSource) {
	super(shaderSource);
    }
}
