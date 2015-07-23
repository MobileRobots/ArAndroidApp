package com.adept.arandroid;

import com.adept.arandroid.R;
import android.app.AlertDialog;
import android.app.ListActivity;
import android.content.DialogInterface;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.Toast;

public class ConnectionActivity extends ListActivity {
	
	private ArjRobot robot;
	private UIUtil uiUtil;

    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
    	super.onCreate(savedInstanceState);
	    
    	// Get the robot
    	robot = ArjRobot.getInstance();
    	
    	// UI helper
    	uiUtil = UIUtil.getInstance();
    	
    	// Create the list of actions
    	setListAdapter(new ArrayAdapter<String>(this, R.layout.actions, ACTIONS));
    	ListView lv = getListView();
    	lv.setTextFilterEnabled(true); 
    	
    	// Handle clicks
    	lv.setClickable(true);
    	lv.performItemClick(getListView(), 4, 0);
    	lv.setOnItemClickListener(new AdapterView.OnItemClickListener() {
    		public void onItemClick(AdapterView<?> arg0, View arg1, int arg2, long arg3) {
    			switch (arg2) {
    			case 0:
    				if (!robot.connected)
    					showConnectDialog(arg0, arg1);
    				break;
    			case 1:
    				if (robot.connected) {
	    				Toast.makeText(getApplicationContext(), "Disconnecting...", 1).show();
	    				robot.shutdown();
	    				arg1.setEnabled(false);
	    				View v0 = arg0.getChildAt(0);
	    				v0.setEnabled(true);
	    				robot.connected = false;
	    				uiUtil.getTabHost().getTabWidget().getChildTabViewAt(1).setEnabled(false);
	    				uiUtil.getTabHost().getTabWidget().getChildTabViewAt(2).setEnabled(false);
    				}
    				break;
    			case 2:
    				robot.shutdown();
    				android.os.Process.killProcess(android.os.Process.myPid());
    			}
    		}
    	});
    }
    
    /** Show a connect alert */
    private void showConnectDialog(final AdapterView<?> arg0, final View arg1) {

    	AlertDialog.Builder alert = new AlertDialog.Builder(this);

    	alert.setTitle("Connect");
    	alert.setMessage("Enter the IP Address of the robot you wish to connect to.");

    	// Set an EditText view to get user input 
    	final EditText input = new EditText(this);
    	input.setText(uiUtil.IP_PC);
    	alert.setView(input);

    	alert.setPositiveButton("Ok", new DialogInterface.OnClickListener() {
    	public void onClick(DialogInterface dialog, int whichButton) {
    	  String value = input.getText().toString();
    	  	Toast.makeText(getApplicationContext(), "Connecting...", 2).show();
		    robot.myHost = value;
		    robot.myPort = 7272;
		    Thread t = new Thread(robot);
		    t.start();
		    try {
				Thread.sleep(1000);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		    if (robot.connected) {
		    	arg1.setEnabled(false);
				View v1 = arg0.getChildAt(1);
				v1.setEnabled(true);
				robot.connected = true;
				uiUtil.getTabHost().getTabWidget().getChildTabViewAt(1).setEnabled(true);
				uiUtil.getTabHost().getTabWidget().getChildTabViewAt(2).setEnabled(true);
		    } else {
		    	Toast.makeText(getApplicationContext(), "Error connecting", 1).show();
		    }
    	  }
    	});

    	alert.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
    	  public void onClick(DialogInterface dialog, int whichButton) {
    		  robot.connected = false;
    	  }
    	});
    	
    	alert.show();

    }
    
    /** The list of actions for the listview */
    static final String[] ACTIONS = new String[] {
    	"Connect", "Disconnect", "Application Exit"
    };


}