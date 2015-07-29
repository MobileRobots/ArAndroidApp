package com.adept.arandroid;

import com.adept.arandroid.R;
import android.app.TabActivity;
import android.content.Intent;
import android.content.res.Resources;
import android.os.Bundle;
import android.widget.TabHost;

public class MainActivity extends TabActivity {
	
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
    	super.onCreate(savedInstanceState);
	    setContentView(R.layout.main);

	    Resources res = getResources(); 	// Resource object to get Drawables
	    TabHost tabHost = getTabHost();  	// The activity TabHost
	    TabHost.TabSpec spec;  				// Resusable TabSpec for each tab
	    Intent intent;  					// Reusable Intent for each tab

	    // Singleton for UI help
	    UIUtil uiUtil = UIUtil.getInstance();
	    uiUtil.setTabHost(tabHost);
	    
	    // Create an intent and tabspec for ConnectionActivity
	    intent = new Intent().setClass(this, ConnectionActivity.class);
	    spec = tabHost.newTabSpec("connection").setIndicator("Connection", res.getDrawable(R.drawable.ic_tab_artists)).setContent(intent);
	    tabHost.addTab(spec);
	    
	    // Create an intent and tabspec for ActionsActivity
	    intent = new Intent().setClass(this, ActionsActivity.class);
	    spec = tabHost.newTabSpec("actions").setIndicator("Actions", res.getDrawable(R.drawable.ic_tab_artists)).setContent(intent);
	    tabHost.addTab(spec);

	    // Create an intent and tabspec for ActionsActivity
	    intent = new Intent().setClass(this, MapActivity.class);
	    spec = tabHost.newTabSpec("map").setIndicator("Map", res.getDrawable(R.drawable.ic_tab_artists)).setContent(intent);
	    tabHost.addTab(spec);

	    // Set the current tab
	    tabHost.setCurrentTab(0);
	    
      // deactivate the others until connected
	    uiUtil.getTabHost().getTabWidget().getChildTabViewAt(1).setEnabled(false);
		  uiUtil.getTabHost().getTabWidget().getChildTabViewAt(2).setEnabled(false);
    }
    
    /*
    private void initialize() {
    	ArjRobot robot = new ArjRobot("172.21.1.110", 7272);
    	Thread t = new Thread(robot);
    	t.start();
    	
    	try {
			Thread.sleep(2000);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		
		
		int stt = 0;
		for (int i=0;i<4;i++) {
			robot.gotoGoal("test 1");
			stt = robot.waitGoalDone(120000);
			System.out.println("Moving complete with code: " + stt);
			
			robot.gotoGoal("test 2");
			stt = robot.waitGoalDone(120000);
			System.out.println("Moving complete with code: " + stt);
			
			robot.gotoGoal("test 3");
			stt = robot.waitGoalDone(120000);
			System.out.println("Moving complete with code: " + stt);
		}
		
		
		robot.gotoGoal("test 2");
		try {
			Thread.sleep(2000);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		double[] status = new double[8];
		status = robot.getRobotStatus();
		for (int i = 0; i < 8; i++) {
			System.out.println(status[i]);
		}
		
		robot.shutdown();
    }*/
}
