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
 * The GL2 Vertex Shader object...
 *
 * @see GL2ShaderProgram
 * @see GL2FragmentShader
 *
 * @since Java 3D 1.4
 */

public class GL2VertexShader extends GL2Shader {
    public GL2VertexShader() {
    }

    public GL2VertexShader(String shaderSource) {
	super(shaderSource);
    }
}
