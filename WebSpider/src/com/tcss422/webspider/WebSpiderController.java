package com.tcss422.webspider;

import java.net.URL;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.RejectedExecutionException;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;

/**
 * The main controller for the WebSpider program.  
 * Manages all the thread pools and data collection classes.
 * @author Kurt Hardin
 */
public class WebSpiderController {
	
	/**
	 * The maximum number of keywords the user can add.
	 */
	public static final int MAX_KEYWORDS = 10;
	
	/**
	 * The default number of threads per pool
	 */
	public static final int DEFAULT_THREADS_PER_POOL = 5;
	
	/**
	 * The amount of time in milliseconds that extra threads remain alive when idle. 
	 */
	public static final long EXTRA_THREAD_STAY_ALIVE_MILLIS = 500;
	
	private final boolean my_is_multithreaded;
	
	/**
	 * The {@link URL} to start crawling from.
	 */
	private final URL my_base_url;
	
	/**
	 * The maximum number of {@link Page}s to retrieve.
	 */
	private final int my_page_limit;
	
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
	
	private final DataGatherer my_gatherer;
	
	/**
	 * The start time for this WebSpider.
	 */
	private final long my_start_time;
	
	/**
	 * The number of {@link URL}s that have been submitted for retrieval.
	 */
	private int my_submitted_url_count = 0;

	public WebSpiderController(final URL the_base_url, final int the_page_limit, final Reporter the_reporter) {
		this(the_base_url, the_page_limit, the_reporter, true);
	}
	
	/**
	 * Creates a new web spider starting from the specified {@link URL}.
	 * @param the_base_url the {@link URL} from which to start crawling
	 * @param the_page_limit the maximum number of {@link Page}s to retrieve
	 */
	public WebSpiderController(final URL the_base_url, final int the_page_limit, final Reporter the_reporter, final boolean is_multithreaded) {
		my_base_url = the_base_url;
		my_is_multithreaded = is_multithreaded;
		my_page_limit = the_page_limit;
		my_keywords = new ArrayList<String>(10);
		int numThreads = 1;
		if (my_is_multithreaded){
			numThreads = DEFAULT_THREADS_PER_POOL;
		}
		my_url_pool = new ThreadPoolExecutor(
				numThreads, 
				numThreads, 
				EXTRA_THREAD_STAY_ALIVE_MILLIS, 
				TimeUnit.MILLISECONDS, 
				new LinkedBlockingQueue<Runnable>()
				);
		my_url_pool.allowCoreThreadTimeOut(true);
		my_page_pool = new ThreadPoolExecutor(
				numThreads, 
				numThreads, 
				EXTRA_THREAD_STAY_ALIVE_MILLIS, 
				TimeUnit.MILLISECONDS, 
				new LinkedBlockingQueue<Runnable>()
				);
		my_page_pool.allowCoreThreadTimeOut(true);
		
		my_start_time = System.nanoTime();
		my_gatherer = new DataGatherer(the_reporter, this);
	}
	
	/**
	 * Adds a word to the list of keywords to be tracked up to a maximum of 10 words.
	 * @param a_word the word to add
	 * @return true if the word is added, otherwise false
	 */
	public boolean addKeyword(final String a_word) {
		if (my_keywords.size() < 10 && !my_keywords.contains(a_word)) {
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
	 * Gets the start time for this WebSpider.
	 * @return the start time for this WebSpider
	 */
	public long getStartTime() {
		return my_start_time;
	}
	
	public DataGatherer getGatherer() {
		return my_gatherer;
	}
	
	public void start() {
		submitUrl(my_base_url);;
	}
	
	/**
	 * Adds a {@link URL} to the queue of {@link URL}s to be retrieved.
	 * @param a_url the {@link URL} to retrieve
	 */
	public synchronized void submitUrl(final URL a_url) {
		if(my_submitted_url_count < my_page_limit) {
			Runnable the_job = new PageRetriever(a_url, this, my_is_multithreaded);
			try {
				my_url_pool.execute(the_job);
				my_submitted_url_count++;
			} catch (final RejectedExecutionException e) {
				// TODO Handle rejected url jobs
			}
		}
	}
	
	/**
	 * Adds a {@link Page} to the queue of {@link Page}s to be parsed.
	 * @param a_page the {@link Page} to parse
	 */
	public synchronized void submitPage(final Page a_page) {
		if(my_is_multithreaded) {
			Runnable the_job = new PageParser(a_page, this);
			try {
				my_page_pool.execute(the_job);
			} catch (final RejectedExecutionException e) {
				// TODO Handle rejected page jobs.
			}
		}
	}
	
	/**
	 * Sets the number of threads to use for each task (page retrieval and page parsing).
	 * @param the_num_threads the number of threads per task
	 */
	public void setNumThreadsPerTask(final int the_num_threads) {
		if(my_is_multithreaded) {
			my_url_pool.setCorePoolSize(the_num_threads);
			my_page_pool.setCorePoolSize(the_num_threads);
		}
	}
	
	public void stop() {
		my_url_pool.shutdownNow();
		my_page_pool.shutdownNow();
	}
	
}
