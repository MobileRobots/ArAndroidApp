package com.adept.arandroid.render;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.nio.ShortBuffer;
import javax.microedition.khronos.opengles.GL10;

/**
 * Represents a unit square that can be rendered to the screen.
 * @author markl
 *
 */
public class ArjShapeRectangle {

	private float width;
	private float height;
	
	// The color, default to white with 100% alpha.
	//public float color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	public float color[] = { 0.0f, 0.0f, 0.0f, 1.0f };

	// The order we like to connect them.
	private short[] indices = { 0, 1, 2, 0, 2, 3 };

	// Our vertex buffer.
	private FloatBuffer vertexBuffer;

	// Our index buffer.
	private ShortBuffer indexBuffer;

	/**
	 * Constructor
	 */
	public ArjShapeRectangle(float w, float h) {
		width = w;
		height = h;
		fillVertices();
	}
	
	/**
	 * Fill the vertices
	 */
	private void fillVertices() {
		float w = width;
		float h = height;
		float vertices[] = {
				-w,  h, 0.0f,  // 0, Top Left
			    -w, -h, 0.0f,  // 1, Bottom Left
			     w, -h, 0.0f,  // 2, Bottom Right
			     w,  h, 0.0f,  // 3, Top Right	
		};
		
		// a float is 4 bytes, therefore we multiply the number of vertices with 4.
		ByteBuffer vbb = ByteBuffer.allocateDirect(vertices.length * 4);
		vbb.order(ByteOrder.nativeOrder());
		vertexBuffer = vbb.asFloatBuffer();
		vertexBuffer.put(vertices);
		vertexBuffer.position(0);

		// short is 2 bytes, therefore we multiply the number of vertices with 2.
		ByteBuffer ibb = ByteBuffer.allocateDirect(indices.length * 2);
		ibb.order(ByteOrder.nativeOrder());
		indexBuffer = ibb.asShortBuffer();
		indexBuffer.put(indices);
		indexBuffer.position(0);
	}

	/**
	 * This function draws our square on screen.
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
		
		// Specifies the location and data format of an array of vertex
		// coordinates to use when rendering.
		gl.glVertexPointer(3, GL10.GL_FLOAT, 0, vertexBuffer);
		gl.glDrawElements(GL10.GL_TRIANGLES, indices.length, GL10.GL_UNSIGNED_SHORT, indexBuffer);

		// Disable the vertices buffer.
		gl.glDisableClientState(GL10.GL_VERTEX_ARRAY);
		
		// Disable face culling.
		gl.glDisable(GL10.GL_CULL_FACE);
	}


}
