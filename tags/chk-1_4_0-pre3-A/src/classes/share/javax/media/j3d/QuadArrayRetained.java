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
import java.lang.Math;

/**
 * The QuadArray object draws the array of vertices as individual
 * quadrilaterals.  Each group
 * of four vertices defines a quadrilateral to be drawn.
 */

class QuadArrayRetained extends GeometryArrayRetained {

    QuadArrayRetained() {
	this.geoType = GEO_TYPE_QUAD_SET;
    }

    boolean intersect(PickShape pickShape, double dist[],  Point3d iPnt) {
	Point3d pnts[] = new Point3d[4];
	double sdist[] = new double[1];
	double minDist = Double.MAX_VALUE;
	double x = 0, y = 0, z = 0;
	int i = ((vertexFormat & GeometryArray.BY_REFERENCE) == 0 ?
		 initialVertexIndex : initialCoordIndex);

	pnts[0] = new Point3d();
	pnts[1] = new Point3d();
	pnts[2] = new Point3d();
	pnts[3] = new Point3d();
    
	switch (pickShape.getPickType()) {
	case PickShape.PICKRAY:
	    PickRay pickRay= (PickRay) pickShape;

	    while (i < validVertexCount) {
		getVertexData(i++, pnts[0]);
		getVertexData(i++, pnts[1]);
		getVertexData(i++, pnts[2]);
		getVertexData(i++, pnts[3]);
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
		getVertexData(i++, pnts[0]);
		getVertexData(i++, pnts[1]);
		getVertexData(i++, pnts[2]);
		getVertexData(i++, pnts[3]);
		if (intersectSegment(pnts, pickSegment.start,
				     pickSegment.end, sdist, iPnt)) {
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
		getVertexData(i++, pnts[0]);
		getVertexData(i++, pnts[1]);
		getVertexData(i++, pnts[2]);
		getVertexData(i++, pnts[3]);

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
		getVertexData(i++, pnts[0]);
		getVertexData(i++, pnts[1]);
		getVertexData(i++, pnts[2]);
		getVertexData(i++, pnts[3]);

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
		getVertexData(i++, pnts[0]);
		getVertexData(i++, pnts[1]);
		getVertexData(i++, pnts[2]);
		getVertexData(i++, pnts[3]);

		if (intersectBoundingPolytope(pnts, bpolytope, sdist, iPnt)) {
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
		getVertexData(i++, pnts[0]);
		getVertexData(i++, pnts[1]);
		getVertexData(i++, pnts[2]);
		getVertexData(i++, pnts[3]);

		if (intersectCylinder(pnts, pickCylinder, sdist, iPnt)) {
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
		getVertexData(i++, pnts[0]);
		getVertexData(i++, pnts[1]);
		getVertexData(i++, pnts[2]);
		getVertexData(i++, pnts[3]);
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
	    throw new IllegalArgumentException(J3dI18N.getString("QuadArrayRetained0"));
	default:
	    throw new RuntimeException("PickShape not supported for intersection "); 
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
  
    // intersect pnts[] with every quad in this object
    boolean intersect(Point3d[] pnts) {
	Point3d[] points = new Point3d[4];
	double dist[] = new double[1];
	int i = ((vertexFormat & GeometryArray.BY_REFERENCE) == 0 ?
		 initialVertexIndex : initialCoordIndex);

	points[0] = new Point3d();
	points[1] = new Point3d();
	points[2] = new Point3d();
	points[3] = new Point3d();
	
	switch (pnts.length) {
	case 3: // Triangle
	    while (i < validVertexCount) {
		getVertexData(i++, points[0]);		
		getVertexData(i++, points[1]);		
		getVertexData(i++, points[2]);		
		getVertexData(i++, points[3]);		
		if (intersectTriTri(points[0], points[1], points[2],
				    pnts[0], pnts[1], pnts[2]) ||
		    intersectTriTri(points[0], points[2], points[3],
				    pnts[0], pnts[1], pnts[2])) {
		    return true;
		}
	    }
	    break;
	case 4: // Quad
	    
	    while (i < validVertexCount) {
		getVertexData(i++, points[0]);		
		getVertexData(i++, points[1]);		
		getVertexData(i++, points[2]);		
		getVertexData(i++, points[3]);		
		if (intersectTriTri(points[0], points[1], points[2],
				    pnts[0], pnts[1], pnts[2]) ||
		    intersectTriTri(points[0], points[1], points[2],
				    pnts[0], pnts[2], pnts[3]) ||
		    intersectTriTri(points[0], points[2], points[3],
				    pnts[0], pnts[1], pnts[2]) ||
		    intersectTriTri(points[0], points[2], points[3],
				    pnts[0], pnts[2], pnts[3])) {
		    return true;
		}
	    }
	    break;
	case 2: // Line
	    while (i < validVertexCount) {
		getVertexData(i++, points[0]);		
		getVertexData(i++, points[1]);		
		getVertexData(i++, points[2]);		
		getVertexData(i++, points[3]);		
		if (intersectSegment(points, pnts[0], pnts[1], dist,
				     null)) {
		    return true;
		}
	    }
	    break;
	case 1: // Point
	    while (i < validVertexCount) {
		getVertexData(i++, points[0]);		
		getVertexData(i++, points[1]);		
		getVertexData(i++, points[2]);		
		getVertexData(i++, points[3]);		
		if (intersectTriPnt(points[0], points[1], points[2],
				    pnts[0]) ||
		    intersectTriPnt(points[0], points[2], points[3],
				    pnts[0])) {
		    return true;
		}
	    }
	    break;
	}
	return false;
    }
    

    boolean intersect(Transform3D thisToOtherVworld,  GeometryRetained geom) {

	Point3d[] points = new Point3d[4];
	int i = ((vertexFormat & GeometryArray.BY_REFERENCE) == 0 ?
		 initialVertexIndex : initialCoordIndex);

	points[0] = new Point3d();
	points[1] = new Point3d();
	points[2] = new Point3d();
	points[3] = new Point3d();
	
	while (i < validVertexCount) {
	    getVertexData(i++, points[0]);		
	    getVertexData(i++, points[1]);		
	    getVertexData(i++, points[2]);		
	    getVertexData(i++, points[3]);		
	    thisToOtherVworld.transform(points[0]);
	    thisToOtherVworld.transform(points[1]);
	    thisToOtherVworld.transform(points[2]);
	    thisToOtherVworld.transform(points[3]);
	    if (geom.intersect(points)) {
		return true;
	    }
	}  // for each quad
	return false;
    }

    // the bounds argument is already transformed
    boolean intersect(Bounds targetBound) {
	Point3d[] points = new Point3d[4];
	int i = ((vertexFormat & GeometryArray.BY_REFERENCE) == 0 ?
		 initialVertexIndex : initialCoordIndex);

	points[0] = new Point3d();
	points[1] = new Point3d();
	points[2] = new Point3d();
	points[3] = new Point3d();

	switch(targetBound.getPickType()) {
	case PickShape.PICKBOUNDINGBOX:
	    BoundingBox box = (BoundingBox) targetBound;

	    while (i < validVertexCount) {
		getVertexData(i++, points[0]);		
		getVertexData(i++, points[1]);		
		getVertexData(i++, points[2]);		
		getVertexData(i++, points[3]);		
		if (intersectBoundingBox(points, box, null, null)) {
		    return true;
		}
	    }
	    break;
	case PickShape.PICKBOUNDINGSPHERE:
	    BoundingSphere bsphere = (BoundingSphere) targetBound;

	    while (i < validVertexCount) {
		getVertexData(i++, points[0]);		
		getVertexData(i++, points[1]);		
		getVertexData(i++, points[2]);		
		getVertexData(i++, points[3]);		
		if (intersectBoundingSphere(points, bsphere, null,
					    null)) {
		    return true;
		}
	    }
	    break;
	case PickShape.PICKBOUNDINGPOLYTOPE:
	    BoundingPolytope bpolytope = (BoundingPolytope) targetBound;

	    while (i < validVertexCount) {
		getVertexData(i++, points[0]);		
		getVertexData(i++, points[1]);		
		getVertexData(i++, points[2]);		
		getVertexData(i++, points[3]);		
		if (intersectBoundingPolytope(points, bpolytope, null, null)) {
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
    
    // From Graphics Gems IV (pg5) and Graphics Gems II, Pg170
    // The centroid is the area-weighted sum of the centroids of
    // disjoint triangles that make up the polygon.
    void computeCentroid() {
	int i = ((vertexFormat & GeometryArray.BY_REFERENCE) == 0 ?
		 initialVertexIndex : initialCoordIndex);

	Point3d pnt0 = getPoint3d();
	Point3d pnt1 = getPoint3d();
	Point3d pnt2 = getPoint3d();
	Point3d pnt3 = getPoint3d();
	Vector3d vec = getVector3d();
	Vector3d normal = getVector3d();
	Vector3d tmpvec = getVector3d();

	double area;
	double totalarea = 0;

	centroid.x = 0;
	centroid.y = 0;
	centroid.z = 0;

	while (i < validVertexCount) {
	    getVertexData(i++, pnt0);
	    getVertexData(i++, pnt1);
	    getVertexData(i++, pnt2);
	    getVertexData(i++, pnt3);

	    // Determine the normal
	    tmpvec.sub(pnt0, pnt1);
	    vec.sub(pnt1, pnt2);

	    // Do the cross product
	    normal.cross(tmpvec, vec);
	    normal.normalize();
	    // If a degenerate triangle, don't include
	    if (Double.isNaN(normal.x+normal.y+normal.z))
		continue;
	    tmpvec.set(0,0,0);
	    // compute the area of each triangle
	    getCrossValue(pnt0, pnt1, tmpvec);
	    getCrossValue(pnt1, pnt2, tmpvec);
	    getCrossValue(pnt2, pnt0, tmpvec);
	    area = normal.dot(tmpvec);
	    totalarea += area;
	    centroid.x += (pnt0.x+pnt1.x+pnt2.x) * area;
	    centroid.y += (pnt0.y+pnt1.y+pnt2.y) * area;
	    centroid.z += (pnt0.z+pnt1.z+pnt2.z) * area;

	    // compute the area of each triangle
	    tmpvec.set(0,0,0);
	    getCrossValue(pnt0, pnt2, tmpvec);
	    getCrossValue(pnt2, pnt3, tmpvec);
	    getCrossValue(pnt3, pnt0, tmpvec);
	    area = normal.dot(tmpvec);
	    totalarea += area;
	    centroid.x += (pnt3.x+pnt0.x+pnt2.x) * area;
	    centroid.y += (pnt3.y+pnt0.y+pnt2.y) * area;
	    centroid.z += (pnt3.z+pnt0.z+pnt2.z) * area;
	}
	if (totalarea != 0.0) {
	    area = 1.0/(3.0 * totalarea);
	    centroid.x *= area;
	    centroid.y *= area;
	    centroid.z *= area;
	}
	freeVector3d(tmpvec);
	freeVector3d(vec);
	freeVector3d(normal);
	freePoint3d(pnt0);
	freePoint3d(pnt1);
	freePoint3d(pnt2);
	freePoint3d(pnt3);
    }

    int getClassType() {
	return QUAD_TYPE;
    }
}
