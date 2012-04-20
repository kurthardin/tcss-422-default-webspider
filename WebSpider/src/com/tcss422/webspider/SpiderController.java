package com.tcss422.webspider;

import java.net.URL;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;

/**
 * The main controller for the WebSpider program.  
 * Manages all the thread pools and data collection classes.
 * @author Kurt Hardin
 */
public class SpiderController {
	
	/**
	 * The maximum number of keywords the user can add.
	 */
	public static final int MAX_KEYWORDS = 10;
	
	/**
	 * The default number of threads per pool
	 */
	public static final int DEFAULT_THREADS_PER_POOL = 1;
	
	/**
	 * The amount of time in milliseconds that extra threads remain alive when idle. 
	 */
	public static final long EXTRA_THREAD_STAY_ALIVE_MILLIS = 500;
	
	/**
	 * The {@link URL} to start crawling from.
	 */
	private final URL my_base_url;
	
	/**
	 * The keywords to track when parsing {@link Page}s.
	 */
	private final List<String> my_keywords;
	
	/**
	 * The thread pool of {@link PageRetriever}s for retrieving {@link Page}s.
	 */
	private final ThreadPoolExecutor my_url_pool;
	
	/**
	 * The thread pool of {@link PageParser}s for parsing {@link Page}s.
	 */
	private final ThreadPoolExecutor my_page_pool;
	
	/**
	 * Creates a new web spider starting from the specified {@link URL}.
	 * @param the_base_url the {@link URL} from which to start crawling
	 */
	public SpiderController(final URL the_base_url) {
		my_base_url = the_base_url;
		my_keywords = new ArrayList<String>(10);
		my_url_pool = new ThreadPoolExecutor(
				DEFAULT_THREADS_PER_POOL, 
				DEFAULT_THREADS_PER_POOL, 
				EXTRA_THREAD_STAY_ALIVE_MILLIS, 
				TimeUnit.MILLISECONDS, 
				new LinkedBlockingQueue<Runnable>()
				);
		my_page_pool = new ThreadPoolExecutor(
				DEFAULT_THREADS_PER_POOL, 
				DEFAULT_THREADS_PER_POOL, 
				EXTRA_THREAD_STAY_ALIVE_MILLIS, 
				TimeUnit.MILLISECONDS, 
				new LinkedBlockingQueue<Runnable>()
				);
		submitUrl(my_base_url);
	}
	
	/**
	 * Adds a word to the list of keywords to be tracked up to a maximum of 10 words.
	 * @param a_word the word to add
	 * @return true if the word is added, otherwise false
	 */
	public boolean addKeyword(final String a_word) {
		if (my_keywords.size() < 10) {
			my_keywords.add(a_word.toLowerCase());
			return true;
		}
		return false;
	}
	
	/**
	 * Gets the list of keywords for this web spider.
	 * @return the list of keywords to be tracked
	 */
	public List<String> getKeywords() {
		return my_keywords;
	}
	
	/**
	 * Adds a {@link URL} to the queue of {@link URL}s to be retrieved.
	 * @param a_url the {@link URL} to retrieve
	 */
	public void submitUrl(final URL a_url) {
		Runnable the_job = new PageRetriever(a_url, this);
		my_url_pool.execute(the_job);
	}
	
	/**
	 * Adds a {@link Page} to the queue of {@link Page}s to be parsed.
	 * @param a_page the {@link Page} to parse
	 */
	public void submitPage(final Page a_page) {
		Runnable the_job = new PageParser(a_page, this);
		my_page_pool.execute(the_job);
	}
	
	/**
	 * Sets the number of threads to use for each task (page retrieval and page parsing).
	 * @param the_num_threads the number of threads per task
	 */
	public void setNumThreadsPerTask(final int the_num_threads) {
		my_url_pool.setCorePoolSize(the_num_threads);
		my_page_pool.setCorePoolSize(the_num_threads);
	}
	
}
