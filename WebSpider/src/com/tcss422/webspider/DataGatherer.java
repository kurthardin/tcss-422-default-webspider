/*
 * TCSS 422 Spring 2012 - Web Spider
 * Team: Default
 * Kurt Hardin
 * Daniel Bartholomew
 */

package com.tcss422.webspider;

import java.net.URL;

/**
 * @author Kurt Hardin
 * @version 1.0
 */
public class DataGatherer {
	
	private final WebSpiderController my_controller;
	private final Reporter my_reporter;
	
	private int my_pages_retrieved;
	private long my_total_words;
	private long my_total_urls;
	private final int [] my_total_keyword_counts;
	private long my_total_parse_time_nanos;
	private long my_total_running_time_nanos;
	
	public DataGatherer(Reporter the_reporter, WebSpiderController the_controller) {
		my_reporter = the_reporter;
		my_controller = the_controller;
		my_total_keyword_counts = new int [WebSpiderController.MAX_KEYWORDS];
		for (int i = 0; i < my_total_keyword_counts.length; i++) {
			my_total_keyword_counts[i] = 0;
		}
	}
	
	public synchronized int getPagesRetrieved() {
		return my_pages_retrieved;
	}
	
	public synchronized void processPage(final Page a_page) {
		my_pages_retrieved++;
		my_total_words += a_page.my_word_count;
		my_total_urls += a_page.my_links.size();
		my_total_parse_time_nanos += ((a_page.my_parse_stop - a_page.my_parse_start));
		my_total_running_time_nanos = System.nanoTime() - my_controller.getStartTime();
		for (int i = 0; i < my_total_keyword_counts.length && 
				i < a_page.my_keyword_counts.length; i++) {
			my_total_keyword_counts[i] += a_page.my_keyword_counts[i];
		}
		updateReporter(a_page.my_address);
		if (my_pages_retrieved >= my_controller.getPageLimit()) {
			my_controller.stop();
			my_reporter.finished();
		}
	}
	
	private void updateReporter(final URL a_url) {
		my_reporter.setPageURL(a_url);
		my_reporter.setPagesRetrieved(my_pages_retrieved);
		my_reporter.setAvgWordsPerPage((float) my_total_words / my_pages_retrieved);
		my_reporter.setAvgURLsPerPage((float) my_total_urls / my_pages_retrieved);
		my_reporter.setAvgParseTimePerPage((float) my_total_parse_time_nanos / my_pages_retrieved);
		my_reporter.setTotalRunningTime(my_total_running_time_nanos);
		for (int i = 0; i < my_controller.getKeywords().size(); i++) {
			my_reporter.setKeywordAvgHitsPerPage(i, (float) my_total_keyword_counts[i] / my_pages_retrieved);
			my_reporter.setKeywordTotalHitsPerPage(i, my_total_keyword_counts[i]);
		}
		my_reporter.refresh();
	}
	
}
