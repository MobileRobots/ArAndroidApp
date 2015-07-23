package com.adept.arandroid.render;

import javax.microedition.khronos.opengles.GL10;

/**
 * Class used to render the robot to the screen.
 * @author markl
 *
 */
public class ArjRobotRender implements IArjRenderable {

	private ArjShapeEllipse circle;
	
	private float x, y;
	
	/** Constructor */
	public ArjRobotRender() {
		circle = new ArjShapeEllipse(.075f, .075f, 120);
		float cc[] = {1.f, 0.f, 0.f, 1.f};
		circle.color = cc;
		x = y = 0.f;
	}

	/** Called every frame */
	public void onDrawFrame(GL10 gl) {
		gl.glPushMatrix();
		gl.glTranslatef(x, y, 0.0f);
		circle.draw(gl);
		gl.glPopMatrix();
	}
	
	/** Called when XY are updated */
	public void onUpdateXY(float _x, float _y) {
		x = _x;
		y = _y;
	}

}
