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
import javax.vecmath.*;
import java.util.ArrayList;

/**
 * The LinearFog leaf node defines distance parameters for
 * linear fog.
 */
class LinearFogRetained extends FogRetained {
    /**
     * Fog front and back   distance
     */
    double frontDistance = 0.1;
    double backDistance = 1.0;
    double localToVworldScale = 1.0;
    double frontDistanceInEc;
    double backDistanceInEc;
    double vworldToCoexistenceScale;

    // dirty bits for LinearFog
    static final int FRONT_DISTANCE_CHANGED	= FogRetained.LAST_DEFINED_BIT << 1;
    static final int BACK_DISTANCE_CHANGED	= FogRetained.LAST_DEFINED_BIT << 2;

    LinearFogRetained() {
        this.nodeType = NodeRetained.LINEARFOG;
    }

    /**
     * Initializes front  distance for fog before the object is live
     */
    void initFrontDistance(double frontDistance){
	this.frontDistance = frontDistance;
    }

    /**
     * Sets front   distance for fog and sends a message
     */
    void setFrontDistance(double frontDistance){
	this.frontDistance = frontDistance;
	J3dMessage createMessage = VirtualUniverse.mc.getMessage();
	createMessage.threads = targetThreads;
	createMessage.type = J3dMessage.FOG_CHANGED;
	createMessage.universe = universe;
	createMessage.args[0] = this;
	createMessage.args[1]= new Integer(FRONT_DISTANCE_CHANGED);
	createMessage.args[2] = new Double(frontDistance);
	VirtualUniverse.mc.processMessage(createMessage);

    }

    /**
     * Gets front   distance for fog
     */
    double getFrontDistance(){
	return this.frontDistance;
    }

    /**
     * Initializes back   distance for fog
     */
    void initBackDistance(double backDistance){
	this.backDistance = backDistance;
    }
    /**
     * Sets back  distance for fog
     */
    void setBackDistance(double backDistance){
	this.backDistance = backDistance;
	J3dMessage createMessage = VirtualUniverse.mc.getMessage();
	createMessage.threads = targetThreads;
	createMessage.type = J3dMessage.FOG_CHANGED;
	createMessage.universe = universe;
	createMessage.args[0] = this;
	createMessage.args[1]= new Integer(BACK_DISTANCE_CHANGED);
	createMessage.args[2] = new Double(backDistance);
	VirtualUniverse.mc.processMessage(createMessage);
    }

    /**
     * Gets back   distance for fog
     */
    double getBackDistance(){
	return this.backDistance;
    }
    /** 
     * This method and its native counterpart update the native context
     * fog values.
     */
    native void update(long ctx, float red, float green, float blue,
		       double fdist, double bdist);

    void update(long ctx, double scale) {
	validateDistancesInEc(scale);
	update(ctx, color.x, color.y, color.z, frontDistanceInEc, backDistanceInEc);
    }

    

    void setLive(SetLiveState s) {
	GroupRetained group;
	
	super.setLive(s);

	// Initialize the mirror object, this needs to be done, when
	// renderBin is not accessing any of the fields
	J3dMessage createMessage = VirtualUniverse.mc.getMessage();
	createMessage.threads = J3dThread.UPDATE_RENDERING_ENVIRONMENT;
	createMessage.universe = universe;
	createMessage.type = J3dMessage.FOG_CHANGED;
	createMessage.args[0] = this;
	// a snapshot of all attributes that needs to be initialized
	// in the mirror object
	createMessage.args[1]= new Integer(INIT_MIRROR);
	ArrayList addScopeList = new ArrayList();
	for (int i = 0; i < scopes.size(); i++) {
	    group = (GroupRetained)scopes.get(i);
	    tempKey.reset();
	    group.addAllNodesForScopedFog(mirrorFog, addScopeList, tempKey);
	}
	Object[] scopeInfo = new Object[2];
	scopeInfo[0] = ((scopes.size() > 0) ? Boolean.TRUE:Boolean.FALSE);
	scopeInfo[1] = addScopeList;	
	createMessage.args[2] = scopeInfo;
	Color3f clr = new Color3f(color);
	createMessage.args[3] = clr;

	Object[] obj = new Object[6];
	obj[0] = boundingLeaf;
	obj[1] = (regionOfInfluence != null?regionOfInfluence.clone():null);
	obj[2] = (inBackgroundGroup? Boolean.TRUE:Boolean.FALSE);
	obj[3] = geometryBackground;
	obj[4] = new Double(frontDistance);
	obj[5] = new Double(backDistance);
	
	createMessage.args[4] = obj;
	VirtualUniverse.mc.processMessage(createMessage);

    }
	

    // The update Object function.
    // Note : if you add any more fields here , you need to update
    // updateFog() in RenderingEnvironmentStructure
    synchronized void updateMirrorObject(Object[] objs) {

	int component = ((Integer)objs[1]).intValue();
	Transform3D trans;

	if ((component & FRONT_DISTANCE_CHANGED) != 0) 
	    ((LinearFogRetained)mirrorFog).frontDistance = ((Double)objs[2]).doubleValue();
	if ((component & BACK_DISTANCE_CHANGED) != 0) 
	    ((LinearFogRetained)mirrorFog).backDistance = ((Double)objs[2]).doubleValue();
	if ((component & INIT_MIRROR) != 0) {
	    ((LinearFogRetained)mirrorFog).frontDistance = ((Double)((Object[])objs[4])[4]).doubleValue();
	    ((LinearFogRetained)mirrorFog).backDistance = ((Double)((Object[])objs[4])[5]).doubleValue();
	    
	}
	((LinearFogRetained)mirrorFog).localToVworldScale = 
	    getLastLocalToVworld().getDistanceScale();	


	super.updateMirrorObject(objs);
    }

    /**
     * Scale distances from local to eye coordinate
     */  

    void validateDistancesInEc(double vworldToCoexistenceScale) {
        // vworldToCoexistenceScale can be used here since
        // CoexistenceToEc has a unit scale
        double localToEcScale = localToVworldScale * vworldToCoexistenceScale;

        frontDistanceInEc = frontDistance * localToEcScale;
        backDistanceInEc = backDistance * localToEcScale;
    }

    // Called on mirror object
    void updateTransformChange() {
	super.updateTransformChange();
        localToVworldScale = sgFog.getLastLocalToVworld().getDistanceScale();
    }
}