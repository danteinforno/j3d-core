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
 * The TriangleArray object draws the array of vertices as individual
 * triangles.  Each group
 * of three vertices defines a triangle to be drawn.
 */

public class TriangleArray extends GeometryArray {

    // non-public, no parameter constructor
    TriangleArray() {}

    /**
     * Constructs an empty TriangleArray object with the specified
     * number of vertices, and vertex format.
     * @param vertexCount the number of vertex elements in this array
     * @param vertexFormat a mask indicating which components are
     * present in each vertex.  This is specified as one or more
     * individual flags that are bitwise "OR"ed together to describe
     * the per-vertex data.
     * The flags include: COORDINATES, to signal the inclusion of
     * vertex positions--always present; NORMALS, to signal 
     * the inclusion of per vertex normals; one of COLOR_3,
     * COLOR_4, to signal the inclusion of per vertex
     * colors (without or with color information); and one of 
     * TEXTURE_COORDINATE_2, TEXTURE_COORDINATE_3 or TEXTURE_COORDINATE_4, 
     * to signal the
     * inclusion of per-vertex texture coordinates 2D, 3D or 4D.
     * @exception IllegalArgumentException if vertexCount is less than 3
     * or vertexCount is <i>not</i> a multiple of 3
     */
    public TriangleArray(int vertexCount, int vertexFormat) {
	super(vertexCount,vertexFormat);

        if (vertexCount < 3 || ((vertexCount%3) != 0))
	    throw new IllegalArgumentException(J3dI18N.getString("TriangleArray0"));
    }

    /**
     * Constructs an empty TriangleArray object with the specified
     * number of vertices, and vertex format, number of texture coordinate
     * sets, and texture coordinate mapping array.
     *
     * @param vertexCount the number of vertex elements in this array<p>
     *
     * @param vertexFormat a mask indicating which components are
     * present in each vertex.  This is specified as one or more
     * individual flags that are bitwise "OR"ed together to describe
     * the per-vertex data.
     * The flags include: COORDINATES, to signal the inclusion of
     * vertex positions--always present; NORMALS, to signal 
     * the inclusion of per vertex normals; one of COLOR_3,
     * COLOR_4, to signal the inclusion of per vertex
     * colors (without or with color information); and one of 
     * TEXTURE_COORDINATE_2, TEXTURE_COORDINATE_3 or TEXTURE_COORDINATE_4, 
     * to signal the
     * inclusion of per-vertex texture coordinates 2D, 3D or 4D.<p>
     *
     * @param texCoordSetCount the number of texture coordinate sets
     * in this GeometryArray object.  If <code>vertexFormat</code>
     * does not include one of <code>TEXTURE_COORDINATE_2</code>,
     * <code>TEXTURE_COORDINATE_3</code> or
     * <code>TEXTURE_COORDINATE_4</code>, the
     * <code>texCoordSetCount</code> parameter is not used.<p>
     *
     * @param texCoordSetMap an array that maps texture coordinate
     * sets to texture units.  The array is indexed by texture unit
     * number for each texture unit in the associated Appearance
     * object.  The values in the array specify the texture coordinate
     * set within this GeometryArray object that maps to the
     * corresponding texture
     * unit.  All elements within the array must be less than
     * <code>texCoordSetCount</code>.  A negative value specifies that
     * no texture coordinate set maps to the texture unit
     * corresponding to the index.  If there are more texture units in
     * any associated Appearance object than elements in the mapping
     * array, the extra elements are assumed to be -1.  The same
     * texture coordinate set may be used for more than one texture
     * unit.  Each texture unit in every associated Appearance must
     * have a valid source of texture coordinates: either a
     * non-negative texture coordinate set must be specified in the
     * mapping array or texture coordinate generation must be enabled.
     * Texture coordinate generation will take precedence for those
     * texture units for which a texture coordinate set is specified
     * and texture coordinate generation is enabled.  If
     * <code>vertexFormat</code> does not include one of
     * <code>TEXTURE_COORDINATE_2</code>,
     * <code>TEXTURE_COORDINATE_3</code> or
     * <code>TEXTURE_COORDINATE_4</code>, the
     * <code>texCoordSetMap</code> array is not used.
     *
     * @exception IllegalArgumentException if vertexCount is less than 3
     * or vertexCount is <i>not</i> a multiple of 3
     *
     * @since Java 3D 1.2
     */
    public TriangleArray(int vertexCount,
			 int vertexFormat,
			 int texCoordSetCount,
			 int[] texCoordSetMap) {

	super(vertexCount, vertexFormat,
	      texCoordSetCount, texCoordSetMap);

        if (vertexCount < 3 || ((vertexCount%3) != 0))
	    throw new IllegalArgumentException(J3dI18N.getString("TriangleArray0"));
    }

