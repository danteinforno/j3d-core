/*
 * $RCSfile$
 *
 * Copyright (c) 2006 Sun Microsystems, Inc. All rights reserved.
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

class NativeScreenInfo {
    private int screen;
    private static long display = 0;
    private static boolean glxChecked = false;
    private static boolean isGLX13;

    private static native long openDisplay();
    private static native int getDefaultScreen(long display);
    private static native boolean queryGLX13(long display);

    // Fix for issue 20.
    // This method will return true if glx version is 1.3 or higher, 
    // else return false.
    synchronized static boolean isGLX13() {
	if (!glxChecked) {
	    // Open a new static display connection if one is not already opened.
	    getStaticDisplay();
	    // Query for glx1.3 support.
	    isGLX13 = queryGLX13(display);
	    glxChecked = true;
	}

	return isGLX13;
    }

    synchronized static long getStaticDisplay() {
	if (display == 0) {
	    display = openDisplay();
	}
	return display;
    }

    NativeScreenInfo(GraphicsDevice graphicsDevice) {
	// Open a new static display connection if one is not already opened
	getStaticDisplay();

	// Get the screen number
	screen = ((X11GraphicsDevice)graphicsDevice).getScreen();
    }

    long getDisplay() {
	return display;
    }

    int getScreen() {
	return screen;
    }

    // Ensure that the native libraries are loaded
    static {
 	VirtualUniverse.loadLibraries();
    }
}