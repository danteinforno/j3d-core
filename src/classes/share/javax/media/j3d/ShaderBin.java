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

import javax.vecmath.*;
import java.util.ArrayList;


// TODO : We should have a common Bin object that all other Bins extend from. 


//class ShaderBin extends Object implements ObjectUpdate, NodeComponentUpdate {
class ShaderBin implements ObjectUpdate {

    /**
     * The RenderBin for this object
     */
    RenderBin renderBin = null;

    /**
     * The AttributeBin that this ShaderBin resides
     */
    AttributeBin attributeBin = null;

    /**
     * The references to the next and previous ShaderBins in the
     * list.
     */
    ShaderBin next = null;
    ShaderBin prev = null;

    /**
     * The list of TextureBins in this ShaderBin
     */
    TextureBin textureBinList = null;

    /**
     * The list of TextureBins to be added for the next frame
     */
    ArrayList addTextureBins = new ArrayList();

    boolean onUpdateList = false;

    int numEditingTextureBins = 0;

    // Should this  be a separate mirror object of ShaderProgram ? 

    // ShaderAppearanceRetained shaderAppearance = null;
    // ShaderProgramRetained shaderProgram = null;
    // ShaderAttributeSetRetained shaderAttributeSet = null;
    ShaderProgram shaderProgram = null;
    ShaderAttributeSet shaderAttributeSet = null;
    
    // ShaderBin(ShaderProgramRetained sp,  RenderBin rBin) {
    ShaderBin(ShaderAppearanceRetained sApp,  RenderBin rBin) {
	reset(sApp, rBin);
    }
    
    void reset(ShaderAppearanceRetained sApp, RenderBin rBin) {
	prev = null;
	next = null;
        renderBin = rBin;
	attributeBin = null;
	textureBinList = null;
	onUpdateList = false;
	numEditingTextureBins = 0;
	addTextureBins.clear();
	if(sApp != null) {
	    shaderProgram = sApp.shaderProgram;
	    shaderAttributeSet = sApp.shaderAttributeSet; 
	}
	else {
	    shaderProgram = null;
	    shaderAttributeSet = null;
	}
    }
    
    void clear() {
	reset(null, null);
    }
    
    /**
     * This tests if the qiven ra.shaderProgram  match this shaderProgram
     */
    boolean equals(ShaderAppearanceRetained sApp) {
	
	// ShaderProgramRetained sp;
	ShaderProgram sp;
	ShaderAttributeSet ss;
	
	if (sApp == null) {
	    sp = null;
	    ss = null;
	} else {
	    sp = sApp.shaderProgram;
	    ss = sApp.shaderAttributeSet;
	}
	
	if((shaderProgram != sp) || (shaderAttributeSet != ss)) {
	    return false;
	}
	
	return true;
    }

    public void updateObject() {
	TextureBin t;
	int i;
	
	if (addTextureBins.size() > 0) {
	    t = (TextureBin)addTextureBins.get(0);
	    if (textureBinList == null) {
		textureBinList = t;

	    }
	    else {
		// Look for a TextureBin that has the same texture
		insertTextureBin(t);	
	    }	    
	    for (i = 1; i < addTextureBins.size() ; i++) {
		t = (TextureBin)addTextureBins.get(i);
		// Look for a TextureBin that has the same texture
		insertTextureBin(t);

	    }
	}
	addTextureBins.clear();
	onUpdateList = false;

    }
    
    void insertTextureBin(TextureBin t) {
	TextureBin tb;
	int i;
	TextureRetained texture = null;

	if (t.texUnitState != null && t.texUnitState.length > 0) {
	    if (t.texUnitState[0] != null) {
	        texture = t.texUnitState[0].texture;
	    }
	}

	// use the texture in the first texture unit as the sorting criteria
	if (texture != null) {
	    tb = textureBinList; 
	    while (tb != null) { 
		if (tb.texUnitState == null || tb.texUnitState[0] == null ||
			tb.texUnitState[0].texture != texture) {
		    tb = tb.next;
		} else {
		    // put it here  
		    t.next = tb; 
		    t.prev = tb.prev; 
		    if (tb.prev == null) { 
		        textureBinList = t; 
		    } 
		    else { 
		        tb.prev.next = t; 
		    } 
		    tb.prev = t; 
		    return; 
	        } 
	    }
	} 
	// Just put it up front
	t.prev = null;
	t.next = textureBinList;
	textureBinList.prev = t;
	textureBinList = t;

	t.tbFlag &= ~TextureBin.RESORT;
    }


