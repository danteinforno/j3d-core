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
 * The GLSL Fragment Shader object...
 *
 * @see GLSLShaderProgram
 * @see GLSLVertexShader
 *
 * @since Java 3D 1.4
 */

public class GLSLFragmentShader extends GLSLShader {
    public GLSLFragmentShader() {
    }

    public GLSLFragmentShader(String shaderSource) {
	super(shaderSource);
    }
}
