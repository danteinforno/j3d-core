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

import javax.vecmath.*;
import java.lang.Math;

/**
 * The IndexedTriangleArray object draws the array of vertices as individual
 * triangles.  Each group
 * of three vertices defines a triangle to be drawn.
 */

class IndexedTriangleArrayRetained extends IndexedGeometryArrayRetained {
  
    IndexedTriangleArrayRetained() {
	this.geoType = GEO_TYPE_INDEXED_TRI_SET;
    }

    boolean intersect(PickShape pickShape, double dist[],  Point3d iPnt) {
	Point3d pnts[] = new Point3d[3];
	double sdist[] = new double[1];
	double minDist = Double.MAX_VALUE;
	double x = 0, y = 0, z = 0;
	int i = ((vertexFormat & GeometryArray.BY_REFERENCE) == 0 ?
		 initialVertexIndex : initialCoordIndex);

	pnts[0] = new Point3d();
	pnts[1] = new Point3d();
	pnts[2] = new Point3d();
    
	switch (pickShape.getPickType()) {
	case PickShape.PICKRAY:
	    PickRay pickRay= (PickRay) pickShape;

	    while (i < validVertexCount) {
		getVertexData(indexCoord[i++], pnts[0]);
		getVertexData(indexCoord[i++], pnts[1]);
		getVertexData(indexCoord[i++], pnts[2]);
		if (intersectRay(pnts, pickRay, sdist, iPnt)) {
		    if (dist == null) {
			return true;
		    }
		    if (sdist[0] < minDist) {
			minDist = sdist[0];
			x = iPnt.x;
			y = iPnt.y;
			z = iPnt.z;
		    }
		}
	    }
	    break;
	case PickShape.PICKSEGMENT:
	    PickSegment pickSegment = (PickSegment) pickShape;
	    while (i < validVertexCount) {
		getVertexData(indexCoord[i++], pnts[0]);
		getVertexData(indexCoord[i++], pnts[1]);
		getVertexData(indexCoord[i++], pnts[2]);
		if (intersectSegment(pnts, pickSegment.start,
				     pickSegment.end, sdist, iPnt)
		    && (sdist[0] <= 1.0)) {
		    if (dist == null) {
			return true;
		    }
		    if (sdist[0] < minDist) {
			minDist = sdist[0];
			x = iPnt.x;
			y = iPnt.y;
			z = iPnt.z;
			}
		}
	    }
	    break;
	case PickShape.PICKBOUNDINGBOX:
	    BoundingBox bbox = (BoundingBox) 
		((PickBounds) pickShape).bounds;
	    
	    while (i < validVertexCount) {
		getVertexData(indexCoord[i++], pnts[0]);
		getVertexData(indexCoord[i++], pnts[1]);
		getVertexData(indexCoord[i++], pnts[2]);
		if (intersectBoundingBox(pnts, bbox, sdist, iPnt)) {
		    if (dist == null) {
			return true;
		    }
		    if (sdist[0] < minDist) {
			minDist = sdist[0];
			x = iPnt.x;
			y = iPnt.y;
			    z = iPnt.z;
		    }
		}
	    }
	    break;
	case PickShape.PICKBOUNDINGSPHERE:
	    BoundingSphere bsphere = (BoundingSphere) 
		((PickBounds) pickShape).bounds;
	    
	    while (i < validVertexCount) {
		getVertexData(indexCoord[i++], pnts[0]);
		getVertexData(indexCoord[i++], pnts[1]);
		getVertexData(indexCoord[i++], pnts[2]);
		if (intersectBoundingSphere(pnts, bsphere, sdist, iPnt)) {
		    if (dist == null) {
			return true;
		    }
		    if (sdist[0] < minDist) {
			minDist = sdist[0];
			x = iPnt.x;
			y = iPnt.y;
			z = iPnt.z;
		    }
		}
	    }
	    break;
	case PickShape.PICKBOUNDINGPOLYTOPE:
	    BoundingPolytope bpolytope = (BoundingPolytope) 
		((PickBounds) pickShape).bounds;
	    
	    while (i < validVertexCount) {
		getVertexData(indexCoord[i++], pnts[0]);
		getVertexData(indexCoord[i++], pnts[1]);
		getVertexData(indexCoord[i++], pnts[2]);
		if (intersectBoundingPolytope(pnts, bpolytope,
					      sdist,iPnt)) { 
		    if (dist == null) {
			return true;
		    }
		    if (sdist[0] < minDist) {
			minDist = sdist[0];
			x = iPnt.x;
			y = iPnt.y;
			z = iPnt.z;
		    }
		}
	    }
	    break;
	case PickShape.PICKCYLINDER:
	    PickCylinder pickCylinder= (PickCylinder) pickShape;
	    while (i < validVertexCount) {
		getVertexData(indexCoord[i++], pnts[0]);
		getVertexData(indexCoord[i++], pnts[1]);
		getVertexData(indexCoord[i++], pnts[2]);
		if (intersectCylinder(pnts, pickCylinder, sdist,
				      iPnt)) {
		    if (dist == null) {
			return true;
		    }
		    if (sdist[0] < minDist) {
			minDist = sdist[0];
			x = iPnt.x;
			y = iPnt.y;
			z = iPnt.z;
		    }
		}
	    }
	    break;
	case PickShape.PICKCONE:
	    PickCone pickCone= (PickCone) pickShape;
	    
	    while (i < validVertexCount) {
		getVertexData(indexCoord[i++], pnts[0]);
		getVertexData(indexCoord[i++], pnts[1]);
		getVertexData(indexCoord[i++], pnts[2]);
		if (intersectCone(pnts, pickCone, sdist, iPnt)) {
		    if (dist == null) {
			return true;
		    }
		    if (sdist[0] < minDist) {
			minDist = sdist[0];
			x = iPnt.x;
			y = iPnt.y;
			z = iPnt.z;
		    }
		}
	    }
	    break;
	case PickShape.PICKPOINT:
	    // Should not happen since API already check for this
	    throw new IllegalArgumentException(J3dI18N.getString("IndexedTriangleArrayRetained0"));
	default:
	    throw new RuntimeException ("PickShape not supported for intersection"); 
	} 

    
	if (minDist < Double.MAX_VALUE) {
	    dist[0] = minDist;
	    iPnt.x = x;
	    iPnt.y = y;
	    iPnt.z = z;
	    return true;
	}
	return false;
    }
  
  
    // intersect pnts[] with every triangle in this object
    boolean intersect(Point3d[] pnts) {
	Point3d[] points = new Point3d[3];
	double dist[] = new double[1];
	int i = ((vertexFormat & GeometryArray.BY_REFERENCE) == 0 ?
		 initialVertexIndex : initialCoordIndex);
	
	points[0] = new Point3d();
	points[1] = new Point3d();	
	points[2] = new Point3d();	

	switch (pnts.length) {
	case 3: // Triangle
	    while (i<validVertexCount) {
		getVertexData(indexCoord[i++], points[0]);
		getVertexData(indexCoord[i++], points[1]);
		getVertexData(indexCoord[i++], points[2]);
		if (intersectTriTri(points[0], points[1], points[2],
				    pnts[0], pnts[1], pnts[2])) {
		    return true;
		}
	    }
	    break;
	case 4: // Quad
	    while (i<validVertexCount) {
		getVertexData(indexCoord[i++], points[0]);
		getVertexData(indexCoord[i++], points[1]);
		getVertexData(indexCoord[i++], points[2]);
		if (intersectTriTri(points[0], points[1], points[2],
				   pnts[0], pnts[1], pnts[2]) ||
		    intersectTriTri(points[0], points[1], points[2],
				    pnts[0], pnts[2], pnts[3])) {
		    return true;
		}
	    }
	    break;
	case 2: // Line
	    while (i<validVertexCount) {
		getVertexData(indexCoord[i++], points[0]);
		getVertexData(indexCoord[i++], points[1]);
		getVertexData(indexCoord[i++], points[2]);
		if (intersectSegment(points, pnts[0], pnts[1], dist,
				     null)) {
		    return true;
		}
	    }
	    break;
	case 1: // Point
	    while (i<validVertexCount) {
		getVertexData(indexCoord[i++], points[0]);
		getVertexData(indexCoord[i++], points[1]);
		getVertexData(indexCoord[i++], points[2]);
		if (intersectTriPnt(points[0], points[1], points[2],
				    pnts[0])) {
		    return true;
		}
	    }
	    break;
	}
	return false;
    }

    
    boolean intersect(Transform3D thisToOtherVworld, GeometryRetained geom) {
	Point3d[] pnts = new Point3d[3];
	int i = ((vertexFormat & GeometryArray.BY_REFERENCE) == 0 ?
		 initialVertexIndex : initialCoordIndex);
	pnts[0] = new Point3d();
	pnts[1] = new Point3d();
	pnts[2] = new Point3d();

	while (i < validVertexCount) {
	    getVertexData(indexCoord[i++], pnts[0]);
	    getVertexData(indexCoord[i++], pnts[1]);
	    getVertexData(indexCoord[i++], pnts[2]);
	    thisToOtherVworld.transform(pnts[0]);
	    thisToOtherVworld.transform(pnts[1]);
	    thisToOtherVworld.transform(pnts[2]);
	    if (geom.intersect(pnts)) {
		return true;
	    }
	}
	return false;
    }

