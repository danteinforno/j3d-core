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
	throw new RuntimeException("not implemented");
    }

    // Implement abstract setValue method
    public void setValue(Object value) {
	if (value == null) {
	    throw new NullPointerException();
	}

	throw new RuntimeException("not implemented");
    }

}
