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

import java.util.Map;
import java.util.HashMap;

/**
 * The Shader Attributes object provides uniform attributes to
 * shader programs.  Uniform attributes (variables) are those
 * attributes whose values are constant during the rendering of a
 * primitive. Their values may change from primitive to primitive, but
 * are constant for each vertex (for vertex shaders) or fragment (for
 * fragment shaders) of a single primitive. Examples of uniform
 * attributes include a transformation matrix, a texture map, lights,
 * lookup tables, etc.
 *
 * <p>
 * There are two ways in which values can be specified for uniform
 * attributes: explicitly, by providing a value; and implicitly, by
 * defining a mapping between a Java 3D system attribute and a uniform
 * attribute. The ShaderAttributes object contains two maps, one for
 * each type of attribute:
 *
 * <ul>
 * <li>An Attributes map, expressed as
 * <code>(attrName,&nbsp;value)</code> pairs, for explicitly defined
 * attributes</li>
 * <li>A SystemAttributes map, expressed as
 * <code>(attrName,&nbsp;j3dAttrName)</code> pairs, for implicitly
 * defined attributes</li>
 * </ul>
 *
 * <p>
 * Note that a given attribute name may not appear in more than one of
 * the two maps.  That is, the Attributes map must not contain any
 * <code>(attrName,&nbsp;value)</code> pairs whose
 * <code>attrName</code> equals the <code>attrName</code> in any
 * <code>(attrName,&nbsp;j3dAttrName)</code> pair in the
 * SystemAttributes map.
 *
 * <p>
 * Details of these two maps are as follows:
 *
 * <ol>
 *
 * <li>
 * Attributes Map
 * <ul>
 * Each uniform variable <code>attrName</code> in the Attributes map is
 * explicitly set to the corresponding <code>value</code> during rendering.
 * Each <code>attrName</code> in the map must be the name of a valid
 * uniform attribute in the shader in which it is used. Otherwise, the
 * attribute name will be ignored and a runtime error may be
 * generated.  Each <code>value</code> in the map must be an instance
 * of one of the allowed classes or an array of one the allowed
 * classes.  The allowed classes are: <code>Integer</code>,
 * <code>Float</code>, <code>Tuple{2,3,4}{i,f,d}</code>,
 * <code>Matrix{3,4}{f,d}</code>. A ClassCastException will be thrown
 * if a specified <code>value</code> object is not one of the allowed
 * types. Further, the type of each <code>value</code> object must
 * match the type of the corresponding <code>attrName</code> variable
 * in the shader in which it is used. Otherwise, the shader will not
 * be able to use the attribute and a runtime error may be generated.
 * </ul> </li> <br>
 *
 * <li>
 * SystemAttributes Map
 *
 * <ul>
 *
 * Each uniform variable <code>attrName</code> in the Attributes map
 * is is implicitly set to the value of the corresponding Java 3D
 * system attribute <code>j3dAttrName</code> during rendering.
 * Each <code>attrName</code> in the map must be the name of a valid
 * uniform attribute in the shader in which it is used. Otherwise, the
 * attribute name will be ignored and a runtime error may be
 * generated.  Each <code>j3dAttrName</code> must be the name of a
 * predefined Java&nbsp;3D system attribute. An
 * IllegalArgumentException will be thrown if a specified
 * <code>j3dAttrName</code> is not one of the predefined system
 * attributes. Further, the type of each <code>j3dAttrName</code>
 * attribute must match the type of the corresponding
 * <code>attrName</code> variable in the shader in which it is
 * used. Otherwise, the shader will not be able to use the attribute
 * and a runtime error may be generated.
 *
 * <p>
 * Following is the list of predefined Java&nbsp;3D system attributes:
 * <br><br>
 *
 * <ul>
 * <font color="#ff0000"><i>TODO: replace the following with
 * the real system attributes table</i></font><br>
 * <table BORDER=1 CELLSPACING=2 CELLPADDING=2>
 * <tr>
 * <td><b>Name</b></td>
 * <td><b>Type</b></td>
 * <td><b>Description</b></td>
 * </tr>
 * <tr>
 * <td><code>something</code></td>
 * <td>Float</td>
 * <td>This is something (of course)</td>
 * </tr>
 * <tr>
 * <td><code>somethingElse</code></td>
 * <td>Tuple3f</td>
 * <td>This is something else</td>
 * </tr>
 * </table>
 * </ul>
 *
 * </ul>
 * </li>
 *
 * </ol>
 *
 * <p>
 * Depending on the shading language (and profile) being used, several
 * Java 3D state attributes are automatically made available to the
 * shader program as pre-defined uniform attributes. The application
 * doesn't need to do anything to pass these attributes in to the
 * shader program. The implementation of each shader language (e.g.,
 * Cg, GLSL) defines its own mapping from Java 3D attribute to uniform
 * variable name.
 *
 * <p>
 * A list of these attributes for each shader language can be found in
 * the concrete subclass of ShaderProgram for that shader language.
 *
 * @see ShaderProgram
 * @see ShaderAppearance#setShaderAttributes
 *
 * @since Java 3D 1.4
 */

