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
 * The Cg Fragment Shader object...
 *
 * @see CgShaderProgram
 * @see CgVertexShader
 *
 * @since Java 3D 1.4
 */

public class CgFragmentShader extends CgShader {
    public CgFragmentShader() {
    }

    public CgFragmentShader(String shaderSource) {
	super(shaderSource);
    }
}
