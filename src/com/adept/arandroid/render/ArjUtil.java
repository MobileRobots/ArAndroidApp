package com.adept.arandroid.render;

/**
 * Utility methods.
 * @author markl
 *
 */
public final class ArjUtil {

	/**
	 * Convert degrees to radians.
	 */
	public static float d2r(float d) {
		float r = 0.0f;
		r = (float)(d * (Math.PI / 180.0f));
		return r;
	}
	
	/**
	 * Define the SCALE constant
	 */
	public static float SCALE = .00015f;
}
