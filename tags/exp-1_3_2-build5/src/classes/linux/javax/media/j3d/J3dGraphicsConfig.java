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

/*
 * Portions of this code were derived from work done by the Blackdown
 * group (www.blackdown.org), who did the initial Linux implementation
 * of the Java 3D API.
 */

package javax.media.j3d;

import sun.awt.*;
import java.awt.*;

class J3dGraphicsConfig {

    static native boolean isValidVisualID(long display, int vid);

    J3dGraphicsConfig(GraphicsDevice gd, int pixelFormat) {
	// a dummy class which this constructor should
	// never invoke under Linux
    }
    
    static boolean isValidPixelFormat(GraphicsConfiguration gc) {
	return isValidVisualID(NativeScreenInfo.getStaticDisplay(),
			       ((X11GraphicsConfig) gc).getVisual());
    }

    static boolean isValidConfig(GraphicsConfiguration gc) {
	// Check to see if a valid visInfo pointer has been cached.
	Object visInfoObject = Canvas3D.visInfoTable.get(gc);
	return (visInfoObject != null) && (visInfoObject instanceof Long);
    }
}