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
 * A Link leaf node allows an application to reference a shared graph,
 * rooted by a SharedGroup node, from within a branch graph or another
 * shared graph.
 * Any number of Link nodes can refer to the same SharedGroup node.
 */

public class Link extends Leaf {
    /**
     * For Link nodes, specifies that the node allows access to 
     * its object's SharedGroup information.
     */
    public static final int
    ALLOW_SHARED_GROUP_READ = CapabilityBits.LINK_ALLOW_SHARED_GROUP_READ;

    /**
     * For Link nodes, specifies that the node allows writing 
     * its object's SharedGroup information.
     */
    public static final int
    ALLOW_SHARED_GROUP_WRITE = CapabilityBits.LINK_ALLOW_SHARED_GROUP_WRITE;

    /**
     * Constructs a Link node object that does not yet point to a
     * SharedGroup node.
     */
    public Link() {
    }

    /**
     * Constructs a Link node object that points to the specified
     * SharedGroup node.
     * @param sharedGroup the SharedGroup node
     */
    public Link(SharedGroup sharedGroup) {
	((LinkRetained)this.retained).setSharedGroup(sharedGroup);
    }

    /**
     * Creates the retained mode LinkRetained object that this
     * Link object will point to.
     */
    void createRetained() {
	this.retained = new LinkRetained();
	this.retained.setSource(this);
    }

    /**
     * Sets the node's SharedGroup reference.
     * @param sharedGroup the SharedGroup node to reference
     * @exception CapabilityNotSetException if appropriate capability is
     * not set and this object is part of live or compiled scene graph
     */
    public void setSharedGroup(SharedGroup sharedGroup) {

	if (isLiveOrCompiled())
	    if (!this.getCapability(ALLOW_SHARED_GROUP_WRITE))
		throw new CapabilityNotSetException(J3dI18N.getString("Link0"));
	((LinkRetained)this.retained).setSharedGroup(sharedGroup);
    }

    /**
     * Retrieves the node's SharedGroup reference.
     * @return the SharedGroup node
     * @exception CapabilityNotSetException if appropriate capability is
     * not set and this object is part of live or compiled scene graph
     */
    public SharedGroup getSharedGroup() {

	if (isLiveOrCompiled())
	    if (!this.getCapability(ALLOW_SHARED_GROUP_READ))
		throw new CapabilityNotSetException(J3dI18N.getString("Link1"));
	return ((LinkRetained)this.retained).getSharedGroup();
    }

    /**
     * Used to create a new instance of the node.  This routine is called
     * by <code>cloneTree</code> to duplicate the current node.
     * <br>
     * The cloned Link node will refer to the same
     * SharedGroup as the original node.  The SharedGroup referred to by
     * this Link node will not be cloned.
     * @param forceDuplicate when set to <code>true</code>, causes the
     *  <code>duplicateOnCloneTree</code> flag to be ignored.  When
     *  <code>false</code>, the value of each node's
     *  <code>duplicateOnCloneTree</code> variable determines whether
     *  NodeComponent data is duplicated or copied.
     *
     * @see Node#cloneTree
     * @see Node#cloneNode
     * @see Node#duplicateNode
     * @see NodeComponent#setDuplicateOnCloneTree
     */
    public Node cloneNode(boolean forceDuplicate) {
	Link l = new Link();
	l.duplicateNode(this, forceDuplicate);
	return l;
    }

    /**
     * Copies all Link information from
     * <code>originalNode</code> into
     * the current node.  This method is called from the
     * <code>cloneNode</code> method which is, in turn, called by the
     * <code>cloneTree</code> method.<P> 
     *
     * @param originalNode the original node to duplicate.
     * @param forceDuplicate when set to <code>true</code>, causes the
     *  <code>duplicateOnCloneTree</code> flag to be ignored.  When
     *  <code>false</code>, the value of each node's
     *  <code>duplicateOnCloneTree</code> variable determines whether
     *  NodeComponent data is duplicated or copied.
     *
     * @exception RestrictedAccessException if this object is part of a live
     *  or compiled scenegraph.
     *
     * @see Node#duplicateNode
     * @see Node#cloneTree
     * @see NodeComponent#setDuplicateOnCloneTree
     */
    void duplicateAttributes(Node originalNode,  boolean forceDuplicate) {
        super.duplicateAttributes(originalNode, forceDuplicate);
	((LinkRetained) retained).setSharedGroup(
		 ((LinkRetained) originalNode.retained).getSharedGroup());
    }
}