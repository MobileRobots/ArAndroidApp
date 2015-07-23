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

public class GoalsActivity extends ListActivity {
	
	private ArjRobot robot;
	
	private String[] myGoals;
	
	/** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
    	  super.onCreate(savedInstanceState);
    	  
    	  // Get the robot
    	  robot = ArjRobot.getInstance();
    	  
    	  // Get the goals
    	  getGoals();
    	  
    	  // Create the list of goals
    	  setListAdapter(new ArrayAdapter<String>(this, R.layout.actions, myGoals));
    	  ListView lv = getListView();
    	  lv.setTextFilterEnabled(true);
    	  
    	  // Handle clicks
    	  lv.setClickable(true);
    	  lv.setOnItemClickListener(new AdapterView.OnItemClickListener() {
    		  public void onItemClick(AdapterView<?> arg0, View arg1, int arg2, long arg3) {
    			  // Start moving the robot
    			  String goal = myGoals[arg2];
    			  robot.gotoGoal(goal);
    			  
    			  // Show notice
    			  Toast.makeText(getApplicationContext(), "Going to goal " + goal + "...", 1).show();

    			  // Push back to last Activity
    			  Intent intent = new Intent();
                  setResult(RESULT_OK, intent);
                  finish();
    		  }
    	  });
    }
    
    /** Get the goals */
    private void getGoals() {
    	String[] goals = robot.getGoals();
    	myGoals = goals;
    }
}
