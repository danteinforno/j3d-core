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

/**
 * ViewPlatform object (retained side)
 */

class ViewPlatformRetained extends LeafRetained {

    // different types of IndexedUnorderedSet that use in BehaviorStructure
    static final int VP_IN_BS_LIST = 0;

    // total number of different IndexedUnorderedSet types
    static final int TOTAL_INDEXED_UNORDER_SET_TYPES = 1;

    /**
     * This variable specifies the policy Java 3D will use in placing the
     * user's eye point as a function of head position.  The variable can
     * contain one of NOMINAL_SCREEN, NOMINAL_HEAD, or NOMINAL_FEET.
     */
    int viewAttachPolicy = View.NOMINAL_HEAD;

    /**
     * The list of views associated with this view platform.
     * Use getViewList() to access this variable.
     */
    private ArrayList viewList = new ArrayList();

    /**
     * Cached list of viewList for synchronization
     */
    private View views[] = null;

    // The locale that this node is decended from
    Locale locale = null;

    // dirty flag for viewList
    boolean viewListDirty = true;

    /** 
     * The current cached view platform transform (vworldToVpc) and
     * its inverse (vpcToVworld).
     */ 
    Transform3D vworldToVpc = null;
    Transform3D vpcToVworld = new Transform3D();


    /**
     * The activation radius.  The value is chosen so that when using a
     * default screen scale and field of view, the entire view frustum
     * is enclosed.
     */

    /**
     * Position used for placing this view platform.
     */
    BoundingSphere sphere =
	new BoundingSphere(new Point3d(0.0,0.0,0.0), 62.0f);

    /**
     * This is the cached bounding sphere used for the activation volume.
     */
    BoundingSphere schedSphere;
    Point3d center = new Point3d();
    final static Point3d zeroPoint = new Point3d();

    // Mask that indicates this ViewPlatformRetained's view dependence info. has changed,
    // and CanvasViewCache may need to recompute the final view matries.
    int vprDirtyMask = (View.VPR_VIEW_ATTACH_POLICY_DIRTY
			| View.VPR_VIEWPLATFORM_DIRTY);

    static final Object emptyObj[] = new Object[0];
    static final Transform3D identity = new Transform3D();

    ViewPlatformRetained() {
        this.nodeType = NodeRetained.VIEWPLATFORM;
	localBounds = new BoundingBox();
	((BoundingBox)localBounds).setLower( 1.0, 1.0, 1.0);
	((BoundingBox)localBounds).setUpper(-1.0,-1.0,-1.0);
	IndexedUnorderSet.init(this, TOTAL_INDEXED_UNORDER_SET_TYPES);
	schedSphere = (BoundingSphere) sphere.clone();
    }

    /**
     * Sets the coexistence center in virtual world policy.
     * This setting determines how Java 3D places the
     * user's eye point as a function of head position.  The variable can
     * contain one of NOMINAL_SCREEN, NOMINAL_HEAD, or NOMINAL_FEET.
     * @param policy the new policy, one of NOMINAL_SCREEN, NOMINAL_HEAD, 
     * or NOMINAL_FEET
     */
    void setViewAttachPolicy(int policy) {
	synchronized(this) {
	    this.viewAttachPolicy = policy;
	    vprDirtyMask |= View.VPR_VIEW_ATTACH_POLICY_DIRTY;
	}
	
	if (source != null && source.isLive()) {
	    repaint();
	}
    }


    void repaint() {
	View views[] = getViewList();
	for (int i=views.length-1; i >=0; i--) {
	    views[i].repaint();
	}
    }

    /**
     * Returns the current coexistence center in virtual-world policy.
     * @return one of: NOMINAL_SCREEN, NOMINAL_HEAD, or NOMINAL_FEET
     */
    int getViewAttachPolicy() {
	return this.viewAttachPolicy;
    }