public class ShaderAttributes extends NodeComponent {
    private Map attrs = new HashMap();
    private Map systemAttrs = new HashMap();

    /**
     * Specifies that this ShaderAttributes object allows reading
     * its Attributes map.
     */
    public static final int
	ALLOW_ATTRIBUTES_READ =
	CapabilityBits.SHADER_ATTRIBUTES_ALLOW_ATTRIBUTES_READ;

    /**
     * Specifies that this ShaderAttributes object allows writing
     * its Attributes map.
     */
    public static final int
	ALLOW_ATTRIBUTES_WRITE =
	CapabilityBits.SHADER_ATTRIBUTES_ALLOW_ATTRIBUTES_WRITE;

    /**
     * Specifies that this ShaderAttributes object allows reading
     * its SystemAttributes map.
     */
    public static final int
	ALLOW_SYSTEM_ATTRIBUTES_READ =
	CapabilityBits.SHADER_ATTRIBUTES_ALLOW_SYSTEM_ATTRIBUTES_READ;

    /**
     * Specifies that this ShaderAttributes object allows writing
     * its SystemAttributes map.
     */
    public static final int
	ALLOW_SYSTEM_ATTRIBUTES_WRITE =
	CapabilityBits.SHADER_ATTRIBUTES_ALLOW_SYSTEM_ATTRIBUTES_WRITE;


    /**
     * Constructs an empty ShaderAttributes object. Both the
     * Attributes map and SystemAttributes map are initially empty.
     */
    public ShaderAttributes() {
	throw new RuntimeException("not implemented");
    }

    //
    // Methods for dealing with the (name, value) pairs for explicit
    // attributes
    //

    /**
     * Adds an entry for the specified <code>(attrName,&nbsp;value)</code>
     * pair to the Attributes map. If attrName already exists in the
     * Attributes map, then its value is replaced with the specified
     * value.
     *
     * @param attrName the name of the uniform attribute to be added
     * @param value the new value of the uniform attribute
     *
     * @exception NullPointerException if attrName or value is null
     *
     * @exception ClassCastException if value is not an instance of
     * one of the allowed classes
     *
     * @exception IllegalArgumentException if attrName exists in the
     * SystemAttributes map
     *
     * @exception CapabilityNotSetException if appropriate capability is 
     * not set and this object is part of live or compiled scene graph
     */
    public void putAttribute(String attrName, Object value) {
	throw new RuntimeException("not implemented");
    }

    /**
     * Copies all of the mappings from the specified map to the
     * Attributes map of this ShaderAttributes. Each entry in the
     * specified map replaces an existing mapping with the same
     * attrName. Note that this method is functionally equivalent to
     * iterating over the map and calling
     * <code>putAttribute(attrName,&nbsp;value)</code> for each entry
     * in the map.
     *
     * @param m a map object containing (name,value) pairs to be added
     * to the Attributes map
     *
     * @exception NullPointerException if the specified map is null, or if
     * any key or value in the map is null
     *
     * @exception ClassCastException if the value of any entry in the
     * specified map is not an instance of one of the allowed classes,
     * or if the key of any entry in the specified map is not a String
     *
     * @exception IllegalArgumentException if the key of any entry
     * in the specified map exists in the SystemAttributes map
     *
     * @exception CapabilityNotSetException if appropriate capability is 
     * not set and this object is part of live or compiled scene graph
     */
    public void putAllAttributes(Map m) {
	throw new RuntimeException("not implemented");
    }

    /**
     * Removes the specified <code>attrName</code> and its associated
     * value from the Attributes map. If attrName does not exist in
     * the Attributes map then nothing happens.
     *
     * @param attrName the name of the uniform attribute to be removed
     *
     * @exception NullPointerException if attrName is null
     *
     * @exception CapabilityNotSetException if appropriate capability is 
     * not set and this object is part of live or compiled scene graph
     */
    public void removeAttribute(String attrName) {
	throw new RuntimeException("not implemented");
    }

    /**
     * Removes all <code>(attrName,&nbsp;value)</code> mappings from
     * from the Attributes map. The Attributes map will be empty following
     * this call.
     *
     * @exception CapabilityNotSetException if appropriate capability is 
     * not set and this object is part of live or compiled scene graph
     */
    public void clearAttributes() {
	throw new RuntimeException("not implemented");
    }

