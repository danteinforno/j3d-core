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

import javax.vecmath.*;
import java.util.Vector;
import java.util.Enumeration;
import java.awt.Dimension;

/**
 * A GraphicsContext3D object is used for immediate mode rendering into
 * a 3D canvas.  It is created by, and associated with, a specific
 * Canvas3D object.  A GraphicsContext3D defines methods to set 3D graphics
 * state and draw 3D geometric primitives.  There are no public
 * constructors of GraphicsContext3D. An application obtains a 3D graphics
 * context object from the Canvas3D object that the application wishes
 * to render into by using the getGraphicsContext3D method. A new graphics
 * context is created if one does not already exist.  A new GraphicsContext3D
 * initializes its state variables to the following defaults:
 * <UL>
 * <LI> Background object: null </LI>
 * <LI> Fog object: null </LI>
 * <LI> ModelClip object: null </LI>
 * <LI> Appearance object: null </LI>
 * <LI> List of Light objects: empty </LI>
 * <LI> high-res coordinate: (0, 0, 0) </LI>
 * <LI> modelTransform: identity </LI>
 * <LI> AuralAttributes object: null </LI>
 * <LI> List of Sound objects: empty </LI>
 * <LI> buffer override: false </LI>
 * <LI> front buffer rendering: false </LI>
 * <LI> stereo mode: <code>STEREO_BOTH</code> </LI>
 * </UL>
 *
 * <p>
 * Note that the drawing methods in this class are not necessarily
 * executed immediately.  They may be buffered up for future
 * execution.  Applications must call the
 * <code><a href="#flush(boolean)">flush</a>(boolean)</code>
 * method to ensure that the rendering actually happens. The flush
 * method is implicitly called in the following cases:
 *
 * <ul>
 * <li>The <code>readRaster</code> method calls
 * <code>flush(true)</code></li>
 * <li>The <code>Canvas3D.swap</code> method calls
 * <code>flush(true)</code></li>
 * <li>The Java 3D renderer calls <code>flush(true)</code> prior to
 * swapping the buffer for a double buffered on-screen Canvas3D</li>
 * <li>The Java 3D renderer calls <code>flush(true)</code> prior to
 * copying into the off-screen buffer of an off-screen Canvas3D</li>
 * <li>The Java 3D renderer calls <code>flush(false)</code> after
 * calling the preRender, renderField, postRender, and postSwap
 * Canvas3D callback methods.</li>
 * </ul>
 *
 * <p>
 * A single-buffered, pure-immediate mode application must explicitly
 * call flush to ensure that the graphics will be rendered to the
 * Canvas3D.
 *
 * @see Canvas3D#getGraphicsContext3D
 */
public class GraphicsContext3D extends Object   {
    /**
     * Specifies that rendering is done to the left eye.
     * @see #setStereoMode
     * @since Java 3D 1.2
     */
    public static final int STEREO_LEFT = 0;

    /**
     * Specifies that rendering is done to the right eye.
     * @see #setStereoMode
     * @since Java 3D 1.2
     */
    public static final int STEREO_RIGHT = 1;

    /**
     * Specifies that rendering is done to both eyes.  This is the
     * default.
     * @see #setStereoMode
     * @since Java 3D 1.2
     */
    public static final int STEREO_BOTH = 2;


    /**
     * Canvas3D in which this GraphicsContext3D will render.
     */
    Canvas3D		canvas3d = null;

    int objectId = -1;

//
// Graphics state
//
// current user specified graphics state
    Background uBackground = null;
    Fog uFog = null;
    Appearance uAppearance = null;
    Vector uLights = new Vector();
    HiResCoord uHiRes = new HiResCoord();
    Vector uSounds = new Vector();
    AuralAttributes uAuralAttributes = null;
    boolean uBufferOverride = false;
    boolean uFrontBufferRendering = false;
    int uStereoMode = STEREO_BOTH;
    ModelClip uModelClip = null;

// Current rendering graphics state
    // Current background
    Background background = null;

    // Background to use if background is null;
    BackgroundRetained black = new BackgroundRetained();

    // Current fog
    Fog fog = null;

    // Current modelClip
    ModelClip modelClip = null;

    // Current appearance object
    Appearance appearance = null;

    // default appearance retained object
    AppearanceRetained defaultAppearanceRetained = new AppearanceRetained();

    // The vector of lights
    Vector lights = new Vector();

    // Current High resolution coordinate
    HiResCoord hiRes = new HiResCoord();

    // Current modeling transform
    Transform3D modelTransform = new Transform3D();
    Transform3D identityTransform = new Transform3D();

    Transform3D modelClipTransform = null;
    Transform3D normalTransform = null;
    boolean normalTransformNeedToUpdate = true;

    // The vector of sounds
    Vector sounds = new Vector();

    // Current AuralAttributes state parameters
    AuralAttributes auralAttributes = null;

    // The render object associated with this context
    LightSet ls = null;

    // The current list of lights
    LightRetained[] lightlist = null;

    // Ambient lights
    Color3f sceneAmbient = new Color3f(0.0f, 0.0f, 0.0f);

    // The current number of lights, may be less than lightlist.length
    int numLights = 0;

    // Current composite transform: hi-res + modelTransform
    Transform3D compTransform = new Transform3D();

    // Draw transform: hi-res + modelTransform + view
    Transform3D drawTransform = new Transform3D();

    // The view transform (VPC to EC).
    // NOTE that this is *read-only*
    Transform3D vpcToEc;

    // A boolean that indicates the lights have changed
    boolean lightsChanged = false;

    // A boolean that indicates the sounds have changed
    // TODO: the soundsChanged flag are set like lights methods set 
    //       lightsChanged? but where is this supposed to be check???
    //       lightsChanged tested in 'draw'; but Sound are not processed
    //       in draw.
    boolean soundsChanged = false;

    // Buffer override flag; enables frontBufferRendering and stereoMode
    // attributes.
    boolean bufferOverride = false;

    // Forces rendering to the front buffer (if bufferOverride is true)
    boolean frontBufferRendering = false;

    // Stereo mode for this buffer (if bufferOverride is true)
    int stereoMode = STEREO_BOTH;

    // Read Buffer for reading raster of color image
    byte[] byteBuffer = new byte[1];      

    // Read Buffer for reading floating depth image
    float[] floatBuffer = new float[1];      

    // Read Buffer for reading integer depth image
    int[] intBuffer = new int[1];

    /** 
     * The cached ColoringAttributes color value.  It is
     * 1.0, 1.0, 1.0 if there is no ColoringAttributes.
     */
    float red = 1.0f;
    float green = 1.0f;
    float blue = 1.0f;


    /**
     * Cached diffuse color value
     */
    float dRed = 1.0f;
    float dGreen = 1.0f;
    float dBlue = 1.0f;

    /** 
     * The cached TransparencyAttributes transparency value.  It is
     * 0.0 if there is no TransparencyAttributes.
     */
    float alpha = 0.0f;

    /** 
     * The cached visible flag for geometry.
     */
    boolean visible = true;

    /** 
     * Cached values for polygonMode, line antialiasing, and point antialiasing
     */
    int polygonMode = PolygonAttributes.POLYGON_FILL;
    boolean lineAA = false;
    boolean pointAA = false;


    /**
    /** 
     * A boolean indicating whether or not lighting should be on.
     */
    boolean enableLighting = false;

    private Appearance defaultAppearance = null;

    private boolean geometryIsLocked = false;

    private boolean ignoreVertexColors = false;

    static final int CLEAR 		= 0;
    static final int DRAW  		= 1;
    static final int SWAP  		= 2;
    static final int READ_RASTER 	= 3;
    static final int SET_APPEARANCE	= 4;
    static final int SET_BACKGROUND	= 5;
    static final int SET_FOG		= 6;
    static final int SET_LIGHT		= 7;
    static final int INSERT_LIGHT	= 8;
    static final int REMOVE_LIGHT	= 9;
    static final int ADD_LIGHT		= 10;
    static final int SET_HI_RES		= 11;
    static final int SET_MODEL_TRANSFORM	= 12;
    static final int MULTIPLY_MODEL_TRANSFORM	= 13;
    static final int SET_SOUND		= 14;
    static final int INSERT_SOUND	= 15;
    static final int REMOVE_SOUND	= 16;
    static final int ADD_SOUND		= 17;
    static final int SET_AURAL_ATTRIBUTES	= 18;
    static final int SET_BUFFER_OVERRIDE	= 19;
    static final int SET_FRONT_BUFFER_RENDERING	= 20;
    static final int SET_STEREO_MODE	= 21;
    static final int FLUSH		= 22;
    static final int FLUSH2D		= 23;
    static final int DRAWANDFLUSH2D	= 24;
    static final int SET_MODELCLIP	= 25;
    static final int NCOMMANDS		= 26; // needs to be incremented
					      // when a new command is to be
					      // added to the list

    static Integer commands[]   = new Integer[NCOMMANDS];
    static Integer stereoModes[] = {new Integer(STEREO_LEFT), 
				   new Integer(STEREO_RIGHT),
				   new Integer(STEREO_BOTH)};

    // dirty bits
    static final int BUFFER_MODE	= 0x1;
    private int dirtyMask = 0;


    // multi-texture
    int numActiveTexUnit = 0;
    int lastActiveTexUnitIndex = 0;
    boolean toSimulateMultiTex = true;

    // for read raster
    volatile boolean readRasterReady = false;

    // for runMonitor
    boolean gcReady = false;
    int waiting = 0;


    /**
     * Constructs and creates a GraphicsContext3D object with default
     * values.  Users do not call this directly, rather they get a
     * graphics context from a Canvas3D.
     */
    GraphicsContext3D(Canvas3D canvas3d) {
	this.canvas3d = canvas3d;
    }

    /**
     * Gets the Canvas3D that created this GraphicsContext3D.
     * @return the Canvas3D that created this GraphicsContext3D
     */
    public Canvas3D getCanvas3D() {
	return this.canvas3d;
    }

//
// Methods to set/get graphics state
//

    /**
     * Sets the current Appearance object to the specified
     * Appearance component object.
     * The graphics context stores a reference to the specified 
     * Appearance object. This means that the application may modify 
     * individual appearance attributes by using the appropriate 
     * methods on the Appearance object.
     * If the Appearance object is null, default values will be used 
     * for all appearance attributes - it is as if an
     * Appearance node were created using the default constructor.
     * @param appearance the new Appearance object
     */
    public void setAppearance(Appearance appearance) {
	
	if(appearance == null) {
	    if(defaultAppearance == null) {
		defaultAppearance = new Appearance();
	    }
	    appearance = defaultAppearance;
	}
	
	NodeComponentRetained nc = ((AppearanceRetained)appearance.retained).material;
        uAppearance = appearance;
        if ((canvas3d.view == null) || 
	    (canvas3d.view.universe == null) ||
	    (!canvas3d.view.active) ||
	    (Thread.currentThread() == canvas3d.screen.renderer)) {
            doSetAppearance(appearance);
        } else if (Thread.currentThread() ==
		   canvas3d.view.universe.behaviorScheduler) {
            sendRenderMessage(false, GraphicsContext3D.SET_APPEARANCE, appearance, null);
        } else {
            sendRenderMessage(true, GraphicsContext3D.SET_APPEARANCE, appearance, null);
        }
    }
    
