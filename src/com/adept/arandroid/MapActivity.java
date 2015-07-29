package com.adept.arandroid;

import android.app.Activity;
import android.os.Bundle;
import android.opengl.GLSurfaceView;

import com.adept.arandroid.render.*;

public class MapActivity extends Activity {

	/*
	GL2JNIView mView;

    @Override protected void onCreate(Bundle icicle) {
        super.onCreate(icicle);
        mView = new GL2JNIView(getApplication());
	setContentView(mView);
    }

    @Override protected void onPause() {
        super.onPause();
        mView.onPause();
    }

    @Override protected void onResume() {
        super.onResume();
        mView.onResume();
    }*/
	    
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        // Create the surface to render on
        GLSurfaceView view = new GLSurfaceView(this);
        
        // Create the renderer
        ArjRenderer myRenderer = new ArjRenderer();
        
        // Create Renders for each object we want to draw
        ArjMapRender mapRender = new ArjMapRender();
        ArjRobotRender robotRender = new ArjRobotRender();
        myRenderer.addRenderable(mapRender);
        myRenderer.addRenderable(robotRender);
 
        // Create an Adapter for the robot
        @SuppressWarnings("unused")
		ArjRobotAdapter robotAdapter = new ArjRobotAdapter(ArjRobot.getInstance(), robotRender);
        
        // Set this view's renderer
        view.setRenderer(myRenderer);
        
        // Show the view
        setContentView(view);
    }
   
}
