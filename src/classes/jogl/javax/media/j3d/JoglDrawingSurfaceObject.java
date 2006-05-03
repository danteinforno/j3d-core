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

/**
 * The DrawingSurfaceObject class is used to manage native drawing surface 
 */
class JoglDrawingSurfaceObject extends DrawingSurfaceObject {
    
    JoglDrawingSurfaceObject(Canvas3D cv) {
        super(cv);
        
        System.err.println("JoglDrawingSurfaceObject constructed");
    }

    synchronized boolean renderLock() {
        System.err.println("JoglDrawingSurfaceObject.renderLock()");
        gotDsiLock = true;
        return true;
    }

    synchronized void unLock() {
        System.err.println("JoglDrawingSurfaceObject.unLock()");
        gotDsiLock = false;
    }

    synchronized void getDrawingSurfaceObjectInfo() {
        if (canvas.drawable == null) {
            System.err.println(
                    "JoglDrawingSurfaceObject.getDrawingSurfaceObjectInfo: window = "
                    + canvas.drawable);

            // TODO: replace with a real JoglDrawable
            canvas.drawable = new JoglDrawable();
        }
    }

    synchronized void invalidate() {
        System.err.println("JoglDrawingSurfaceObject.invalidate()");
    }

    /**
     * Dummy Jogl Drawable class.
     * TODO: wrap a GLdrawable
     */
    static class JoglDrawable implements Drawable {
       // TODO: implement this for real
    }
}
