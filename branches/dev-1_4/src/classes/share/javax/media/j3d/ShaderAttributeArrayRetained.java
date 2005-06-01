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
 * The ShaderAttributeArray object encapsulates a uniform shader
 * attribute whose value is specified explicitly. The shader variable
 * <code>attrName</code> is explicitly set to the specified
 * <code>value</code> during rendering. <code>attrName</code> must be
 * the name of a valid uniform attribute in the shader in which it is
 * used. Otherwise, the attribute name will be ignored and a runtime
 * error may be generated. The <code>value</code> must be an array
 * of one of the allowed classes. The allowed classes are:
 * <code>Integer[]</code>, <code>Float[]</code>, <code>Double[]</code>,
 * <code>Tuple{2,3,4}{i,f,d}[]</code>, <code>Matrix{3,4}{f,d}[]</code>. A
 * ClassCastException will be thrown if a specified <code>value</code>
 * object is not one of the allowed types. Further, the type and length of the
 * value is immutable once a ShaderAttributeArray is constructed.
 * Subsequent setValue operations must be called with an array of the
 * same type and length as the one that was used to construct the
 * ShaderAttributeArray. Finally, the type of the <code>value</code>
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

class ShaderAttributeArrayRetained extends ShaderAttributeObjectRetained {

    ShaderAttributeArrayRetained() {
    }

    /**
     * Sets the specified array element of the value of this shader
     * attribute to the specified value.
     * A copy of the object is stored.
     *
     * @param value the new value of the shader attribute
     *
     * @exception NullPointerException if value is null
     *
     * @exception ClassCastException if value is not an instance of
     * the same base class as the individual elements of the array object
     * used to construct this shader attribute object.
     *
     * @exception CapabilityNotSetException if appropriate capability is 
     * not set and this object is part of live or compiled scene graph
     */
    void setValue(int index, Object value) {
	if (value == null) {
	    throw new NullPointerException();
	}

	((ArrayWrapper)attrWrapper).set(index, value);
    }

    /**
     * Returns the number of elements in the value array.
     *
     * @return the number of elements in the value array
     *
     * @exception CapabilityNotSetException if appropriate capability is 
     * not set and this object is part of live or compiled scene graph
     */
    int length() {
	return ((ArrayWrapper)attrWrapper).length();

    }

    // Helper methods ...



    /**
     * Computes the base class from the specified object. A
     * ClassCastException is thrown if the object is not an array of
     * one of the allowed classes.
     */
    int computeClassType(Object value) {
	Class objClass = value.getClass();
	if (!objClass.isArray()) {
	    throw new ClassCastException(objClass + " -- must be array class");
	}

	for (int i = 0; i < classTable.length; i++) {
	    if (classTableArr[i].isInstance(value)) {
		return i;
	    }
	}
	throw new ClassCastException(objClass + " -- unrecognized class");
    }

    /**
     * Returns the base class represented by the specified class type.
     */
    Class getBaseClass(int classType) {
	return classTableArr[classType];
    }