    void doSetAppearance(Appearance appearance) {
	
	if (appearance != null) {   
	    NodeComponentRetained nc;
	    nc = ((AppearanceRetained)appearance.retained).material;
	    if (nc != null) {
		nc.setInImmCtx(true);
		enableLighting = ((MaterialRetained) nc).lightingEnable;
		dRed = ((MaterialRetained) nc).diffuseColor.x;
		dGreen = ((MaterialRetained) nc).diffuseColor.y;
		dBlue = ((MaterialRetained) nc).diffuseColor.z;
	    }
	    else {
		enableLighting = false;
	    }

	    if (((AppearanceRetained)appearance.retained).texUnitState != null) {
		TextureUnitStateRetained[] texUnitState = 
		    ((AppearanceRetained)appearance.retained).texUnitState;

		for (int i = 0 ; i < texUnitState.length; i++) {
		    if (texUnitState[i] != null) {
			texUnitState[i].setInImmCtx(true);	    
		    }
		}
	    }

	    nc = ((AppearanceRetained)appearance.retained).texture;
	    if (nc != null) {
		nc.setInImmCtx(true);
	    }

	    nc = ((AppearanceRetained)appearance.retained).texCoordGeneration;
	    if (nc != null) {
		nc.setInImmCtx(true);
	    }

	    nc = ((AppearanceRetained)appearance.retained).textureAttributes;
	    if (nc != null) {
		nc.setInImmCtx(true);
	    }

	    nc = ((AppearanceRetained)appearance.retained).coloringAttributes;
	    if (nc != null) {
		nc.setInImmCtx(true);
		red = ((ColoringAttributesRetained)nc).color.x;
		green = ((ColoringAttributesRetained)nc).color.y;
		blue = ((ColoringAttributesRetained)nc).color.z;
	    }
	    else {
		red = 1.0f;
		green = 1.0f;
		blue = 1.0f;
	    }

	    nc = ((AppearanceRetained)appearance.retained).transparencyAttributes;
	    if (nc != null) {
		nc.setInImmCtx(true);
		alpha = 1.0f - ((TransparencyAttributesRetained) nc).transparency;
	    } else {
		alpha = 1.0f;
	    }

	    nc = ((AppearanceRetained)appearance.retained).renderingAttributes;
	    if (nc != null) {
		nc.setInImmCtx(true);
		visible = ((RenderingAttributesRetained)nc).visible;
	    }
	    else
		visible = true;

	    nc = ((AppearanceRetained)appearance.retained).polygonAttributes;
	    if (nc != null) {
		nc.setInImmCtx(true);
		polygonMode = ((PolygonAttributesRetained)nc).polygonMode;
	    }
	    else {
		polygonMode = PolygonAttributes.POLYGON_FILL;
	    }

	    nc = ((AppearanceRetained)appearance.retained).lineAttributes;
	    if (nc != null) {
		nc.setInImmCtx(true);
		lineAA = ((LineAttributesRetained)nc).lineAntialiasing;

	    }
	    else {
		lineAA = false;
	    }

	    nc = ((AppearanceRetained)appearance.retained).pointAttributes;
	    if (nc != null) {
		if (nc.source.isLive())
		    nc.setInImmCtx(true);
		pointAA = ((PointAttributesRetained)nc).pointAntialiasing;
	    }
	    else {
		pointAA = false;
	    }

	    
	    if (this.appearance != null) {
		AppearanceRetained app = (AppearanceRetained)this.appearance.retained;
		app.setInImmCtx(false);
		if (app.material != null) {
		    app.material.setInImmCtx(false);
		}
		if (app.texUnitState != null) {
		    for (int i = 0; i < app.texUnitState.length; i++) {
			if (app.texUnitState[0] != null)
			    app.texUnitState[0].setInImmCtx(false);
		    }
		}
		if (app.texture != null) {
		    app.texture.setInImmCtx(false);
		}
		if (app.texCoordGeneration != null) {
		    app.texCoordGeneration.setInImmCtx(false);
		}
		if (app.textureAttributes != null) {
		    app.textureAttributes.setInImmCtx(false);
		}
		if (app.coloringAttributes != null) {
		    app.coloringAttributes.setInImmCtx(false);
		}
		if (app.transparencyAttributes != null) {
		    app.transparencyAttributes.setInImmCtx(false);
		}
		if (app.renderingAttributes != null) {
		    app.renderingAttributes.setInImmCtx(false);
		}
		if (app.polygonAttributes != null) {
		    app.polygonAttributes.setInImmCtx(false);
		}
		if (app.lineAttributes != null) {
		    app.lineAttributes.setInImmCtx(false);
		}
		if (app.pointAttributes != null) {
		    app.pointAttributes.setInImmCtx(false);
		}
	    }
	    ((AppearanceRetained)appearance.retained).setInImmCtx(true);	
	}
	this.appearance = appearance;
    }

    /**
     * Retrieves the current Appearance component object.
     * @return the current Appearance object
     */
    public Appearance getAppearance() {
	return this.uAppearance;
    }

    /**
     * Sets the current Background to the specified Background
     * leaf node object.
     * The graphics context stores a reference to the specified 
     * Background node. This means that the application may modify 
     * the background color or image by using the appropriate 
     * methods on the Background node. The Background node must 
     * not be part of a live scene graph, nor may it subsequently 
     * be made part of a live scene graph-an IllegalSharingException 
     * is thrown in such cases. If the Background object is null, 
     * the default background color of black (0,0,0) is used to clear
     * the canvas prior to rendering a new frame. The Background 
     * node's application region is ignored for immediate-mode 
     * rendering.
     * @param background the new Background object
     * @exception IllegalSharingException if the Background node
     * is part of or is subsequently made part of a live scene graph.
     */
    public void setBackground(Background background) {
        if (background.isLive()) {
           throw new IllegalSharingException(J3dI18N.getString("GraphicsContext3D11"));
        }
        if (((BackgroundRetained)background.retained).geometryBranch != null)
           throw new IllegalSharingException(J3dI18N.getString("GraphicsContext3D22"));
        uBackground = background;
        if ((canvas3d.view == null) || 
	    (canvas3d.view.universe == null) ||
	    (!canvas3d.view.active) ||
	    (Thread.currentThread() == canvas3d.screen.renderer)) {
            doSetBackground(background);
        } else if (Thread.currentThread() ==
                        canvas3d.view.universe.behaviorScheduler) {
            sendRenderMessage(false, GraphicsContext3D.SET_BACKGROUND, background, null);
        } else {
            sendRenderMessage(true, GraphicsContext3D.SET_BACKGROUND, background, null);
        }
    }

    void doSetBackground(Background background) {
	BackgroundRetained bg; 

	if (this.background != null) {
	    bg = (BackgroundRetained)this.background.retained;
	    bg.setInImmCtx(false);
	    if (bg.image != null) {
		bg.image.freeSurface();
	    }
	}
	bg = (BackgroundRetained)background.retained;
	bg.setInImmCtx(true);
	if (bg.image != null) {	
	    bg.image.freeSurface();
	}

	this.background = background;
    }

    /**
     * Retrieves the current Background leaf node object.
     * @return the current Background object
     */
    public Background getBackground() {
	return this.uBackground;
    }

    /**
     * Sets the current Fog to the specified Fog
     * leaf node object.
     * The graphics context stores a reference to the specified 
     * Fog node. This means that the application may modify the 
     * fog attributes using the appropriate methods on the Fog 
     * node object. The Fog node must not be part of a live 
     * scene graph, nor may it subsequently be made part of a
     * live scene graph-an IllegalSharingException is thrown in 
     * such cases. If the Fog object is null, fog is disabled. 
     * Both the region of influence and the hierarchical scope 
     * of the Fog node are ignored for immediate-mode rendering.
     * @param fog the new Fog object
     * @exception IllegalSharingException if the Fog node
     * is part of or is subsequently made part of a live scene graph.
     */
    public void setFog(Fog fog) {
        if (fog != null && fog.isLive()) {
           throw new IllegalSharingException(J3dI18N.getString("GraphicsContext3D12"));
        }
        uFog = fog;
        if ((canvas3d.view == null) || 
	    (canvas3d.view.universe == null) ||
	    (!canvas3d.view.active) ||
            (Thread.currentThread() == canvas3d.screen.renderer)) {
            doSetFog(fog);
        } else if (Thread.currentThread() ==
                        canvas3d.view.universe.behaviorScheduler) {
            sendRenderMessage(false, GraphicsContext3D.SET_FOG, fog, null);
        } else {
            sendRenderMessage(true, GraphicsContext3D.SET_FOG, fog, null);
        }
    }

    void doSetFog(Fog fog) {
	if (this.fog != null) {
	   ((FogRetained)this.fog.retained).setInImmCtx(false);
	}
	this.fog = fog;
	if (fog != null) {
	    ((FogRetained)fog.retained).setInImmCtx(true);


	    if (fog.retained instanceof LinearFogRetained)
		updateFogState((LinearFogRetained)fog.retained);
	}
    }

    /**
     * Retrieves the current Fog leaf node object.
     * @return the current Fog object
     */
    public Fog getFog() {
	return this.uFog;
    }


    /**
     * Sets the current ModelClip leaf node to the specified object.
     * The graphics context stores a reference to the specified
     * ModelClip node. This means that the application may modify the
     * model clipping attributes using the appropriate methods on the
     * ModelClip node object. The ModelClip node must not be part of a
     * live scene graph, nor may it subsequently be made part of a
     * live scene graph-an IllegalSharingException is thrown in such
     * cases. If the ModelClip object is null, model clipping is
     * disabled.  Both the region of influence and the hierarchical
     * scope of the ModelClip node are ignored for immediate-mode
     * rendering.
     *
     * @param modelClip the new ModelClip node
     *
     * @exception IllegalSharingException if the ModelClip node
     * is part of or is subsequently made part of a live scene graph.
     *
     * @since Java 3D 1.2
     */
    public void setModelClip(ModelClip modelClip) {
        if ((modelClip != null) && modelClip.isLive()) {
           throw new IllegalSharingException(J3dI18N.getString("GraphicsContext3D25"));
        }
        uModelClip = modelClip;
        if ((canvas3d.view == null) || 
	    (canvas3d.view.universe == null) ||
	    (!canvas3d.view.active) ||
            (Thread.currentThread() == canvas3d.screen.renderer)) {
            doSetModelClip(modelClip);
        } else if (Thread.currentThread() ==
                        canvas3d.view.universe.behaviorScheduler) {
            sendRenderMessage(false, GraphicsContext3D.SET_MODELCLIP, 
					modelClip, null);
        } else {
            sendRenderMessage(true, GraphicsContext3D.SET_MODELCLIP, 
					modelClip, null);
        }
    }

    void doSetModelClip(ModelClip modelClip) {
	ModelClipRetained mc = null;

	this.modelClip = modelClip;

	if (this.modelClip != null) {
	    mc = (ModelClipRetained)this.modelClip.retained;
	    mc.setInImmCtx(true);

	    if (modelClipTransform == null)
		modelClipTransform = new Transform3D();

	    // save the current model Transform
	    modelClipTransform.set(compTransform);
	} 
    }

    /**
     * Retrieves the current ModelClip leaf node object.
     * @return the current ModelClip object
     *
     * @since Java 3D 1.2
     */
    public ModelClip getModelClip() {
	return this.uModelClip;
    }


    /**
     * Replaces the specified light with the light provided.
     * The graphics context stores a reference to each light 
     * object in the list of lights. This means that the 
     * application may modify the light attributes for
     * any of the lights using the appropriate methods on that 
     * Light node object. None of the Light nodes in the list 
     * of lights may be part of a live scene graph, nor may 
     * they subsequently be made part of a live scene graph -
     * an IllegalSharingException is thrown in such cases. 
     * @param light the new light
     * @param index which light to replace
     * @exception IllegalSharingException if the Light node
     * is part of or is subsequently made part of a live scene graph.
     * @exception NullPointerException if the Light object is null.
     */
    public void setLight(Light light, int index) {
        if (light == null) {
           throw new NullPointerException(J3dI18N.getString("GraphicsContext3D13"));
        }
        if (light.isLive()) {
           throw new IllegalSharingException(J3dI18N.getString("GraphicsContext3D14"));
        }
        uLights.setElementAt(light, index);
        if ((canvas3d.view == null) || 
	    (canvas3d.view.universe == null) ||
	    (!canvas3d.view.active) ||
            (Thread.currentThread() == canvas3d.screen.renderer)) {
            doSetLight(light, index);
        } else if (Thread.currentThread() ==
                        canvas3d.view.universe.behaviorScheduler) {
            sendRenderMessage(false, GraphicsContext3D.SET_LIGHT, light, 
			new Integer(index));
        } else {
            sendRenderMessage(true, GraphicsContext3D.SET_LIGHT, light,
			new Integer(index));
        }
    }

    void doSetLight(Light light, int index) {

	Light oldlight;
	oldlight = (Light)this.lights.elementAt(index);
	if (oldlight != null) {
	   ((LightRetained)oldlight.retained).setInImmCtx(false);
	}
	((LightRetained)light.retained).setInImmCtx(true);
	updateLightState((LightRetained)light.retained);
	this.lights.setElementAt(light, index);
	this.lightsChanged = true;
    }

    /**
     * Inserts the specified light at the specified index location.
     * @param light the new light
     * @param index at which location to insert
     * @exception IllegalSharingException if the Light node
     * is part of or is subsequently made part of a live scene graph.
     * @exception NullPointerException if the Light object is null.
     */
    public void insertLight(Light light, int index) {
        if (light == null) {
           throw new NullPointerException(J3dI18N.getString("GraphicsContext3D13"));
        }       
        if (light.isLive()) {
           throw new IllegalSharingException(J3dI18N.getString("GraphicsContext3D14"));
        }
        uLights.insertElementAt(light, index);
        if ((canvas3d.view == null) || 
	    (canvas3d.view.universe == null) ||
	    (!canvas3d.view.active) ||
            (Thread.currentThread() == canvas3d.screen.renderer)) {
            doInsertLight(light, index);
        } else if (Thread.currentThread() ==
                        canvas3d.view.universe.behaviorScheduler) {
            sendRenderMessage(false, GraphicsContext3D.INSERT_LIGHT, light,
			new Integer(index));
        } else {
            sendRenderMessage(true, GraphicsContext3D.INSERT_LIGHT, light,
			new Integer(index));
        }
    }

    void doInsertLight(Light light, int index) {
	((LightRetained)light.retained).setInImmCtx(true);
	updateLightState((LightRetained)light.retained);
	this.lights.insertElementAt(light, index);
	this.lightsChanged = true;
    }

    /**
     * Removes the light at the specified index location.
     * @param index which light to remove
     */
    public void removeLight(int index) {
        uLights.removeElementAt(index);
        if ((canvas3d.view == null) || 
	    (canvas3d.view.universe == null) ||
	    (!canvas3d.view.active) ||
            (Thread.currentThread() == canvas3d.screen.renderer)) {
            doRemoveLight(index);
        } else if (Thread.currentThread() ==
                        canvas3d.view.universe.behaviorScheduler) {
            sendRenderMessage(false, GraphicsContext3D.REMOVE_LIGHT,
			new Integer(index), null);
        } else {
            sendRenderMessage(true, GraphicsContext3D.REMOVE_LIGHT,
			new Integer(index), null);
        }
    }

