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
 
    protected ShaderProgramRetained shaderProgram = null;
    protected ShaderAttributeSetRetained shaderAttributeSet = null;

    static final int SHADER_PROGRAM = 0x0800;
    static final int SHADER_ATTRIBUTE_SET = 0x1000;    

    /**
     * Set the shader program object to the specified object.
     * @param shaderProgram object that specifies the desired shader program
     * and shader program attributes.
     */
    void setShaderProgram(ShaderProgram sp) {

	synchronized(liveStateLock) {
	    if (source.isLive()) {

		if (this.shaderProgram != null) {
		    this.shaderProgram.clearLive(refCount);
		    this.shaderProgram.removeMirrorUsers(this);
		}

		if (sp != null) {
		    ((ShaderProgramRetained)sp.retained).setLive(inBackgroundGroup, 
								 refCount);
		    ((ShaderProgramRetained)sp.retained).copyMirrorUsers(this);
	    	}
		
		// TODO : Need to implement RenderBin side of code.
		System.out.print("**** ShaderAppearceRetained.setShaderProgram()  more work needed!");
		sendMessage(SHADER_PROGRAM,  
			    (sp != null ? ((ShaderProgramRetained)sp.retained).mirror : null), 
			    true);
	       
	    }

	    if (sp == null) {
		this.shaderProgram = null;
	    } else {
		this.shaderProgram = (ShaderProgramRetained)sp.retained;
	    }
	}
    }


    /**
     * Retrieves the current shader program object.
     * @return current shader program object
     */
    ShaderProgram getShaderProgram() {
	return (shaderProgram == null ? null : (ShaderProgram)shaderProgram.source);	
    }


    /**
     * Sets the ShaderAttributeSet object to the specified object.  Setting it to
     * null is equivalent to specifying an empty set of attributes.
     *
     * @param shaderAttributeSet object that specifies the desired shader attributes
     */
    void setShaderAttributeSet(ShaderAttributeSet sas) {
	//TODO : Mirror object --- Chien
	if (sas == null) {
	    this.shaderAttributeSet = null;
	} else {
	    this.shaderAttributeSet = (ShaderAttributeSetRetained)sas.retained;
	}
    }


    /**
     * Retrieves the current ShaderAttributeSet object.
     * @return current ShaderAttributeSet object
     */
    ShaderAttributeSet getShaderAttributeSet() {
	return (shaderAttributeSet == null ? null : (ShaderAttributeSet)shaderAttributeSet.source);	

    }

    /* TODO : Need to expand from AppearanceRetained 

    public boolean equals(Object obj) {
	return ((obj instanceof AppearanceRetained) &&
		equals((AppearanceRetained) obj));
    }

    boolean equals(AppearanceRetained app) {
        boolean flag;

    }

    */

    synchronized void createMirrorObject() {
	// System.out.println("ShaderAppearanceRetained : createMirrorObject()");

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
	// System.out.println("ShaderAppearanceRetained : initMirrorObject()");

	super.initMirrorObject();

	ShaderAppearanceRetained mirrorApp = (ShaderAppearanceRetained)mirror;

	mirrorApp.shaderProgram = (ShaderProgramRetained)shaderProgram.mirror;

	if(shaderAttributeSet != null) {
	    mirrorApp.shaderAttributeSet = 
		(ShaderAttributeSetRetained)shaderAttributeSet.mirror;
	}
	else {
	    // System.out.println("shaderAttributeSet is null");
	    mirrorApp.shaderAttributeSet = null;
	}

    }

  /**
   * Update the "component" field of the mirror object with the
   *  given "value"
   */
    synchronized void updateMirrorObject(int component, Object value) {

	// System.out.println("ShaderAppearanceRetained : updateMirrorObject()");
	super.updateMirrorObject(component, value);
 	ShaderAppearanceRetained mirrorApp = (ShaderAppearanceRetained)mirror;
	if ((component & SHADER_PROGRAM) != 0) {
	    mirrorApp.shaderProgram = (ShaderProgramRetained)value;
	}
	else if ((component & SHADER_ATTRIBUTE_SET) != 0) {
	    mirrorApp.shaderAttributeSet = (ShaderAttributeSetRetained)value;
	}
	
    }

    /**
     * This method calls the setLive method of all appearance bundle
     * objects.
     */
    void doSetLive(boolean backgroundGroup, int refCount) {
	// System.out.println("ShaderAppearceRetained.doSetLive()");


	if (shaderProgram != null) {
	    shaderProgram.setLive(backgroundGroup, refCount);
	}

	if (shaderAttributeSet != null) {
	    shaderAttributeSet.setLive(backgroundGroup, refCount);
	}


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

	if (shaderProgram != null) {
	    shaderProgram.clearLive(refCount);
	}

	if (shaderAttributeSet != null) {
	    shaderAttributeSet.clearLive(refCount);
	}
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


