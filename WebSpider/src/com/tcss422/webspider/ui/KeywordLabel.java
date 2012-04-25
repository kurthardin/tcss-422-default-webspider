/*
 * TCSS 422 Spring 2012 - Web Spider
 * Team: Default
 * Kurt Hardin
 * Daniel Bartholomew
 */

package com.tcss422.webspider.ui;

import java.awt.BorderLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JPanel;
import javax.swing.JTextField;

/**
 * @author Kurt Hardin
 * @version 1.0
 */
public class KeywordLabel extends JPanel {
	
	private static final long serialVersionUID = 1L;
	
	private final JTextField keywordField;
	private final JButton removeButton;
	private final RemoveKeywordLabelHandler removeHandler;
	
	public KeywordLabel(final String the_word, final RemoveKeywordLabelHandler the_remove_handler) {
		super(new BorderLayout());
		keywordField = new JTextField(the_word);
		keywordField.setEditable(false);
		
		removeButton = new JButton("X");
//		final KeywordLabel this_label = this;
		removeButton.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				removeHandler.remove(KeywordLabel.this);
			}
		});
		
		removeHandler = the_remove_handler;

		add(removeButton, BorderLayout.LINE_START);
		add(keywordField, BorderLayout.CENTER);
	}
	
	public String getText() {
		return keywordField.getText();
	}
	
	@Override
	public void setEnabled(final boolean enable) {
		super.setEnabled(enable);
		removeButton.setEnabled(enable);
	}
	
	public interface RemoveKeywordLabelHandler {
		public void remove(KeywordLabel the_keyword_label);
	}
}
