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
	void setAvgWordsPerPage(float val);
	void setAvgURLsPerPage(float val);
	void setKeywordAvgHitsPerPage(int keywordId, float val);
	void setKeywordTotalHitsPerPage(int keywordId, int val);
	void setAvgParseTimePerPage(float val);
	void setTotalRunningTime(long val);
	void refresh();
	void finished();
}
