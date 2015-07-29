package com.adept.arandroid.render;

import javax.microedition.khronos.opengles.GL10;

/**
 * Interface representing an object that can be rendered to the screen.
 * @author markl
 *
 */
public interface IArjRenderable {

	/** Called every frame. */
	public void onDrawFrame(GL10 gl);
}
