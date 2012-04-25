/*
 * TCSS 422 Spring 2012 - Web Spider
 * Team: Default
 * Kurt Hardin
 * Daniel Bartholomew
 */

package com.tcss422.webspider;

import com.tcss422.webspider.ui.WebSpiderFrame;

/**
 * @author Kurt Hardin
 * @version 1.0
 */
public class WebSpiderMain {
	
	private WebSpiderMain() {
		// Private constructor to avoid instantiation.
	}

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		new WebSpiderFrame();
	}

}
