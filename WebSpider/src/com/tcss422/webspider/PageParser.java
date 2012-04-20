/**
 * 
 */
package com.tcss422.webspider;

import java.net.MalformedURLException;
import java.net.URL;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * @author Kurt Hardin
 *
 */
public class PageParser implements Runnable {
	
	/**
	 * The main controller for the web spider. 
	 */
	private final SpiderController my_controller;
	
	/**
	 * The {@link Page} to parse.
	 */
	private final Page my_page;
	
	/**
	 * Creates a new page parser with the specified {@link Page}. 
	 * @param aPage the page to parse
	 */
	public PageParser(final Page aPage, final SpiderController aController) {
		my_controller = aController;
		my_page = aPage;
	}
	
	/**
	 * Parses the associated {@link Page} for words and links.
	 */
	@Override
	public void run() {
		Pattern ptrn = Pattern.compile("a href=\\'\\S*|\\w*");
		Matcher matcher = ptrn.matcher(my_page.my_content);
		my_page.my_parse_start = System.nanoTime();
		while (matcher.find()) {
			String match = matcher.group();
			if (match.length() > 0) {
				System.out.println(match);
				if (match.startsWith("a href")) {
					String urlStr = match.substring(8, match.length() - 1);
					System.out.println("   url: " + urlStr);
					
					// TODO: Check for local links
					
					try {
						URL url = new URL(urlStr);
						my_page.my_links.add(url);
					} catch (MalformedURLException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
				} else {
					processWord(match);
				}
			}
		}
		my_page.my_parse_stop = System.nanoTime();
	}
	
	/**
	 * Checks each word found by the parser against the list of keywords incrementing any counts appropriately.
	 * @param a_word the word to check
	 */
	private void processWord(final String a_word) {
		my_page.my_word_count++;
		int keywordIndex = my_controller.getKeywords().indexOf(a_word.toLowerCase());
		if (keywordIndex > -1) {
			my_page.my_keyword_counts[keywordIndex] += 1;
		}
	}

}