    /**
     * Set the ViewPlatform's activation radius
     */
    void setActivationRadius(float activationRadius) {
	sphere.setRadius(activationRadius);

	if (source != null && source.isLive()) {
	    repaint();
	}
	// Notify behavior scheduler & RenderBin
	if (source.isLive()) {
	    J3dMessage message = VirtualUniverse.mc.getMessage();
	    message.type = J3dMessage.UPDATE_VIEWPLATFORM;
	    message.threads = J3dThread.UPDATE_RENDER|J3dThread.UPDATE_BEHAVIOR;
	    message.universe = universe;
	    message.args[0] = this;
	    message.args[1] = new Float(activationRadius);
	    VirtualUniverse.mc.processMessage(message);
	} else {
	    schedSphere.setRadius(activationRadius);
	}

    }
 
    /**
     * Get the ViewPlatform's activation radius
     */
    float getActivationRadius() {
      return (float) sphere.getRadius();
    }

    /**
     * This sets the view that is associated with this view platform.
     */
    // TODO: This must be changed to a list of views!
    void setView(View v) {
	synchronized (viewList) {
	    if (!viewList.contains(v)) {
		viewList.add(v);
	    }
	    viewListDirty = true;
	}
    }

    void removeView(View v) {
	synchronized (viewList) {
	    if (viewList.contains(v)) {
		viewList.remove(viewList.indexOf(v));
	    }
	    viewListDirty = true;
	}
    }
  
    Transform3D getVworldToVpc() {
	if (vworldToVpc == null) 
	    vworldToVpc = VirtualUniverse.mc.getTransform3D(null);
	vworldToVpc.set(getCurrentLocalToVworld(null));
       	vworldToVpc.invert();
	return vworldToVpc;
    }

    Transform3D getVpcToVworld() {
	vpcToVworld .set(getCurrentLocalToVworld(null));
	return vpcToVworld;
    }


    void evaluateViewPlatformTransform() {
	if (vworldToVpc != null) {
	    FreeListManager.freeObject(FreeListManager.TRANSFORM3D,
				       vworldToVpc);
	} 
	// clear cache so that next time getVworldToVpc() can recompute
	vworldToVpc = null;
    }

    /**
     * Evaluate the view platform transform by traversing *up* the tree from
     * this ViewPlatform node, computing the composite model transform
     * along the way.  Because we are traversing bottom to top, we must
     * multiply each TransformGroup's matrix on the left by the
     * composite transform on the right (rather than the other way
     * around as is usually done).  Once we have the composite model
     * transform for this ViewPlatform--the vpcToVworld transform--we
     * simply invert it to get the vworldToVpc transform.
     */
    void evaluateInitViewPlatformTransform(NodeRetained node, Transform3D trans) {
        if (node instanceof TransformGroupRetained) {
	    Transform3D tmpTrans = new Transform3D();
	    TransformGroupRetained tgr = (TransformGroupRetained)node;
	    tgr.transform.getWithLock(tmpTrans);
	    trans.mul(tmpTrans, trans);
	}

        NodeRetained parent = node.getParent();
        if (parent != null) {
	    // Not at the top yet.
	    evaluateInitViewPlatformTransform(parent, trans);
	}
    }

    void evaluateInitViewPlatformTransform() {

	Transform3D lastLocalToVworld;

	synchronized (this) {
	    lastLocalToVworld = getLastLocalToVworld();

	    if (lastLocalToVworld.equals(identity)) {
		// lastLocalToVworld not yet updated 
		// for Renderer viewCache when startup
		evaluateInitViewPlatformTransform((NodeRetained)this, 
						  lastLocalToVworld);
	    }
	}
    }


    // This is invoke from BehaviorStructure
    void updateActivationRadius(float radius) {
	schedSphere.setCenter(zeroPoint);
	schedSphere.setRadius(radius);
	schedSphere.transform(getCurrentLocalToVworld(null));
    }

    // This is invoke from BehaviorStructure when TransformGroup
    // above this viewplatform changed
    void updateTransformRegion() {
	Transform3D tr = getCurrentLocalToVworld(null);
	schedSphere.setCenter(zeroPoint);
	schedSphere.transform(tr);
	tr.transform(zeroPoint, center);
    }

