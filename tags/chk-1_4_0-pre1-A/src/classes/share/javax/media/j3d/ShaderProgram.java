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
 * The ShaderProgram node component object is the abstract base class
 * for programmable shader programs. Each concrete instance of a
 * ShaderProgram is a container for a set of Shader objects. The set
 * of Shaders contained in the ShaderProgram is a complete program for
 * the Graphics Pipeline Unit (GPU) of the graphics accelerator. It is
 * specified using the shader language defined by the
 * ShaderProgram. The currently defined shader languages are: Cg and
 * GLSL.
 *
 * <p>
 * NOTE: Applications should <i>not</i> extend this class.
 *
 * @see Shader
 * @see ShaderAppearance#setShaderProgram
 *
 * @since Java 3D 1.4
 */

public abstract class ShaderProgram extends NodeComponent {

    /**
     * Specifies that this ShaderProgram object allows reading
     * its shaders.
     */
    public static final int ALLOW_SHADERS_READ =
	CapabilityBits.SHADER_PROGRAM_ALLOW_SHADERS_READ;

    /**
     * Specifies that this ShaderProgram object allows writing
     * its shaders.
     */
    public static final int ALLOW_SHADERS_WRITE =
	CapabilityBits.SHADER_PROGRAM_ALLOW_SHADERS_WRITE;


    /**
     * Package scope constructor so it can't be subclassed by classes
     * outside the javax.media.j3d package.
     */
    ShaderProgram() {
    }

    /**
     * Copies the specified array of shaders into this shader
     * program. This method makes a shallow copy of the array. The
     * array of shaders may be null or empty (0 length), but the
     * elements of the array must be non-null. The shading
     * language of each shader in the array must match the
     * subclass. Subclasses may impose additional restrictions.
     *
     * @param shaders array of Shader objects to be copied into this
     * ShaderProgram
     *
     * @exception CapabilityNotSetException if appropriate capability is
     * not set and this object is part of live or compiled scene graph
     *
     * @exception IllegalArgumentException if the shading language of
     * any shader in the shaders array doesn't match the type of the
     * subclass.
     */
    public abstract void setShaders(Shader[] shaders);

    /**
     * Retrieves the array of shaders from this shader program. A
     * shallow copy of the array is returned. The return value may
     * be null.
     *
     * @return a copy of this ShaderProgram's array of Shader objects
     *
     * @exception CapabilityNotSetException if appropriate capability is
     * not set and this object is part of live or compiled scene graph
     */
    public abstract Shader[] getShaders();


    /**
     * Method to update the native shader program.
     */
    abstract void updateNative(long ctx);

    /**
     * Method to disable the native shader program.
     */
    abstract void disableNative(long ctx);

    /**
     * Method to update the native shader attributes
     */
    abstract void setUniformAttrValue(long ctx, ShaderAttributeValue sav);

}
