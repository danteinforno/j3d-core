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
import java.util.ArrayList;


//class ShaderBin implements ObjectUpdate {
class ShaderBin {

    /**
     * The AttributeBin that this ShaderBin resides
     */
    AttributeBin attributeBin = null;

    /**
     * The references to the next and previous ShaderBins in the
     * list.
     */
    ShaderBin next = null;
    ShaderBin prev = null;

    /**
     * The list of TextureBins in this ShaderBin
     */
    TextureBin textureBinList = null;


    /**
     * The list of TextureBins to be added for the next frame
     */
    ArrayList addTextureBins = new ArrayList();

    /**
     *  List of TextureBins to be added next frame
     */
    ArrayList addTBs = new ArrayList();



}
