package com.tcss422.webspider;

import java.net.URL;

/**
 * Interface for updating the UI.
 * @author Kurt Hardin
 */
public interface Reporter {
	void setPageURL(URL url);
	void setPagesRetrieved(int val);
	void setAvgWordsPerPage(long val);
	void setAvgURLsPerPage(long val);
	void setKeywordAvgHitsPerPage(int keywordId, long val);
	void setKeywordTotalHitsPerPage(int the_keywordId, long the_val);
	void setAvgParseTimePerPage(long val);
	void setTotalRunningTime(long val);
	void refresh();
}
