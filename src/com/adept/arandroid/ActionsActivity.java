package com.adept.arandroid;

import com.adept.arandroid.R;

import android.app.ListActivity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.Toast;

public class ActionsActivity extends ListActivity {
	
	private ArjRobot robot;
	
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
    	super.onCreate(savedInstanceState);
	    
    	// Get the robot
    	robot = ArjRobot.getInstance();
    	
    	// Create the list of actions
    	setListAdapter(new ArrayAdapter<String>(this, R.layout.actions, ACTIONS));
    	ListView lv = getListView();
    	lv.setTextFilterEnabled(true); 
    	
    	//View v = lv.getChildAt(4);
    	//v.setEnabled(false);
    	
    	// Create an Intent
    	final Intent goalsIntent = new Intent().setClass(this, GoalsActivity.class);
    	//final Intent connectIntent = new Intent().setClass(this, ConnectActivity.class);
    	
    	// Handle clicks
    	lv.setClickable(true);
    	lv.performItemClick(getListView(), 4, 0);
    	lv.setOnItemClickListener(new AdapterView.OnItemClickListener() {
    		public void onItemClick(AdapterView<?> arg0, View arg1, int arg2, long arg3) {
    			if (!robot.connected) {
    				Toast.makeText(getApplicationContext(), "Not connected.", 1);
    			}
    			switch (arg2) {
    			case 0:
    				if (robot.connected) {
    					startActivity(goalsIntent);
    				}
    				break;
    			case 1:
    				if (robot.connected) {
    					robot.stop();
    					Toast.makeText(getApplicationContext(), "Stopping robot...", 1).show();
    				}
    				break;
    			case 2:
    				if (robot.connected) {
    					robot.dock();
    					Toast.makeText(getApplicationContext(), "Sending to Dock...", 1).show();
    				}
    				break;
    			}
    		}
    	});
    }
    
    static final String[] ACTIONS = new String[] {
    	"Goto Goal", "Stop", "Dock"
    };


}
