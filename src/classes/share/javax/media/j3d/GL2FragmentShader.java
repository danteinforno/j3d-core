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
 * The GL2 Fragment Shader object...
 *
 * @see GL2ShaderProgram
 * @see GL2VertexShader
 *
 * @since Java 3D 1.4
 */

public class GL2FragmentShader extends GL2Shader {
    public GL2FragmentShader() {
    }

    public GL2FragmentShader(String shaderSource) {
	super(shaderSource);
    }
}