    /**
     * Retrieves the value associated with the specified attrName in
     * the Attributes map. If the specified name is not in the
     * Attributes map, then null is returned.
     *
     * @param attrName the name of the uniform attribute to be retrieved
     *
     * @exception NullPointerException if attrName is null
     *
     * @return the value associated with the specified attribute name, or null
     * if the name is not in the Attributes map
     *
     * @exception CapabilityNotSetException if appropriate capability is 
     * not set and this object is part of live or compiled scene graph
     */
    public Object getAttribute(String attrName) {
	throw new RuntimeException("not implemented");
	//return attrs.get(attrName);
    }

    /**
     * Returns a shallow copy of the Attributes map. The the attribute
     * names and values themselves are not cloned.
     *
     * @return a shallow copy of the Attributes map
     *
     * @exception CapabilityNotSetException if appropriate capability is 
     * not set and this object is part of live or compiled scene graph
     */
    public Map getAttributesMap() {
	throw new RuntimeException("not implemented");
	//return attrs.clone();
    }

    //
    // Methods for dealing with the (name, j3dName) pairs for implicit
    // attribute mappings
    //

    /**
     * Adds an entry for the specified <code>(attrName,&nbsp;j3dAttrName)</code>
     * pair to the SystemAttributes map. If attrName already exists in the
     * SystemAttributes map, then its value is replaced with the specified
     * j3dAttrName.
     *
     * @param attrName the name of the uniform attribute to be added
     * @param j3dAttrName the name of the new Java&nbsp;3D attribute
     * to be associated with the uniform attribute
     *
     * @exception NullPointerException if attrName or j3dAttrName is null
     *
     * @exception IllegalArgumentException if attrName exists in the
     * Attributes map
     *
     * @exception CapabilityNotSetException if appropriate capability is 
     * not set and this object is part of live or compiled scene graph
     */
    public void putSystemAttribute(String attrName, String j3dAttrName) {
	throw new RuntimeException("not implemented");
    }

    /**
     * Copies all of the mappings from the specified map to the
     * SystemAttributes map of this ShaderAttributes. Each entry in the
     * specified map replaces an existing mapping with the same
     * attrName. Note that this method is functionally equivalent to
     * iterating over the map and calling
     * <code>putSystemAttribute(attrName,&nbsp;j3dAttrName)</code> for each entry
     * in the map.
     *
     * @param m a map object containing (name,value) pairs to be added
     * to the SystemAttributes map
     *
     * @exception NullPointerException if the specified map is null, or if
     * any key or value in the map is null
     *
     * @exception ClassCastException if the key or value of any entry
     * in the specified map is not a String
     *
     * @exception IllegalArgumentException if the key of any entry
     * in the specified map exists in the Attributes map
     *
     * @exception CapabilityNotSetException if appropriate capability is 
     * not set and this object is part of live or compiled scene graph
     */
    public void putAllSystemAttributes(Map m) {
	throw new RuntimeException("not implemented");
    }

    /**
     * Removes the specified <code>attrName</code> and its associated
     * value from the SystemAttributes map. If attrName does not exist in
     * the SystemAttributes map then nothing happens.
     *
     * @param attrName the name of the uniform attribute to be removed
     *
     * @exception NullPointerException if attrName is null
     *
     * @exception CapabilityNotSetException if appropriate capability is 
     * not set and this object is part of live or compiled scene graph
     */
    public void removeSystemAttribute(String attrName) {
	throw new RuntimeException("not implemented");
    }

    /**
     * Removes all <code>(attrName,&nbsp;j3dAttrName)</code> mappings
     * from from the SystemAttributes map. The SystemAttributes map
     * will be empty following this call.
     *
     * @exception CapabilityNotSetException if appropriate capability is 
     * not set and this object is part of live or compiled scene graph
     */
    public void clearSystemAttributes() {
	throw new RuntimeException("not implemented");
    }

    /**
     * Retrieves the j3dAttrName associated with the specified attrName in
     * the SystemAttributes map. If the specified name is not in the
     * SystemAttributes map, then null is returned.
     *
     * @param attrName the name of the uniform attribute to be retrieved
     *
     * @exception NullPointerException if attrName is null
     *
     * @return the value associated with the specified attribute name, or null
     * if the name is not in the SystemAttributes map
     *
     * @exception CapabilityNotSetException if appropriate capability is 
     * not set and this object is part of live or compiled scene graph
     */
    public String getSystemAttribute(String attrName) {
	throw new RuntimeException("not implemented");
	//return (String)systemAttrs.get(attrName);
    }

    /**
     * Returns a shallow copy of the SystemAttributes map. The the attribute
     * names and values themselves are not cloned.
     *
     * @return a shallow copy of the SystemAttributes map
     *
     * @exception CapabilityNotSetException if appropriate capability is 
     * not set and this object is part of live or compiled scene graph
     */
    public Map getSystemAttributesMap() {
	throw new RuntimeException("not implemented");
	//return systemAttrs.clone();
    }

}
