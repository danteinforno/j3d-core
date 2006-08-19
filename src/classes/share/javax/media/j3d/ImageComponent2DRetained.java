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

import java.awt.image.*;
import java.awt.color.ColorSpace;

/**
 * This class defines a 2D image component.
 * This is used for texture images, background images and raster components
 * of Shape3D nodes.
 */

class ImageComponent2DRetained extends ImageComponentRetained {

    // used in D3D to map object to surface pointer
    int hashId;       

    float[] lastAlpha = new float[1];
    
    static final double EPSILON = 1.0e-6;

    // dirty mask to track if the image has been changed since the last
    // alpha update. The nth bit in the mask represents the dirty bit
    // of the image for screen nth. If nth bit is set, then the image
    // needs to be updated with the current alpha values.
    int imageChanged = 0;

    ImageComponent2DRetained() {
	hashId = hashCode();
    }
    
     /**
     * This method handles both BufferedImage and RenderedImage
     * Copies the specified RenderedImage to this 2D image component object.
     * @param image RenderedImage object containing the image.
     * The format and size must be the same as the current format in this
     * ImageComponent2D object.
     */
    void set(RenderedImage image) {
        
        int width = image.getWidth();
        int height = image.getHeight();
        
        if (width != this.width)
            throw new IllegalArgumentException(J3dI18N.getString("ImageComponent2DRetained0"));
        
        if (height != this.height)
            throw new IllegalArgumentException(J3dI18N.getString("ImageComponent2DRetained1"));
        
        geomLock.getLock();        
        
        if(byReference) {            
            setRefImage(image,0);    
        }
        
        // Reset this flag to true, incase it was set to false due to 
        // the previous image type.
        abgrSupported = true;
        
        imageTypeIsSupported = isImageTypeSupported(image);
        
        if (imageTypeIsSupported) {

            if(byReference && yUp) {
                /* Use reference when ( format is OK, Yup is true, and byRef is true). */
                // System.err.println("ImageComponent2DRetained.set() : (imageTypeSupported && byReference && yUp) --- (1)");
                if(image instanceof BufferedImage) {
                    // Create image data object with the byRef image. */                    
                    imageData = createImageDataObject(image); 
                }
                else {
                    // System.err.println("byRef and not BufferedImage !!!");
                    imageData = null;
                }

            } else { 
                // Either not byRef or not yUp or not both
                // System.err.println("ImageComponent2DRetained.set() : (imageTypeSupported && ((!byReference && yUp) || (imageTypeSupported && !yUp)) --- (2)");

                // Create image data object with buffer for image. */
                imageData = createImageDataObject(null);
                copySupportedImageToImageData(image, 0);
            }

        } else {
            // image type is unsupported, need to create a supported local copy.
            // TODO : borrow code from JAI to convert to right format.
            // System.err.println("ImageComponent2DRetained.set() : (imageTypeSupported == false) --- (4)");
            /* Will use the code segment in copy() method */

            // Create image data object with buffer for image. */
            imageData = createImageDataObject(null);
            copyUnsupportedImageToImageData(image, 0);

        }    
        
        geomLock.unLock();
        
        if (source.isLive()) {
            freeSurface();
            
            // send a IMAGE_CHANGED message in order to
            // notify all the users of the change
            sendMessage(IMAGE_CHANGED, null);
        }
    }

    void setSubImage(RenderedImage image, int width, int height, 
			   int srcX, int srcY, int dstX, int dstY) {

        if(!isSubImageTypeEqual(image)) {
            throw new IllegalStateException(
                                J3dI18N.getString("ImageComponent2D6"));           
        }

        // Can't be byReference
        assert (!byReference);
        assert (imageData != null);

        geomLock.getLock();

        if (imageTypeIsSupported) {            
            // Either not byRef or not yUp or not both
            // System.err.println("ImageComponent2DRetained.setSubImage() : (imageTypeSupported ) --- (1)");
            if (image instanceof BufferedImage) {
                copyImageLineByLine((BufferedImage)image, srcX, srcY, dstX, dstY, 0, width, height);
            }
            else {
                copySupportedImageToImageData(image, srcX, srcY, dstX, dstY, 0, width, height);
            }
       } else {
            // image type is unsupported, need to create a supported local copy.
            // TODO : Should look into borrow code from JAI to convert to right format.
            // System.err.println("ImageComponent2DRetained.setSubImage() : (imageTypeSupported == false) --- (2)");
             if (image instanceof BufferedImage) {
                copyUnsupportedImageToImageData((BufferedImage)image, srcX, srcY, dstX, dstY, 0, width, height);             
            }
            else {
                copyUnsupportedImageToImageData(image, srcX, srcY, dstX, dstY, 0, width, height);
            }
        }    
        geomLock.unLock();

        if (source.isLive()) {

            // XXXX: check whether this is needed
            freeSurface();

            // send a SUBIMAGE_CHANGED message in order to
            // notify all the users of the change

            ImageComponentUpdateInfo info;

            // TODO : Should we still use freelist ? -- Chien.
            info = VirtualUniverse.mc.getFreeImageUpdateInfo();
            info.x = dstX;
            info.y = dstY;
	    info.z = 0;
            info.width = width;
            info.height = height;

            sendMessage(SUBIMAGE_CHANGED, info);
        }
    }    
    
