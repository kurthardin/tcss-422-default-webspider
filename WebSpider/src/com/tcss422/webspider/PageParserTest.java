package com.tcss422.webspider;

public class PageParserTest {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		Page testPage = new Page();
		testPage.content = "<html>\n<head>\n<title>\nJava Practices -> Parse text\n</title>\n<link rel=\"stylesheet\" type=\"text/css\" href=\"../stylesheet8.css\" media=\"all\">\n<link rel=\"shortcut icon\" href='/images/favicon.ico' type=\"image/vnd.microsoft.icon\">\n<meta name=\"description\" content=\"Concise presentations of java programming practices, tasks, and conventions, amply illustrated with syntax highlighted code examples.\">\n<a href='/home/HomeAction.do' title='Table of Contents'>Home</a> |\n<div class='menu-bar'>";
		SpiderController ctrl = new SpiderController();
		ctrl.addKeyword("the");
		ctrl.addKeyword("a");
		ctrl.addKeyword("title");
		PageParser parser = new PageParser(testPage, ctrl);
		parser.run();
		System.out.println("Keyword counts:");
		for (int i = 0; i < ctrl.getKeywords().size(); i++) {
			System.out.println("   " + ctrl.getKeywords().get(i) + ": " + testPage.keywordCounts[i]);
		}
	}

}
