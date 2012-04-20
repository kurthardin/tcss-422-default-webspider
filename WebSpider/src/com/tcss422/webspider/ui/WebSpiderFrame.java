package com.tcss422.webspider.ui;

/*
 * TCSS 422 Spring 2012 - Web Spider
 * Author: Kurt Hardin
 */

import java.awt.GridLayout;
import java.net.URL;
import java.util.ArrayList;

import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;

import com.tcss422.webspider.Reporter;
import com.tcss422.webspider.SpiderController;

/**
 * WebSpiderFrame provides the user interface for the web spider program.
 * 
 * @author Kurt Hardin
 * @version 1.0 (Winter 2012)
 */
public final class WebSpiderFrame extends JFrame implements Reporter {
  
  /**
   * The Serialization ID.
   */
  private static final long serialVersionUID = 0;
  
  /**
   * The absolute maximum number of pages to retrieve.
   */
  private static final int MAX_PAGE_LIMIT = 10000;

  /**
   * The shopping cart used by this GUI.
   */
  private SpiderController my_spider;
  
  private JPanel my_report_panel;

  /**
   * The text field used to display the current page url.
   */
  private final JLabel my_page_url;

  /**
   * The text field used to display the current number of pages retrieved.
   */
  private final JLabel my_page_count;

  /**
   * The text field used to display the average number of words per page.
   */
  private final JLabel my_avg_words_per_page;

  /**
   * The text field used to display the average number of URLs per page.
   */
  private final JLabel my_avg_urls_per_page;

  /**
   * The text fields used to display the average number of hits per page for each keyword.
   */
  private final ArrayList<JLabel> my_keywords_avg_hits_per_page;

  /**
   * The text field used to display the maximum number of pages to retrieve.
   */
  private final JLabel my_page_limit;

  /**
   * The text field used to display the average parse time per page.
   */
  private final JLabel my_avg_parse_time_per_page;

  /**
   * The text field used to display the current total running time.
   */
  private final JLabel my_total_runtime;

  /**
   * Creates a new ShoppingGUI to sell the specified list of items.
   * 
   * @param the_items The list of items.
   */
  public WebSpiderFrame() {
    // create frame and report panel
    super("TCSS 422 Spring 2012 - Web Spider");
    setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
    
    my_page_url = new JLabel("NONE");
    my_page_count = new JLabel("0");
    my_avg_words_per_page = new JLabel("0.0");
    my_avg_urls_per_page = new JLabel("0.0");
    my_page_limit = new JLabel(String.valueOf(MAX_PAGE_LIMIT));
    my_avg_parse_time_per_page = new JLabel("0.0 ms");
    my_total_runtime = new JLabel("0.0 s");
    
    my_keywords_avg_hits_per_page = new ArrayList<JLabel>(10);
    
    my_report_panel = makeReportPanel();
    add(my_report_panel, "North");
    
    // adjust size to just fit
    pack();
    setVisible(true);
  }
  
  /**
   * Creates a panel to hold the total.
   * 
   * @return The created panel.
   */
  private JPanel makeReportPanel() {
    // create the report panel, and its label
    final JPanel p = new JPanel();
    
    final JPanel labelsPanel = new JPanel(new GridLayout(my_keywords_avg_hits_per_page.size() + 7, 1));
    labelsPanel.add(new JLabel("Parsed URL:"));
    labelsPanel.add(new JLabel("Pages retrieved:"));
    labelsPanel.add(new JLabel("Average words per page:"));
    labelsPanel.add(new JLabel("Average URLs per page:"));
    if (my_spider != null) {
    	for (String text : my_spider.getKeywords()) {
    		labelsPanel.add(new JLabel("   " + text));
    	}
    }
    labelsPanel.add(new JLabel("Page limit:"));
    labelsPanel.add(new JLabel("Average parse time per page:"));
    labelsPanel.add(new JLabel("Total running time:")); 
    
    final JPanel valuesPanel = new JPanel(new GridLayout(my_keywords_avg_hits_per_page.size() + 7, 1));
    valuesPanel.add(my_page_url);
    valuesPanel.add(my_page_count);
    valuesPanel.add(my_avg_words_per_page);
    valuesPanel.add(my_avg_urls_per_page);
    for (JLabel label : my_keywords_avg_hits_per_page) {
    	valuesPanel.add(label);
    }
    valuesPanel.add(my_page_limit);
    valuesPanel.add(my_avg_parse_time_per_page);
    valuesPanel.add(my_total_runtime);
    
    p.add(labelsPanel);
    p.add(valuesPanel);
    
    return p;
  }
  
  @Override
  public void setPageURL(URL the_url) {
	  my_page_url.setText(the_url.toString());
  }

  @Override
  public void setPagesRetrieved(int the_val) {
	  my_page_count.setText(String.valueOf(the_val));
  }

  @Override
  public void setAvgWordsPerPage(long the_val) {
	  my_avg_words_per_page.setText(String.valueOf(the_val));
  }

  @Override
  public void setAvgURLsPerPage(long the_val) {
	  my_avg_urls_per_page.setText(String.valueOf(the_val));
  }

  @Override
  public void setKeywordAvgHitsPerPage(int the_keywordId, long the_val) {
	  JLabel label = my_keywords_avg_hits_per_page.get(the_keywordId);
	  if (label != null) {
		  label.setText(String.valueOf(the_val));
	  }
  }

  @Override
  public void setPageLimit(int the_val) {
	  my_page_limit.setText(String.valueOf(the_val));
  }

  @Override
  public void setAvgParseTimePerPage(long the_val) {
	  my_avg_parse_time_per_page.setText(String.valueOf(the_val));
  }

  @Override
  public void setTotalRunningTime(long the_val) {
	  my_total_runtime.setText(String.valueOf(the_val));
  }

}

