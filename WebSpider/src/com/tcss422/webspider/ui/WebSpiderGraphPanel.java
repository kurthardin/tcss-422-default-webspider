/**
 * 
 */
package com.tcss422.webspider.ui;

import java.awt.BasicStroke;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Point;
import java.util.ArrayList;
import java.util.List;

import javax.swing.JPanel;

/**
 * @author Kurt Hardin
 *
 */
public class WebSpiderGraphPanel extends JPanel {
	
	private static final long serialVersionUID = 1L;
	
	private static final int GRAPH_AXES_OFFSET = 50;
	private static final int GRAPH_X_PADDING = 10;
	private static final int GRAPH_Y_PADDING = 20;
	
//	private static final String GRAPH_X_LABEL = "THREADS";
//	private static final String GRAPH_Y_LABEL = "RUNNING TIME (ms)";
	
	private static final int MIN_RUNNING_TIME = 0;
	private static final int MIN_THREADS = 0;
	
	private int my_max_running_time = MIN_RUNNING_TIME;
	private int my_max_threads = MIN_THREADS;
	
	List<Point> points = new ArrayList<Point>();
	
	public void paintComponent(final Graphics g) {
		super.paintComponent(g);  // Typical Swing approach
		Graphics2D g2d = (Graphics2D)g;
		Dimension windowSize = getSize();
		g2d.clearRect(0, 0, windowSize.width, windowSize.height);
		drawAxes(g2d);
		plotGraph(g2d);
	}
	
	private void drawAxes(final Graphics2D g2d) {
		Dimension dim = getSize();
		g2d.setStroke(new BasicStroke(3));
		g2d.drawLine(
				GRAPH_AXES_OFFSET, 
				0, 
				GRAPH_AXES_OFFSET, 
				dim.height - GRAPH_AXES_OFFSET);
		g2d.drawLine(
				GRAPH_AXES_OFFSET, 
				dim.height - GRAPH_AXES_OFFSET, 
				dim.width, 
				dim.height - GRAPH_AXES_OFFSET);
		g2d.drawString(String.valueOf(MIN_THREADS), GRAPH_AXES_OFFSET, 
				dim.height - GRAPH_AXES_OFFSET + GRAPH_Y_PADDING);
		g2d.drawString(String.valueOf(my_max_threads), dim.width - GRAPH_X_PADDING, 
				dim.height - GRAPH_AXES_OFFSET + GRAPH_Y_PADDING);
		String minRuntime = String.valueOf(MIN_RUNNING_TIME);
		g2d.drawString(minRuntime, GRAPH_AXES_OFFSET - g2d.getFontMetrics().stringWidth(minRuntime) - GRAPH_X_PADDING, 
				dim.height - GRAPH_AXES_OFFSET);
		String maxRuntime = String.valueOf(my_max_running_time);
		g2d.drawString(maxRuntime, GRAPH_AXES_OFFSET - g2d.getFontMetrics().stringWidth(maxRuntime) - GRAPH_X_PADDING, 
				GRAPH_Y_PADDING);
		
		
//		int centerOffset = g2d.getFontMetrics().stringWidth(GRAPH_X_LABEL) / 2;
//		g2d.drawString(
//				GRAPH_X_LABEL, 
//				((windowSize.width - GRAPH_AXES_OFFSET) / 2) - centerOffset + GRAPH_AXES_OFFSET, 
//				windowSize.height - 15);
//		centerOffset = g2d.getFontMetrics().stringWidth(GRAPH_Y_LABEL) / 2;
//		Font theFont = g2d.getFont();
//		AffineTransform fontAT = new AffineTransform();
//		fontAT.rotate(Math.toRadians(90));
//		Font theDerivedFont = theFont.deriveFont(fontAT);
//		g2d.setFont(theDerivedFont);
//		g2d.drawString(
//				GRAPH_Y_LABEL, 
//				15,
//				((windowSize.height - GRAPH_AXES_OFFSET) / 2) - centerOffset + GRAPH_AXES_OFFSET);
//		g2d.setFont(theFont);
	}
	
	private void plotGraph(final Graphics2D g2d) {
		Dimension dim = getSize();
		g2d.setStroke(new BasicStroke(1));
		Point prevPlotPoint = null;
		for (Point currentPoint : points) {
			float curr_x_ratio = (float) currentPoint.x / my_max_threads;
			float curr_y_ratio = (float) currentPoint.y / my_max_running_time;
			int x = (int) ((dim.width - GRAPH_AXES_OFFSET) * curr_x_ratio) + GRAPH_AXES_OFFSET;
			x += (currentPoint.x == 0 ? GRAPH_X_PADDING : -GRAPH_X_PADDING);
			int y = dim.height - (int) ((dim.height - GRAPH_AXES_OFFSET) * curr_y_ratio) - GRAPH_AXES_OFFSET;
			final Point currPlotPoint = new Point(x, y);
			if (prevPlotPoint != null) {
				g2d.drawLine(prevPlotPoint.x, prevPlotPoint.y, currPlotPoint.x, currPlotPoint.y);
			}
			g2d.fillOval(currPlotPoint.x, currPlotPoint.y, 5, 5);
			prevPlotPoint = currPlotPoint;
		}
	}
	
	public void addPoint(final int the_thread_count, final int the_running_time_seconds) {
		my_max_threads = Math.max(the_thread_count, my_max_threads);
		my_max_running_time = Math.max(the_running_time_seconds, my_max_running_time);
		final Point newPoint = new Point(the_thread_count, the_running_time_seconds);
		points.add(newPoint);
		repaint();
	}
}