    void doRemoveLight(int index) {
	Light light = (Light) this.lights.elementAt(index);

	((LightRetained)light.retained).setInImmCtx(false);
	this.lights.removeElementAt(index);
	this.lightsChanged = true;
    }

    /**
     * Retrieves the index selected light.
     * @param index which light to return
     * @return the light at location index
     */
    public Light getLight(int index) {
	return (Light) uLights.elementAt(index);
    }

    /**
     * Retrieves the enumeration object of all the lights.
     * @return the enumeration object of all the lights
     */  
    public Enumeration getAllLights() {
        return uLights.elements();
    }  
 
    /**
     * Appends the specified light to this graphics context's list of lights.
     * Adding a null Light object to the list will result 
     * in a NullPointerException. Both the region of influence 
     * and the hierarchical scope of all lights in the list 
     * are ignored for immediate-mode rendering.
     * @param light the light to add
     * @exception IllegalSharingException if the Light node
     * is part of or is subsequently made part of a live scene graph.
     * @exception NullPointerException if the Light object is null.
     */
    public void addLight(Light light) {
        if (light == null) {
           throw new NullPointerException(J3dI18N.getString("GraphicsContext3D13"));
        }
 
        if (light.isLive()) {
           throw new IllegalSharingException(J3dI18N.getString("GraphicsContext3D14"));
        }
        uLights.addElement(light);
        if ((canvas3d.view == null) || 
	    (canvas3d.view.universe == null) ||
	    (!canvas3d.view.active) ||
            (Thread.currentThread() == canvas3d.screen.renderer)) {
            doAddLight(light);
        } else if (Thread.currentThread() ==
                        canvas3d.view.universe.behaviorScheduler) {
            sendRenderMessage(false, GraphicsContext3D.ADD_LIGHT, light, null);
        } else {
            sendRenderMessage(true, GraphicsContext3D.ADD_LIGHT, light, null);
        }
    }

    void doAddLight(Light light) {

	((LightRetained)light.retained).setInImmCtx(true);
	updateLightState((LightRetained)light.retained);
	this.lights.addElement(light);
	this.lightsChanged = true;
    }

    /**
     * Retrieves the current number of lights in this graphics context.
     * @return the current number of lights
     */
    public int numLights() {
	return this.uLights.size();
    }


    private Transform3D getNormalTransform() {
	if (compTransform.isRigid()) {
	    return compTransform;
	}
	if (normalTransform == null) {
	    normalTransform = new Transform3D();
	} 

	if (normalTransformNeedToUpdate) {
	    normalTransform.invert(compTransform);
	    normalTransform.transpose();
	    normalTransformNeedToUpdate = false;
	}
	return normalTransform;
    }


    void updateFogState(LinearFogRetained lfog) {
	lfog.localToVworldScale = modelTransform.getDistanceScale();
    }


    void updateLightState(LightRetained light) {

	if (light instanceof DirectionalLightRetained) {
	   DirectionalLightRetained dl = (DirectionalLightRetained) light;

	   Transform3D xform = getNormalTransform();
	   xform.transform(dl.direction, dl.xformDirection);
           dl.xformDirection.normalize();

   	} else if (light instanceof SpotLightRetained) {
	   SpotLightRetained sl = (SpotLightRetained) light;

	   Transform3D xform = getNormalTransform();
	   xform.transform(sl.direction, sl.xformDirection);
           sl.xformDirection.normalize();
           this.modelTransform.transform(sl.position, sl.xformPosition);

   	} else if (light instanceof PointLightRetained) {
	   PointLightRetained pl = (PointLightRetained) light;

           this.modelTransform.transform(pl.position,pl.xformPosition);

	   pl.localToVworldScale = modelTransform.getDistanceScale();

	}
    }

    /**
     * Sets the HiRes coordinate of this context to the location
     * specified by the parameters provided.
     * The parameters x, y, and z are arrays of eight 32-bit 
     * integers that specify the high-resolution coordinates point.
     * @param x an eight element array specifying the x position
     * @param y an eight element array specifying the y position
     * @param z an eight element array specifying the z position
     * @see HiResCoord
     */
    public void setHiRes(int[] x, int[] y, int[] z) {
	HiResCoord hiRes = new HiResCoord(x, y, z);
	setHiRes(hiRes);
    }
   
    /**
     * Sets the HiRes coordinate of this context
     * to the location specified by the HiRes argument.
     * @param hiRes the HiRes coordinate specifying the a new location
     */
    public void setHiRes(HiResCoord hiRes) {
        uHiRes.setHiResCoord(hiRes);
        if ((canvas3d.view == null) || 
	    (canvas3d.view.universe == null) ||
	    (!canvas3d.view.active) ||
            (Thread.currentThread() == canvas3d.screen.renderer)) {
            doSetHiRes(hiRes);
        } else if (Thread.currentThread() ==
                        canvas3d.view.universe.behaviorScheduler) {
            sendRenderMessage(false, GraphicsContext3D.SET_HI_RES, hiRes, null);
        } else {
            sendRenderMessage(true, GraphicsContext3D.SET_HI_RES, hiRes, null);
        }
    }

    void doSetHiRes(HiResCoord hiRes) {
	this.hiRes.setHiResCoord(hiRes);
	computeCompositeTransform();
    }

    /**
     * Retrieves the current HiRes coordinate of this context.
     * @param hiRes a HiResCoord object that will receive the
     * HiRes coordinate of this context
     */
    public void getHiRes(HiResCoord hiRes) {
	uHiRes.getHiResCoord(hiRes);
    }

    /**
     * Sets the current model transform to a copy of the specified
     * transform.
     * A BadTransformException is thrown if an attempt is made 
     * to specify an illegal Transform3D.
     * @param t the new model transform
     * @exception BadTransformException if the transform is not affine.
     */
    public void setModelTransform(Transform3D t) {
	
        if ((canvas3d.view == null) || 
	    (canvas3d.view.universe == null) ||
	    (!canvas3d.view.active) ||
            (Thread.currentThread() == canvas3d.screen.renderer)) {
            doSetModelTransform(t);
        }
	else {
	    Transform3D uModelTransform = VirtualUniverse.mc.getTransform3D(t);
	    //Transform3D uModelTransform = t;
	    if (Thread.currentThread() ==
		canvas3d.view.universe.behaviorScheduler) {
		sendRenderMessage(false, GraphicsContext3D.SET_MODEL_TRANSFORM, 
				  uModelTransform, null);
	    } else {
		sendRenderMessage(true, GraphicsContext3D.SET_MODEL_TRANSFORM, 
				  uModelTransform, null);
	    }
	}
    }

    void doSetModelTransform(Transform3D t) {
	this.modelTransform.set(t);
	computeCompositeTransform();
	normalTransformNeedToUpdate = true;
    }

    /**
     * Multiplies the current model transform by the specified
     * transform and stores the result back into the current
     * transform. The specified transformation must be affine.
     * @param t the model transform to be concatenated with the
     * current model transform
     * @exception BadTransformException if the transform is not affine.
     */
    public void multiplyModelTransform(Transform3D t) {
        if ((canvas3d.view == null) || 
	    (canvas3d.view.universe == null) || 
	    (!canvas3d.view.active) ||
            (Thread.currentThread() == canvas3d.screen.renderer)) {
            doMultiplyModelTransform(t);
        } else {
	    Transform3D tt = VirtualUniverse.mc.getTransform3D(t);
	    if (Thread.currentThread() == canvas3d.view.universe.behaviorScheduler) {
		sendRenderMessage(false, GraphicsContext3D.MULTIPLY_MODEL_TRANSFORM,
				  tt, null);
	    } else {
		sendRenderMessage(true, GraphicsContext3D.MULTIPLY_MODEL_TRANSFORM,
				  tt, null);
	    }
	}
    }
    
    void doMultiplyModelTransform(Transform3D t) {
	this.modelTransform.mul(t);
	computeCompositeTransform();
	normalTransformNeedToUpdate = true;
    }
  
    /**
     * Retrieves the current model transform.
     * @param t the model transform that will receive the current
     * model transform
     */
    public void getModelTransform(Transform3D t) {
	t.set(modelTransform);
    }

    /**
     * Replaces the specified sound with the sound provided.
     * The graphics context stores a reference to each sound 
     * object in the list of sounds. This means that the 
     * application may modify the sound attributes for
     * any of the sounds by using the appropriate methods on 
     * that Sound node object.
     * @param sound the new sound
     * @param index which sound to replace
     * @exception IllegalSharingException if the Sound node
     * is part of or is subsequently made part of a live scene graph.
     * @exception NullPointerException if the Sound object is null.
     */
    public void setSound(Sound sound, int index) {
        if (sound == null) {
           throw new NullPointerException(J3dI18N.getString("GraphicsContext3D17"));
        }
        if (sound.isLive()) {
           throw new IllegalSharingException(J3dI18N.getString("GraphicsContext3D23"));
        }
        uSounds.setElementAt(sound, index);
        if ((canvas3d.view == null) || 
	    (canvas3d.view.universe == null) ||
	    (!canvas3d.view.active) ||
            (Thread.currentThread() == canvas3d.screen.renderer)) {
            doSetSound(sound, index);
        } else if (Thread.currentThread() ==
                        canvas3d.view.universe.behaviorScheduler) {
            sendRenderMessage(false, GraphicsContext3D.SET_SOUND, sound,
			new Integer(index));
        } else {
            sendRenderMessage(true, GraphicsContext3D.SET_SOUND, sound,
			new Integer(index));
        }
    }

    void doSetSound(Sound sound, int index) {
        Sound oldSound;
        oldSound = (Sound)(this.sounds.elementAt(index));
        ((SoundRetained)sound.retained).setInImmCtx(true);
        if (oldSound != null) {
           ((SoundRetained)oldSound.retained).setInImmCtx(false);
        }
        ((SoundRetained)sound.retained).setInImmCtx(true);
        updateSoundState((SoundRetained)(sound.retained));
	this.sounds.setElementAt(sound, index);
        this.soundsChanged = true;

        sendSoundMessage(GraphicsContext3D.SET_SOUND, sound, oldSound);
    }

    /**
     * Inserts the specified sound at the specified index location.
     * Inserting a sound to the list of sounds implicitly starts the
     * sound playing. Once a sound is finished playing, it can be 
     * restarted by setting the sound's enable flag to true. 
     * The scheduling region of all sounds in the list is ignored 
     * for immediate-mode rendering.
     * @param sound the new sound
     * @param index at which location to insert
     * @exception IllegalSharingException if the Sound node
     * is part or is subsequently made part of a live scene graph.
     * @exception NullPointerException if the Sound object is null.
     */
    public void insertSound(Sound sound, int index) {
        if (sound == null) { 
           throw new NullPointerException(J3dI18N.getString("GraphicsContext3D17"));         } 
 
        if (sound.isLive()) { 
           throw new IllegalSharingException(J3dI18N.getString("GraphicsContext3D23"));
        }
        uSounds.insertElementAt(sound, index);
        if ((canvas3d.view == null) || 
	    (canvas3d.view.universe == null) ||
	    (!canvas3d.view.active) ||
            (Thread.currentThread() == canvas3d.screen.renderer)) {
            doInsertSound(sound, index);
        } else if (Thread.currentThread() ==
                        canvas3d.view.universe.behaviorScheduler) {
            sendRenderMessage(false, GraphicsContext3D.INSERT_SOUND, sound,
			new Integer(index));
        } else {
            sendRenderMessage(true, GraphicsContext3D.INSERT_SOUND, sound,
			new Integer(index));
        }
    }

    void doInsertSound(Sound sound, int index) {
        updateSoundState((SoundRetained)sound.retained);
	this.sounds.insertElementAt(sound, index);
        this.soundsChanged = true;
        sendSoundMessage(GraphicsContext3D.INSERT_SOUND, sound, null);
    }

    /**
     * Removes the sound at the specified index location.
     * @param index which sound to remove
     */
    public void removeSound(int index) {
        uSounds.removeElementAt(index);
        if ((canvas3d.view == null) || 
	    (canvas3d.view.universe == null) ||
	    (!canvas3d.view.active) ||
            (Thread.currentThread() == canvas3d.screen.renderer)) {
            doRemoveSound(index);
        } else if (Thread.currentThread() ==
                        canvas3d.view.universe.behaviorScheduler) {
            sendRenderMessage(false, GraphicsContext3D.REMOVE_SOUND, 
				new Integer(index), null);
        } else {
            sendRenderMessage(true, GraphicsContext3D.REMOVE_SOUND, 
				new Integer(index), null);
        }
    }

    void doRemoveSound(int index) {
        Sound sound = (Sound)(this.sounds.elementAt(index));
        SoundScheduler soundScheduler = getSoundScheduler();
        ((SoundRetained)(sound.retained)).setInImmCtx(false);
	this.sounds.removeElementAt(index);
        this.soundsChanged = true;
        // stop sound if playing on audioDevice
        sendSoundMessage(GraphicsContext3D.REMOVE_SOUND, null, sound);
     }

