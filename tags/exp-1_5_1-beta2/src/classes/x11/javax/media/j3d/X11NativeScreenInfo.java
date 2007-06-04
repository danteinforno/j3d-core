/*
 * $RCSfile$
 *
 * Copyright (c) 2007 Sun Microsystems, Inc. All rights reserved.
 *
 * Use is subject to license terms.
 *
 * $Revision$
 * $Date$
 * $State$
 */

package javax.media.j3d;

import java.awt.GraphicsDevice;
import sun.awt.X11GraphicsDevice;

/**
 * Native screen info class. A singleton instance of this class is created by
 * a factory method in the base class using reflection.
 */
class X11NativeScreenInfo extends NativeScreenInfo {
    private static long display = 0;
    private static boolean glxChecked = false;
    private static boolean isGLX13;

    private static native long openDisplay();
    private static native boolean queryGLX13(long display);

    X11NativeScreenInfo() {
    }

    // Fix for issue 20.
    // This method will return true if glx version is 1.3 or higher, 
    // else return false.
    static synchronized boolean isGLX13() {
	if (!glxChecked) {
	    // Open a new static display connection if one is not already opened.
	    getStaticDisplay();

            // Query for glx1.3 support.
	    isGLX13 = queryGLX13(getStaticDisplay());
	    glxChecked = true;
	}

	return isGLX13;
    }

    private static synchronized long getStaticDisplay() {
	if (display == 0) {
	    display = openDisplay();
	}
	return display;
    }

    @Override
    long getDisplay() {
	// Open a new static display connection if one is not already opened
        return getStaticDisplay();
    }

    @Override
    int getScreen(GraphicsDevice graphicsDevice) {
	// Get the screen number
	return ((X11GraphicsDevice)graphicsDevice).getScreen();
    }

    // Ensure that the native libraries are loaded
    static {
        VirtualUniverse.loadLibraries();
    }
}