    /**
     * This setLive routine first calls the superclass's method, then
     * it evaluates the view platform transform, and then it activates
     * all canvases that are associated with the attached view.
     */
    void setLive(SetLiveState s) {
	View views[] = getViewList();

	for (int i = views.length-1; i>=0; i--) {
	    views[i].checkView();
	}
	
        super.doSetLive(s);

        if (inBackgroundGroup) {
            throw new
               IllegalSceneGraphException(J3dI18N.getString("ViewPlatformRetained1"));
        }

	if (inSharedGroup) {
	    throw new
		IllegalSharingException(J3dI18N.getString("ViewPlatformRetained2"));
	}

	if (s.viewLists != null) {
	    throw new
		IllegalSceneGraphException(J3dI18N.getString("ViewPlatformRetained3"));
	}	    
	/*
	if (false) {
	    System.out.println("setLive: vworldToVpc = ");
	    System.out.println(this.vworldToVpc);
	    System.out.println("setLive: vpcToVworld = ");
	    System.out.println(this.vpcToVworld);
	}
	*/
	this.locale = s.locale;
		

	if (s.transformTargets != null && s.transformTargets[0] != null) {
            s.transformTargets[0].addNode(this, Targets.VPF_TARGETS);
	    s.notifyThreads |= J3dThread.UPDATE_TRANSFORM;
	}
        // process switch leaf
        if (s.switchTargets != null &&
                        s.switchTargets[0] != null) {
            s.switchTargets[0].addNode(this, Targets.VPF_TARGETS);
        }
        switchState = (SwitchState)s.switchStates.get(0);
	s.nodeList.add(this);
	s.notifyThreads |= (J3dThread.UPDATE_BEHAVIOR);
	super.markAsLive();
	for (int i = views.length-1; i>=0; i--) {
	    views[i].setUniverse(s.universe);
	    views[i].evaluateActive();
	}

	universe.addViewPlatform(this);
	s.traverseFlags |= NodeRetained.CONTAINS_VIEWPLATFORM;
    }
 
    /**
     * This clearLive routine first calls the superclass's method, then
     * it deactivates all canvases that are associated with the attached
     * view.
     */
    void clearLive(SetLiveState s) {
        super.clearLive(s);
        if (s.switchTargets != null &&
                        s.switchTargets[0] != null) {
            s.switchTargets[0].addNode(this, Targets.VPF_TARGETS);
        }

	View views[] = getViewList();
	for (int i = views.length-1; i>=0; i--) {
	    views[i].evaluateActive();
        }
	s.nodeList.add(this);
	if (s.transformTargets != null && s.transformTargets[0] != null) {
            s.transformTargets[0].addNode(this, Targets.VPF_TARGETS);
	    s.notifyThreads |= J3dThread.UPDATE_TRANSFORM;
	    
	}
	s.notifyThreads |= (J3dThread.UPDATE_BEHAVIOR | 
			    J3dThread.SOUND_SCHEDULER);
	universe.removeViewPlatform(this);
    }

    /**
     * Re-evaluate all View active status reference to this view
     * platform. This procedure is called from RenderBin when switch
     * above a view platform changed.
     */
    void reEvaluateView() {
	View views[] = getViewList();

	for (int i=views.length-1; i >=0; i--) {
	    views[i].evaluateActive();
	}
    }

    /**
     * Get a copy of cached view list
     */
    View[] getViewList() {
	synchronized (viewList) {
	    if (viewListDirty) {
		views = (View []) viewList.toArray(new View[viewList.size()]);
		viewListDirty = false;
	    }
	    return views;
	}
    }

    /**
     * Use by BehaviorStructure to determine whether current
     * ViewPlatform is active or not.
     */
    boolean isActiveViewPlatform() {
	View v[] = getViewList();
	if (v != null) {
	    for (int i=0; i < v.length; i++) {
		if (v[i].active) {
		    return true;
		}
	    }
	}
	return false;
    }

    void processSwitchChanged() {
        reEvaluateView();
    }


    void compile(CompileState compState) {

        super.compile(compState);

	// keep the parent transform group. It's not worth
	// to push the static transform here.
        compState.keepTG = true;
    }
}