package com.adept.arandroid;

import com.adept.arandroid.R;
import android.app.ListActivity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.Toast;

public class TeleopActivity extends ListActivity {

	private ArjRobot robot;
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
    	super.onCreate(savedInstanceState);
    	
    	robot = ArjRobot.getInstance();
    	
    	setContentView(R.layout.teleop);
    	final LinearLayout ll = (LinearLayout) findViewById(R.layout.teleop);
    	
    	ll.setClickable(true);
    	
    	
    	final Button button0 = (Button) findViewById(R.id.button0);
    	final Button button1 = (Button) findViewById(R.id.button1);
    	final Button button2 = (Button) findViewById(R.id.button2);
    	final Button button3 = (Button) findViewById(R.id.button3);
    	final Button button4 = (Button) findViewById(R.id.button4);
    	
    	button0.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
            	robot.stop();
  			    Intent intent = new Intent();
  			    setResult(RESULT_OK, intent);
  			    finish();
            }
    	});
    	
    	button1.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
            	robot.forward();
  			    Intent intent = new Intent();
  			    setResult(RESULT_OK, intent);
  			    finish();
            }
    	});
    	
    	button2.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
            	robot.spin_clockwise();
  			    Intent intent = new Intent();
  			    setResult(RESULT_OK, intent);
  			    finish();
            }
    	});
    	
    	button3.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
            	robot.backward();
  			    Intent intent = new Intent();
  			    setResult(RESULT_OK, intent);
  			    finish();
            }
    	});
    	
    	button4.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
            	robot.spin_counterclockwise();
  			    Intent intent = new Intent();
  			    setResult(RESULT_OK, intent);
  			    finish();
            }
    	});
    	
    }
}
