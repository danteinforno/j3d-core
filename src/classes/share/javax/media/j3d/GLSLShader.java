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
 * The GLSL Shader object...
 *
 * <p>
 * NOTE: Applications should <i>not</i> extend this class.
 *
 * @see GLSLShaderProgram
 *
 * @since Java 3D 1.4
 */

public abstract class GLSLShader extends Shader {
    String shaderSource = null;

    /**
     * Package scope constructor so it can't be subclassed by classes
     * outside the javax.media.j3d package.
     */
    GLSLShader() {
    }

    /**
     * Package scope constructor so it can't be subclassed by classes
     * outside the javax.media.j3d package.
     */
    GLSLShader(String shaderSource) {
	this.shaderSource = shaderSource;
    }

    public void setShaderSource(String shaderSource) {
	this.shaderSource = shaderSource;
    }

    public String getShaderSource() {
	return shaderSource;
    }
}
