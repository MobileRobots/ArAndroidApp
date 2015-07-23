package com.adept.arandroid;

import com.adept.arandroid.render.*;

/**
 * Requests data from the Robot and passes it to the RenderRobot on a seperate thread.
 * 
 * @author markl
 *
 */
public class ArjRobotAdapterRunnable implements Runnable {

	private final ArjRobot robot;
	private final ArjRobotRender render;
	private Boolean shouldRun = true;
	
	/** Constructor */
	public ArjRobotAdapterRunnable(ArjRobot rbt, ArjRobotRender rnd) {
		robot = rbt;
		render = rnd;
	}
	
	/** Implement run() to do work on a different thread. */
	public void run() {
		double xy[] = new double[8];
		while (shouldRun) {
			xy = robot.getRobotStatus();
			render.onUpdateXY((float)xy[1] * ArjUtil.SCALE, (float)xy[2] * ArjUtil.SCALE);
		}
	}
	
	/** Implement stop() to stop the thread safely */
	public void stop() {
		shouldRun = false;
	}
}
