/*
 * TCSS 422 Spring 2012 - Web Spider
 * Team: Default
 * Kurt Hardin
 * Daniel Bartholomew
 */

package com.tcss422.webspider.ui;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.net.MalformedURLException;
import java.net.URL;
import java.text.DecimalFormat;
import java.util.ArrayList;

import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JComboBox;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JTextField;
import javax.swing.JToolBar;
import javax.swing.UIManager;
import javax.swing.UnsupportedLookAndFeelException;

import com.tcss422.webspider.Reporter;
import com.tcss422.webspider.WebSpiderController;
import com.tcss422.webspider.ui.KeywordLabel.RemoveKeywordLabelHandler;

/**
 * WebSpiderFrame provides the user interface for the web spider program.
 * 
 * Team: Default
 * @author Kurt Hardin
 * @author Daniel Bartholomew
 * @version 1.0 (Winter 2012)
 */
public final class WebSpiderFrame extends JFrame implements Reporter, RemoveKeywordLabelHandler {

	/**
	 * The Serialization ID.
	 */
	private static final long serialVersionUID = 0;

	/**
	 * The absolute maximum number of pages to retrieve.
	 */
	public static final int MAX_PAGE_LIMIT = 10000;

	public static final int MINIMUM_COMPONENT_WIDTH = 768;
	public static final int MINIMUM_COMPONENT_HEIGHT = 256;
	
	private static final Dimension GRAPH_SIZE = new Dimension(512, 512);

	public static final int URL_TEXT_FIELD_WIDTH = 65;

	public static final String [] MAX_THREAD_OPTIONS = {"1", "2", "3", "4", "5", "10", "25"};
	public static final String [] PAGE_LIMITS = {"100", "500", "1000", "2500", "5000", "10000"};

	public static final String DEFAULT_URL_STR = "http://faculty.washington.edu/gmobus/index.html";
	
	private final DecimalFormat my_decimal_format = new DecimalFormat("#.####"); 

	/**
	 * The controller used by this GUI.
	 */
	private WebSpiderController my_spider;

	private final JCheckBox my_is_multithreaded_checkbox;
	private final JComboBox my_max_url_threads_combo_box;
	private final JComboBox my_max_page_threads_combo_box;
	private final JComboBox my_page_limit_combo_box;
	private final JCheckBox my_is_batch_test;
	private final JButton my_start_stop_button;

	private JPanel my_report_panel;
	private final JPanel my_keywords_panel;
	/**
	 * The text field used to display the current page url.
	 */
	private final JTextField my_page_url;

	/**
	 * The text field used to display the current number of pages retrieved.
	 */
	private final JTextField my_page_count;

	/**
	 * The text field used to display the average number of words per page.
	 */
	private final JTextField my_avg_words_per_page;

	/**
	 * The text field used to display the average number of URLs per page.
	 */
	private final JTextField my_avg_urls_per_page;

	private final ArrayList<KeywordLabel> my_keyword_labels;

	/**
	 * The text fields used to display the average number of hits per page for each keyword.
	 */
	private final ArrayList<JTextField> my_keywords_avg_hits_per_page;

	/**
	 * The text fields used to display the average number of hits per page for each keyword.
	 */
	private final ArrayList<JTextField> my_keywords_total_hits_per_page;

	/**
	 * The text field used to display the average parse time per page.
	 */
	private final JTextField my_avg_parse_time_per_page;

	/**
	 * The text field used to display the current total running time.
	 */
	private final JTextField my_total_runtime;

	private String my_previous_base_url_str;
	private boolean my_is_running = false;
	
	private WebSpiderGraphPanel my_graph_panel;
	private int my_batch_test_num_threads_idx = 0;

