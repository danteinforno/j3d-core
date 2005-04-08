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
 * The SourceCodeShader object is a shader that is defined using
 * text-based source code. It is used to define the source code for
 * both vertex and fragment shaders. The currently supported shading
 * languages are Cg and GLSL.
 *
 * @see ShaderProgram
 *
 * @since Java 3D 1.4
 */

public class SourceCodeShader extends Shader {
    String shaderSource = null;

    /**
     * Constructs a new shader object of the specified shading
     * language and shader type from the specified source string.
     *
     * @param shadingLanguage the specified shading language, one of:
     * <code>SHADING_LANGUAGE_GLSL</code> or
     * <code>SHADING_LANGUAGE_CG</code>.
     *
     * @param shaderType the shader type, one of:
     * <code>SHADER_TYPE_VERTEX</code> or
     * <code>SHADER_TYPE_FRAGMENT</code>.
     *
     * @param shaderSource the shader source code
     */
    public SourceCodeShader(int shadingLanguage, int shaderType, String shaderSource) {
	super(shadingLanguage, shaderType);
	this.shaderSource = shaderSource;
    }

    /**
     * Retrieves the shader source string from this shader object.
     *
     * @return the shader source string.
     */
    public String getShaderSource() {
	return shaderSource;
    }
}