    /**
     * reInsert textureBin if the first texture is different from
     * the previous bin and different from the next bin
     */
    void reInsertTextureBin(TextureBin tb) {

        TextureRetained texture = null,
                        prevTexture = null,
                        nextTexture = null;

        if (tb.texUnitState != null && tb.texUnitState[0] != null) {
            texture = tb.texUnitState[0].texture;
        }

        if (tb.prev != null && tb.prev.texUnitState != null) {
            prevTexture = tb.prev.texUnitState[0].texture;
        }

        if (texture != prevTexture) {
            if (tb.next != null && tb.next.texUnitState != null) {
                nextTexture = tb.next.texUnitState[0].texture;
            }
            if (texture != nextTexture) {
                if (tb.prev != null && tb.next != null) {
                    tb.prev.next = tb.next;
		    tb.next.prev = tb.prev;
                    insertTextureBin(tb);
                }
            }
        }
    }



    /**
     * Adds the given TextureBin to this AttributeBin.
     */
    void addTextureBin(TextureBin t, RenderBin rb, RenderAtom ra) {
	 
	t.environmentSet = this.attributeBin.environmentSet;
	t.attributeBin = this.attributeBin;
	t.shaderBin = this;

	attributeBin.updateFromShaderBin(ra);
	addTextureBins.add(t);

	if (!onUpdateList) {
	    rb.objUpdateList.add(this);
	    onUpdateList = true;
	}
    }

    /**
     * Removes the given TextureBin from this ShaderBin.
     */
    void removeTextureBin(TextureBin t) {
	
	// If the TextureBin being remove is contained in addTextureBins, 
	// then remove the TextureBin from the addList
	if (addTextureBins.contains(t)) {
	    addTextureBins.remove(addTextureBins.indexOf(t));
	}
	else {
	    if (t.prev == null) { // At the head of the list
		textureBinList = t.next;
		if (t.next != null) {
		    t.next.prev = null;
		}
	    } else { // In the middle or at the end.
		t.prev.next = t.next;
		if (t.next != null) {
		    t.next.prev = t.prev;
		}
	    }
	}

	t.shaderBin = null;
	t.prev = null;
	t.next = null;

	t.clear();

	renderBin.textureBinFreelist.add(t);

	if (textureBinList == null && addTextureBins.size() == 0 ) {
	    // Note: Removal of this shaderBin as a user of the rendering
	    // atttrs is done during removeRenderAtom() in RenderMolecule.java
	    attributeBin.removeShaderBin(this);
	}
    }

    /**
     * Renders this ShaderBin
     */
    void render(Canvas3D cv) {

	TextureBin tb;	        	

        // include this ShaderBin to the to-be-updated list in canvas
        cv.setStateToUpdate(Canvas3D.SHADERBIN_BIT, this);

	tb = textureBinList;
	while (tb != null) {
	    tb.render(cv);
	    tb = tb.next;
	}
    }


    void updateAttributes(Canvas3D cv) {

	//System.out.println("ShaderBin.updateAttributes() not implemented yet.");
	 
	if (shaderProgram != null) {
	    shaderProgram.updateNative(cv.ctx);

	    if (shaderAttributeSet != null) {
		shaderAttributeSet.updateNative(cv.ctx, shaderProgram);
	    }
	}


	/*


	// KCR hack ....

	if (shaderProgram != null) {
	    // Update the native shader program attributes. Note that
	    // the current hack only works when the sole user
	    // optimization is in effect. The appearance with the
	    // Shader Program must have a frequently-writable texture
	    // and it must be the sole user of this texture bin.
	    shaderProgram.updateNative(cv.ctx);
	    
	    ShaderAttributeSet shaderAttributeSet =
		((ShaderAppearanceRetained)app).shaderAttributeSet;
	    
	    if (shaderAttributeSet != null) {
		shaderAttributeSet.updateNative(cv.ctx, shaderProgram);
	    }
	    
	}
	else {
	    // Hack to disable shaders
	    if (lastShaderProgram != null) {
		lastShaderProgram.disableNative(cv.ctx);
	    }
	}

	*/


	/* NOT TESTED YET --- Chien.
	if ((cv.canvasDirty & Canvas3D.SHADERBIN_DIRTY) != 0) {

	    // Update Shader Bundles
	    shaderProgram.updateNative(cv.ctx);
	    cv.shaderProgram = shaderProgram;
	}
	else if (cv.shaderProgram != shaderProgram && 
		 cv.shaderBin != this) {
	    // Update Shader Bundles
	    if (shaderProgram == null) {
		// Reset
	    } else {
		shaderProgram.updateNative(cv.ctx);
	    }
	    cv.shaderProgram = shaderProgram;
	} 
	cv.ShaderBin = this;
	cv.canvasDirty &= ~Canvas3D.SHARDERBIN_DIRTY;

	*/







    }

    void updateNodeComponent() {
	System.out.println("ShaderBin.updateNodeComponent() not implemented yet.");
    }

    void incrActiveTextureBin() {
	numEditingTextureBins++;
    }

    void decrActiveTextureBin() {
	numEditingTextureBins--;
    }
}