    /**
     * Retrieves the index selected sound.
     * @param index which sound to return
     * @return the sound at location index
     */
    public Sound getSound(int index) {
	Sound sound = (Sound)(uSounds.elementAt(index));
	return sound;
    }

    /**
     * Retrieves the enumeration object of all the sounds.
     * @return the enumeration object of all the sounds
     */  
    public Enumeration getAllSounds() {
        return uSounds.elements();
    }   

    /**
     * Appends the specified sound to this graphics context's list of sounds.
     * Adding a sound to the list of sounds implicitly starts the
     * sound playing. Once a sound is finished playing, it can be 
     * restarted by setting the sound's enable flag to true. 
     * The scheduling region of all sounds in the list is ignored 
     * for immediate-mode rendering.
     * @param sound the sound to add
     * @exception IllegalSharingException if the Sound node
     * is part of or is subsequently made part of a live scene graph.
     * @exception NullPointerException if the Sound object is null.
     */
    public void addSound(Sound sound) {
        if (sound == null) { 
           throw new NullPointerException(J3dI18N.getString("GraphicsContext3D17"));         } 
 
        if (sound.isLive()) {
           throw new IllegalSharingException(J3dI18N.getString("GraphicsContext3D23"));
 
        }
        uSounds.addElement(sound);
        if ((canvas3d.view == null) || 
	    (canvas3d.view.universe == null) ||
	    (!canvas3d.view.active) ||
            (Thread.currentThread() == canvas3d.screen.renderer)) {
            doAddSound(sound);
        } else if (Thread.currentThread() ==
                        canvas3d.view.universe.behaviorScheduler) {
            sendRenderMessage(false, GraphicsContext3D.ADD_SOUND, sound, null);
        } else {
            sendRenderMessage(true, GraphicsContext3D.ADD_SOUND, sound, null);
        }
    }

    void doAddSound(Sound sound) {
        ((SoundRetained)(sound.retained)).setInImmCtx(true);
        updateSoundState((SoundRetained)(sound.retained));
	this.sounds.addElement(sound);
        this.soundsChanged = true;
        sendSoundMessage(GraphicsContext3D.ADD_SOUND, sound, null);
    }

    /**
     * Retrieves the current number of sounds in this graphics context.
     * @return the current number of sounds
     */
    public int numSounds() {
	return uSounds.size();
    }
 
    SoundScheduler getSoundScheduler() {
        if (canvas3d != null && canvas3d.view != null)
            return canvas3d.view.soundScheduler;  // could be null as well
        else
            return (SoundScheduler)null;
    }

    void updateSoundState(SoundRetained sound) {
        View view = null;
        if (canvas3d != null)
            view = canvas3d.view;
	// Make sure that:
	//   . Current view is not null
	//   . The sound scheduler running (reference to it is not null)
        if (view != null) {
            SoundScheduler soundScheduler = getSoundScheduler();
            if (soundScheduler == null) {
		// TODO: Re-implement
                // start up SoundScheduler since it hasn't already been started
            }
        }

	// Update sound fields related to transforms
	if (sound instanceof ConeSoundRetained) {
	   ConeSoundRetained cs = (ConeSoundRetained) sound;
	   this.modelTransform.transform(cs.direction, cs.xformDirection);
           cs.xformDirection.normalize();
	   this.modelTransform.transform(cs.position, cs.xformPosition);
           // TODO (Question) Is drawTranform equivalent to Vworld-to-Local?
           cs.trans.setWithLock(drawTransform);

   	} else if (sound instanceof PointSoundRetained) {
	   PointSoundRetained ps = (PointSoundRetained) sound;
           this.modelTransform.transform(ps.position, ps.xformPosition);
           // TODO (Question) Is drawTranform equivalent to Vworld-to-Local?
           ps.trans.setWithLock(drawTransform);
       }
    }

    /**
     * Retrieves the sound playing flag.
     * @param index which sound 
     * @return flag denoting if sound is currently playing
     */
    public boolean isSoundPlaying(int index) {
        Sound sound;
        // uSounds isPlaying field is NOT updated, sounds elements are used
	sound = (Sound)(this.sounds.elementAt(index));
	return sound.isPlaying();
    }

    /**
     * Sets the current AuralAttributes object to the specified
     * AuralAttributes component object.
     * This means that the application may modify individual 
     * audio attributes by using the appropriate methods in 
     * the Aural-Attributes object.
     * @param attributes the new AuralAttributes object
     */
    public void setAuralAttributes(AuralAttributes attributes) {
        uAuralAttributes = attributes;

        if ((canvas3d.view == null) || 
	    (canvas3d.view.universe == null) ||
	    (!canvas3d.view.active) ||
            (Thread.currentThread() == canvas3d.screen.renderer)) {
            doSetAuralAttributes(attributes);
        } else if (Thread.currentThread() ==
                        canvas3d.view.universe.behaviorScheduler) {
            sendRenderMessage(false, GraphicsContext3D.SET_AURAL_ATTRIBUTES,
				attributes, null);
        } else {
            sendRenderMessage(true, GraphicsContext3D.SET_AURAL_ATTRIBUTES,
				attributes, null);
        }
    }

    void doSetAuralAttributes(AuralAttributes attributes) {
	this.auralAttributes = attributes;
        sendSoundMessage(GraphicsContext3D.SET_AURAL_ATTRIBUTES, attributes, null);
    }
    /**
     * Retrieves the current AuralAttributes component object.
     * @return the current AuralAttributes object
     */
    public AuralAttributes getAuralAttributes() {
	return uAuralAttributes;
    }


    /**
     * Sets a flag that specifies whether the double buffering and
     * stereo mode from the Canvas3D are overridden.  When set to
     * true, this attribute enables the
     * <code>frontBufferRendering</code> and <code>stereoMode</code>
     * attributes.
     *
     * @param bufferOverride the new buffer override flag
     *
     * @see #setFrontBufferRendering
     * @see #setStereoMode
     *
     * @since Java 3D 1.2
     */
    public void setBufferOverride(boolean bufferOverride) {
        uBufferOverride = bufferOverride;
        if ((canvas3d.view == null) || 
	    (canvas3d.view.universe == null) ||
	    (!canvas3d.view.active) ||
            (Thread.currentThread() == canvas3d.screen.renderer)) {
            doSetBufferOverride(bufferOverride);
        } else if (Thread.currentThread() ==
                        canvas3d.view.universe.behaviorScheduler) {
            sendRenderMessage(false, GraphicsContext3D.SET_BUFFER_OVERRIDE,
			new Boolean(bufferOverride), null);
        } else {
            sendRenderMessage(true, GraphicsContext3D.SET_BUFFER_OVERRIDE,
			new Boolean(bufferOverride), null);
        }
    }
 
    void doSetBufferOverride(boolean bufferOverride) {
	if (bufferOverride != this.bufferOverride) {
	    this.bufferOverride = bufferOverride;
	    dirtyMask |= BUFFER_MODE;
        }
    }


    /**
     * Returns the current buffer override flag.
     * @return true if buffer override is enabled; otherwise,
     * false is returned
     *
     * @since Java 3D 1.2
     */
    public boolean getBufferOverride() {
	return uBufferOverride;
    }


    /**
     * Sets a flag that enables or disables immediate mode rendering
     * into the front buffer of a double buffered Canvas3D.
     * This attribute is only used when the
     * <code>bufferOverride</code> flag is enabled.
     * <p>
     * Note that this attribute has no effect if double buffering
     * is disabled or is not available on the Canvas3D.
     *
     * @param frontBufferRendering the new front buffer rendering flag
     *
     * @see #setBufferOverride
     *
     * @since Java 3D 1.2
     */
    public void setFrontBufferRendering(boolean frontBufferRendering) {
        uFrontBufferRendering = frontBufferRendering;
        if ((canvas3d.view == null) || 
	    (canvas3d.view.universe == null) ||
	    (!canvas3d.view.active) ||
            (Thread.currentThread() == canvas3d.screen.renderer)) {
            doSetFrontBufferRendering(frontBufferRendering);
        } else if (Thread.currentThread() ==
                        canvas3d.view.universe.behaviorScheduler) {
            sendRenderMessage(false, GraphicsContext3D.SET_FRONT_BUFFER_RENDERING,
				new Boolean(frontBufferRendering), null);
        } else {
            sendRenderMessage(true, GraphicsContext3D.SET_FRONT_BUFFER_RENDERING,
				new Boolean(frontBufferRendering), null);
        }
    }

    void doSetFrontBufferRendering(boolean frontBufferRendering) {
	if (frontBufferRendering != this.frontBufferRendering) {
	    this.frontBufferRendering = frontBufferRendering;
	    dirtyMask |= BUFFER_MODE;
        }
    }


    /**
     * Returns the current front buffer rendering flag.
     * @return true if front buffer rendering is enabled; otherwise,
     * false is returned
     *
     * @since Java 3D 1.2
     */
    public boolean getFrontBufferRendering() {
	return uFrontBufferRendering;
    }


    /**
     * Sets the stereo mode for immediate mode rendering.  The
     * parameter specifies which stereo buffer or buffers is rendered
     * into.  This attribute is only used when the
     * <code>bufferOverride</code> flag is enabled.
     * <ul>
     * <li>
     * <code>STEREO_LEFT</code> specifies that rendering is done into
     * the left eye.
     * </li>
     * <li>
     * <code>STEREO_RIGHT</code> specifies that rendering is done into
     * the right eye.
     * </li>
     * <li>
     * <code>STEREO_BOTH</code> specifies that rendering is done into
     * both eyes.  This is the default.
     * </li>
     * </ul>
     *
     * <p>
     * Note that this attribute has no effect if stereo is disabled or
     * is not available on the Canvas3D.
     *
     * @param stereoMode the new stereo mode
     *
     * @see #setBufferOverride
     *
     * @since Java 3D 1.2
     */
    public void setStereoMode(int stereoMode) {
        uStereoMode = stereoMode;
        if ((canvas3d.view == null) || 
	    (canvas3d.view.universe == null) ||
	    (!canvas3d.view.active) ||
            (Thread.currentThread() == canvas3d.screen.renderer)) {
            doSetStereoMode(stereoMode);
        } else if (Thread.currentThread() ==
                        canvas3d.view.universe.behaviorScheduler) {
            sendRenderMessage(false, GraphicsContext3D.SET_STEREO_MODE,
			stereoModes[stereoMode], null);
        } else {
            sendRenderMessage(true, GraphicsContext3D.SET_STEREO_MODE,
			stereoModes[stereoMode], null);
        }
    }

    void doSetStereoMode(int stereoMode) {
	if (stereoMode != this.stereoMode) {
	    this.stereoMode = stereoMode;
	    dirtyMask |= BUFFER_MODE;
	}
    }


    /**
     * Returns the current stereo mode.
     * @return the stereo mode, one of <code>STEREO_LEFT</code>,
     * <code>STEREO_RIGHT</code>, or <code>STEREO_BOTH</code>.
     *
     * @since Java 3D 1.2
     */
    public int getStereoMode() {
	return uStereoMode;
    }


//
// Methods to draw graphics objects
//

    /**
     * Clear the Canvas3D to the color or image specified by the
     * current background node.
     */
    public void clear() {
        if ((canvas3d.view == null) || (canvas3d.view.universe == null) ||
	    (!canvas3d.view.active)) {
	    return;
        } else if (Thread.currentThread() == canvas3d.screen.renderer) {
            doClear();
        } else if (Thread.currentThread() ==
                        canvas3d.view.universe.behaviorScheduler) {
            sendRenderMessage(false, GraphicsContext3D.CLEAR, null, null);
        } else {
            sendRenderMessage(true, GraphicsContext3D.CLEAR, null, null);
        }
    }

