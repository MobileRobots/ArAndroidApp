package com.adept.arandroid;

/**
 * Represents a mobile robot singleton.
 * @author markl
 *
 */
public class ArjRobot implements Runnable {

	private static ArjRobot instance = null;
	
	/** Only exists to defeat instantiation */
	protected ArjRobot() { }
	
	/** Return the only allowed instance of ArjRobot */
	public static ArjRobot getInstance() {
		if(instance == null) {
			instance = new ArjRobot();
		}
		return instance;
	}

	/** Load native libraries */
	static {
		System.loadLibrary("ArNetworking_android");
	}
	
	public String myHost;
	public int myPort;
	public Boolean connected = false;
	public double data[] = new double[8];
	
	/** Implement run to start this on a background thread */
	public void run() {
		connected = true;
		if (initialize(myHost, myPort) < 0)
			connected = false;
		
		// Start getting data at a regular rate
		/*
		while (true) {
			data = getRobotStatus();
			try {
				Thread.sleep(100);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}*/
	}
	
	/*
	 * Native methods from arnetworking-android.so
	 */
	
	/** Initialize the robot */
	public native int initialize(String host, int port);
	
	/** Send the robot to a named goal */
	public native int gotoGoal(String goal);
	
	/** Stop the robot */
	public native int stop();
	
	/** Send the robot to dock */
	public native int dock();
	
	/** Wait for current goal to complete, or timeout in specified ms */
	public native int waitGoalDone(int timeout);
	
	/** Teleop mode, move robot forward */
	public native int forward();
	
	/** Teleop mode, spin robot clockwise */
	public native int spin_clockwise();
	
	/** Teleop mode, running on reverse mode */
	public native int backward();
	
	/**  */
	public native int spin_counterclockwise();

	/** Get robot status array 
	 * 	status[0] = voltage
	 * 	status[1] = X 		(mm)
	 * 	status[2] = Y 		(mm)
	 * 	status[3] = Theta 	(degrees)
	 * 	status[4] = Vel 	(mm/s)
	 * 	status[5] = Rot Vel (deg/s)
	 * 	status[6] = Lat Vel (mm/s)
	 * 	status[7] = Temp	(F) */
	public native double[] getRobotStatus();
	
	/** Get the list of goals */
	public native String[] getGoals();
	
	/** Get the map */
	public native double[] getMap();
	
	/** Shutdown */
	public native void shutdown();

}
