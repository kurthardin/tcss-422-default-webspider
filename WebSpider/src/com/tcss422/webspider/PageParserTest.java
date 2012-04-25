/*
 * TCSS 422 Spring 2012 - Web Spider
 * Team: Default
 * Kurt Hardin
 * Daniel Bartholomew
 */

package com.tcss422.webspider;

import java.net.MalformedURLException;
import java.net.URL;

import com.tcss422.webspider.ui.WebSpiderFrame;

public class PageParserTest {
	
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		PageParserTest.run();
	}
	
	/**
	 * Runs Tests on the PageParser class.
	 */
	public static void run() {
		URL test_url = null;
		try {
			test_url = new URL("http://www.test.com/some/url/page.html");
		} catch (MalformedURLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		if (test_url != null) {
			Page test_page = new Page(test_url);
			test_page.my_content = "<html>\n<head>\n<title>\nJava Practices -> Parse text\n</title>\n<link rel=\"stylesheet\" type=\"text/css\" href=\"../stylesheet8.css\" media=\"all\">\n<link rel=\"shortcut icon\" href='/images/favicon.ico' type=\"image/vnd.microsoft.icon\">\n<meta name=\"description\" content=\"Concise presentations of java programming practices, tasks, and conventions, amply illustrated with syntax highlighted code examples.\">\n<a href=\"HomeAction.do\">Home</a>\n<a href='./home/HomeAction.do' title='Table of Contents'>Home</a>\n<a href='../../home/HomeAction.do' title='Table of Contents'>Home</a>\n<a href='../home/HomeAction.do' title='Table of Contents'>Home</a>\n<a href='/home/HomeAction.do' title='Table of Contents'>Home</a> |\n<div class='menu-bar'>";
			WebSpiderController ctrl = new WebSpiderController(test_url, WebSpiderFrame.MAX_PAGE_LIMIT);
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
