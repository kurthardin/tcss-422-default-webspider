/*
 * TCSS 422 Spring 2012 - Web Spider
 * Team: Default
 * Kurt Hardin
 * Daniel Bartholomew
 */

package com.tcss422.webspider;

import java.net.MalformedURLException;
import java.net.URL;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * @author Kurt Hardin
 * @version 1.0
 */
public class PageParser implements Runnable {
	
	/**
	 * The main controller for the web spider. 
	 */
	private final WebSpiderController my_controller;
	
	/**
	 * The {@link Page} to parse.
	 */
	private final Page my_page;
	
	/**
	 * Creates a new page parser with the specified {@link Page}. 
	 * @param a_page the page to parse
	 */
	public PageParser(final Page a_page, final WebSpiderController a_controller) {
		my_controller = a_controller;
		my_page = a_page;
	}
	
	/**
	 * Parses the associated {@link Page} for words and links.
	 */
	@Override
	public void run() {
		Pattern pattern = Pattern.compile("a\\shref=[\\'\\\"]\\S+[\\'\\\"]|\\w+");
		Matcher matcher = pattern.matcher(my_page.my_content);
		my_page.my_parse_start = System.nanoTime();
		while (matcher.find()) {
			String match = matcher.group();
//			System.out.println(match);

			if (match.startsWith("a href")) {									// Found a link
				String url_str = match.substring(8, match.length() - 1);
				
				if (url_str.endsWith("html") || url_str.endsWith("htm") || 
						url_str.endsWith("txt") || url_str.endsWith("/")) {
					if (!url_str.startsWith("http://")) {							// Fix url for local links 
						String url_str_prefix = my_page.my_address.toString();
						url_str_prefix = url_str_prefix.substring(
								0, url_str_prefix.lastIndexOf("/"));

						if (url_str.startsWith("./")) {
							url_str = url_str.substring(2);
						} else if (url_str.startsWith("/")) {
							url_str = url_str.substring(1);
						} else {
							while(url_str.startsWith("../")) {
								url_str = url_str.substring(3);
								url_str_prefix = url_str_prefix.substring(
										0, url_str_prefix.lastIndexOf("/"));
							}
						}
						url_str = url_str_prefix + "/" + url_str;
					}

//					System.out.println("   url: " + url_str);
					if (!url_str.startsWith("http://questioneverything.typepad.com/")) {
						try {
							URL url = new URL(url_str);
							my_page.my_links.add(url);
							my_controller.submitUrl(url);
						} catch (MalformedURLException e) {
//							e.printStackTrace();
						}
					}
				}

			} else {															// Found a word
				processWord(match);
			}
		}
		my_page.my_parse_stop = System.nanoTime();
		
		my_controller.getGatherer().processPage(my_page);
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
