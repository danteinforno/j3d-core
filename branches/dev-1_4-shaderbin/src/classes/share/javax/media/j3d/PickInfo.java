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


public class PickInfo extends Object {
    
    /* The SceneGraphPath of the intersected pickable item */
    private SceneGraphPath sgp;

    /* The intersected pickable node object */
    private  Node node;
    
    /* A copy of LocalToVworld transform of the pickable node */
    private Transform3D l2vw;

    /* The closest intersection point */
    private Point3d closestIntersectionPoint;
 
    /* Distance between start point of pickShape and intersection point */
    private double  distance;

    /* An array to store intersection results */
    private IntersectionInfo[] intersectionInfos;


    /** PickInfo Constructor */
    PickInfo() {

    }
    

    /**
     * Retrieves the reference to the SceneGraphPath in this PickInfo object.
     * @return the SceneGraphPath object, or null if  flag is not set with SCENEGRAPHPATH.
     * @see the new set of pick methods in Local and BranchGroup
     */
    public SceneGraphPath getSceneGraphPath() {

	return sgp;

    }
    

    /**
     * Retrieves the reference to the picked node, either a Shape3D or a Morph, in this PickInfo object.
     * @return the picked leaf node object, or null if  flag is not set with NODE.
     * @see the new set of pick methods in Local and BranchGroup
     */
    public Node getNode() {

	return node;
    }

    /**
     * Retrieves the reference to the LocalToVworld transform of the picked node in this PickInfo object.
     * @return the local to vworld transform, or null if  flag is not set with LOCAL_TO_VWORLD.
     * @see the new set of pick methods in Local and BranchGroup
     */
    public Transform3D getLocalToVWorld() {
	
	return l2vw;

    }
	
    /**
     * Retrieves the reference to the closest intersection point in this PickInfo object.
     * @return the closest intersection point, or null if  flag is not set with CLOSEST_INTERSECTION_POINT.
     * @see the new set of pick methods in Local and BranchGroup
     */
    public Point3d getClosestIntersectionPoint() {

	return closestIntersectionPoint;
    
    }

    /**
     * Retrieves the distance between the start point of the pickShape and the intersection point.
     * @return double, or null if  flag is not set with CLOSEST_INTERSECTION_POINT.
     * @see the new set of pick methods in Local and BranchGroup
     */
    public double getDistance() {

	return distance;
    }

    /**
     * Retrieves the reference to the array of intersection results in this PickInfo object.
     * @return an array of  IntersectionInfo, with length 1, if  flag is to set  CLOSEST_GEOM_INFO,
     * or an array of IntersectionInfo contains all intersections of the picked node in sorted order.
     * @exception IllegalStateException if  both CLOSEST_GEOM_INFO and ALL_GEOM_INFO are set.
     * @see the new set of pick methods in Local and BranchGroup
     */
    public IntersectionInfo[] getIntersectionInfos() {

	return intersectionInfos;	
    }

    
    public class IntersectionInfo extends Object {
	
	/* The index to the intersected geometry in the pickable node */
	private int geomIndex;

        /* The reference to the intersected geometry in the pickable object */
	private Geometry geom;

	/* The interpolation weights for each of the verticies of the primitive */
	private float[] weights;
	
	/* The intersection point */
	private Point3d intersectionPoint;
     
	/* The vertex indices of the intersected primitive in the geometry */   
	private int[] vertexIndices;

	/** IntersectionInfo Constructor */
	IntersectionInfo() {

	}

	/**
	 * Retrieves the index to the intersected geometry in the picked node, either a Shape3D or Morph.
	 * @return the index of the intersected geometry in the pickable node.
	 */
	public int getGeometryIndex() {
	    return geomIndex;
	}

	/**
	 * Retrieves the reference to the intersected geometry in the picked object, either a Shape3D or Morph.
	 * @return the intersected geometry in the pickable node.
	 */
	public Geometry getGeometry() {
	    return geom;
	}

	/**
	 * Retrieves the interpolation weights for each of the verticies of the  intersected primitive.
	 * Quad needs to be co-planar.
	 * @return the interpolation weights for each of the verticies.
	 */
	public float[] getWeights() {
	    return weights;
	}

	/**
	 * Retrieves the reference to the intersection point in the pickable node.
	 * @return the intersected point in the pickable node.
	 */
	public Point3d getIntersectionPoint() {
	    return intersectionPoint;
	}

	/**
	 * Retrieves the vertex indices of the intersected primitive in the geometry.
	 * @return the vertex indices of the intersected primitive.
	 */
	public int[] getVertexIndices() {
	    return vertexIndices;
	}
    }
}


