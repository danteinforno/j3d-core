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

/**
 * The ShaderAttributeValue object encapsulates a uniform shader
 * attribute whose value is specified explicitly. The shader variable
 * <code>attrName</code> is explicitly set to the specified
 * <code>value</code> during rendering. <code>attrName</code> must be
 * the name of a valid uniform attribute in the shader in which it is
 * used. Otherwise, the attribute name will be ignored and a runtime
 * error may be generated. The <code>value</code> must be an instance
 * of one of the allowed classes. The allowed classes are:
 * <code>Integer</code>, <code>Float</code>, <code>Double</code>,
 * <code>Tuple{2,3,4}{i,f,d}</code>, <code>Matrix{3,4}{f,d}</code>. A
 * ClassCastException will be thrown if a specified <code>value</code>
 * object is not one of the allowed types. Further, the type of the
 * value is immutable once a ShaderAttributeValue is constructed.
 * Subsequent setValue operations must be called with an object of the
 * same type as the one that was used to construct the
 * ShaderAttributeValue. Finally, the type of the <code>value</code>
 * object must match the type of the corresponding
 * <code>attrName</code> variable in the shader in which it is
 * used. Otherwise, the shader will not be able to use the attribute
 * and a runtime error may be generated.
 *
 * @see ShaderAttributeSet
 * @see ShaderProgram
 *
 * @since Java 3D 1.4
 */

public class ShaderAttributeValue extends ShaderAttributeObject {
    /**
     * Constructs a new ShaderAttributeValue object with the specified
     * <code>(attrName,&nbsp;value)</code> pair. If value is a mutable
     * object (for example, is an instance of a
     * <code>javax.vecmath</code> class), a copy of the object is
     * stored.
     *
     * @param attrName the name of the shader attribute
     * @param value the value of the shader attribute
     *
     * @exception NullPointerException if attrName or value is null
     *
     * @exception ClassCastException if value is not an instance of
     * one of the allowed classes
     */
    public ShaderAttributeValue(String attrName, Object value) {
	super(attrName, value);
    }

    // Implement abstract getValue method
    public Object getValue() {
	return attrWrapper.get();
    }

    // Implement abstract setValue method
    public void setValue(Object value) {
	if (value == null) {
	    throw new NullPointerException();
	}

	attrWrapper.set(value);
    }

    /**
     * Computes the base class from the specified object. A
     * ClassCastException is thrown if the object is not an instance
     * of one of the allowed classes.
     */
    int computeClassType(Object value) {
	Class objClass = value.getClass();
	if (objClass.isArray()) {
	    throw new ClassCastException(objClass + " -- array class not allowed");
	}

	for (int i = 0; i < classTable.length; i++) {
	    if (classTable[i].isInstance(value)) {
		return i;
	    }
	}
	throw new ClassCastException(objClass + " -- unrecognized class");
    }

    /**
     * Returns the base class represented by the specified class type.
     */
    Class getBaseClass(int classType) {
	return classTable[classType];
    }

    /**
     * Creates an attribute wrapper object of the specified class
     * type, and stores the specified object.
     */
    AttrWrapper createAttrWrapper(Object value, int classType) {
	ValueWrapper attrWrapper = null;
	switch (classType) {
	case TYPE_INTEGER:
	    attrWrapper = new IntegerWrapper();
	    break;
	case TYPE_FLOAT:
	    attrWrapper = new FloatWrapper();
	    break;
	case TYPE_DOUBLE:
	    attrWrapper = new DoubleWrapper();
	    break;
	case TYPE_TUPLE2I:
	    attrWrapper = new Tuple2iWrapper();
	    break;
	case TYPE_TUPLE2F:
	    attrWrapper = new Tuple2fWrapper();
	    break;
	case TYPE_TUPLE2D:
	    attrWrapper = new Tuple2dWrapper();
	    break;
	case TYPE_TUPLE3I:
	    attrWrapper = new Tuple3iWrapper();
	    break;
	case TYPE_TUPLE3F:
	    attrWrapper = new Tuple3fWrapper();
	    break;
	case TYPE_TUPLE3D:
	    attrWrapper = new Tuple3dWrapper();
	    break;
	case TYPE_TUPLE4I:
	    attrWrapper = new Tuple4iWrapper();
	    break;
	case TYPE_TUPLE4F:
	    attrWrapper = new Tuple4fWrapper();
	    break;
	case TYPE_TUPLE4D:
	    attrWrapper = new Tuple4dWrapper();
	    break;
	case TYPE_MATRIX3F:
	    attrWrapper = new Matrix3fWrapper();
	    break;
	case TYPE_MATRIX3D:
	    attrWrapper = new Matrix3dWrapper();
	    break;
	case TYPE_MATRIX4F:
	    attrWrapper = new Matrix4fWrapper();
	    break;
	case TYPE_MATRIX4D:
	    attrWrapper = new Matrix4dWrapper();
	    break;
	default:
	    // Should never get here
	    assert(false);
	    return null;
	}

	attrWrapper.set(value);
	return attrWrapper;
    }

