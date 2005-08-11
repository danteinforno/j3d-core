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

import java.util.ArrayList;
import javax.vecmath.*;

/**
 * The ShaderAttributeObjectRetained class is an abstract class that
 * encapsulates a uniform shader attribute whose value is specified
 * explicitly. This class has concrete subclasses for single-value
 * attributes (ShaderAttributeValueRetained) and array attributes
 * (ShaderAttributeArrayRetained). The shader variable <code>attrName</code>
 * is explicitly set to the specified <code>value</code> during
 * rendering. <code>attrName</code> must be the name of a valid
 * uniform attribute in the shader in which it is used. Otherwise, the
 * attribute name will be ignored and a runtime error may be
 * generated. The <code>value</code> must be an instance of one of the
 * allowed classes or an array of one the allowed classes. The allowed
 * classes are: <code>Integer</code>, <code>Float</code>,
 * <code>Tuple{2,3,4}{i,f}</code>,
 * <code>Matrix{3,4}f</code>. A ClassCastException will be thrown
 * if a specified <code>value</code> object is not one of the allowed
 * types. Further, the type of the value is immutable once a
 * ShaderAttributeObjectRetained is constructed.  Subsequent setValue
 * operations must be called with an object of the same type as the
 * one that was used to construct the ShaderAttributeObjectRetained. 
 * Finally, the type of the <code>value</code> object must match the type 
 * of the corresponding <code>attrName</code> variable in the shader in
 * which it is used. Otherwise, the shader will not be able to use the
 * attribute and a runtime error may be generated.
 *
 * @see ShaderAttributeSetRetained
 * @see ShaderProgramRetained
 *
 * @since Java 3D 1.4
 */

abstract class ShaderAttributeObjectRetained extends ShaderAttributeRetained {

    private int classType;
    private Class baseClass;
    AttrWrapper attrWrapper;

    /**
     * Package scope constructor
     */
    ShaderAttributeObjectRetained() {
    }

    void createObjectData(Object value) {
        
  	classType = computeClassType(value);
	baseClass = getBaseClass(classType);
	attrWrapper = createAttrWrapper(value, classType);      
 	/*
	System.err.println("    classType = " + classType +
			   ", baseClass = " + baseClass +
			   ", attrWrapper.get() = " + attrWrapper.get());
	*/   
    }
    
    
    void initValue(Object value) {
	/*
	System.err.println("ShaderAttributeObjectRetained : attrName = " + attrName +
			   ", value = " + value +
			   ", value.class = " + value.getClass());
	*/
	attrWrapper.set(value);

    }

    /**
     * Retrieves the value of this shader attribute.
     * A copy of the object is returned.
     */
    Object getValue() {
	return attrWrapper.get();
    }

    /**
     * Sets the value of this shader attribute to the specified value.
     * A copy of the object is stored.
     *
     * @param value the new value of the shader attribute
     *
     * @exception NullPointerException if value is null
     *
     * @exception ClassCastException if value is not an instance of
     * the same base class as the object used to construct this shader
     * attribute object.
     *
     */
    void setValue(Object value) {
        initValue(value);
	AttrWrapper valueWrapper = createAttrWrapper(value, this.classType);	
	sendMessage(SHADER_ATTRIBUTE_VALUE_UPDATE, valueWrapper);
    }

    /**
     * Retrieves the base class of the value of this shader attribute.
     * This class will always be one of the allowable classes, even if
     * a subclass was used to construct this shader attribute object.
     * For example, if this shader attribute object was constructed
     * with an instance of <code>javax.vecmath.Point3f</code>, the
     * returned class would be <code>javax.vecmath.Tuple3f</code>.
     *
     * @return the base class of the value of this shader attribute
     */
    Class getValueClass() {
	return baseClass;
    }
    
   /**
     * Initializes a mirror object.
     */
    synchronized void initMirrorObject() {
	super.initMirrorObject();        
	((ShaderAttributeObjectRetained)mirror).initValue(getValue());
    }
    
     /**
     * Update the "component" field of the mirror object with the  given "value"
     */
    synchronized void updateMirrorObject(int component, Object value) {

	//System.out.println("ShaderAttributeObjectRetained : updateMirrorObject");
	ShaderAttributeObjectRetained mirrorSAV = (ShaderAttributeObjectRetained)mirror;
        if ((component & SHADER_ATTRIBUTE_VALUE_UPDATE) != 0) {
	    //System.out.println("     -- SHADER_ATTRIBUTE_VALUE_UPDATE");
	    mirrorSAV.attrWrapper = (AttrWrapper) value;
	}
    }
    
