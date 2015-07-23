package com.adept.arandroid.render;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import javax.microedition.khronos.opengles.GL10;

/**
 * Represents a line that can be rendered to the screen.
 * @author markl
 *
 */
public class ArjShapeLine {
	
	// The color, default to black with 100% alpha.
	//public float color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	public float color[] = { 0.0f, 0.0f, 0.0f, 1.0f };

	// Our vertex buffer.
	private FloatBuffer vertexBuffer;

	/**
	 * Constructor
	 */
	public ArjShapeLine(float x1, float y1, float x2, float y2) {
		fillVertices(x1, y1, x2, y2);
	}
	
	/**
	 * Fill the vertices
	 */
	private void fillVertices(float x1, float y1, float x2, float y2) {
		float vertices[] = {
				x1, y1, 0.0f,
			    x2, y2, 0.0f,
		};
		
		// A float is 4 bytes, therefore we multiply the number of vertices with 4.
		ByteBuffer vbb = ByteBuffer.allocateDirect(vertices.length * 4);
		vbb.order(ByteOrder.nativeOrder());
		vertexBuffer = vbb.asFloatBuffer();
		vertexBuffer.put(vertices);
		vertexBuffer.position(0);
	}

	/**
	 * This function draws our square on screen.
	 * @param gl
	 */
	public void draw(GL10 gl) {
		
		// Set the color.
		gl.glColor4f(color[0], color[1], color[2], color[3]);

		// Counter-clockwise winding.
		//gl.glFrontFace(GL10.GL_CCW);
		
		// Enable face culling.
		//gl.glEnable(GL10.GL_CULL_FACE);
		
		// What faces to remove with the face culling.
		//gl.glCullFace(GL10.GL_BACK);

		// Enabled the vertices buffer for writing and to be used during rendering.
		gl.glEnableClientState(GL10.GL_VERTEX_ARRAY);
		
		// Specifies the location and data format of an array of vertex
		// coordinates to use when rendering.
		gl.glVertexPointer(3, GL10.GL_FLOAT, 0, vertexBuffer);
		gl.glDrawArrays(GL10.GL_LINES, 0, 2);
		
		// Disable the vertices buffer.
		gl.glDisableClientState(GL10.GL_VERTEX_ARRAY);
		
		// Disable face culling.
		//gl.glDisable(GL10.GL_CULL_FACE);
	}


}
