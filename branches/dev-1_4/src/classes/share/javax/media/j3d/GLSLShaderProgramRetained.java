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
 * The GLSLShaderProgram object is a concrete implementation of a
 * ShaderProgram node component for the OpenGL GLSL shading language.
 *
 * @see SourceCodeShader
 *
 * @since Java 3D 1.4
 */

class GLSLShaderProgramRetained extends ShaderProgramRetained {

    // A list of pre-defined bits to indicate which component
    // in this GLSLShaderProgram object changed.
    static final int SHADER_PROGRAM_CREATE              = 0x001;
    static final int SHADER_UPDATE                      = 0x002;
    static final int VERTEX_ATTRIBUTE_NAME_UPDATE       = 0x004;
    static final int SHADER_ATTRIBUTE_UPDATE            = 0x008;
    static final int SHADER_PROGRAM_DESTROY             = 0x010;

    // TODO : Use the members in ShaderProgramRetained -- Chien.
    private int shaderProgramId = 0;

    // For debugging only
    static int GLSLCounter = 0;
    int spId;
    
    /**
     * Constructs a GLSL shader program node component.
     *
     * <br>
     * TODO: ADD MORE DOCUMENTATION HERE.
     */
    GLSLShaderProgramRetained() {
       	// For debugging only
	// spId = GLSLCounter++;
        // System.out.println("GLSLShaderProgramRetained : creation " + spId);
    }


    /**
     * Copies the specified array of shaders into this shader
     * program. This method makes a shallow copy of the array. The
     * array of shaders may be null or empty (0 length), but the
     * elements of the array must be non-null. The shading language of
     * each shader in the array must be
     * <code>SHADING_LANGUAGE_GLSL</code>. Each shader in the array must
     * be a SourceCodeShader.
     *
     * @param shaders array of Shader objects to be copied into this
     * ShaderProgram
     *
     */
    void setShaders(Shader[] shaders) {

	if (shaders == null) {
	    this.shaders = null;
	    return;
	}
	
	// Check shaders for valid shading language and class type
	for (int i = 0; i < shaders.length; i++) {
	    if (shaders[i].getShadingLanguage() != Shader.SHADING_LANGUAGE_GLSL) {
		throw new IllegalArgumentException(J3dI18N.getString("GLSLShaderProgram2"));
	    }

	    // Try to cast shader to SourceCodeShader; it will throw
	    // ClassCastException if it isn't.
	    SourceCodeShader shad = (SourceCodeShader)shaders[i];
	}

	this.shaders = new ShaderRetained[shaders.length];

	// Copy vertex and fragment shader
	for (int i = 0; i < shaders.length; i++) {
	    this.shaders[i] = (ShaderRetained)shaders[i].retained;
	}

    }

    // Implement abstract getShaders method (inherit javadoc from parent class)
    Shader[] getShaders() {

	if (shaders == null) {
	    return null;
	} else {
	    Shader shads[] = 
		new Shader[shaders.length];
	    for (int i = 0; i < shaders.length; i++) {
		if (shaders[i] != null) {
		    shads[i] = (Shader) shaders[i].source;
		} else {
		    shads[i] = null;
		}
	    }
	    return shads;
	}
    }

    void setLive(boolean backgroundGroup, int refCount) {
	
	// System.out.println("GLSLShaderProgramRetained.setLive()");

	if (shaders != null) {
	    for (int i = 0; i < shaders.length; i++){
		shaders[i].setLive(backgroundGroup, refCount);
	    }
	}
	
	super.doSetLive(backgroundGroup, refCount);

        // Send a message to Rendering Attr stucture to update the resourceMask
	// via updateMirrorObject().
	J3dMessage createMessage = VirtualUniverse.mc.getMessage();
	createMessage.threads = J3dThread.UPDATE_RENDERING_ATTRIBUTES;
	createMessage.type = J3dMessage.SHADER_PROGRAM_CHANGED;
	createMessage.args[0] = this;
	createMessage.args[1]= new Integer(SHADER_PROGRAM_CREATE);
 	createMessage.args[2] = null;
 	createMessage.args[3] = new Integer(changedFrequent);
	VirtualUniverse.mc.processMessage(createMessage);
	
	super.markAsLive();
    }

    void clearLive(int refCount) {

        // System.out.println("GLSLShaderProgramRetained.clearLive()");

	super.clearLive(refCount);

	if (shaders != null) {
	    for (int i = 0; i < shaders.length; i++) {
		shaders[i].clearLive(refCount);
	    }
	}
    }


    synchronized void createMirrorObject() {
	// System.out.println("GLSLShaderProgramRetained : createMirrorObject");
	if (mirror == null) {
	    GLSLShaderProgramRetained  mirrorGLSLSP = new GLSLShaderProgramRetained();	    
	    mirror = mirrorGLSLSP;
	}
	initMirrorObject();
    }