    // the bounds argument is already transformed
    boolean intersect(Bounds targetBound) {
	Point3d[] pnts = new Point3d[3];
	int i = ((vertexFormat & GeometryArray.BY_REFERENCE) == 0 ?
		 initialVertexIndex : initialCoordIndex);
	pnts[0] = new Point3d();
	pnts[1] = new Point3d();
	pnts[2] = new Point3d();

	switch(targetBound.getPickType()) {
	case PickShape.PICKBOUNDINGBOX:
	    BoundingBox box = (BoundingBox) targetBound;
	    
	    while (i < validVertexCount) {
		getVertexData(indexCoord[i++], pnts[0]);
		getVertexData(indexCoord[i++], pnts[1]);
		getVertexData(indexCoord[i++], pnts[2]);
		if (intersectBoundingBox(pnts, box, null, null)) {
		    return true;
		}
	    }
	    break;
	case PickShape.PICKBOUNDINGSPHERE:
	    BoundingSphere bsphere = (BoundingSphere) targetBound;
	    
	    while (i < validVertexCount) {
		getVertexData(indexCoord[i++], pnts[0]);
		getVertexData(indexCoord[i++], pnts[1]);
		getVertexData(indexCoord[i++], pnts[1]);
		if (intersectBoundingSphere(pnts, bsphere, null,
					    null)) {
		    return true;
		}
	    }
	    break;
	case PickShape.PICKBOUNDINGPOLYTOPE:
	    BoundingPolytope bpolytope = (BoundingPolytope) targetBound;
	    
	    while (i < validVertexCount) {
		getVertexData(indexCoord[i++], pnts[0]);
		getVertexData(indexCoord[i++], pnts[1]);
		getVertexData(indexCoord[i++], pnts[2]);
		if (intersectBoundingPolytope(pnts, bpolytope,
					      null, null)) {
		    return true;
		}
	    }
	    break;
	default:
	    throw new RuntimeException("Bounds not supported for intersection "
				       + targetBound); 
	}
	return false;
    }
  
    int getClassType() {
	return TRIANGLE_TYPE;
    }
}