    //
    // The following wrapper classes are used to store a copy of the
    // user-specified shader attribute value. There is a wrapper class
    // for each supported base class.
    //

    // Base wrapper class for non-array attribute types
    static abstract class ValueWrapper extends AttrWrapper {
	// No additional fields or methods are defined in this class
    }

    // Wrapper class for Integer
    static class IntegerWrapper extends ValueWrapper {
	private Integer value;

	void set(Object value) {
	    // Since Integer is immutable we can just store the reference
	    this.value = (Integer)value;
	}

	Object get() {
	    // Since Integer is immutable we can just return the reference
	    return this.value;
	}

	Object getRef() {
	    return this.value;
	}
    }

    // Wrapper class for Float
    static class FloatWrapper extends ValueWrapper {
	private Float value;

	void set(Object value) {
	    // Since Float is immutable we can just store the reference
	    this.value = (Float)value;
	}

	Object get() {
	    // Since Float is immutable we can just return the reference
	    return this.value;
	}

	Object getRef() {
	    return this.value;
	}
    }

    // Wrapper class for Double
    static class DoubleWrapper extends ValueWrapper {
	private Double value;

	void set(Object value) {
	    // Since Double is immutable we can just store the reference
	    this.value = (Double)value;
	}

	Object get() {
	    // Since Double is immutable we can just return the reference
	    return this.value;
	}

	Object getRef() {
	    return this.value;
	}
    }

    // Wrapper class for Tuple2i
    static class Tuple2iWrapper extends ValueWrapper {
	private Tuple2i value = new Point2i();

	void set(Object value) {
	    // Since Tuple2i is mutable we must copy the data
	    this.value.set((Tuple2i)value);
	}

	Object get() {
	    // Since Tuple2i is mutable we must return a copy of the data
	    return this.value.clone();
	}

	Object getRef() {
	    return this.value;
	}
    }

    // Wrapper class for Tuple2f
    static class Tuple2fWrapper extends ValueWrapper {
	private Tuple2f value = new Point2f();

	void set(Object value) {
	    // Since Tuple2f is mutable we must copy the data
	    this.value.set((Tuple2f)value);
	}

	Object get() {
	    // Since Tuple2f is mutable we must return a copy of the data
	    return this.value.clone();
	}

	Object getRef() {
	    return this.value;
	}
    }

    // Wrapper class for Tuple2d
    static class Tuple2dWrapper extends ValueWrapper {
	private Tuple2d value = new Point2d();

	void set(Object value) {
	    // Since Tuple2d is mutable we must copy the data
	    this.value.set((Tuple2d)value);
	}

	Object get() {
	    // Since Tuple2d is mutable we must return a copy of the data
	    return this.value.clone();
	}

	Object getRef() {
	    return this.value;
	}
    }

    // Wrapper class for Tuple3i
    static class Tuple3iWrapper extends ValueWrapper {
	private Tuple3i value = new Point3i();

	void set(Object value) {
	    // Since Tuple3i is mutable we must copy the data
	    this.value.set((Tuple3i)value);
	}

	Object get() {
	    // Since Tuple3i is mutable we must return a copy of the data
	    return this.value.clone();
	}

