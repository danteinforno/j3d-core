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

import java.util.Vector;
import java.util.BitSet;
import java.util.ArrayList;


/**
 * The Appearance object defines all rendering state that can be set
 * as a component object of a Shape3D node.
 */
class ShaderAppearanceRetained extends AppearanceRetained {

    //
    // State variables: these should all be initialized to approproate
    // Java 3D defaults.
    //

    /* KCR: BEGIN CG SHADER HACK */
    // TODO: Change this to ShaderProgramRetained shaderProgram ...
    // Shader program  object
    ShaderProgram shaderProgram = null;
    /* KCR: END CG SHADER HACK */

    /*
// TODO: IMPLEMENT THIS
    // Cache used during compilation.  If map == compState, then
    // mapAppearance can be used for this appearance
    CompileState map = null;
    AppearanceRetained mapAppearance = null;

// TODO: FIGURE OUT HOW TO CLEANLY ADD IN SHADER_PROGRAM to components mask

    static final int MATERIAL           = 0x0001;
    static final int TEXTURE            = 0x0002;
    static final int TEXCOORD_GEN       = 0x0004;
    static final int TEXTURE_ATTR       = 0x0008;
    static final int COLOR              = 0x0010;
    static final int TRANSPARENCY       = 0x0020;
    static final int RENDERING          = 0x0040;
    static final int POLYGON            = 0x0080;
    static final int LINE               = 0x0100;
    static final int POINT              = 0x0200;
    static final int TEXTURE_UNIT_STATE = 0x0400;

    static final int ALL_COMPONENTS = (MATERIAL|TEXTURE|TEXCOORD_GEN|TEXTURE_ATTR|COLOR|TRANSPARENCY|
				       RENDERING|POLYGON|LINE|POINT|TEXTURE_UNIT_STATE);

    static final int ALL_SOLE_USERS = 0;
    */

    // A pointer to the scene graph appearance object
    ShaderAppearanceRetained sgApp = null;


    /**
     * Set the shader program object to the specified object.
     * @param shaderProgram object that specifies the desired shader program
     */
    void setShaderProgram(ShaderProgram shaderProgram) {
	/* KCR: BEGIN CG SHADER HACK */
	// TODO: implement this for real once we have a ShaderProgramRetained object
	this.shaderProgram = shaderProgram;
	/* KCR: END CG SHADER HACK */
    }


    /**
     * Retrieves the current shader program object.
     * @return current shader program object
     */
    ShaderProgram getShaderProgram() {
	/* KCR: BEGIN CG SHADER HACK */
	// TODO: implement this for real once we have a ShaderProgramRetained object
	return this.shaderProgram;
	/* KCR: END CG SHADER HACK */
    }


    synchronized void createMirrorObject() {
	if (mirror == null) {
	    // we can't check isStatic() since it sub-NodeComponent
	    // create a new one, we should create a
	    // new AppearanceRetained() even though isStatic() = true.
	    // For simplicity, always create a retained side.
	    mirror = new ShaderAppearanceRetained();
	}
	initMirrorObject();
    }

    /**
     * This routine updates the mirror appearance for this appearance.
     * It also calls the update method for each node component if it
     * is not null.
     */
    synchronized void initMirrorObject() {

	super.initMirrorObject();

	ShaderAppearanceRetained mirrorApp = (ShaderAppearanceRetained)mirror;

	/* KCR: BEGIN CG SHADER HACK */
	mirrorApp.shaderProgram = shaderProgram;
	/* KCR: END CG SHADER HACK */
    }

  /**
   * Update the "component" field of the mirror object with the
   *  given "value"
   */
    synchronized void updateMirrorObject(int component, Object value) {
	super.updateMirrorObject(component, value);

	/*
// TODO: IMPLEMENT THIS
	ShaderAppearanceRetained mirrorApp = (ShaderAppearanceRetained)mirror;
	if ((component & SHADER_PROGRAM) != 0) {
	    mirrorApp.shaderProgram = (ShaderProgramRetained)value;
	}
	else if ((component & SHADER_PARAMETERS) != 0) {
	    mirrorApp.shaderProgram = (ShaderParametersRetained)value;
	}
	*/
    }


    void setLive(boolean backgroundGroup, int refCount) {
	doSetLive(backgroundGroup, refCount);
	markAsLive();
    }

    /**
     * This method calls the setLive method of all appearance bundle
     * objects.
     */
    void doSetLive(boolean backgroundGroup, int refCount) {

	/*
// TODO: IMPLEMENT THIS
	if (shaderProgram != null) {
	    shaderProgram.setLive(backgroundGroup, refCount);
	}

	if (shaderParameters != null) {
	    shaderParameters.setLive(backgroundGroup, refCount);
	}
	*/

	// Increment the reference count and initialize the appearance
	// mirror object
        super.doSetLive(backgroundGroup, refCount);
    }


    /**
     * This clearLive routine first calls the superclass's method, then
     * it removes itself to the list of lights
     */
    void clearLive(int refCount) {
	super.clearLive(refCount);

	/*
// TODO: IMPLEMENT THIS
	if (shaderParameters != null) {
	    shaderParameters.clearLive(refCount);
	}

	if (shaderProgram != null) {
	    shaderProgram.clearLive(refCount);
	}
	*/
    }


    boolean isStatic() {
	if (!super.isStatic()) {
	    return false;
	}

	/*
// TODO: IMPLEMENT THIS
	boolean flag =
	    source.capabilityBitsEmpty() &&
	    ((shaderProgram == null) ||
	     shaderProgram.source.capabilityBitsEmpty()) &&
	    ((shaderParameters == null) ||
	     shaderParameters.source.capabilityBitsEmpty());

	return flag;
	*/

	return false;
    }


    /*
// TODO: IMPLEMENT THIS
    // TODO: How do we determine whether a ShaderAppearance is opaque???
    boolean isOpaque(int geoType) {
    }
    */

    void handleFrequencyChange(int bit) {
	super.handleFrequencyChange(bit);

	int mask = 0;
	/*
// TODO: IMPLEMENT THIS
	if (bit == ShaderAppearance.ALLOW_SHADER_PROGRAM_WRITE)
	    mask = SHADER_PROGRAM;
	else if (bit == ShaderAppearance.ALLOW_SHADER_PARAMETERS_WRITE)
	    mask = SHADER_PARAMETERS;
	*/

	if (mask != 0)
	    setFrequencyChangeMask(bit, mask);
    }
}


