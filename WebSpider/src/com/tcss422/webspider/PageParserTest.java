package com.tcss422.webspider;

import java.net.MalformedURLException;
import java.net.URL;

public class PageParserTest {

	/**
	 * Runs Tests on the PageParser class.
	 */
	public static void run() {
		URL test_url = null;
		try {
			test_url = new URL("http://www.google.com/");
		} catch (MalformedURLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		if (test_url != null) {
			Page test_page = new Page(test_url);
			test_page.my_content = "<html>\n<head>\n<title>\nJava Practices -> Parse text\n</title>\n<link rel=\"stylesheet\" type=\"text/css\" href=\"../stylesheet8.css\" media=\"all\">\n<link rel=\"shortcut icon\" href='/images/favicon.ico' type=\"image/vnd.microsoft.icon\">\n<meta name=\"description\" content=\"Concise presentations of java programming practices, tasks, and conventions, amply illustrated with syntax highlighted code examples.\">\n<a href='/home/HomeAction.do' title='Table of Contents'>Home</a> |\n<div class='menu-bar'>";
			SpiderController ctrl = new SpiderController(test_url);
			ctrl.addKeyword("the");
			ctrl.addKeyword("a");
			ctrl.addKeyword("title");
			PageParser parser = new PageParser(test_page, ctrl);
			parser.run();
			System.out.println("Keyword counts:");
			for (int i = 0; i < ctrl.getKeywords().size(); i++) {
				System.out.println("   " + ctrl.getKeywords().get(i) + ": " + test_page.my_keyword_counts[i]);
			}
		}
	}
}
