/*
 * TCSS 422 Spring 2012 - Web Spider
 * Team: Default
 * Kurt Hardin
 * Daniel Bartholomew
 */

package com.tcss422.webspider;

import java.net.URL;
import java.util.LinkedList;
import java.util.List;

/**
 * @author Daniel Bartholomew
 * @version 1.0
 */
public class Page {
	
	/**
	 * The address of this page.
	 */
	public final URL my_address;
	
	/**
	 * The HTML content of this page.
	 */
	public String my_content;
	
	/**
	 * The number of times each keyword appears on this page.
	 */
	public final int [] my_keyword_counts;
	
	/**
	 * The links found on this page.
	 */
	public final List<URL> my_links;
	
	/**
	 * The total word count for this page.
	 */
	public int my_word_count;
	
	/**
	 * The start time for parsing this page.
	 */
	public long my_parse_start;
	
	/**
	 * The stop time for parsing this page.
	 */
	public long my_parse_stop;
	
	/**
	 * Creates a new page for the specified {@link URL}.
	 * @param a_url the {@link URL} of the newly created page
	 */
	public Page(final URL a_url) {
		my_address = a_url;
		my_keyword_counts = new int [WebSpiderController.MAX_KEYWORDS];
		for (int i = 0; i < my_keyword_counts.length; i++) {
			my_keyword_counts[i] = 0;
		}
		my_links = new LinkedList<URL>();
	}
	
}