    /**
     * Initializes a mirror object.
     */
    synchronized void initMirrorObject() {
	mirror.source = source;

	((GLSLShaderProgramRetained)mirror).shaders = new ShaderRetained[this.shaders.length];
	// Copy vertex and fragment shader
	for (int i = 0; i < this.shaders.length; i++) {
	    ((GLSLShaderProgramRetained)mirror).shaders[i] = (ShaderRetained)this.shaders[i].mirror;
	}
	((GLSLShaderProgramRetained)mirror).shaderProgramIds = null;
	((GLSLShaderProgramRetained)mirror).resourceCreationMask = 0x0;
    }

    /**
     * Update the "component" field of the mirror object with the 
     *  given "value"
     */
    synchronized void updateMirrorObject(int component, Object value) {

	// System.out.println("GLSLShaderProgramRetained : updateMirrorObject");

	GLSLShaderProgramRetained mirrorGLSLSp = (GLSLShaderProgramRetained)mirror;

	if ((component & SHADER_PROGRAM_CREATE) != 0) {
	    // Note: update from the mirror object only
	    mirrorGLSLSp.resourceCreationMask = 0x0;
	    mirrorGLSLSp.shaderProgramIds = null;
	}
    }

    private native void setUniform1i(long ctx,
				     int shaderProgram,
				     String attrName,
				     int value);
    private native void setUniform1f(long ctx,
				     int shaderProgram,
				     String attrName,
				     float value);
    private native void setUniform2i(long ctx,
				     int shaderProgram,
				     String attrName,
				     int[] value);
    private native void setUniform2f(long ctx,
				     int shaderProgram,
				     String attrName,
				     float[] value);
    private native void setUniform3i(long ctx,
				     int shaderProgram,
				     String attrName,
				     int[] value);
    private native void setUniform3f(long ctx,
				     int shaderProgram,
				     String attrName,
				     float[] value);
    private native void setUniform4i(long ctx,
				     int shaderProgram,
				     String attrName,
				     int[] value);
    private native void setUniform4f(long ctx,
				     int shaderProgram,
				     String attrName,
				     float[] value);
    private native void setUniformMatrix3f(long ctx,
					   int shaderProgram,
					   String attrName,
					   float[] value);
    private native void setUniformMatrix4f(long ctx,
					   int shaderProgram,
					   String attrName,
					   float[] value);

    /* New native interfaces */
    private native ShaderError createShader(long ctx, int shaderType, long[] shaderId);
    private native ShaderError destroyShader(long ctx, long shaderId);
    private native ShaderError compileShader(long ctx, long shaderId, String program);

    private native ShaderError createShaderProgram(long ctx, long[] shaderProgramId);
    private native ShaderError destroyShaderProgram(long ctx, long shaderProgramId);
    private native ShaderError linkShaderProgram(long ctx, long shaderProgramId,
						 long[] shaderId);
    private native ShaderError useShaderProgram(long ctx, long shaderProgramId);
 
    /*
    private native ShaderError createUniformLocation(long ctx,
						     long shaderProgramId,
						     String attrName,
						     long[] uniformLocation);

    private native ShaderError setUniform1i(long ctx,
					    long shaderProgramId,
					    long uniformLocation,
					    int value);

    private native ShaderError setUniform3f(long ctx,
					    long shaderProgramId,
					    long uniformLocation,
					    float[] value);    
    */

    /**
     * Method to create the native shader.
     */
    ShaderError createShader(long ctx, ShaderRetained shader, long[] shaderIdArr) {	
	  return  createShader(ctx, shader.shaderType, shaderIdArr);
    }
    
    /**
     * Method to destroy the native shader.
     */
    ShaderError destroyShader(long ctx, int cvRdrIndex, ShaderRetained shader) {
	System.out.println("GLSLShaderProgram : destroyShader not implemented yet!");
	return null;
    }
    
    /**
     * Method to compile the native shader.
     */
    ShaderError compileShader(long ctx, int cvRdrIndex, ShaderRetained shader) {

        return compileShader(ctx, shader.shaderIds[cvRdrIndex],
                ((SourceCodeShaderRetained)shader).getShaderSource());

    }

    /**
     * Method to create the native shader program.
     */
    ShaderError createShaderProgram(long ctx, int cvRdrIndex, long[] shaderProgramIdArr) {

	    return createShaderProgram(ctx, shaderProgramIdArr);  

    }

    /**
     * Method to destroy the native shader program.
     */
    ShaderError destroyShaderProgram(long ctx, int cvRdrIndex) {
	System.out.println("GLSLShaderProgram : destroyShaderProgram not implemented yet!");
	return null;
    }

