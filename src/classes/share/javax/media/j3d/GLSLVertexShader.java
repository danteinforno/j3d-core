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
 * The GLSL Vertex Shader object...
 *
 * @see GLSLShaderProgram
 * @see GLSLFragmentShader
 *
 * @since Java 3D 1.4
 */

public class GLSLVertexShader extends GLSLShader {
    public GLSLVertexShader() {
    }

    public GLSLVertexShader(String shaderSource) {
	super(shaderSource);
    }
}
