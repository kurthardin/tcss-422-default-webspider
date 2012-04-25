/**
 * 
 */
package com.tcss422.webspider;

import java.net.URL;

/**
 * 
 * @author Kurt Hardin
 */
public class DataGatherer {
	
	private final WebSpiderController my_controller;
	private final Reporter my_reporter;
	
	private int my_pages_retrieved;
	private long my_total_words;
	private long my_total_urls;
	private final int [] my_total_keyword_counts;
	private long my_total_parse_time;
	private long my_total_running_time;
	
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
		my_total_parse_time += ((a_page.my_parse_stop - a_page.my_parse_start) / 1000);
		my_total_running_time = System.nanoTime() - my_controller.getStartTime();
		for (int i = 0; i < my_total_keyword_counts.length && 
				i < a_page.my_keyword_counts.length; i++) {
			my_total_keyword_counts[i] += a_page.my_keyword_counts[i];
		}
		updateReporter(a_page.my_address);
	}
	
	private void updateReporter(final URL a_url) {
		my_reporter.setPageURL(a_url);
		my_reporter.setPagesRetrieved(my_pages_retrieved);
		my_reporter.setAvgWordsPerPage(my_total_words / my_pages_retrieved);
		my_reporter.setAvgURLsPerPage(my_total_urls / my_pages_retrieved);
		my_reporter.setAvgParseTimePerPage(my_total_parse_time / my_pages_retrieved);
		my_reporter.setTotalRunningTime(my_total_running_time);
		for (int i = 0; i < my_controller.getKeywords().size(); i++) {
			my_reporter.setKeywordAvgHitsPerPage(i, my_total_keyword_counts[i] / my_pages_retrieved);
			my_reporter.setKeywordTotalHitsPerPage(i, my_total_keyword_counts[i]);
		}
		my_reporter.refresh();
	}
	
}
