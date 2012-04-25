/*
 * TCSS 422 Spring 2012 - Web Spider
 * Team: Default
 * Kurt Hardin
 * Daniel Bartholomew
 */

package com.tcss422.webspider;

import java.net.URL;

/**
 * Interface for updating the UI.
 * @author Kurt Hardin
 * @version 1.0
 */
public interface Reporter {
	void setPageURL(URL url);
	void setPagesRetrieved(int val);
	void setAvgWordsPerPage(double val);
	void setAvgURLsPerPage(double val);
	void setKeywordAvgHitsPerPage(int keywordId, double val);
	void setKeywordTotalHitsPerPage(int the_keywordId, double the_val);
	void setAvgParseTimePerPage(double val);
	void setTotalRunningTime(double val);
	void refresh();
	void finished();
}