    /**
     * Method to link the native shader program.
     */
    ShaderError linkShaderProgram(long ctx, int cvRdrIndex, long[] shaderIds) {
	   return linkShaderProgram(ctx, shaderProgramIds[cvRdrIndex], shaderIds);
    }
 

    /**
     * Method to link the native shader program.
     */
    ShaderError enableShaderProgram(Canvas3D cv, int cvRdrIndex) {

	synchronized(resourceLock) {
	    if(cvRdrIndex < 0) {
		// System.out.println("GLSLShaderProgramRetained.useShaderProgram[ 0 ]");
	
		// disable shading by binding to program 0
		useShaderProgram(cv.ctx, 0);
	    }
	    else {
		//System.out.println("GLSLShaderProgramRetained.useShaderProgram[ " +
		//		   shaderProgramIds[cvRdrIndex]+ " ]");
		useShaderProgram(cv.ctx, shaderProgramIds[cvRdrIndex]);
	    }
	}

	// Need to handle the returned ShaderError.
	return null;
    }
	

    void disableShaderProgram(Canvas3D cv) {
	// System.out.println("GLSLShaderProgramRetained.disableShaderProgram ...");
	useShaderProgram(cv.ctx, -1);
    }


    /**
     * Update native value for ShaderAttributeValue class
     */
    void setUniformAttrValue(long ctx, ShaderAttributeValue sav) {
	switch (sav.classType) {
	case ShaderAttributeObject.TYPE_INTEGER:
	    setUniform1i(ctx,
			 shaderProgramId,
			 sav.getAttributeName(),
			 ((int[])sav.attrWrapper.getRef())[0]);
	    break;

	case ShaderAttributeObject.TYPE_FLOAT:
	    setUniform1f(ctx,
			 shaderProgramId,
			 sav.getAttributeName(),
			 ((float[])sav.attrWrapper.getRef())[0]);
	    break;

	case ShaderAttributeObject.TYPE_DOUBLE:
	    throw new RuntimeException("not implemented");

	case ShaderAttributeObject.TYPE_TUPLE2I:
	    setUniform2i(ctx,
			 shaderProgramId,
			 sav.getAttributeName(),
			 (int[])sav.attrWrapper.getRef());
	    break;

	case ShaderAttributeObject.TYPE_TUPLE2F:
	    setUniform2f(ctx,
			 shaderProgramId,
			 sav.getAttributeName(),
			 (float[])sav.attrWrapper.getRef());
	    break;

	case ShaderAttributeObject.TYPE_TUPLE2D:
	    throw new RuntimeException("not implemented");

	case ShaderAttributeObject.TYPE_TUPLE3I:
	    setUniform3i(ctx,
			 shaderProgramId,
			 sav.getAttributeName(),
			 (int[])sav.attrWrapper.getRef());
	    break;

	case ShaderAttributeObject.TYPE_TUPLE3F:
	    setUniform3f(ctx,
			 shaderProgramId,
			 sav.getAttributeName(),
			 (float[])sav.attrWrapper.getRef());
	    break;

	case ShaderAttributeObject.TYPE_TUPLE3D:
	    throw new RuntimeException("not implemented");

	case ShaderAttributeObject.TYPE_TUPLE4I:
	    setUniform4i(ctx,
			 shaderProgramId,
			 sav.getAttributeName(),
			 (int[])sav.attrWrapper.getRef());
	    break;

	case ShaderAttributeObject.TYPE_TUPLE4F:
	    setUniform4f(ctx,
			 shaderProgramId,
			 sav.getAttributeName(),
			 (float[])sav.attrWrapper.getRef());
	    break;

	case ShaderAttributeObject.TYPE_TUPLE4D:
	    throw new RuntimeException("not implemented");

	case ShaderAttributeObject.TYPE_MATRIX3F:
	    throw new RuntimeException("not implemented");
	    /*
	    setUniformMatrix3f(ctx,
			 shaderProgramId,
			       sav.getAttributeName(),
			       (float[])sav.attrWrapper.getRef());
	    break;
	    */
	case ShaderAttributeObject.TYPE_MATRIX3D:
	    throw new RuntimeException("not implemented");
	case ShaderAttributeObject.TYPE_MATRIX4F:
	    throw new RuntimeException("not implemented");
	    /*
	    setUniformMatrix4f(ctx,
			 shaderProgramId,
			       sav.getAttributeName(),
			       (float[])sav.attrWrapper.getRef());
	    break;
	    */
	case ShaderAttributeObject.TYPE_MATRIX4D:
	    throw new RuntimeException("not implemented");

	default:
	    // Should never get here
	    assert(false);
	    return;
	}
    }


}
