package com.tcss422.webspider;

import java.net.URL;
import java.util.LinkedList;
import java.util.List;

public class Page {
	
	public String content;
	public final int [] keywordCounts;
	public final List<URL> links;
	
	public Page() {
		keywordCounts = new int [SpiderController.MAX_KEYWORDS];
		for (int i = 0; i < keywordCounts.length; i++) {
			keywordCounts[i] = 0;
		}
		links = new LinkedList<URL>();
	}
	
}