    /**
     * Creates an attribute wrapper object of the specified class
     * type, and stores the specified array of objects.
     */
    AttrWrapper createAttrWrapper(Object value, int classType) {
	ArrayWrapper attrWrapper = null;
	switch (classType) {
	case TYPE_INTEGER:
	    attrWrapper = new IntegerArrayWrapper();
	    break;
	case TYPE_FLOAT:
	    attrWrapper = new FloatArrayWrapper();
	    break;
	case TYPE_DOUBLE:
	    throw new RuntimeException("not implemented");
	    /*
	    attrWrapper = new DoubleArrayWrapper();
	    break;
	    */
	case TYPE_TUPLE2I:
	    attrWrapper = new Tuple2iArrayWrapper();
	    break;
	case TYPE_TUPLE2F:
	    attrWrapper = new Tuple2fArrayWrapper();
	    break;
	case TYPE_TUPLE2D:
	    throw new RuntimeException("not implemented");
	    /*
	    attrWrapper = new Tuple2dArrayWrapper();
	    break;
	    */
	case TYPE_TUPLE3I:
	    attrWrapper = new Tuple3iArrayWrapper();
	    break;
	case TYPE_TUPLE3F:
	    attrWrapper = new Tuple3fArrayWrapper();
	    break;
	case TYPE_TUPLE3D:
	    throw new RuntimeException("not implemented");
	    /*
	    attrWrapper = new Tuple3dArrayWrapper();
	    break;
	    */
	case TYPE_TUPLE4I:
	    attrWrapper = new Tuple4iArrayWrapper();
	    break;
	case TYPE_TUPLE4F:
	    attrWrapper = new Tuple4fArrayWrapper();
	    break;
	case TYPE_TUPLE4D:
	    throw new RuntimeException("not implemented");
	    /*
	    attrWrapper = new Tuple4dArrayWrapper();
	    break;
	    */
	case TYPE_MATRIX3F:
	    attrWrapper = new Matrix3fArrayWrapper();
	    break;
	case TYPE_MATRIX3D:
	    throw new RuntimeException("not implemented");
	    /*
	    attrWrapper = new Matrix3dArrayWrapper();
	    break;
	    */
	case TYPE_MATRIX4F:
	    attrWrapper = new Matrix4fArrayWrapper();
	    break;
	case TYPE_MATRIX4D:
	    throw new RuntimeException("not implemented");
	    /*
	    attrWrapper = new Matrix4dArrayWrapper();
	    break;
	    */
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

    // Base wrapper class for array attribute types
    static abstract class ArrayWrapper extends AttrWrapper {
	/**
	 * Returns the length of the array
	 */
	abstract int length();

	/**
	 * Sets the specified array element of the value of this
	 * shader attribute to the specified value.
	 */
	abstract void set(int index, Object value);
    }

    // Wrapper class for Integer
    static class IntegerArrayWrapper extends ArrayWrapper {
	private Integer[] value = new Integer[0];

	void set(Object value) {
	    // Since Integer is immutable we can just copy the references
	    Integer[] arr = (Integer[])value;
	    if (this.value.length != arr.length) {
		this.value = new Integer[arr.length];
	    }
	    System.arraycopy(arr, 0, this.value, 0, arr.length);
	}

	void set(int index, Object value) {
	    // Since Integer is immutable we can just copy the reference
	    this.value[index] = (Integer)value;
	}

	Object get() {
	    // Since Integer is immutable we can just return the references
	    Integer[] arr = new Integer[this.value.length];
	    System.arraycopy(this.value, 0, arr, 0, this.value.length);
	    return arr;
	}

	int length() {
	    return this.value.length;
	}

	Object getRef() {
	    return this.value;
	}
    }

    // Wrapper class for Float
    static class FloatArrayWrapper extends ArrayWrapper {
	private Float[] value = new Float[0];

	void set(Object value) {
	    // Since Float is immutable we can just copy the references
	    Float[] arr = (Float[])value;
	    if (this.value.length != arr.length) {
		this.value = new Float[arr.length];
	    }
	    System.arraycopy(arr, 0, this.value, 0, arr.length);
	}

	void set(int index, Object value) {
	    // Since Float is immutable we can just copy the reference
	    this.value[index] = (Float)value;
	}

	Object get() {
	    // Since Float is immutable we can just return the references
	    Float[] arr = new Float[this.value.length];
	    System.arraycopy(this.value, 0, arr, 0, this.value.length);
	    return arr;
	}

	int length() {
	    return this.value.length;
	}

	Object getRef() {
	    return this.value;
	}
    }

    // Wrapper class for Double
    static class DoubleArrayWrapper extends ArrayWrapper {
	private Double[] value = new Double[0];

	void set(Object value) {
	    // Since Double is immutable we can just copy the references
	    Double[] arr = (Double[])value;
	    if (this.value.length != arr.length) {
		this.value = new Double[arr.length];
	    }
	    System.arraycopy(arr, 0, this.value, 0, arr.length);
	}

	void set(int index, Object value) {
	    // Since Double is immutable we can just copy the reference
	    this.value[index] = (Double)value;
	}

	Object get() {
	    // Since Double is immutable we can just return the references
	    Double[] arr = new Double[this.value.length];
	    System.arraycopy(this.value, 0, arr, 0, this.value.length);
	    return arr;
	}

	int length() {
	    return this.value.length;
	}

	Object getRef() {
	    return this.value;
	}
    }

    // Wrapper class for Tuple2i
    static class Tuple2iArrayWrapper extends ArrayWrapper {
	private Tuple2i[] value = new Tuple2i[0];

	void set(Object value) {
	    // Since Tuple2i is mutable we must copy the data for each element
	    Tuple2i[] arr = (Tuple2i[])value;
	    int i;
	    if (this.value.length != arr.length) {
		this.value = new Tuple2i[arr.length];
		for (i = 0; i < arr.length; i++) {
		    this.value[i] = new Point2i();
		}
	    }
	    for (i = 0; i < arr.length; i++) {
		this.value[i].set(arr[i]);
	    }
	}

	void set(int index, Object value) {
	    // Since Tuple2i is mutable we must copy the data
	    this.value[index].set((Tuple2i)value);
	}

	Object get() {
	    // Since Tuple2i is immutable we must return a copy of the data
	    Tuple2i[] arr = new Tuple2i[this.value.length];
	    for (int i = 0; i < this.value.length; i++) {
		arr[i] = (Tuple2i)this.value[i].clone();
	    }
	    return arr;
	}

	int length() {
	    return this.value.length;
	}

	Object getRef() {
	    return this.value;
	}
    }

    // Wrapper class for Tuple2f
    static class Tuple2fArrayWrapper extends ArrayWrapper {
	private Tuple2f[] value = new Tuple2f[0];

	void set(Object value) {
	    // Since Tuple2f is mutable we must copy the data for each element
	    Tuple2f[] arr = (Tuple2f[])value;
	    int i;
	    if (this.value.length != arr.length) {
		this.value = new Tuple2f[arr.length];
		for (i = 0; i < arr.length; i++) {
		    this.value[i] = new Point2f();
		}
	    }
	    for (i = 0; i < arr.length; i++) {
		this.value[i].set(arr[i]);
	    }
	}

	void set(int index, Object value) {
	    // Since Tuple2f is mutable we must copy the data
	    this.value[index].set((Tuple2f)value);
	}

	Object get() {
	    // Since Tuple2f is immutable we must return a copy of the data
	    Tuple2f[] arr = new Tuple2f[this.value.length];
	    for (int i = 0; i < this.value.length; i++) {
		arr[i] = (Tuple2f)this.value[i].clone();
	    }
	    return arr;
	}

	int length() {
	    return this.value.length;
	}

	Object getRef() {
	    return this.value;
	}
    }

    // Wrapper class for Tuple2d
    static class Tuple2dArrayWrapper extends ArrayWrapper {
	private Tuple2d[] value = new Tuple2d[0];

	void set(Object value) {
	    // Since Tuple2d is mutable we must copy the data for each element
	    Tuple2d[] arr = (Tuple2d[])value;
	    int i;
	    if (this.value.length != arr.length) {
		this.value = new Tuple2d[arr.length];
		for (i = 0; i < arr.length; i++) {
		    this.value[i] = new Point2d();
		}
	    }
	    for (i = 0; i < arr.length; i++) {
		this.value[i].set(arr[i]);
	    }
	}

	void set(int index, Object value) {
	    // Since Tuple2d is mutable we must copy the data
	    this.value[index].set((Tuple2d)value);
	}

	Object get() {
	    // Since Tuple2d is immutable we must return a copy of the data
	    Tuple2d[] arr = new Tuple2d[this.value.length];
	    for (int i = 0; i < this.value.length; i++) {
		arr[i] = (Tuple2d)this.value[i].clone();
	    }
	    return arr;
	}

	int length() {
	    return this.value.length;
	}

	Object getRef() {
	    return this.value;
	}
    }

    // Wrapper class for Tuple3i
    static class Tuple3iArrayWrapper extends ArrayWrapper {
	private Tuple3i[] value = new Tuple3i[0];

	void set(Object value) {
	    // Since Tuple3i is mutable we must copy the data for each element
	    Tuple3i[] arr = (Tuple3i[])value;
	    int i;
	    if (this.value.length != arr.length) {
		this.value = new Tuple3i[arr.length];
		for (i = 0; i < arr.length; i++) {
		    this.value[i] = new Point3i();
		}
	    }
	    for (i = 0; i < arr.length; i++) {
		this.value[i].set(arr[i]);
	    }
	}

	void set(int index, Object value) {
	    // Since Tuple3i is mutable we must copy the data
	    this.value[index].set((Tuple3i)value);
	}

	Object get() {
	    // Since Tuple3i is immutable we must return a copy of the data
	    Tuple3i[] arr = new Tuple3i[this.value.length];
	    for (int i = 0; i < this.value.length; i++) {
		arr[i] = (Tuple3i)this.value[i].clone();
	    }
	    return arr;
	}

	int length() {
	    return this.value.length;
	}

	Object getRef() {
	    return this.value;
	}
    }

    // Wrapper class for Tuple3f
    static class Tuple3fArrayWrapper extends ArrayWrapper {
	private Tuple3f[] value = new Tuple3f[0];

	void set(Object value) {
	    // Since Tuple3f is mutable we must copy the data for each element
	    Tuple3f[] arr = (Tuple3f[])value;
	    int i;
	    if (this.value.length != arr.length) {
		this.value = new Tuple3f[arr.length];
		for (i = 0; i < arr.length; i++) {
		    this.value[i] = new Point3f();
		}
	    }
	    for (i = 0; i < arr.length; i++) {
		this.value[i].set(arr[i]);
	    }
	}

	void set(int index, Object value) {
	    // Since Tuple3f is mutable we must copy the data
	    this.value[index].set((Tuple3f)value);
	}

	Object get() {
	    // Since Tuple3f is immutable we must return a copy of the data
	    Tuple3f[] arr = new Tuple3f[this.value.length];
	    for (int i = 0; i < this.value.length; i++) {
		arr[i] = (Tuple3f)this.value[i].clone();
	    }
	    return arr;
	}

	int length() {
	    return this.value.length;
	}

	Object getRef() {
	    return this.value;
	}
    }

    // Wrapper class for Tuple3d
    static class Tuple3dArrayWrapper extends ArrayWrapper {
	private Tuple3d[] value = new Tuple3d[0];

	void set(Object value) {
	    // Since Tuple3d is mutable we must copy the data for each element
	    Tuple3d[] arr = (Tuple3d[])value;
	    int i;
	    if (this.value.length != arr.length) {
		this.value = new Tuple3d[arr.length];
		for (i = 0; i < arr.length; i++) {
		    this.value[i] = new Point3d();
		}
	    }
	    for (i = 0; i < arr.length; i++) {
		this.value[i].set(arr[i]);
	    }
	}

	void set(int index, Object value) {
	    // Since Tuple3d is mutable we must copy the data
	    this.value[index].set((Tuple3d)value);
	}

	Object get() {
	    // Since Tuple3d is immutable we must return a copy of the data
	    Tuple3d[] arr = new Tuple3d[this.value.length];
	    for (int i = 0; i < this.value.length; i++) {
		arr[i] = (Tuple3d)this.value[i].clone();
	    }
	    return arr;
	}

	int length() {
	    return this.value.length;
	}

	Object getRef() {
	    return this.value;
	}
    }

    // Wrapper class for Tuple4i
    static class Tuple4iArrayWrapper extends ArrayWrapper {
	private Tuple4i[] value = new Tuple4i[0];

	void set(Object value) {
	    // Since Tuple4i is mutable we must copy the data for each element
	    Tuple4i[] arr = (Tuple4i[])value;
	    int i;
	    if (this.value.length != arr.length) {
		this.value = new Tuple4i[arr.length];
		for (i = 0; i < arr.length; i++) {
		    this.value[i] = new Point4i();
		}
	    }
	    for (i = 0; i < arr.length; i++) {
		this.value[i].set(arr[i]);
	    }
	}

	void set(int index, Object value) {
	    // Since Tuple4i is mutable we must copy the data
	    this.value[index].set((Tuple4i)value);
	}

	Object get() {
	    // Since Tuple4i is immutable we must return a copy of the data
	    Tuple4i[] arr = new Tuple4i[this.value.length];
	    for (int i = 0; i < this.value.length; i++) {
		arr[i] = (Tuple4i)this.value[i].clone();
	    }
	    return arr;
	}

	int length() {
	    return this.value.length;
	}

	Object getRef() {
	    return this.value;
	}
    }

    // Wrapper class for Tuple4f
    static class Tuple4fArrayWrapper extends ArrayWrapper {
	private Tuple4f[] value = new Tuple4f[0];

	void set(Object value) {
	    // Since Tuple4f is mutable we must copy the data for each element
	    Tuple4f[] arr = (Tuple4f[])value;
	    int i;
	    if (this.value.length != arr.length) {
		this.value = new Tuple4f[arr.length];
		for (i = 0; i < arr.length; i++) {
		    this.value[i] = new Point4f();
		}
	    }
	    for (i = 0; i < arr.length; i++) {
		this.value[i].set(arr[i]);
	    }
	}

	void set(int index, Object value) {
	    // Since Tuple4f is mutable we must copy the data
	    this.value[index].set((Tuple4f)value);
	}

	Object get() {
	    // Since Tuple4f is immutable we must return a copy of the data
	    Tuple4f[] arr = new Tuple4f[this.value.length];
	    for (int i = 0; i < this.value.length; i++) {
		arr[i] = (Tuple4f)this.value[i].clone();
	    }
	    return arr;
	}

	int length() {
	    return this.value.length;
	}

	Object getRef() {
	    return this.value;
	}
    }

    // Wrapper class for Tuple4d
    static class Tuple4dArrayWrapper extends ArrayWrapper {
	private Tuple4d[] value = new Tuple4d[0];

	void set(Object value) {
	    // Since Tuple4d is mutable we must copy the data for each element
	    Tuple4d[] arr = (Tuple4d[])value;
	    int i;
	    if (this.value.length != arr.length) {
		this.value = new Tuple4d[arr.length];
		for (i = 0; i < arr.length; i++) {
		    this.value[i] = new Point4d();
		}
	    }
	    for (i = 0; i < arr.length; i++) {
		this.value[i].set(arr[i]);
	    }
	}

	void set(int index, Object value) {
	    // Since Tuple4d is mutable we must copy the data
	    this.value[index].set((Tuple4d)value);
	}

	Object get() {
	    // Since Tuple4d is immutable we must return a copy of the data
	    Tuple4d[] arr = new Tuple4d[this.value.length];
	    for (int i = 0; i < this.value.length; i++) {
		arr[i] = (Tuple4d)this.value[i].clone();
	    }
	    return arr;
	}

	int length() {
	    return this.value.length;
	}

	Object getRef() {
	    return this.value;
	}
    }

    // Wrapper class for Matrix3f
    static class Matrix3fArrayWrapper extends ArrayWrapper {
	private Matrix3f[] value = new Matrix3f[0];

	void set(Object value) {
	    // Since Matrix3f is mutable we must copy the data for each element
	    Matrix3f[] arr = (Matrix3f[])value;
	    int i;
	    if (this.value.length != arr.length) {
		this.value = new Matrix3f[arr.length];
		for (i = 0; i < arr.length; i++) {
		    this.value[i] = new Matrix3f();
		}
	    }
	    for (i = 0; i < arr.length; i++) {
		this.value[i].set(arr[i]);
	    }
	}

	void set(int index, Object value) {
	    // Since Matrix3f is mutable we must copy the data
	    this.value[index].set((Matrix3f)value);
	}

	Object get() {
	    // Since Matrix3f is immutable we must return a copy of the data
	    Matrix3f[] arr = new Matrix3f[this.value.length];
	    for (int i = 0; i < this.value.length; i++) {
		arr[i] = (Matrix3f)this.value[i].clone();
	    }
	    return arr;
	}

	int length() {
	    return this.value.length;
	}

	Object getRef() {
	    return this.value;
	}
    }

    // Wrapper class for Matrix3d
    static class Matrix3dArrayWrapper extends ArrayWrapper {
	private Matrix3d[] value = new Matrix3d[0];

	void set(Object value) {
	    // Since Matrix3d is mutable we must copy the data for each element
	    Matrix3d[] arr = (Matrix3d[])value;
	    int i;
	    if (this.value.length != arr.length) {
		this.value = new Matrix3d[arr.length];
		for (i = 0; i < arr.length; i++) {
		    this.value[i] = new Matrix3d();
		}
	    }
	    for (i = 0; i < arr.length; i++) {
		this.value[i].set(arr[i]);
	    }
	}

	void set(int index, Object value) {
	    // Since Matrix3d is mutable we must copy the data
	    this.value[index].set((Matrix3d)value);
	}

	Object get() {
	    // Since Matrix3d is immutable we must return a copy of the data
	    Matrix3d[] arr = new Matrix3d[this.value.length];
	    for (int i = 0; i < this.value.length; i++) {
		arr[i] = (Matrix3d)this.value[i].clone();
	    }
	    return arr;
	}

	int length() {
	    return this.value.length;
	}

	Object getRef() {
	    return this.value;
	}
    }

    // Wrapper class for Matrix4f
    static class Matrix4fArrayWrapper extends ArrayWrapper {
	private Matrix4f[] value = new Matrix4f[0];

	void set(Object value) {
	    // Since Matrix4f is mutable we must copy the data for each element
	    Matrix4f[] arr = (Matrix4f[])value;
	    int i;
	    if (this.value.length != arr.length) {
		this.value = new Matrix4f[arr.length];
		for (i = 0; i < arr.length; i++) {
		    this.value[i] = new Matrix4f();
		}
	    }
	    for (i = 0; i < arr.length; i++) {
		this.value[i].set(arr[i]);
	    }
	}

	void set(int index, Object value) {
	    // Since Matrix4f is mutable we must copy the data
	    this.value[index].set((Matrix4f)value);
	}

	Object get() {
	    // Since Matrix4f is immutable we must return a copy of the data
	    Matrix4f[] arr = new Matrix4f[this.value.length];
	    for (int i = 0; i < this.value.length; i++) {
		arr[i] = (Matrix4f)this.value[i].clone();
	    }
	    return arr;
	}

	int length() {
	    return this.value.length;
	}

	Object getRef() {
	    return this.value;
	}
    }

    // Wrapper class for Matrix4d
    static class Matrix4dArrayWrapper extends ArrayWrapper {
	private Matrix4d[] value = new Matrix4d[0];

	void set(Object value) {
	    // Since Matrix4d is mutable we must copy the data for each element
	    Matrix4d[] arr = (Matrix4d[])value;
	    int i;
	    if (this.value.length != arr.length) {
		this.value = new Matrix4d[arr.length];
		for (i = 0; i < arr.length; i++) {
		    this.value[i] = new Matrix4d();
		}
	    }
	    for (i = 0; i < arr.length; i++) {
		this.value[i].set(arr[i]);
	    }
	}

	void set(int index, Object value) {
	    // Since Matrix4d is mutable we must copy the data
	    this.value[index].set((Matrix4d)value);
	}

	Object get() {
	    // Since Matrix4d is immutable we must return a copy of the data
	    Matrix4d[] arr = new Matrix4d[this.value.length];
	    for (int i = 0; i < this.value.length; i++) {
		arr[i] = (Matrix4d)this.value[i].clone();
	    }
	    return arr;
	}

	int length() {
	    return this.value.length;
	}

	Object getRef() {
	    return this.value;
	}
    }

}
