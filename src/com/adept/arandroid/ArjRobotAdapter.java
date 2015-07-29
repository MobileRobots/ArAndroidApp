package com.adept.arandroid;

import com.adept.arandroid.render.*;

/**
 * This class forms an Adapter to handle requests between the Robot and the RenderRobot.
 * 
 * The idea is that neither the Robot nor the RenderRobot know about eachother, and this
 * class will act as an intermidiary to send/receive data between them.  It works by 
 * creating an instance of ArjRobotAdapterRunnable, which will run on a seperate thread
 * and request updates from the Robot and pass them to the RenderRobot.
 * 
 * @author markl
 *
 */
public class ArjRobotAdapter {
	
	ArjRobotAdapterRunnable run;
	
	/** Create a new instance of the Adapter and run it */
	public ArjRobotAdapter(ArjRobot rbt, ArjRobotRender rnd) {
		run = new ArjRobotAdapterRunnable(rbt, rnd);
		Thread thread = new Thread(run);
		thread.start();
	}
	
	/** Stop the adapter */
	public void stop() {
		run.stop();
	}
	
}