    /**
     * Retrieves a copy of the image in this ImageComponent2D object.
     * @return a new RenderedImage object created from the image in this
     * ImageComponent2D object
     */
    RenderedImage getImage() {
        
        if (isByReference()) {
            return getRefImage(0);
        }
        
        if(imageData != null) {
            return imageData.createBufferedImage(0);
        }
        
        return null;
    }

    /**
     * Update data.
     * x and y specifies the x & y offset of the image data in
     * ImageComponent.  It assumes that the origin is (0, 0).
     */
    void updateData(ImageComponent2D.Updater updater,
		    int x, int y, int width, int height) {

	geomLock.getLock();
	// call the user supplied updateData method to update the data
	updater.updateData((ImageComponent2D)source, x, y, width, height);

        RenderedImage refImage = getRefImage(0);
        assert (refImage != null);
        assert (imageData != null);
        
        if(!imageData.isDataByRef()) {
            // update the internal copy of the image data if a copy has been
            // made
            int srcX = x + refImage.getMinX();
            int srcY = y + refImage.getMinY();
            
            if (imageTypeIsSupported) {
                if (refImage instanceof BufferedImage) {
                    copyImageLineByLine((BufferedImage)refImage, srcX, srcY, x, y, 0, width, height);
                } else {
                    copySupportedImageToImageData(refImage, srcX, srcY, x, y, 0, width, height);
                }
            } else {
                // image type is unsupported, need to create a supported local copy.
                // TODO : Should look into borrow code from JAI to convert to right format.
                if (refImage instanceof BufferedImage) {
                    copyUnsupportedImageToImageData((BufferedImage)refImage, srcX, srcY, x, y, 0, width, height);
                } else {
                    copyUnsupportedImageToImageData(refImage, srcX, srcY, x, y, 0, width, height);
                }
            }
        }
	geomLock.unLock();

	
	if (source.isLive()) {

	    //XXXX: check whether this is needed
	    freeSurface();

	    // send a SUBIMAGE_CHANGED message in order to 
	    // notify all the users of the change

	    ImageComponentUpdateInfo info;

	    info = VirtualUniverse.mc.getFreeImageUpdateInfo();
	    info.x = x;
	    info.y = y;
	    info.z = 0;
	    info.width = width;
	    info.height = height;

            sendMessage(SUBIMAGE_CHANGED, info);
	}
    }
    
    ImageComponentRetained createNextLevelMipMapImage() {
 
	int xScale, yScale, newWidth, newHeight;

        if (width > 1) {
            newWidth = width >> 1;
            xScale = 2;
        } else {
            newWidth = 1;
            xScale = 1;
        }
        if (height > 1) {
            newHeight = height >> 1; 
            yScale = 2; 
        } else { 
            newHeight = 1;
            yScale = 1; 
        }   
        
        ImageComponent2DRetained newImage = new ImageComponent2DRetained();
        newImage.processParams(getFormat(), newWidth, newHeight, 1);        
        newImage.imageData = createImageDataObject(null);

        newImage.scaleImage(xScale, yScale, 0, this);
        
        return newImage;
    }   
    

    void setLive(boolean inBackgroundGroup, int refCount) {
	super.setLive(inBackgroundGroup, refCount);
    }

    void clearLive(int refCount) {
	super.clearLive(refCount);
	if (this.refCount <= 0) {
	    freeSurface();
	}
    }

    void freeSurface() {
	if (VirtualUniverse.mc.isD3D()) {
	    Pipeline.getPipeline().freeD3DSurface(this, hashId);
	}
    }

    // Issue 121 : Stop using finalize() to clean up state
    // Use similar approach as in handling ogl Texture resource cleanup.
    protected void finalize() {
	// For Pure immediate mode, there is no clearLive so
	// surface will free when JVM do GC 
	freeSurface();
    }
    

}
