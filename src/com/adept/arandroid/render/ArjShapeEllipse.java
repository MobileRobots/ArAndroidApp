package com.adept.arandroid.render;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import javax.microedition.khronos.opengles.GL10;

/**
 * Represents an ellipse that can be rendered to the screen.
 * @author markl
 *
 */
public class ArjShapeEllipse {

	private float height;
	private float width;
	private int segments;

	// Our vertex buffer.
	private FloatBuffer vertexBuffer;
	
	// The color, default to black with 100% alpha.
	//public float color[] = {1.0f, 1.0f, 1.0f, 1.0f};
	public float color[] = {0.0f, 0.0f, 0.0f, 1.0f};

	/**
	 * Constructor
	 */
	public ArjShapeEllipse(float w, float h, int s) {
		width = w;
		height = h;
		segments = s;
		fillVertices();
		System.out.println("***********filled vertices");
	}
	
	/**
	 * Fill vertices
	 */
	private void fillVertices() {
		float vertices[] = new float[segments * 2];
		int count = 0;
		for (float i = 0; i < 360.0f; i += (360.0f/segments)) {
			vertices[count++] = (float) (Math.cos(ArjUtil.d2r(i)) * width);
			vertices[count++] = (float) (Math.sin(ArjUtil.d2r(i)) * height);
		}
		ByteBuffer vbb = ByteBuffer.allocateDirect(vertices.length * 4);
		vbb.order(ByteOrder.nativeOrder());
		vertexBuffer = vbb.asFloatBuffer();
		vertexBuffer.put(vertices);
		vertexBuffer.position(0);
	}

	/**
	 * This function draws our ellipse on screen.
	 * @param gl
	 */
	public void draw(GL10 gl) {
		
		// Set the color.
		gl.glColor4f(color[0], color[1], color[2], color[3]);

		// Counter-clockwise winding.
		gl.glFrontFace(GL10.GL_CCW);
		
		// Enable face culling.
		gl.glEnable(GL10.GL_CULL_FACE);
		
		// What faces to remove with the face culling.
		gl.glCullFace(GL10.GL_BACK);

		// Enabled the vertices buffer for writing and to be used during rendering.
		gl.glEnableClientState(GL10.GL_VERTEX_ARRAY);
		
		// Specifies the location and data format of an array of vertex coordinates to use when rendering.
		gl.glVertexPointer(2, GL10.GL_FLOAT, 0, vertexBuffer);

		// Draw.
		gl.glDrawArrays(GL10.GL_TRIANGLE_FAN, 0, segments);
		
		// Disable the vertices buffer.
		gl.glDisableClientState(GL10.GL_VERTEX_ARRAY);
		
		// Disable face culling.
		gl.glDisable(GL10.GL_CULL_FACE);
	}


}