    void doClear() {

        if (!canvas3d.firstPaintCalled)
	    return;

	RenderBin rb = canvas3d.view.renderBin;
	BackgroundRetained back = null;


	if (this.background != null)
	    back = (BackgroundRetained)this.background.retained;
	else
	    back = this.black;

	// TODO: This should ideally be done by the renderer (or by the
	// canvas itself) when the canvas is first added to a view.
	/*
	if ((canvas3d.screen.renderer != null) &&
  	    (canvas3d.screen.renderer.renderBin == null))
	    canvas3d.screen.renderer.renderBin = rb;
	*/
	// If we are in pure immediate mode, update the view cache
	if (!canvas3d.isRunning)
	    updateViewCache(rb);

        // We need to catch NullPointerException when the dsi
        // gets yanked from us during a remove.

        try {
	    if (canvas3d.drawingSurfaceObject.renderLock()) {
		// TODO : Fix texture
		/*
		if (canvas3d.useSharedCtx) {
		    if (canvas3d.screen.renderer.sharedCtx == 0) {
			synchronized (VirtualUniverse.mc.contextCreationLock) {
			    canvas3d.screen.renderer.sharedCtx = canvas3d.createNewContext(
					canvas3d.screen.display,
					canvas3d.window, canvas3d.vid, 0, true,
					canvas3d.offScreen);
			    canvas3d.screen.renderer.sharedCtxTimeStamp = 
				VirtualUniverse.mc.getContextTimeStamp();
			    canvas3d.screen.renderer.needToRebuildDisplayList = true;
			}
		    }
		}
		*/
		
		if (canvas3d.ctx == 0) {
		    synchronized (VirtualUniverse.mc.contextCreationLock) {
			canvas3d.ctx =
			    canvas3d.createNewContext(canvas3d.screen.display, 
						   canvas3d.window,
						   canvas3d.vid,
						   canvas3d.fbConfig,
						   0, false,
						   canvas3d.offScreen);
			if (canvas3d.ctx == 0) {
			    canvas3d.drawingSurfaceObject.unLock();
			    return;
			}

			canvas3d.ctxTimeStamp =
			    VirtualUniverse.mc.getContextTimeStamp();
			canvas3d.screen.renderer.listOfCtxs.add(
								new Long(canvas3d.ctx));
			canvas3d.screen.renderer.listOfCanvases.add(canvas3d);

			canvas3d.beginScene();

			if (canvas3d.graphics2D != null) {
			    canvas3d.graphics2D.init();
			}

                        // query for the number of texture units
			// supported
                        if (canvas3d.multiTexAccelerated) {
                            canvas3d.numTexUnitSupported =
                                    canvas3d.getTextureUnitCount(canvas3d.ctx);
                        }
			
			// enable separate specular color
			canvas3d.enableSeparateSpecularColor();
		    }

                    // create the cache texture state in canvas
                    // for state download checking purpose

                    if (canvas3d.texUnitState == null) {
                        canvas3d.texUnitState =
                                new TextureUnitStateRetained[
                                        canvas3d.numTexUnitSupported];
                        for (int t = 0; t < canvas3d.numTexUnitSupported; t++) {
                            canvas3d.texUnitState[t] =
                                        new TextureUnitStateRetained();
                            canvas3d.texUnitState[t].texture = null;
                            canvas3d.texUnitState[t].mirror = null;
                        }
                    }


                    // also create the texture unit state map
                    // which is a mapping from texture unit state to
                    // the actual underlying texture unit

                    if (canvas3d.texUnitStateMap == null) {
                        canvas3d.texUnitStateMap = 
                                        new int[canvas3d.numTexUnitSupported];
                    }


		    canvas3d.drawingSurfaceObject.contextValidated();
		    canvas3d.screen.renderer.currentCtx = canvas3d.ctx;
		    initializeState();
		    canvas3d.ctxChanged = true;
		    canvas3d.canvasDirty = 0xffff;
		    // Update Appearance
		    updateState(rb, RenderMolecule.SURFACE);
		    
		    canvas3d.currentLights = new 
			LightRetained[canvas3d.getNumCtxLights(canvas3d.ctx)];
		    
		    for (int j=0; j<canvas3d.currentLights.length; j++) {
			canvas3d.currentLights[j] = null;
		    }
		}

	  
		canvas3d.makeCtxCurrent();
		
		if ((dirtyMask & BUFFER_MODE) != 0) {
		    if (bufferOverride) {
			canvas3d.setRenderMode(canvas3d.ctx, stereoMode,
					       canvas3d.useDoubleBuffer && !frontBufferRendering);
		    } else {
			if (!canvas3d.isRunning) {
			    canvas3d.setRenderMode(canvas3d.ctx, 
						   Canvas3D.FIELD_ALL,
						   canvas3d.useDoubleBuffer);
			}
		    }
		    dirtyMask &= ~BUFFER_MODE;
		}
	    
		Dimension size = canvas3d.getSize();
		int winWidth  = size.width;
		int winHeight = size.height;
		
		if (back.image != null && back.image.isByReference()) {
		back.image.geomLock.getLock();
		back.image.evaluateExtensions(canvas3d.extensionsSupported);
		if (!VirtualUniverse.mc.isBackgroundTexture) {
		    canvas3d.clear(canvas3d.ctx,
				   back.color.x, back.color.y, 
				   back.color.z,  winWidth, winHeight, back.image,
				   back.imageScaleMode,
				   back.image != null?back.image.imageYdown[0]:null);
		}
		else {

		    // this is if the background image resizes with the canvas
// 		    Dimension size = null;
// 		    canvas3d.getSize(size);
// 		    int xmax = size.width;
// 		    int ymax = size.height;
		    if (objectId == -1) {
			objectId = VirtualUniverse.mc.getTexture2DId();
		    }

		    canvas3d.textureclear(canvas3d.ctx,
					  back.xmax, back.ymax,
					  back.color.x, back.color.y,
					  back.color.z, winWidth, winHeight,
					  objectId, back.imageScaleMode, back.texImage, true);
		}
		back.image.geomLock.unLock();
	    }
	    else {
		if (!VirtualUniverse.mc.isBackgroundTexture) {
		    canvas3d.clear(canvas3d.ctx,
				   back.color.x, back.color.y, 
				   back.color.z, winWidth, winHeight, back.image,
				   back.imageScaleMode,
				   back.image != null?back.image.imageYdown[0]:null);
		}
		else {

		    // this is if the background image resizes with the canvas
// 		    Dimension size = null;
// 		    canvas3d.getSize(size);
// 		    int xmax = size.width;
// 		    int ymax = size.height;
		    if (objectId == -1) {
			objectId = VirtualUniverse.mc.getTexture2DId();
		    }
		    
		    canvas3d.textureclear(canvas3d.ctx,
					  back.xmax, back.ymax,
					  back.color.x, back.color.y,
					  back.color.z,
					  winWidth, winHeight,
					  objectId, back.imageScaleMode, back.texImage, true);
		}
	    }

	    // Set the viewport and view matrices
		if (!canvas3d.isRunning) {
		    CanvasViewCache cvCache = canvas3d.canvasViewCache;
		    canvas3d.setViewport(canvas3d.ctx,
					 0, 0,
					 cvCache.getCanvasWidth(),
					 cvCache.getCanvasHeight());
		    if (bufferOverride && (stereoMode == STEREO_RIGHT)) {
			canvas3d.setProjectionMatrix(canvas3d.ctx,
						     cvCache.getRightProjection().mat);
			canvas3d.setModelViewMatrix(canvas3d.ctx,
						    cvCache.getRightVpcToEc().mat,
						    rb.vworldToVpc);
		    }
		    else {
			canvas3d.setProjectionMatrix(canvas3d.ctx,
						     cvCache.getLeftProjection().mat);
			canvas3d.setModelViewMatrix(canvas3d.ctx,
						    cvCache.getLeftVpcToEc().mat,
						    rb.vworldToVpc);
		    }
		}

	    canvas3d.drawingSurfaceObject.unLock();
	  }
	} catch (NullPointerException ne) {
	    canvas3d.drawingSurfaceObject.unLock();
	    throw ne;
	}
    }
  
    // Method to update compTransform.
    private void computeCompositeTransform() {
	ViewPlatform vp;
 	
	if ((canvas3d == null) || 
	    (canvas3d.view == null) ||
	    (((vp = canvas3d.view.getViewPlatform()) == null)) ||
	    (((ViewPlatformRetained)(vp.retained)) == null)) {
	    compTransform.set(modelTransform);
	    return;
	}
	    
        ViewPlatformRetained vpR = (ViewPlatformRetained)vp.retained;
	if ((vpR == null) || (vpR.locale == null)) {
	    compTransform.set(modelTransform);
	    return;
	}

	HiResCoord localeHiRes = vpR.locale.hiRes;

	if (localeHiRes.equals(hiRes)) {
	    compTransform.set(modelTransform);
	} else {
	    Transform3D trans = new Transform3D();
	    Vector3d localeTrans = new Vector3d();
	    localeHiRes.difference( hiRes, localeTrans );
	    trans.setTranslation( localeTrans );
	    compTransform.mul(trans, modelTransform);
	}
    }
  
    // Method to update the view cache in pure immediate mode
    private void updateViewCache(RenderBin rb) {

	ViewPlatform vp = canvas3d.view.getViewPlatform();

	if (vp == null)
	    return;

	ViewPlatformRetained vpR = (ViewPlatformRetained)vp.retained;

	if (!canvas3d.isRunning) {
	    // in pure immediate mode, notify platform transform change
	    vpR.evaluateInitViewPlatformTransform();
	}


	//	rb.setVworldToVpc(vp.getVworldToVpc());
	//	rb.setVpcToVworld(vp.getVpcToVworld());

	// TODO: Fix this
	rb.vpcToVworld = vpR.getVpcToVworld();
	rb.vworldToVpc = vpR.getVworldToVpc();

	canvas3d.updateViewCache(true, null, null, false);
    }

