package com.adept.arandroid;

import android.widget.TabHost;

/**
 * UI Utility class.
 * @author markl
 *
 */
public class UIUtil {

	/** IP address when home */
	public final String IP_HOME = "192.168.0.111";
	
	/** IP address at work */
	public final String IP_PC = "10.0.153.110";
	
	/** IP address of Powerbot */
	public final String IP_PB = "172.21.1.141";
	
	/** The only instance of UIUtil */
	private static UIUtil instance = null;
	
	/** Handle to the TabHost */
	private TabHost myTabHost;
	
	/** Gets a handle to the TabHost */
	public TabHost getTabHost() {
		return myTabHost;
	}
	
	/** Sets the handle to the TabHost */
	public void setTabHost(TabHost host) {
		myTabHost = host;
	}
	
	/** Only exists to defeat instantiation */
	protected UIUtil() { }
	
	/** Return the one UIUtil instance */
	public static UIUtil getInstance() {
		if(instance == null) {
			instance = new UIUtil();
		}
		return instance;
	}
}
