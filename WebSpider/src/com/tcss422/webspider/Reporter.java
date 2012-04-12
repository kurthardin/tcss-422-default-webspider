package com.tcss422.webspider;

import java.net.URL;

public interface Reporter {
	void setPageURL(URL url);
	void setPagesRetrieved(int val);
	void setAvgWordsPerPage(long val);
	void setAvgURLsPerPage(long val);
	void setKeywordAvgHitsPerPage(int keywordId, long val);
	void setPageLimit(int val);
	void setAvgParseTimePerPage(long val);
	void setTotalRunningTime(long val);
}