    void doDraw(Geometry geometry) {

	boolean useAlpha;
	GeometryRetained drawGeo;
	GeometryArrayRetained geoRetained = null;


        if (!canvas3d.firstPaintCalled || !visible) {
	    return;
	}

	RenderBin rb = canvas3d.view.renderBin;
	int i, nlights, activeLights;
	LightRetained light;
	boolean lightingOn = true;

	if (canvas3d.ctx == 0) {
	    // Force an initial clear if one has not yet been done
	    doClear();
	}


        if (J3dDebug.devPhase && J3dDebug.debug) {
            J3dDebug.doAssert(canvas3d.ctx != 0, "canvas3d.ctx != 0");
        }

        // We need to catch NullPointerException when the dsi 
        // gets yanked from us during a remove.
        try { 
	  if (canvas3d.drawingSurfaceObject.renderLock()) {

	    // Make the context current
	    canvas3d.makeCtxCurrent(); 

	    if ((dirtyMask & BUFFER_MODE) != 0) {
		if (bufferOverride) {
	 	    canvas3d.setRenderMode(canvas3d.ctx, stereoMode,
        		canvas3d.useDoubleBuffer && !frontBufferRendering);
		} else {
	 	    canvas3d.setRenderMode(canvas3d.ctx, Canvas3D.FIELD_ALL,
        		canvas3d.useDoubleBuffer);
		}
		dirtyMask &= ~BUFFER_MODE;
	    }
		
	    CanvasViewCache cvCache = canvas3d.canvasViewCache; 
	    Transform3D proj;

//  	    vpcToEc = cvCache.getLeftVpcToEc(); 
	    if (bufferOverride) {
		switch(stereoMode) {
		case STEREO_RIGHT:
		    vpcToEc = cvCache.getRightVpcToEc();
		    // TODO: move this under check for 
		    // (dirtyMask & BUFFER_MODE) above after testing
		    // PureImmediate mode
		    canvas3d.setProjectionMatrix(canvas3d.ctx,
						 cvCache.getRightProjection().
						 mat);
		    break;
		case STEREO_LEFT:
		case STEREO_BOTH:
		default:
		    vpcToEc = cvCache.getLeftVpcToEc();
		    // TODO: move this under check for 
		    // (dirtyMask & BUFFER_MODE) above after testing
		    // PureImmediate mode
		    canvas3d.setProjectionMatrix(canvas3d.ctx,
						 cvCache.getLeftProjection().
						 mat);
		}
	    }
	    else if (!canvas3d.isRunning || 
		     // vpcToEc is not set in the first frame
		     // of preRender() callback
		     (canvas3d.vpcToEc == null)) {
		    vpcToEc = cvCache.getLeftVpcToEc();
	    } 
	    else {
		vpcToEc = canvas3d.vpcToEc;
	    }

	    // referred by RenderQueue.updateState
	    //	    canvas3d.screen.renderer.vpcToEc = vpcToEc;
	    //	    rb.updateState(canvas3d.screen.renderer.rId, ro, canvas3d, true);


	    //	    this.drawTransform.mul(rb.vworldToVpc,
	    //	    this.compTransform); 

	    boolean isNonUniformScale = !drawTransform.isCongruent(); 

	    int geometryType = 0;
	    switch (((GeometryRetained)geometry.retained).geoType) {
            case GeometryRetained.GEO_TYPE_POINT_SET:
            case GeometryRetained.GEO_TYPE_INDEXED_POINT_SET:
                geometryType = RenderMolecule.POINT;
                break;
            case GeometryRetained.GEO_TYPE_LINE_SET:
            case GeometryRetained.GEO_TYPE_LINE_STRIP_SET:
            case GeometryRetained.GEO_TYPE_INDEXED_LINE_SET:
            case GeometryRetained.GEO_TYPE_INDEXED_LINE_STRIP_SET:
                geometryType = RenderMolecule.LINE;
                break;
            case GeometryRetained.GEO_TYPE_RASTER:
                geometryType = RenderMolecule.RASTER;
                break;
            case GeometryRetained.GEO_TYPE_COMPRESSED:
                geometryType = RenderMolecule.COMPRESSED;
    
                switch (((CompressedGeometryRetained)geometry.retained).getBufferType()) {
                case CompressedGeometryHeader.POINT_BUFFER:
                    geometryType |= RenderMolecule.POINT ;
                    break ;
                case CompressedGeometryHeader.LINE_BUFFER:
                    geometryType |= RenderMolecule.LINE ;
                    break ;
                default:
                case CompressedGeometryHeader.TRIANGLE_BUFFER:
                    geometryType |= RenderMolecule.SURFACE ;
                    break ;
                }
                break;
            default:
                geometryType = RenderMolecule.SURFACE;
                break;
            }
	    
	    useAlpha = updateState(rb, geometryType);

	    canvas3d.setModelViewMatrix(canvas3d.ctx,
					vpcToEc.mat,
					rb.vworldToVpc);
	    updateLightAndFog();

	    updateModelClip(rb.vworldToVpc);

	    this.drawTransform.mul(rb.vworldToVpc, this.compTransform); 
	    canvas3d.setModelViewMatrix(canvas3d.ctx,
					vpcToEc.mat, this.drawTransform);

	    if (geometry.retained instanceof GeometryArrayRetained) {
		geoRetained = (GeometryArrayRetained)geometry.retained;

	        geoRetained.geomLock.getLock();
	        // If the geometry is by refernence, then see if we are using alpha
	        // and that there is no global alpha sun extension defined ..
	        if ((( geoRetained.vertexFormat & GeometryArray.BY_REFERENCE)!=0) &&
		    (geoRetained.c4fAllocated == 0) &&
		    ((geoRetained.vertexFormat & GeometryArray.COLOR) != 0) &&
		    useAlpha && (canvas3d.extensionsSupported &Canvas3D.SUN_GLOBAL_ALPHA) == 0 ) {

		    if ((geoRetained.vertexFormat & GeometryArray.INTERLEAVED) != 0) {
		        geoRetained.setupMirrorInterleavedColorPointer(true);
		    }
		    else {
		        geoRetained.setupMirrorColorPointer((geoRetained.vertexType & GeometryArrayRetained.COLOR_DEFINED),true);
		    }
	        }
		
	        if ((geometry.retained instanceof IndexedGeometryArrayRetained) &&
		    ((((GeometryArrayRetained)geometry.retained).vertexFormat & GeometryArray.USE_COORD_INDEX_ONLY) == 0)) {
		    if (geoRetained.dirtyFlag != 0) {
			geoRetained.mirrorGeometry = (GeometryRetained) 
			    ((IndexedGeometryArrayRetained)geoRetained).cloneNonIndexedGeometry();
			// Change the source geometry dirtyFlag
			// drawGeo.execute() will change the
			// destination geometry dirtyFlag only.
			geoRetained.dirtyFlag = 0;
		    }
		    drawGeo = (GeometryRetained)geoRetained.mirrorGeometry;
	        } else {
		    drawGeo = geoRetained;
                }

	        geoRetained.setVertexFormat(false, ignoreVertexColors, canvas3d.ctx );

	    } else if (geometry.retained instanceof Text3DRetained) { 
	        ((Text3DRetained)geometry.retained).setModelViewMatrix(
			vpcToEc, this.drawTransform);
		drawGeo = (GeometryRetained)geometry.retained;
	    } else if (geometry.retained instanceof RasterRetained) {
		ImageComponent2DRetained img = ((RasterRetained)geometry.retained).image;
		if (img != null && img.isByReference()) {
		    img.geomLock.getLock();
		    img.evaluateExtensions(canvas3d.extensionsSupported);
		    img.geomLock.unLock();
		}
		drawGeo = (GeometryRetained)geometry.retained;
	    } else {
		drawGeo = (GeometryRetained)geometry.retained;
	    }

	    if (!toSimulateMultiTex) {
		drawGeo.execute(canvas3d, null, isNonUniformScale,
				false, alpha, 
				((canvas3d.view.getScreens()).length > 1), 
				canvas3d.screen.screen,
				ignoreVertexColors, 
				-1);
	    } else {
		// TODO: need to leverage the code in textureBin
		boolean startToSimulate = false;
		if (numActiveTexUnit < 1) {
		    // no active texture unit
		    drawGeo.execute(canvas3d, null, isNonUniformScale,
				false, alpha, 
				((canvas3d.view.getScreens()).length > 1), 
				canvas3d.screen.screen,
				ignoreVertexColors, 
				0);
		} else if (numActiveTexUnit == 1) {
		    // one active texture unit
		    drawGeo.execute(canvas3d, null, isNonUniformScale,
				    false,   alpha, 
				((canvas3d.view.getScreens()).length > 1), 
				canvas3d.screen.screen,
				ignoreVertexColors, 
				lastActiveTexUnitIndex);
		} else {
		    // simulate multiple texture units
		    AppearanceRetained app = 
				(AppearanceRetained)appearance.retained;

		    // first turn off fog
		    if (fog != null)
			canvas3d.setFogEnableFlag(canvas3d.ctx, false);

		    for (i = 0; i < app.texUnitState.length; i++) {
			 if (app.texUnitState[i] != null &&
				app.texUnitState[i].isTextureEnabled()) {
			 
			     // turn on fog for the last pass 
			     if (i == lastActiveTexUnitIndex)
				 canvas3d.setFogEnableFlag(canvas3d.ctx, true);

			     app.texUnitState[i].updateNative(-1, canvas3d,
				false, startToSimulate);

			     startToSimulate = true;
		    	     drawGeo.execute(canvas3d, null, 
				isNonUniformScale, false, alpha, 
				((canvas3d.view.getScreens()).length > 1), 
				canvas3d.screen.screen,
				ignoreVertexColors, 
				i);
			 }
		    }

		    // adjust the depth test back to what it was
		    // and adjust the blend func to what it it was
		    if (startToSimulate) {
		        app.transparencyAttributes.updateNative(
				canvas3d.ctx, alpha, geometryType,
				polygonMode, lineAA, pointAA);
		    }

		    if (fog != null) {
			canvas3d.setFogEnableFlag(canvas3d.ctx, true);
		    }
		}
	    }
	    if (geoRetained != null)
	        geoRetained.geomLock.unLock();

	    canvas3d.drawingSurfaceObject.unLock(); 
	  }
	} catch (NullPointerException ne) { 
	    canvas3d.drawingSurfaceObject.unLock();
	    throw ne;
	} 
    }

    /**
     * Draw the specified Geometry component object.
     * @param geometry the Geometry object to draw.
     */
    public void draw(Geometry geometry) {
        if ((canvas3d.view == null) || (canvas3d.view.universe == null) ||
		(!canvas3d.view.active)) {
	    return;
        } else if (Thread.currentThread() == canvas3d.screen.renderer) {
            doDraw(geometry);
        } else {
	    if (Thread.currentThread() ==
                        canvas3d.view.universe.behaviorScheduler) {
                sendRenderMessage(false, GraphicsContext3D.DRAW, 
						geometry, null);
            } else {
                sendRenderMessage(true, GraphicsContext3D.DRAW, geometry, 
			null);
            }
	}
    }

    /**
     * Draw the specified Shape3D leaf node object.  This is
     * a convenience method that is identical to calling the
     * setAppearance(Appearance) and draw(Geometry) methods
     * passing the appearance and geometry component objects of
     * the specified shape node as arguments.
     * @param shape the Shape3D node containing the Appearance component
     * object to set and Geometry component object to draw
     * @exception IllegalSharingException if the Shape3D node
     * is part of or is subsequently made part of a live scene graph.
     */
    public void draw(Shape3D shape) {
	if (shape.isLive()) {
	    throw new IllegalSharingException(J3dI18N.getString("GraphicsContext3D26"));
	}
        ((Shape3DRetained)shape.retained).setInImmCtx(true); 
	setAppearance(shape.getAppearance());
	draw(shape.getGeometry());
    }

    /**
     * Native method for readRaster
     */  
    native void readRasterNative(long d3dctx,
				 int type, int xSrcOffset, int ySrcOffset,
				 int width, int height, int hCanvas, int format,
				 ImageComponentRetained image, 
				 DepthComponentRetained depth, 
				 GraphicsContext3D ctx);

    /**
     * Read an image from the frame buffer and copy it into the
     * ImageComponent and/or DepthComponent
     * objects referenced by the specified Raster object.
     * All parameters of the Raster object and the component ImageComponent
     * and/or DepthComponentImage objects must be set to the desired values
     * prior to calling this method.  These values determine the location,
     * size, and format of the pixel data that is read.
     * This method calls <code>flush(true)</code> prior to reading the
     * frame buffer.
     *
     * @param raster the Raster object used to read the
     * contents of the frame buffer
     *
     * @exception IllegalArgumentException if the Raster's
     * ImageComponent2D is in by-reference mode and its RenderedImage
     * is not an instance of a BufferedImage.
     *
     * @exception IllegalSharingException if the Raster object
     * is part of a live scene graph.
     *
     * @see #flush
     * @see ImageComponent
     * @see DepthComponent
     */
    public void readRaster(Raster raster) {
        if ((canvas3d.view == null) || (canvas3d.view.universe == null) ||
		(!canvas3d.view.active)) {
            return;
        } else if (Thread.currentThread() == canvas3d.screen.renderer) {
            doReadRaster(raster);
        } else if (Thread.currentThread() ==
                        canvas3d.view.universe.behaviorScheduler) {
	    readRasterReady = false;	    
            sendRenderMessage(false, GraphicsContext3D.READ_RASTER, raster, null);
	    while (!readRasterReady) {
		MasterControl.threadYield();
	    } 
        } else {
	    // call from user thread
	    readRasterReady = false;
            sendRenderMessage(true, GraphicsContext3D.READ_RASTER, raster, null);
	    while (!readRasterReady) {
		MasterControl.threadYield();
	    } 
        }
    }



    void doReadRaster(Raster raster) {


	if (!canvas3d.firstPaintCalled) {
	    readRasterReady = true;
	    return;
	}

	RasterRetained ras = (RasterRetained)raster.retained;
        Dimension canvasSize = canvas3d.getSize();
	int format = 0; // Not use in case of DepthComponent read

	if (raster.isLive()) {
	    readRasterReady = true;
           throw new IllegalSharingException(J3dI18N.getString("GraphicsContext3D21"));
        }

	// TODO: implement illegal argument exception
	/*
	if (ras.image.byReference &&
	    !(ras.image.imageReference instanceof BufferedImage)) {

	    throw new IllegalArgumentException(...);
	}
	*/

	if (canvas3d.ctx == 0) {
	    // Force an initial clear if one has not yet been done
	    doClear();
	}

        if (J3dDebug.devPhase && J3dDebug.debug) {
            J3dDebug.doAssert(canvas3d.ctx != 0, "canvas3d.ctx != 0");
        }


	// allocate read buffer space
        if ( (ras.type & Raster.RASTER_COLOR) != 0) {
	    int bpp = ras.image.getEffectiveBytesPerPixel();
            int size = ras.image.height * ras.image.width 
			* bpp;
	    format = ras.image.getEffectiveFormat();
	    if ((ras.width > ras.image.width) ||
		(ras.height > ras.image.height)) {
		throw new RuntimeException(J3dI18N.getString("GraphicsContext3D27"));
	    }
            if (byteBuffer.length < size)
                byteBuffer = new byte[size];
        }

        if ( (ras.type & Raster.RASTER_DEPTH) != 0) {
            int size = ras.depthComponent.height * ras.depthComponent.width;
            if (ras.depthComponent.type 
		== DepthComponentRetained.DEPTH_COMPONENT_TYPE_FLOAT) {
                if (floatBuffer.length < size)
                    floatBuffer = new float[size];
            } else { // type INT or NATIVE
                if (intBuffer.length < size)
                    intBuffer = new int[size];
            }
	    if ((ras.width > ras.depthComponent.width) ||
		(ras.height > ras.depthComponent.height)) {
		throw new RuntimeException(J3dI18N.getString("GraphicsContext3D28"));		
	    }
        }
	
        if ( (ras.type & Raster.RASTER_COLOR) != 0) {
	    // If by reference, check if a copy needs to be made
	    // and also evaluate the storedFormat ..
	    if (ras.image.isByReference()) {
		ras.image.geomLock.getLock();
		ras.image.evaluateExtensions(canvas3d.extensionsSupported);
		ras.image.geomLock.unLock();
	    }
	}

        // We need to catch NullPointerException when the dsi
        // gets yanked from us during a remove.
        try {
	  if (canvas3d.drawingSurfaceObject.renderLock()) {
	    // Make the context current and read the raster information
	    canvas3d.makeCtxCurrent();
	    canvas3d.syncRender(canvas3d.ctx, true);
            readRasterNative(canvas3d.ctx,
			     ras.type, ras.xSrcOffset, ras.ySrcOffset,
			     ras.width, ras.height, canvasSize.height, format,
			     ras.image, ras.depthComponent, this);
	    canvas3d.drawingSurfaceObject.unLock();
	  }
	} catch (NullPointerException ne) {
	    canvas3d.drawingSurfaceObject.unLock();
	    throw ne;
	}

	// flip color image: yUp -> yDown and convert to BufferedImage
        if ( (ras.type & Raster.RASTER_COLOR) != 0) {
            ras.image.retrieveImage(byteBuffer, ras.width, ras.height);
        }

        if ( (ras.type & Raster.RASTER_DEPTH) != 0) {
	    if (ras.depthComponent.type == 
			DepthComponentRetained.DEPTH_COMPONENT_TYPE_FLOAT)
                ((DepthComponentFloatRetained)ras.depthComponent).retrieveDepth(
				floatBuffer, ras.width, ras.height);
	    else if (ras.depthComponent.type == 
			DepthComponentRetained.DEPTH_COMPONENT_TYPE_INT)
                ((DepthComponentIntRetained)ras.depthComponent).retrieveDepth(
				intBuffer, ras.width, ras.height);
	    else if (ras.depthComponent.type == 
			DepthComponentRetained.DEPTH_COMPONENT_TYPE_NATIVE)
               ((DepthComponentNativeRetained)ras.depthComponent).retrieveDepth(
				intBuffer, ras.width, ras.height);
        }
	readRasterReady = true;
    }