	/**
	 * Creates a new WebSpider GUI.
	 */
	public WebSpiderFrame() {
		// create frame and report panel
		super("TCSS 422 Spring 2012 - Web Spider");
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		
		/* Use an appropriate Look and Feel */
        try {
            //UIManager.setLookAndFeel("com.sun.java.swing.plaf.windows.WindowsLookAndFeel");
            UIManager.setLookAndFeel("javax.swing.plaf.metal.MetalLookAndFeel");
            /* Turn off metal's use of bold fonts */
            UIManager.put("swing.boldMetal", Boolean.FALSE);
        } catch (UnsupportedLookAndFeelException ex) {
            ex.printStackTrace();
        } catch (IllegalAccessException ex) {
            ex.printStackTrace();
        } catch (InstantiationException ex) {
            ex.printStackTrace();
        } catch (ClassNotFoundException ex) {
            ex.printStackTrace();
        }

		setMinimumSize(new Dimension(MINIMUM_COMPONENT_WIDTH, MINIMUM_COMPONENT_HEIGHT));
		
		my_keywords_panel = new JPanel();
		
		my_is_multithreaded_checkbox = new JCheckBox();
		my_is_multithreaded_checkbox.setSelected(true);
		my_max_url_threads_combo_box = new JComboBox(MAX_THREAD_OPTIONS);
		my_max_url_threads_combo_box.setSelectedIndex(4);
		my_max_page_threads_combo_box = new JComboBox(MAX_THREAD_OPTIONS);
		my_max_page_threads_combo_box.setSelectedIndex(4);
		my_page_limit_combo_box = new JComboBox(PAGE_LIMITS);
		my_page_limit_combo_box.setSelectedIndex(4);
		my_is_batch_test = new JCheckBox();
		my_is_batch_test.setSelected(false);
		my_start_stop_button = new JButton("Start");
		my_start_stop_button.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				if (my_start_stop_button.getText().equals("Stop")) {
					my_is_batch_test.setSelected(false);
				}
				toggleControls();
			}
		});

		my_page_url = new JTextField(DEFAULT_URL_STR, URL_TEXT_FIELD_WIDTH);
		my_page_url.setDisabledTextColor(Color.BLACK);
		my_page_count = createStaticTextField("0");
		my_avg_words_per_page = createStaticTextField("0.0");
		my_avg_urls_per_page = createStaticTextField("0.0");
		my_avg_parse_time_per_page = createStaticTextField("0.0 ms");
		my_total_runtime = createStaticTextField("0.0 s");

		my_keyword_labels = new ArrayList<KeywordLabel>();
		my_keywords_avg_hits_per_page = new ArrayList<JTextField>();
		my_keywords_total_hits_per_page = new ArrayList<JTextField>();

		add(makeControlsPanel(), BorderLayout.PAGE_START);
		my_report_panel = makeReportPanel();
		add(my_report_panel, BorderLayout.CENTER);
		add(makeUrlPanel(), BorderLayout.PAGE_END);

		// adjust size to just fit
		pack();
		setVisible(true);
	}
	
	private JTextField createStaticTextField(final String the_text) {
		final JTextField field = new JTextField(the_text);
		field.setEditable(false);
		field.setDisabledTextColor(Color.BLACK);
		return field;
	}

	private JToolBar makeControlsPanel() {
		final JToolBar controlsToolBar = new JToolBar();
		controlsToolBar.add(new JLabel("Multithreaded:"));
		controlsToolBar.add(my_is_multithreaded_checkbox);
		controlsToolBar.addSeparator();
		controlsToolBar.add(new JLabel("Retriever threads:"));
		controlsToolBar.add(my_max_url_threads_combo_box);
		controlsToolBar.addSeparator();
		controlsToolBar.add(new JLabel("Parser threads:"));
		controlsToolBar.add(my_max_page_threads_combo_box);
		controlsToolBar.addSeparator();
		controlsToolBar.add(new JLabel("Page limit: "));
		controlsToolBar.add(my_page_limit_combo_box);
		controlsToolBar.addSeparator();
		controlsToolBar.add(new JLabel("Batch Test:"));
		controlsToolBar.add(my_is_batch_test);
		controlsToolBar.addSeparator();
		controlsToolBar.add(my_start_stop_button);
		return controlsToolBar;
	}

	/**
	 * Creates a panel to hold the total.
	 * 
	 * @return The created panel.
	 */
	private JPanel makeReportPanel() {
		// create the report panel, and its label
		final JPanel p = new JPanel(new FlowLayout(FlowLayout.LEFT, 15, 15));
		p.setBackground(Color.DARK_GRAY);
		p.add(makeGeneralStatsPanel());//, BorderLayout.PAGE_START);
		layoutKeywordsPanel();
		p.add(my_keywords_panel);//, BorderLayout.PAGE_START);
		return p;
	}

	private JPanel makeGeneralStatsPanel() {
		final JPanel statsPanel = new JPanel(new GridLayout(5, 2));

		statsPanel.add(new JLabel("Pages retrieved: "));
		statsPanel.add(my_page_count);

		statsPanel.add(new JLabel("Average words per page: "));
		statsPanel.add(my_avg_words_per_page);

		statsPanel.add(new JLabel("Average URLs per page: "));
		statsPanel.add(my_avg_urls_per_page);

		statsPanel.add(new JLabel("Average parse time per page: "));
		statsPanel.add(my_avg_parse_time_per_page);

		statsPanel.add(new JLabel("Total running time: "));
		statsPanel.add(my_total_runtime); 

		return statsPanel;
	}

	private JPanel layoutKeywordsPanel() {
		int rows = my_keyword_labels.size()+2;
		my_keywords_panel.setLayout(new GridLayout(rows,3));
		my_keywords_panel.setAlignmentX(TOP_ALIGNMENT);
		
		my_keywords_panel.removeAll();
		my_keywords_panel.add(new JLabel("Keywords:"));
		my_keywords_panel.add(new JLabel("Avg hits per page     "));
		my_keywords_panel.add(new JLabel("Total hits"));
		for (int i = 0; i < my_keyword_labels.size(); i++) {
			my_keywords_panel.add(my_keyword_labels.get(i));
			my_keywords_avg_hits_per_page.add(createStaticTextField("0.0"));
			my_keywords_panel.add(my_keywords_avg_hits_per_page.get(i));
			my_keywords_total_hits_per_page.add(createStaticTextField("0.0"));
			my_keywords_panel.add(my_keywords_total_hits_per_page.get(i));
		}
		final JTextField addKeywordField = new JTextField();
		addKeywordField.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent arg0) {
				if (addKeyword(addKeywordField.getText())) {
					layoutKeywordsPanel();
					pack();
					repaint();
				}
			}
		});
		my_keywords_panel.add(addKeywordField);
		my_keywords_panel.add(new JLabel());
		my_keywords_panel.add(new JLabel());
		return my_keywords_panel;
	}

	private JPanel makeUrlPanel() {
		final JPanel urlPanel = new JPanel(new FlowLayout());
		urlPanel.setBackground(Color.WHITE);
		urlPanel.add(my_page_url);
		return urlPanel;
	}
	
	private boolean addKeyword(final String the_keyword) {
		String lowerCaseKeyword = the_keyword.toLowerCase(getLocale());
		if (my_keyword_labels.size() < 10) {
			boolean labelExists = false;
			for (KeywordLabel existingKeywordLabel : my_keyword_labels) {
				if (lowerCaseKeyword.equals(existingKeywordLabel.getText())) {
					labelExists = true;
					break;
				}
			}
			if (!labelExists) {
				my_keyword_labels.add(new KeywordLabel(lowerCaseKeyword, this));
				return true;
			}
		}
		return false;
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
	public void setAvgWordsPerPage(float the_val) {
		my_avg_words_per_page.setText(my_decimal_format.format(the_val));
	}

	@Override
	public void setAvgURLsPerPage(float the_val) {
		my_avg_urls_per_page.setText(my_decimal_format.format(the_val));
	}

	@Override
	public void setKeywordAvgHitsPerPage(int the_keywordId, float the_val) {
		JTextField label = my_keywords_avg_hits_per_page.get(the_keywordId);
		if (label != null) {
			label.setText(my_decimal_format.format(the_val));
		}
	}

	@Override
	public void setKeywordTotalHitsPerPage(int the_keywordId, int the_val) {
		JTextField label = my_keywords_total_hits_per_page.get(the_keywordId);
		if (label != null) {
			label.setText(String.valueOf(the_val));
		}
	}

	@Override
	public void setAvgParseTimePerPage(float the_val_nanos) {
		my_avg_parse_time_per_page.setText(my_decimal_format.format(the_val_nanos / 1000000) + " ms");
	}

	@Override
	public void setTotalRunningTime(long the_val_nanos) {
		my_total_runtime.setText(my_decimal_format.format((float) the_val_nanos / 1000000000) + " s");
	}

	@Override
	public void refresh() {
		repaint();
	}

	/**
	 * {@inheritDoc}
	 */
	@Override
	public void remove(final KeywordLabel the_keyword_label) {
		if (!my_is_running) {
			int idx = my_keyword_labels.indexOf(the_keyword_label);
			my_keyword_labels.remove(idx);
			my_keywords_avg_hits_per_page.remove(idx);
			my_keywords_total_hits_per_page.remove(idx);
			layoutKeywordsPanel();
			pack();
			repaint();
		}
	}

	/**
	 * {@inheritDoc}
	 */
	@Override
	public void finished() {
		toggleControls();
	}
	
	private void toggleControls() {
		if (my_is_running) {
			my_page_url.setText(my_previous_base_url_str);
			
			boolean batchTestDone = true;
			if (my_is_batch_test.isSelected() ){
				
				if (my_graph_panel != null) {
					int threadCount = (my_batch_test_num_threads_idx > 0) ? 
							Integer.valueOf(MAX_THREAD_OPTIONS[my_batch_test_num_threads_idx]) : 0;
					String totalRuntimeStr = my_total_runtime.getText();
					my_graph_panel.addPoint(threadCount, 
							(int) (Float.valueOf(totalRuntimeStr.substring(0, totalRuntimeStr.length()-2))*1000));
					my_batch_test_num_threads_idx++;
				}
				
				URL baseUrl = makeBaseUrl();
				if (my_batch_test_num_threads_idx < MAX_THREAD_OPTIONS.length && baseUrl != null) {
					batchTestDone = false;
					int pageLimit = Integer.valueOf((String) my_page_limit_combo_box.getSelectedItem());
					boolean multithreaded = true;
					int maxUrlThreads = Integer.valueOf(MAX_THREAD_OPTIONS[my_batch_test_num_threads_idx]);
					int maxPageThreads = Integer.valueOf((String) my_max_page_threads_combo_box.getSelectedItem());
					startNewSpider(baseUrl, pageLimit, multithreaded, maxUrlThreads, maxPageThreads);
				}
			}
			
			if (batchTestDone){
				my_graph_panel = null;
				my_batch_test_num_threads_idx = 0;
				enableControls();
				my_start_stop_button.setText("Start");
				my_is_running = false;
				
				if (my_spider != null) {
					my_spider.stop();
				}
			}
			
		} else {
			URL baseUrl = makeBaseUrl();

			if (baseUrl != null) {
				my_start_stop_button.setText("Stop");
				disableControls();
				my_previous_base_url_str = my_page_url.getText();
				
				int pageLimit = Integer.valueOf((String) my_page_limit_combo_box.getSelectedItem());
				boolean multithreaded = false;
				int maxUrlThreads = 1;
				int maxPageThreads = 1;
				
				if (my_is_batch_test.isSelected()) {
					startGraphFrame();
				} else {
					multithreaded = my_is_multithreaded_checkbox.isSelected();
					maxUrlThreads = Integer.valueOf((String) my_max_url_threads_combo_box.getSelectedItem());
					maxPageThreads = Integer.valueOf((String) my_max_page_threads_combo_box.getSelectedItem());
				}
				startNewSpider(baseUrl, pageLimit, multithreaded, maxUrlThreads, maxPageThreads);
				my_is_running = true;
			}
		}
	}
	
	private void startNewSpider(final URL baseUrl, final int pageLimit, 
			final boolean multithreaded, final int maxUrlThreads, final int maxPageThreads) {
		my_spider = new WebSpiderController(
				baseUrl, pageLimit, WebSpiderFrame.this, multithreaded);
		for (KeywordLabel keyword : my_keyword_labels) {
			my_spider.addKeyword(keyword.getText());
		}
		my_spider.setNumUrlThreads(maxUrlThreads);
		my_spider.setNumPageThreads(maxPageThreads);
		my_spider.start();
	}
	
	private void startGraphFrame() {
		JFrame graphFrame = new JFrame("Thread Test Graph");
		my_graph_panel = new WebSpiderGraphPanel();
		graphFrame.add(my_graph_panel);
		graphFrame.setMinimumSize(GRAPH_SIZE);
		graphFrame.setSize(GRAPH_SIZE);
		graphFrame.pack();
		graphFrame.setVisible(true);
	}
	
	private URL makeBaseUrl() {
		URL baseUrl = null;
		try {
			baseUrl = new URL(my_page_url.getText());
		} catch (MalformedURLException e1) {
			JOptionPane.showMessageDialog(
					WebSpiderFrame.this, 
					"The base URL is invalid: '" + my_page_url.getText() + "'", 
					"Bad URL!", 
					JOptionPane.ERROR_MESSAGE);
		}
		return baseUrl;
	}
	
	private void enableControls() {
		my_page_url.setEnabled(true);
		for(KeywordLabel keywordLabel : my_keyword_labels) {
			keywordLabel.setEnabled(true);
		}
		my_is_multithreaded_checkbox.setEnabled(true);
		my_max_url_threads_combo_box.setEnabled(true);
		my_max_page_threads_combo_box.setEnabled(true);
		my_page_limit_combo_box.setEnabled(true);
	}
	
	private void disableControls() {
		my_page_limit_combo_box.setEnabled(false);
		my_max_page_threads_combo_box.setEnabled(false);
		my_max_url_threads_combo_box.setEnabled(false);
		my_is_multithreaded_checkbox.setEnabled(false);
		for(KeywordLabel keywordLabel : my_keyword_labels) {
			keywordLabel.setEnabled(false);
		}
		my_page_url.setEnabled(false);
	}
	
}