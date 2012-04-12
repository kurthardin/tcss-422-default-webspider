package com.tcss422.webspider;

import java.util.ArrayList;
import java.util.List;

public class SpiderController {
	
	public static final int MAX_KEYWORDS = 10;
	
	private final List<String> keywords;
	
	public SpiderController() {
		keywords = new ArrayList<String>(10);
	}
	
	/**
	 * Adds a word to the list of keywords until a maximum of 10 words have been added.
	 * @param aWord the word to add
	 * @return true if the word is added, otherwise false
	 */
	public boolean addKeyword(final String aWord) {
		if (keywords.size() < 10) {
			keywords.add(aWord.toLowerCase());
			return true;
		}
		return false;
	}
	
	public List<String> getKeywords() {
		return keywords;
	}
	
}
