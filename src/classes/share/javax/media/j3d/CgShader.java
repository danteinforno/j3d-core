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
 * The Cg Shader object...
 *
 * <p>
 * NOTE: Applications should <i>not</i> extend this class.
 *
 * @see CgShaderProgram
 *
 * @since Java 3D 1.4
 */

public abstract class CgShader extends Shader {
    String shaderSource = null;

    /**
     * Package scope constructor so it can't be subclassed by classes
     * outside the javax.media.j3d package.
     */
    CgShader() {
    }

    /**
     * Package scope constructor so it can't be subclassed by classes
     * outside the javax.media.j3d package.
     */
    CgShader(String shaderSource) {
	this.shaderSource = shaderSource;
    }

    public void setShaderSource(String shaderSource) {
	this.shaderSource = shaderSource;
    }

    public String getShaderSource() {
	return shaderSource;
    }
}
