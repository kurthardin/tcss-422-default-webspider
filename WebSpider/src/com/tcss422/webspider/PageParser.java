/**
 * 
 */
package com.tcss422.webspider;

import java.net.MalformedURLException;
import java.net.URL;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * @author Kurt Hardin
 *
 */
public class PageParser implements Runnable {
	
	private final SpiderController myController;
	private final Page myPage;
	
	/**
	 * Creates a new page parser with the specified {@link Page}. 
	 * @param aPage the page to parse
	 */
	public PageParser(final Page aPage, final SpiderController aController) {
		myController = aController;
		myPage = aPage;
	}
	
	/**
	 * {@inheritDoc}
	 */
	@Override
	public void run() {
		Pattern ptrn = Pattern.compile("a href=\\'\\S*|\\w*");
		Matcher matcher = ptrn.matcher(myPage.content);
		while (matcher.find()) {
			String match = matcher.group();
			if (match.length() > 0) {
				System.out.println(match);
				if (match.startsWith("a href")) {
					String urlStr = match.substring(8, match.length() - 1);
					System.out.println("   url: " + urlStr);
					
					// TODO: Check for local links
					
					try {
						URL url = new URL(urlStr);
						myPage.links.add(url);
					} catch (MalformedURLException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
				} else {
					processWord(match);
				}
			}
		}
	}
	
	private void processWord(final String aWord) {
		int keywordIndex = myController.getKeywords().indexOf(aWord.toLowerCase());
		if (keywordIndex > -1) {
			myPage.keywordCounts[keywordIndex] += 1;
		}
	}

}