    /**
     * Constructs an empty TriangleArray object with the specified
     * number of vertices, vertex format, number of texture coordinate
     * sets, and texture coordinate mapping array.  Defaults are used
     * for all other parameters.
     *
     * @param vertexCount the number of vertex elements in this array<p>
     *
     * @param vertexFormat a mask indicating which components are
     * present in each vertex.  This is specified as one or more
     * individual flags that are bitwise "OR"ed together to describe
     * the per-vertex data.
     * The flags include: <code>COORDINATES</code>, to signal the inclusion of
     * vertex positions--always present; <code>NORMALS</code>, to signal
     * the inclusion of per vertex normals; one of <code>COLOR_3</code> or
     * <code>COLOR_4</code>, to signal the inclusion of per vertex
     * colors (without or with alpha information); one of
     * <code>TEXTURE_COORDINATE_2</code> or <code>TEXTURE_COORDINATE_3</code>
     * or <code>TEXTURE_COORDINATE_4</code>,
     * to signal the
     * inclusion of per-vertex texture coordinates (2D , 3D or 4D);
     * <code>VERTEX_ATTRIBUTES</code>, to signal
     * the inclusion of one or more arrays of vertex attributes;
     * <code>BY_REFERENCE</code>, to indicate that the data is passed
     * by reference
     * rather than by copying; <code>INTERLEAVED</code>, to indicate
     * that the referenced
     * data is interleaved in a single array;
     * <code>USE_NIO_BUFFER</code>, to indicate that the referenced data
     * is accessed via a J3DBuffer object that wraps an NIO buffer;
     * <code>USE_COORD_INDEX_ONLY</code>,
     * to indicate that only the coordinate indices are used for indexed
     * geometry arrays.<p>
     *
     * @param texCoordSetCount the number of texture coordinate sets
     * in this GeometryArray object.  If <code>vertexFormat</code>
     * does not include one of <code>TEXTURE_COORDINATE_2</code> or
     * <code>TEXTURE_COORDINATE_3</code>, the
     * <code>texCoordSetCount</code> parameter is not used.<p>
     *
     * <a name="texCoordSetMap">
     * @param texCoordSetMap an array that maps texture coordinate
     * sets to texture units.  The array is indexed by texture unit
     * number for each texture unit in the associated Appearance
     * object.  The values in the array specify the texture coordinate
     * set within this GeometryArray object that maps to the
     * corresponding texture
     * unit.  All elements within the array must be less than
     * <code>texCoordSetCount</code>.  A negative value specifies that
     * no texture coordinate set maps to the texture unit
     * corresponding to the index.  If there are more texture units in
     * any associated Appearance object than elements in the mapping
     * array, the extra elements are assumed to be -1.  The same
     * texture coordinate set may be used for more than one texture
     * unit.  Each texture unit in every associated Appearance must
     * have a valid source of texture coordinates: either a
     * non-negative texture coordinate set must be specified in the
     * mapping array or texture coordinate generation must be enabled.
     * Texture coordinate generation will take precedence for those
     * texture units for which a texture coordinate set is specified
     * and texture coordinate generation is enabled.  If
     * <code>vertexFormat</code> does not include one of
     * <code>TEXTURE_COORDINATE_2</code> or
     * <code>TEXTURE_COORDINATE_3</code> or
     * <code>TEXTURE_COORDINATE_4</code>, the
     * <code>texCoordSetMap</code> array is not used.  The following example
     * illustrates the use of the <code>texCoordSetMap</code> array.
     *
     * <p>
     * <ul>
     * <table BORDER=1 CELLSPACING=2 CELLPADDING=2>
     * <tr>
     * <td><center><b>Index</b></center></td>
     * <td><center><b>Element</b></center></td>
     * <td><b>Description</b></td>
     * </tr>
     * <tr>
     * <td><center>0</center></td>
     * <td><center>1</center></td>
     * <td>Use tex coord set 1 for tex unit 0</td>
     * </tr>
     * <tr>
     * <td><center>1</center></td>
     * <td><center>-1</center></td>
     * <td>Use no tex coord set for tex unit 1</td>
     * </tr>
     * <tr>
     * <td><center>2</center></td>
     * <td><center>0</center></td>
     * <td>Use tex coord set 0 for tex unit 2</td>
     * </tr>
     * <tr>
     * <td><center>3</center></td>
     * <td><center>1</center></td>
     * <td>Reuse tex coord set 1 for tex unit 3</td>
     * </tr>
     * </table>
     * </ul>
     * <p>
     *
     * @param vertexAttrCount the number of vertex attributes
     * in this GeometryArray object. If <code>vertexFormat</code>
     * does not include <code>VERTEX_ATTRIBUTES</code>, the
     * <code>vertexAttrCount</code> parameter must be 0.<p>
     *
     * @param vertexAttrSizes is an array that specifes the size of
     * each vertex attribute. Each element in the array specifies the
     * number of components in the attribute, from 1 to 4. The length
     * of the array must be equal to <code>vertexAttrCount</code>.<p>
     *
     * @param vertexAttrNames is an array of names for the vertex
     * attributes. Each element in the array is a String that
     * specifies the shader attribute name that is bound to the
     * corresponding vertex attribute. The length of the array must be
     * equal to <code>vertexAttrCount</code>.<p>
     *
     * @exception IllegalArgumentException if
     * <code>vertexCount&nbsp;&lt;&nbsp;0</code>, if vertexFormat does
     * NOT include <code>COORDINATES</code>, if the
     * <code>INTERLEAVED</code> bit is set without the
     * <code>BY_REFERENCE</code> bit being set, if the
     * <code>USE_NIO_BUFFER</code> bit is set without the
     * <code>BY_REFERENCE</code> bit being set, if
     * the <code>USE_COORD_INDEX_ONLY</code> bit is set for non-indexed
     * geometry arrays (that is, GeometryArray objects that are not a
     * subclass of IndexedGeometryArray), if
     * <code>texCoordSetCount&nbsp;&lt;&nbsp;0</code>, if any element
     * in <code>texCoordSetMap[]&nbsp;&gt;=&nbsp;texCoordSetCount</code>,
     * if <code>vertexAttrCount&nbsp;&gt;&nbsp;0</code> and the
     * <code>VERTEX_ATTRIBUTES</code> bit is not set,
     * if <code>vertexAttrCount&nbsp;&lt;&nbsp;0</code>, if
     * <code>vertexAttrSizes.length&nbsp;!=&nbsp;vertexAttrCount</code>,
     * if any element in <code>vertexAttrSizes[]</code> is <code>&lt; 1</code> or
     * <code>&gt; 4</code>, or if
     * <code>vertexAttrNames.length&nbsp;!=&nbsp;vertexAttrCount</code>.
     *
     * @exception IllegalArgumentException if vertexCount is less than 3
     * or vertexCount is <i>not</i> a multiple of 3
     *
     * @since Java 3D 1.4
     */
    public TriangleArray(int vertexCount,
			 int vertexFormat,
			 int texCoordSetCount,
			 int[] texCoordSetMap,
			 int vertexAttrCount,
			 int[] vertexAttrSizes,
			 String[] vertexAttrNames) {


	super(vertexCount, vertexFormat,
	      texCoordSetCount, texCoordSetMap,
	      vertexAttrCount, vertexAttrSizes, vertexAttrNames);

        if (vertexCount < 3 || ((vertexCount%3) != 0))
	    throw new IllegalArgumentException(J3dI18N.getString("TriangleArray0"));
    }


    /**
     * Creates the retained mode TriangleArrayRetained object that this
     * TriangleArray object will point to.
     */
    void createRetained() {
	this.retained = new TriangleArrayRetained();
	this.retained.setSource(this);
    }


    /**
     * @deprecated replaced with cloneNodeComponent(boolean forceDuplicate)
     */
    public NodeComponent cloneNodeComponent() {
	TriangleArrayRetained rt = (TriangleArrayRetained) retained;
	int texSetCount = rt.getTexCoordSetCount();
	TriangleArray t;

	// TODO KCR: need to clone the VertexAttr* state

	if (texSetCount == 0) {
	    t = new TriangleArray(rt.getVertexCount(), 
				  rt.getVertexFormat());
	} else {
	    int texMap[] = new int[rt.getTexCoordSetMapLength()];
	    rt.getTexCoordSetMap(texMap);
	    t = new TriangleArray(rt.getVertexCount(), 
				  rt.getVertexFormat(),
				  texSetCount,
				  texMap);
	}
	t.duplicateNodeComponent(this);
        return t;
     }
}
