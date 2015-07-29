package com.adept.arandroid.render;

import java.util.ArrayList;
import java.util.Iterator;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.opengl.GLU;
import android.opengl.GLSurfaceView.Renderer;

/**
 * Class used to render objects to the screen.
 * @author markl
 *
 */
public class ArjRenderer implements Renderer {
	
	/** List of objects to render */
	private ArrayList<IArjRenderable> renderable = new ArrayList<IArjRenderable>();
	
	/** Add a renderable object */
	public void addRenderable(IArjRenderable obj) {
		renderable.add(obj);
	}

	/** Constructor */
	public ArjRenderer() { }
	
	/** Called every frame */
	public void onDrawFrame(GL10 gl) {
		
		// Clears the screen and depth buffer.
		gl.glClear(GL10.GL_COLOR_BUFFER_BIT | GL10.GL_DEPTH_BUFFER_BIT);
		
		// Replace the current matrix with the identity matrix.
		gl.glLoadIdentity();
		
		// Translates 10 units into the screen so we can see the objects.
		gl.glTranslatef(0, 0, -10); 
		
		// Render each object.
		Iterator<IArjRenderable> itr = renderable.iterator();
		IArjRenderable rend;
		while (itr.hasNext()) {
			rend = (IArjRenderable) itr.next();
			rend.onDrawFrame(gl);
		}
	}

	/** Called when the surface is changed */
	public void onSurfaceChanged(GL10 gl, int width, int height) {
		
		// Sets the current view port to the new size.
		gl.glViewport(0, 0, width, height);
		
		// Select the projection matrix
		gl.glMatrixMode(GL10.GL_PROJECTION);
		
		// Reset the projection matrix
		gl.glLoadIdentity();
		
		// Calculate the aspect ratio of the window
		GLU.gluPerspective(gl, 45.0f, (float) width / (float) height, 0.1f, 100.0f);
		
		// Select the modelview matrix
		gl.glMatrixMode(GL10.GL_MODELVIEW);
		
		// Reset the modelview matrix
		gl.glLoadIdentity();
	}

	/** Called when the surface is created */
	public void onSurfaceCreated(GL10 gl, EGLConfig config) {
		
		// Set the background color to white ( rgba ).
		//gl.glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
		gl.glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		
		// Enable Smooth Shading, default not really needed.
		gl.glShadeModel(GL10.GL_SMOOTH);
		
		// Depth buffer setup.
		gl.glClearDepthf(1.0f);
		
		// Enables depth testing.
		gl.glEnable(GL10.GL_DEPTH_TEST);
		
		// The type of depth testing to do.
		gl.glDepthFunc(GL10.GL_LEQUAL);
		
		// Fast perspective calculations.
		gl.glHint(GL10.GL_PERSPECTIVE_CORRECTION_HINT, GL10.GL_FASTEST);
	}

}