    /**
     * Flushes all previously executed rendering operations to the
     * drawing buffer for this 3D graphics context.
     *
     * @param wait flag indicating whether or not to wait for the
     * rendering to be complete before returning from this call.
     *
     * @since Java 3D 1.2
     */
    public void flush(boolean wait) {
        if ((canvas3d.view == null) || 
	    (canvas3d.view.universe == null) ||
	    (!canvas3d.view.active) ||
            (Thread.currentThread() == canvas3d.screen.renderer)) {
            doFlush(wait);
        } else  {
	    Boolean waitArg = (wait ? Boolean.TRUE : Boolean.FALSE);

	    if (Thread.currentThread() ==
                        canvas3d.view.universe.behaviorScheduler) {
                sendRenderMessage(false, GraphicsContext3D.FLUSH, waitArg, 
					null);
	    } else {
                sendRenderMessage(true, GraphicsContext3D.FLUSH, waitArg, 
					null);
	    }
	    if (wait && canvas3d.active && canvas3d.isRunningStatus &&
		!canvas3d.offScreen) { 
		// No need to wait if renderer thread is not schedule
		runMonitor(J3dThread.WAIT);
	    } 
        }
    }

    void doFlush(boolean wait) {
	try {
	  if (canvas3d.drawingSurfaceObject.renderLock()) {	
	      canvas3d.syncRender(canvas3d.ctx, wait);
	      canvas3d.drawingSurfaceObject.unLock();
	      if (wait) {
		  runMonitor(J3dThread.NOTIFY);
	      }
	  }
	} catch (NullPointerException ne) {
	    canvas3d.drawingSurfaceObject.unLock();
	    throw ne;
	}
    }

    void updateLightAndFog() {
	int enableMask = 0;
	int i;
	sceneAmbient.x = 0.0f;
	sceneAmbient.y = 0.0f;
	sceneAmbient.z = 0.0f;

	int n = 0;
	int nLight = lights.size();;
	for (i = 0; i < nLight;i++) {
	    LightRetained lt = (LightRetained)((Light)lights.get(i)).retained;
	    if (lt instanceof AmbientLightRetained) {
		sceneAmbient.x += lt.color.x; 
		sceneAmbient.y += lt.color.y; 
		sceneAmbient.z += lt.color.z;
		continue;
	    }
	    
	    lt.update(canvas3d.ctx, n, 
		      canvas3d.canvasViewCache.getVworldToCoexistenceScale());
	    if (lt.lightOn)
		enableMask |= (1 << n);
	    n++;
	}
	if (sceneAmbient.x > 1.0f) {
	    sceneAmbient.x = 1.0f;
	}
	if (sceneAmbient.y > 1.0f) {
	    sceneAmbient.y = 1.0f;
	}
	if (sceneAmbient.z > 1.0f) {
	    sceneAmbient.z = 1.0f;
	}
	
	canvas3d.setSceneAmbient(canvas3d.ctx, sceneAmbient.x,
				 sceneAmbient.y, sceneAmbient.z);

	canvas3d.canvasDirty |= Canvas3D.AMBIENTLIGHT_DIRTY;
	canvas3d.sceneAmbient.set(sceneAmbient);  

	if (canvas3d.enableMask != enableMask) {
	    canvas3d.canvasDirty |= Canvas3D.LIGHTENABLES_DIRTY;
	    // TODO: 32 => renderBin.maxLights
	    canvas3d.setLightEnables(canvas3d.ctx, enableMask, 32);
	    canvas3d.enableMask = enableMask;
	}

	// Force LightBin.updateAttributes and EnvironmentSet.updateAttributes 
	// to use the within frame case.
	canvas3d.lightBin = null;
	canvas3d.environmentSet = null;

	if (fog != null) {
	    if (fog.retained != canvas3d.fog) {
		((FogRetained)fog.retained).update(canvas3d.ctx,
			   canvas3d.canvasViewCache.getVworldToCoexistenceScale());
		canvas3d.fog = (FogRetained) fog.retained;
		canvas3d.canvasDirty |= Canvas3D.FOG_DIRTY;
	    }
	} else { // Turn off fog
	    if (canvas3d.fog != null) {
		canvas3d.setFogEnableFlag(canvas3d.ctx, false);
		canvas3d.fog = null;
		canvas3d.canvasDirty |= Canvas3D.FOG_DIRTY;
	    }
	}
    }

    void updateModelClip(Transform3D vworldToVpc) {
 	if (modelClip != null) {
	    int enableMask = 0;
	    for (int i = 0; i < 6; i++) {
	         if (((ModelClipRetained)modelClip.retained).enables[i])
		     enableMask |= 1 << i;
	    }
	    // planes are already transformed to eye coordinates
	    // in immediate mode
	    if (enableMask != 0) {
	        this.drawTransform.mul(vworldToVpc, this.modelClipTransform);
	    	canvas3d.setModelViewMatrix(canvas3d.ctx, vpcToEc.mat,
						this.drawTransform);
	    }
	    ((ModelClipRetained)modelClip.retained).update(
			canvas3d.ctx, enableMask, 
			this.drawTransform);
	    canvas3d.canvasDirty |= Canvas3D.MODELCLIP_DIRTY;
	    canvas3d.modelClip = (ModelClipRetained) modelClip.retained;
	} else {
	    if (canvas3d.modelClip != null) {
		canvas3d.disableModelClip(canvas3d.ctx);
		canvas3d.modelClip = null;
		canvas3d.canvasDirty |= Canvas3D.MODELCLIP_DIRTY;
	    }
	}

	// Force EnvironmentSet.updateAttributes to  use the within frame case.
	canvas3d.environmentSet = null;

    }

    

    boolean updateState(RenderBin rb, int geometryType) {

	boolean useAlpha = false;;
	toSimulateMultiTex = true;
	numActiveTexUnit = 0;
	lastActiveTexUnitIndex = 0;

	// Update Appearance
	if (appearance != null) {
	    AppearanceRetained app = (AppearanceRetained) appearance.retained;

	    // If the material is not null then check if the one in the canvas
	    // is equivalent to the one being sent down. If Yes, do nothing
	    // Otherwise, cache the sent down material and mark the canvas
	    // dirty flag so that the compiled/compiled-retained rendering
	    // catches the change
	    // if material != null, we will need to load the material
	    // parameter again, because the apps could have changed
	    // the material parameter

	    if (app.material != null) {
		app.material.updateNative(canvas3d.ctx,
					      red,green,blue,
					      alpha,enableLighting);	    
		canvas3d.material = app.material;
		canvas3d.canvasDirty |= Canvas3D.MATERIAL_DIRTY;
	    } else {
		if (canvas3d.material != null) {
		    canvas3d.updateMaterial(canvas3d.ctx, 
					    red, green, blue, alpha);		    
		    canvas3d.material = null;
		    canvas3d.canvasDirty |= Canvas3D.MATERIAL_DIRTY;
		}
	    }

            int prevNumActiveTexUnit = canvas3d.getNumActiveTexUnit();

	    if (app.texUnitState != null) {
		boolean d3dBlendMode = false;

		TextureUnitStateRetained tus;

		for (int i = 0; i < app.texUnitState.length; i++) {
		    tus = app.texUnitState[i];
		    if (tus != null && tus.isTextureEnabled()) {
			numActiveTexUnit++;
			lastActiveTexUnitIndex = i;
			useAlpha = useAlpha ||
				 (tus.texAttrs.textureMode ==
				  TextureAttributes.BLEND);
			if (tus.needBlend2Pass(canvas3d)) {
		            // use multi-pass if one of the stage use blend mode
			    d3dBlendMode = true;
			}
		    }
		}

		if (canvas3d.numTexUnitSupported >= numActiveTexUnit &&
			canvas3d.multiTexAccelerated && !d3dBlendMode) {

		    int j = 0;

                    // update all active texture unit states

		    for (int i = 0; i < app.texUnitState.length; i++) {
			if ((app.texUnitState[i] != null) &&
				    app.texUnitState[i].isTextureEnabled()) {
			    app.texUnitState[i].updateNative(j, canvas3d, 
								false, false);
			    canvas3d.setTexUnitStateMap(i, j++);
			} 
		    }

                    // reset the remaining texture units

                    for (int i = j; i < prevNumActiveTexUnit; i++) {
			if (canvas3d.texUnitState[i].texture != null) {
                            canvas3d.resetTexture(canvas3d.ctx, i);
			    canvas3d.texUnitState[i].texture = null;
			}
                    }

                    // set the number active texture unit in Canvas3D
                    canvas3d.setNumActiveTexUnit(numActiveTexUnit);

                    // set the active texture unit back to 0
                    canvas3d.activeTextureUnit(canvas3d.ctx, 0);

		    toSimulateMultiTex = false;

		} else {

                    // will fall back to the multi-pass case;
                    // reset all the texture units first

                    for (int i = 0; i < prevNumActiveTexUnit; i++) {
			if (canvas3d.texUnitState[i].texture != null) {
                            canvas3d.resetTexture(canvas3d.ctx, i);
			    canvas3d.texUnitState[i].texture = null;
			}
                    }
                }
	    } else {
		// if texUnitState is null, let's disable
		// all texture units first
		if (canvas3d.multiTexAccelerated) {
		    if (canvas3d.texUnitState != null) {
			for (int i = 0; i < prevNumActiveTexUnit; i++) {
			    TextureUnitStateRetained tur = canvas3d.texUnitState[i];
			    if ((tur != null) && (tur.texture != null)) {
				canvas3d.resetTexture(canvas3d.ctx, i);
				canvas3d.texUnitState[i].texture = null;
			    }
			}	    
		    }

                    // set the active texture unit back to 0
                    canvas3d.activeTextureUnit(canvas3d.ctx, 0);
		}

	        if ((canvas3d.texUnitState != null) && 
		    (canvas3d.texUnitState[0] != null) &&
		    (canvas3d.texUnitState[0].texture != app.texture)) {

		    // If the image is by reference, check if the image
		    // should be processed
		    if (app.texture != null) {
			for (int f = 0; f < app.texture.numFaces; f++) {
			    for (int k = 0; k < app.texture.maxLevels; k++) {
			        if (app.texture.images[f][k].isByReference()) {
				    app.texture.images[f][k].geomLock.getLock();
				    app.texture.images[f][k].evaluateExtensions(canvas3d.extensionsSupported);
				    app.texture.images[f][k].geomLock.unLock();
				}
			    }
			}
		        app.texture.updateNative(canvas3d);
			canvas3d.setTexUnitStateMap(0, 0);
		        canvas3d.canvasDirty |= Canvas3D.TEXTUREBIN_DIRTY|Canvas3D.TEXTUREATTRIBUTES_DIRTY;
			numActiveTexUnit = 1;
		        lastActiveTexUnitIndex = 0;
		    }
		    else {
		        numActiveTexUnit = 0;
		        canvas3d.resetTexture(canvas3d.ctx, -1);
		        canvas3d.canvasDirty |= Canvas3D.TEXTUREBIN_DIRTY|Canvas3D.TEXTUREATTRIBUTES_DIRTY;
		    }

		    canvas3d.texUnitState[0].texture = app.texture;
	        }

                // set the number active texture unit in Canvas3D
                canvas3d.setNumActiveTexUnit(numActiveTexUnit);

	        if (app.texCoordGeneration != null) {
		    app.texCoordGeneration.updateNative(canvas3d);
		    canvas3d.canvasDirty |= Canvas3D.TEXTUREBIN_DIRTY|Canvas3D.TEXTUREATTRIBUTES_DIRTY;
		    if ((canvas3d.texUnitState != null) && 
			(canvas3d.texUnitState[0] != null)) {
			canvas3d.texUnitState[0].texGen = app.texCoordGeneration;
		    }
	        }
	        else {
		    // If the canvas does not alreadt have a null texCoordGeneration
		    // load the default
		    if ((canvas3d.texUnitState != null) && 
			(canvas3d.texUnitState[0] != null) &&
			(canvas3d.texUnitState[0].texGen != null)) {
		        canvas3d.resetTexCoordGeneration(canvas3d.ctx);
		        canvas3d.canvasDirty |= Canvas3D.TEXTUREBIN_DIRTY|Canvas3D.TEXTUREATTRIBUTES_DIRTY;		    
		        canvas3d.texUnitState[0].texGen = app.texCoordGeneration;
		    }
	        }
    
    
	        if (app.textureAttributes != null) {
		    if ((canvas3d.texUnitState != null) && 
			(canvas3d.texUnitState[0] != null)) {
			
			if (canvas3d.texUnitState[0].texture != null) {
			    app.textureAttributes.updateNative(canvas3d, false,
				       canvas3d.texUnitState[0].texture.format);
			} else {
			    app.textureAttributes.updateNative(canvas3d, false,
							       Texture.RGBA);
			}
			canvas3d.canvasDirty |= Canvas3D.TEXTUREBIN_DIRTY|Canvas3D.TEXTUREATTRIBUTES_DIRTY;
			canvas3d.texUnitState[0].texAttrs =
			    app.textureAttributes;
		    }
	        }
	        else {
		    // If the canvas does not already have a null texAttribute
		    // load the default if necessary
		    if ((canvas3d.texUnitState != null) && 
			(canvas3d.texUnitState[0] != null) && 
			(canvas3d.texUnitState[0].texAttrs != null)) {
		        canvas3d.resetTextureAttributes(canvas3d.ctx);
		        canvas3d.canvasDirty |= Canvas3D.TEXTUREBIN_DIRTY|Canvas3D.TEXTUREATTRIBUTES_DIRTY;		    
		        canvas3d.texUnitState[0].texAttrs = null;
		    }
	        }
	    }

	    if (app.coloringAttributes != null) {
		app.coloringAttributes.updateNative(canvas3d.ctx,
							dRed, dBlue,
							dGreen,
							alpha, enableLighting);
		canvas3d.canvasDirty |= Canvas3D.COLORINGATTRS_DIRTY;
		canvas3d.coloringAttributes = app.coloringAttributes;
	    }
	    else {
		if (canvas3d.coloringAttributes != null) {
		    canvas3d.resetColoringAttributes(canvas3d.ctx,
						     red, green, blue, alpha,
						     enableLighting);
		    canvas3d.canvasDirty |= Canvas3D.COLORINGATTRS_DIRTY;
		    canvas3d.coloringAttributes = null;
		}
	    }


	    if (app.transparencyAttributes != null) {
		app.transparencyAttributes.updateNative(canvas3d.ctx,
							alpha, geometryType, 
							polygonMode, 
							lineAA, pointAA);
		canvas3d.canvasDirty |= Canvas3D.TRANSPARENCYATTRS_DIRTY;
		canvas3d.transparency = app.transparencyAttributes;

		useAlpha = useAlpha || ((app.transparencyAttributes.transparencyMode !=
					TransparencyAttributes.NONE)
					&&
					(VirtualUniverse.mc.isD3D()
					 ||
					 (!VirtualUniverse.mc.isD3D() &&
					  (app.transparencyAttributes. transparencyMode !=
					   TransparencyAttributes.SCREEN_DOOR))));
	    } else {
		canvas3d.resetTransparency(canvas3d.ctx, geometryType,
					       polygonMode, lineAA, pointAA);
		canvas3d.canvasDirty |= Canvas3D.TRANSPARENCYATTRS_DIRTY;
		canvas3d.transparency = null;
	    }


	    if (app.renderingAttributes != null) {
		ignoreVertexColors =app.renderingAttributes.ignoreVertexColors;
		app.renderingAttributes.updateNative(canvas3d.ctx,
				canvas3d.depthBufferWriteEnableOverride,
				canvas3d.depthBufferEnableOverride);
		canvas3d.canvasDirty |= Canvas3D.ATTRIBUTEBIN_DIRTY|Canvas3D.TEXTUREATTRIBUTES_DIRTY;
		canvas3d.renderingAttrs = app.renderingAttributes;

		useAlpha = useAlpha || 
				(app.renderingAttributes.alphaTestFunction 
					!= RenderingAttributes.ALWAYS);
	    } else {
		// If the canvas does not alreadt have a null renderingAttrs
		// load the default
		ignoreVertexColors = false;
		if (canvas3d.renderingAttrs != null) {
		    canvas3d.resetRenderingAttributes(canvas3d.ctx, 
				canvas3d.depthBufferWriteEnableOverride, 
				canvas3d.depthBufferEnableOverride);
		    canvas3d.canvasDirty |= Canvas3D.ATTRIBUTEBIN_DIRTY|Canvas3D.TEXTUREATTRIBUTES_DIRTY;
		    canvas3d.renderingAttrs = null;
		}
	    }	    	    


	    if (app.polygonAttributes != null) {
		app.polygonAttributes.updateNative(canvas3d.ctx);
		canvas3d.canvasDirty |= Canvas3D.POLYGONATTRS_DIRTY;
		canvas3d.polygonAttributes = app.polygonAttributes;
	    } else {
		// If the canvas does not alreadt have a null polygonAttr
		// load the default
		if (canvas3d.polygonAttributes != null) {
		    canvas3d.resetPolygonAttributes(canvas3d.ctx);
		    canvas3d.canvasDirty |= Canvas3D.POLYGONATTRS_DIRTY;
		    canvas3d.polygonAttributes = null;
		}
	    }



	    if (app.lineAttributes != null) {
		app.lineAttributes.updateNative(canvas3d.ctx);
		canvas3d.canvasDirty |= Canvas3D.LINEATTRS_DIRTY;
		canvas3d.lineAttributes = app.lineAttributes;
	    } else {
		// If the canvas does not already have a null lineAttr
		// load the default
		if (canvas3d.lineAttributes != null) {
		    canvas3d.resetLineAttributes(canvas3d.ctx);
		    canvas3d.canvasDirty |= Canvas3D.LINEATTRS_DIRTY;
		    canvas3d.lineAttributes = null;
		}
	    }



	    if (app.pointAttributes != null) {
		app.pointAttributes.updateNative(canvas3d.ctx);
		canvas3d.canvasDirty |= Canvas3D.POINTATTRS_DIRTY;
		canvas3d.pointAttributes = app.pointAttributes;
	    } else {
		// If the canvas does not already have a null pointAttr
		// load the default
		if (canvas3d.pointAttributes != null) {
		    canvas3d.resetPointAttributes(canvas3d.ctx);
		    canvas3d.canvasDirty |= Canvas3D.POINTATTRS_DIRTY;
		    canvas3d.pointAttributes = null;
		}
	    }
	    
	    canvas3d.appearance = app;

	} else {
	    if (canvas3d.appearance != null) {
		resetAppearance();
	        canvas3d.appearance = null;
	    }
	}

	
	return (useAlpha );
    }

