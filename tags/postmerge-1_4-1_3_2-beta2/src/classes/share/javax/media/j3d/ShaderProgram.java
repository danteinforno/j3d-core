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
 * The ShaderProgram object is the abstract base class for
 * programmable shader programs.  It contains no public methods,
 * fields or constructors.  Each concrete instance of a ShaderProgram
 * is a container for a set of Shader objects. The set of Shaders
 * contained in the ShaderProgram is a complete program for the
 * Graphics Pipeline Unit (GPU) of the graphics accelerator. It is
 * specified using the shader language defined by the
 * ShaderProgram. The currently defined shader languages are: Cg and
 * GLSL.
 *
 * <p>
 * NOTE: Applications should <i>not</i> extend this class.
 *
 * @see Shader
 *
 * @since Java 3D 1.4
 */

public abstract class ShaderProgram extends NodeComponent {

    /**
     * Package scope constructor so it can't be subclassed by classes
     * outside the javax.media.j3d package.
     */
    ShaderProgram() {
    }

    /**
     * Method to update the native shader program.
     */
    abstract void updateNative(long ctx);
}
