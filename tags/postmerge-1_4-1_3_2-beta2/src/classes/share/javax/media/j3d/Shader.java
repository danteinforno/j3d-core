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
 * The Shader object is the abstract base class for programmable
 * shader source code.  It contains no public methods, fields or
 * constructors.  Each concrete instance of a Shader object allows an
 * application to specify the source code used in programming the
 * Graphics Pipeline Unit (GPU) of the graphics accelerator, using the
 * shader language defined by that Shader. The currently defined
 * shader languages are: Cg and GLSL.
 *
 * <p>
 * NOTE: Applications should <i>not</i> extend this class.
 *
 * @see ShaderProgram
 *
 * @since Java 3D 1.4
 */

public abstract class Shader extends NodeComponent {
    /**
     * Package scope constructor so it can't be subclassed by classes
     * outside the javax.media.j3d package.
     */
    Shader() {
    }
}
