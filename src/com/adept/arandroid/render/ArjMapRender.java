package com.adept.arandroid.render;

import javax.microedition.khronos.opengles.GL10;
import com.adept.arandroid.ArjRobot;

/**
 * Class used to render the map to the screen.
 * @author markl
 *
 */
public class ArjMapRender implements IArjRenderable {

	private ArjShapeEllipse circle;
	private ArjRobot robot;
	private double[] map;
	
	/** Constructor */
	public ArjMapRender() {
		robot = ArjRobot.getInstance();
		map = robot.getMap();
		
		circle = new ArjShapeEllipse(.01f, .01f, 12);
		float cc[] = {1.f, 1.f, 1.f, 1.f};
		circle.color = cc;
	}

	/** Called every frame */
	public void onDrawFrame(GL10 gl) {
		
		int count = 0;
		for (int i = 0; i < (map.length/2); i++) {
			gl.glPushMatrix();
			gl.glTranslatef((float)map[count++] * ArjUtil.SCALE, (float)map[count++] * ArjUtil.SCALE, 0.0f);
			circle.draw(gl);
			gl.glPopMatrix();
		}
	}
}
