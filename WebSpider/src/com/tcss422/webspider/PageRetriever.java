package com.tcss422.webspider;

import java.io.IOException;
import java.io.InputStream;
import java.net.URL;

public class PageRetriever implements Runnable {
	
	/**
	 * The main controller for the web spider. 
	 */
	private final WebSpiderController my_controller;
	
	/**
	 * The url to be retrieved.
	 */
	private final URL my_url;
	
	/**
	 * The {@link Page} created from the given url.
	 */
	private Page my_page;
	
	public PageRetriever(final URL a_url, final WebSpiderController a_controller) {
		my_controller = a_controller;
		my_url = a_url;
		my_page = new Page(my_url);
	}
	
	@Override
	public void run() {
		InputStream is;
		try {
			is = my_url.openStream();
			int ptr = 0;
			StringBuffer buffer = new StringBuffer();
			while ((ptr = is.read()) != -1) {
			    buffer.append((char)ptr);
			my_page.my_content = buffer.toString();
			my_controller.submitPage(my_page);
			}
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

	}

}