    final void sendMessage(int attrMask, Object attr) {
	
	ArrayList univList = new ArrayList();
	ArrayList gaList = Shape3DRetained.getGeomAtomsList(mirror.users, univList);  

	// Send to rendering attribute structure, regardless of
	// whether there are users or not (alternate appearance case ..)
	J3dMessage createMessage = VirtualUniverse.mc.getMessage();
	createMessage.threads = J3dThread.UPDATE_RENDERING_ATTRIBUTES;
	createMessage.type = J3dMessage.SHADER_ATTRIBUTE_CHANGED;
	createMessage.universe = null;
	createMessage.args[0] = this;
	createMessage.args[1]= new Integer(attrMask);
	createMessage.args[2] = attr;
	//	System.out.println("changedFreqent1 = "+changedFrequent);
	createMessage.args[3] = new Integer(changedFrequent);
	VirtualUniverse.mc.processMessage(createMessage);

	// System.out.println("univList.size is " + univList.size());
	for(int i=0; i<univList.size(); i++) {
	    createMessage = VirtualUniverse.mc.getMessage();
	    createMessage.threads = J3dThread.UPDATE_RENDER;
	    createMessage.type = J3dMessage.SHADER_ATTRIBUTE_CHANGED;
		
	    createMessage.universe = (VirtualUniverse) univList.get(i);
	    createMessage.args[0] = this;
	    createMessage.args[1]= new Integer(attrMask);
	    createMessage.args[2] = attr;

	    ArrayList gL = (ArrayList)gaList.get(i);
	    GeometryAtom[] gaArr = new GeometryAtom[gL.size()];
	    gL.toArray(gaArr);
	    createMessage.args[3] = gaArr;  
	    
	    VirtualUniverse.mc.processMessage(createMessage);
	}

    }
        
        
    // Enumerated types representing allowed classes for shader
    // attributes.
    //
    // NOTE that the values for these enums are used as an index into
    // the tables of classes, so the values must start at 0 and
    // increment by 1. Also, the order must be the same as the order
    // of the entries in each of the two class tables.
    static final int TYPE_INTEGER  =  0;
    static final int TYPE_FLOAT    =  1;
    static final int TYPE_TUPLE2I  =  2;
    static final int TYPE_TUPLE2F  =  3;
    static final int TYPE_TUPLE3I  =  4;
    static final int TYPE_TUPLE3F  =  5;
    static final int TYPE_TUPLE4I  =  6;
    static final int TYPE_TUPLE4F  =  7;
    static final int TYPE_MATRIX3F =  8;
    static final int TYPE_MATRIX4F =  9;

    // Double-precision is not supported in the current version. Uncomment the
    // following if future support is done.
//    static final int TYPE_DOUBLE   = 10;
//    static final int TYPE_TUPLE2D  = 11;
//    static final int TYPE_TUPLE3D  = 12;
//    static final int TYPE_TUPLE4D  = 13;
//    static final int TYPE_MATRIX3D = 14;
//    static final int TYPE_MATRIX4D = 15;

    static final Class classTable[] = {
	Integer.class,
	Float.class,
	Tuple2i.class,
	Tuple2f.class,
	Tuple3i.class,
	Tuple3f.class,
	Tuple4i.class,
	Tuple4f.class,
	Matrix3f.class,
	Matrix4f.class,

        // Double-precision is not supported in the current version. Uncomment the
        // following if future support is done.
//	Double.class,
//	Tuple2d.class,
//	Tuple3d.class,
//	Tuple4d.class,
//	Matrix3d.class,
//	Matrix4d.class,
    };

    static final Class classTableArr[] = {
	Integer[].class,
	Float[].class,
	Tuple2i[].class,
	Tuple2f[].class,
	Tuple3i[].class,
	Tuple3f[].class,
	Tuple4i[].class,
	Tuple4f[].class,
	Matrix3f[].class,
	Matrix4f[].class,

        // Double-precision is not supported in the current version. Uncomment the
        // following if future support is done.
//	Double[].class,
//	Tuple2d[].class,
//	Tuple3d[].class,
//	Tuple4d[].class,
//	Matrix3d[].class,
//	Matrix4d[].class,
    };


    /**
     * Computes the base class from the specified object. A
     * ClassCastException is thrown if the object is not an instance
     * or array of one of the allowed classes.
     */
    abstract int computeClassType(Object value);

    /**
     * Returns the base class represented by the specified class type.
     */
    abstract Class getBaseClass(int classType);

    /**
     * Creates an attribute wrapper object of the specified class
     * type, and stores the specified object.
     */
    abstract AttrWrapper createAttrWrapper(Object value, int classType);


    /**
     * Base wrapper class for subclasses that are used to store a copy
     * of the user-specified shader attribute value. There is a
     * wrapper class for each supported base class in ShaderAttributeValue
     * and ShaderAttributeArray. The value is stored in a Java primitive array.
     */
    static abstract class AttrWrapper {
	/**
	 * Stores a copy of the specified object in the wrapper object
	 */
	abstract void set(Object value);

	/**
	 * Returns a copy of the wrapped object
	 */
	abstract Object get();

	/**
	 * Returns a reference to the internal primitive array used to
	 * wrap the object; note that the caller of this method must
	 * treat the data as read-only. It is intended only as a means
	 * to pass data down to native methods.
	 */
	abstract Object getRef();
    }

    int getClassType() {
        return classType;
    }

    void setClassType(int classType) {
        this.classType = classType;
    }

}
