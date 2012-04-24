package com.tcss422.webspider;

import java.net.MalformedURLException;
import java.net.URL;

public class PageRetriverTest {
	
	
	
	public static void main(String[] args) {
		
		URL test_url;
		try {
			test_url = new URL("http://faculty.washington.edu/gmobus/");
			PageRetriever testRetriever = new PageRetriever(test_url, null);
			testRetriever.run();
		} catch (MalformedURLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}
