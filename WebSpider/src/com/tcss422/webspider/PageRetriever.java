package com.tcss422.webspider;

import java.io.IOException;
import java.io.InputStream;
import java.net.URL;

public class PageRetriever implements Runnable {
	
	/**
	 * The main controller for the web spider. 
	 */
	private final WebSpiderController my_controller;
	
	private final boolean my_is_multithreaded;
	
	/**
	 * The url to be retrieved.
	 */
	private final URL my_url;
	
	/**
	 * The {@link Page} created from the given url.
	 */
	private Page my_page;
	
	public PageRetriever(final URL a_url, final WebSpiderController a_controller) {
		my_is_multithreaded = true;
		my_controller = a_controller;
		my_url = a_url;
		my_page = new Page(my_url);
	}
	
	public PageRetriever(final URL a_url, final WebSpiderController a_controller, final boolean is_threaded) {
		my_is_multithreaded = is_threaded;
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
			}
			my_page.my_content = buffer.toString();
			if (my_is_multithreaded) {
				my_controller.submitPage(my_page);	
			} else {
				(new PageParser(my_page, my_controller)).run();
			}
		} catch (IOException e) {
//			e.printStackTrace();
		}

	}

}