	Object getRef() {
	    return this.value;
	}
    }

    // Wrapper class for Tuple3f
    static class Tuple3fWrapper extends ValueWrapper {
	private Tuple3f value = new Point3f();

	void set(Object value) {
	    // Since Tuple3f is mutable we must copy the data
	    this.value.set((Tuple3f)value);
	}

	Object get() {
	    // Since Tuple3f is mutable we must return a copy of the data
	    return this.value.clone();
	}

	Object getRef() {
	    return this.value;
	}
    }

    // Wrapper class for Tuple3d
    static class Tuple3dWrapper extends ValueWrapper {
	private Tuple3d value = new Point3d();

	void set(Object value) {
	    // Since Tuple3d is mutable we must copy the data
	    this.value.set((Tuple3d)value);
	}

	Object get() {
	    // Since Tuple3d is mutable we must return a copy of the data
	    return this.value.clone();
	}

	Object getRef() {
	    return this.value;
	}
    }

    // Wrapper class for Tuple4i
    static class Tuple4iWrapper extends ValueWrapper {
	private Tuple4i value = new Point4i();

	void set(Object value) {
	    // Since Tuple4i is mutable we must copy the data
	    this.value.set((Tuple4i)value);
	}

	Object get() {
	    // Since Tuple4i is mutable we must return a copy of the data
	    return this.value.clone();
	}

	Object getRef() {
	    return this.value;
	}
    }

    // Wrapper class for Tuple4f
    static class Tuple4fWrapper extends ValueWrapper {
	private Tuple4f value = new Point4f();

	void set(Object value) {
	    // Since Tuple4f is mutable we must copy the data
	    this.value.set((Tuple4f)value);
	}

	Object get() {
	    // Since Tuple4f is mutable we must return a copy of the data
	    return this.value.clone();
	}

	Object getRef() {
	    return this.value;
	}
    }

    // Wrapper class for Tuple4d
    static class Tuple4dWrapper extends ValueWrapper {
	private Tuple4d value = new Point4d();

	void set(Object value) {
	    // Since Tuple4d is mutable we must copy the data
	    this.value.set((Tuple4d)value);
	}

	Object get() {
	    // Since Tuple4d is mutable we must return a copy of the data
	    return this.value.clone();
	}

	Object getRef() {
	    return this.value;
	}
    }

    // Wrapper class for Matrix3f
    static class Matrix3fWrapper extends ValueWrapper {
	private Matrix3f value = new Matrix3f();

	void set(Object value) {
	    // Since Matrix3f is mutable we must copy the data
	    this.value.set((Matrix3f)value);
	}

	Object get() {
	    // Since Matrix3f is mutable we must return a copy of the data
	    return this.value.clone();
	}

	Object getRef() {
	    return this.value;
	}
    }

    // Wrapper class for Matrix3d
    static class Matrix3dWrapper extends ValueWrapper {
	private Matrix3d value = new Matrix3d();

	void set(Object value) {
	    // Since Matrix3d is mutable we must copy the data
	    this.value.set((Matrix3d)value);
	}

	Object get() {
	    // Since Matrix3d is mutable we must return a copy of the data
	    return this.value.clone();
	}

	Object getRef() {
	    return this.value;
	}
    }

    // Wrapper class for Matrix4f
    static class Matrix4fWrapper extends ValueWrapper {
	private Matrix4f value = new Matrix4f();

	void set(Object value) {
	    // Since Matrix4f is mutable we must copy the data
	    this.value.set((Matrix4f)value);
	}

	Object get() {
	    // Since Matrix4f is mutable we must return a copy of the data
	    return this.value.clone();
	}

	Object getRef() {
	    return this.value;
	}
    }

    // Wrapper class for Matrix4d
    static class Matrix4dWrapper extends ValueWrapper {
	private Matrix4d value = new Matrix4d();

	void set(Object value) {
	    // Since Matrix4d is mutable we must copy the data
	    this.value.set((Matrix4d)value);
	}

	Object get() {
	    // Since Matrix4d is mutable we must return a copy of the data
	    return this.value.clone();
	}

	Object getRef() {
	    return this.value;
	}
    }

}