    void initializeState() {

	canvas3d.setSceneAmbient(canvas3d.ctx, 0.0f, 0.0f, 0.0f);
	canvas3d.disableFog(canvas3d.ctx);
	canvas3d.resetRenderingAttributes(canvas3d.ctx,false, false);

        // reset the previously enabled texture units

        int prevNumActiveTexUnit = canvas3d.getNumActiveTexUnit();

	if (prevNumActiveTexUnit > 0) {
            for (int i = 0; i < prevNumActiveTexUnit; i++) {
                if (canvas3d.texUnitState[i].texture != null) {
                    canvas3d.resetTexture(canvas3d.ctx, i);
                    canvas3d.texUnitState[i].texture = null;
                }
                if (canvas3d.texUnitState[i].texAttrs != null) {
                    canvas3d.resetTextureAttributes(canvas3d.ctx);
                    canvas3d.texUnitState[i].texAttrs = null;
                }
                if (canvas3d.texUnitState[i].texGen != null) {
                    canvas3d.resetTexCoordGeneration(canvas3d.ctx);
                    canvas3d.texUnitState[i].texGen = null;
                }
		canvas3d.texUnitState[i].mirror = null;
	    }
            canvas3d.setNumActiveTexUnit(0);
        }

	canvas3d.resetPolygonAttributes(canvas3d.ctx);
	canvas3d.resetLineAttributes(canvas3d.ctx);
	canvas3d.resetPointAttributes(canvas3d.ctx);
	canvas3d.resetTransparency(canvas3d.ctx, RenderMolecule.SURFACE,
				   PolygonAttributes.POLYGON_FILL,
				   false, false);
	canvas3d.resetColoringAttributes(canvas3d.ctx,1.0f, 1.0f, 1.0f, 1.0f, false);
	canvas3d.updateMaterial(canvas3d.ctx, 1.0f, 1.0f, 1.0f, 1.0f);
    }


    void resetAppearance() {

	if (canvas3d.material != null) {
	    canvas3d.updateMaterial(canvas3d.ctx, 
					red, green, blue, alpha);
	    canvas3d.material = null;
	    canvas3d.canvasDirty |= Canvas3D.MATERIAL_DIRTY;
	}

        // reset the previously enabled texture units

        int prevNumActiveTexUnit = canvas3d.getNumActiveTexUnit();

	if (prevNumActiveTexUnit > 0) {
            for (int i = 0; i < prevNumActiveTexUnit; i++) {
                if (canvas3d.texUnitState[i].texture != null) {
                    canvas3d.resetTexture(canvas3d.ctx, i);
                    canvas3d.texUnitState[i].texture = null;
                }
                if (canvas3d.texUnitState[i].texAttrs != null) {
                    canvas3d.resetTextureAttributes(canvas3d.ctx);
                    canvas3d.texUnitState[i].texAttrs = null;
                }
                if (canvas3d.texUnitState[i].texGen != null) {
                    canvas3d.resetTexCoordGeneration(canvas3d.ctx);
                    canvas3d.texUnitState[i].texGen = null;
                }
		canvas3d.texUnitState[i].mirror = null;
	    }
            canvas3d.canvasDirty |= Canvas3D.TEXTUREBIN_DIRTY|Canvas3D.TEXTUREATTRIBUTES_DIRTY;
            canvas3d.setNumActiveTexUnit(0);
        }

	if (canvas3d.coloringAttributes != null) {
	    canvas3d.resetColoringAttributes(canvas3d.ctx,
			red, green, blue, alpha, enableLighting);
	    canvas3d.coloringAttributes = null;
	    canvas3d.canvasDirty |= Canvas3D.COLORINGATTRS_DIRTY;
	}

	if (canvas3d.transparency != null) {
	    canvas3d.resetTransparency(canvas3d.ctx, RenderMolecule.SURFACE,
			PolygonAttributes.POLYGON_FILL, lineAA, pointAA);
	    canvas3d.transparency = null;
            canvas3d.canvasDirty |= Canvas3D.TRANSPARENCYATTRS_DIRTY;
	}

	if (canvas3d.renderingAttrs != null) {
	    ignoreVertexColors = false;
	    canvas3d.resetRenderingAttributes(canvas3d.ctx,
				canvas3d.depthBufferWriteEnableOverride,
				canvas3d.depthBufferEnableOverride);
	    canvas3d.renderingAttrs = null;
	    canvas3d.canvasDirty |= Canvas3D.ATTRIBUTEBIN_DIRTY|Canvas3D.TEXTUREATTRIBUTES_DIRTY;
	}

	if (canvas3d.polygonAttributes != null) {
	    canvas3d.resetPolygonAttributes(canvas3d.ctx);
	    canvas3d.polygonAttributes = null;
	    canvas3d.canvasDirty |= Canvas3D.POLYGONATTRS_DIRTY;
	}

	if (canvas3d.lineAttributes != null) {
	    canvas3d.resetLineAttributes(canvas3d.ctx);
	    canvas3d.lineAttributes = null;
	    canvas3d.canvasDirty |= Canvas3D.LINEATTRS_DIRTY;
	}

	if (canvas3d.pointAttributes != null) {
	    canvas3d.resetPointAttributes(canvas3d.ctx);
	    canvas3d.pointAttributes = null;
	    canvas3d.canvasDirty |= Canvas3D.POINTATTRS_DIRTY;
	}
    }

    void sendRenderMessage(boolean renderRun, int command, 
				Object arg1, Object arg2) {

        // send a message to the request renderer

        J3dMessage renderMessage = VirtualUniverse.mc.getMessage();
        renderMessage.threads = J3dThread.RENDER_THREAD;
        renderMessage.type = J3dMessage.RENDER_IMMEDIATE;
        renderMessage.universe = null;
        renderMessage.view = null;
        renderMessage.args[0] = canvas3d;
        renderMessage.args[1] = getImmCommand(command);
        renderMessage.args[2] = arg1;
        renderMessage.args[3] = arg2;

	while (!canvas3d.view.inRenderThreadData) {
	    // wait until the renderer thread data in added in
	    // MC:RenderThreadData array ready to receive message
	    MasterControl.threadYield();
	}

        canvas3d.screen.renderer.rendererStructure.addMessage(renderMessage);
	
        if (renderRun) {
            // notify mc that there is work to do
            VirtualUniverse.mc.sendRunMessage(canvas3d.view, J3dThread.RENDER_THREAD);
        } else {
	    // notify mc that there is work for the request renderer
	    VirtualUniverse.mc.setWorkForRequestRenderer();
	}
    }

    void sendSoundMessage(int command, Object arg1, Object arg2) {
        if ((canvas3d.view == null) || 
	    (canvas3d.view.universe == null) ) {
            return;
        }
        // send a message to the request sound scheduling
        J3dMessage soundMessage = VirtualUniverse.mc.getMessage();
        soundMessage.threads = J3dThread.SOUND_SCHEDULER;
        soundMessage.type = J3dMessage.RENDER_IMMEDIATE;
        soundMessage.universe = canvas3d.view.universe;
        soundMessage.view = canvas3d.view;
        soundMessage.args[0] = getImmCommand(command);
        soundMessage.args[1] = arg1;
        soundMessage.args[2] = arg2;
        // notify mc that there is work to do
        VirtualUniverse.mc.processMessage(soundMessage);
    }

    static Integer getImmCommand(int command) {
	if (commands[command] == null) {
	    commands[command] = new Integer(command);
	}
	return commands[command];
    }

    synchronized void runMonitor(int action) {
        if (action == J3dThread.WAIT) {
            while (!gcReady) {
                waiting++;
                try {
                    wait();
                } catch (InterruptedException e){}
                waiting--;
            }
            gcReady = false;
        } else {
            gcReady = true;
            if (waiting > 0) {
                notify();
	    }
        }
    }

}

