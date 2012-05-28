//
//  GUI.c
//  SchedSim
//
//  Created by Kurt Hardin on 5/28/12.
//  Copyright (c) 2012 University of Washington at Tacoma. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

#include "GUI.h"
#include "Scheduler.h"

SchedSimGUI * SchedSimGUI_init(CPU *cpu) {
    SchedSimGUI *gui = (SchedSimGUI *) malloc(sizeof(SchedSimGUI));
    
    initscr();                  // Start curses mode
    if(has_colors()) {	
        start_color();          // Start color functionality
	}
	cbreak();                   // Line buffering disabled, pass on everything to me 		
    noecho();
    curs_set(0);
    
    init_pair(0, COLOR_BLACK, COLOR_BLUE);
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_RED, COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);
    
    bkgd(COLOR_PAIR(0));
    refresh();
    
    int tbPadding = 1;
    int lrPadding = 2;
    int width = COLS * 0.5;
    int height = LINES * 0.5;
    
    
    int upperHalfHeight = height - (2 * tbPadding);
    int upperHalfY = tbPadding;
    
    int processWindowWidth = COLS - width - (3 * lrPadding);
    int processWindowX = lrPadding;
    WINDOW* statusWindowBorder = newwin(upperHalfHeight, processWindowWidth, upperHalfY, processWindowX);
    wbkgd(statusWindowBorder, COLOR_PAIR(1));
    wrefresh(statusWindowBorder);
    gui->processStatusWindow = newwin(upperHalfHeight - (2 * tbPadding), 
                               processWindowWidth - (2 * lrPadding), 
                               upperHalfY + tbPadding, 
                               processWindowX + lrPadding);
    wbkgd(gui->processStatusWindow, COLOR_PAIR(3));
    
    wattron(gui->processStatusWindow, COLOR_PAIR(1));
    wprintw(gui->processStatusWindow, "PROCESS STATUS\n");
    wattroff(gui->processStatusWindow, COLOR_PAIR(1));
    
    wprintw(gui->processStatusWindow, "PID         %% DONE\n");
    
    int i;
    for (i = 0; i < cpu->scheduler->processCount; i++) {
        wprintw(gui->processStatusWindow, "%d", i);
        wmove(gui->processStatusWindow, getcury(gui->processStatusWindow), 12);
        wprintw(gui->processStatusWindow, "0%%\n");
    }
    wrefresh(gui->processStatusWindow);
    
    
    int deviceWindowX = COLS - width - lrPadding;
    WINDOW* deviceWindowBorder = newwin(upperHalfHeight, width, upperHalfY, deviceWindowX);
    wbkgd(deviceWindowBorder, COLOR_PAIR(1));
    wrefresh(deviceWindowBorder);
    gui->deviceStatusWindow = newwin(upperHalfHeight - (2 * tbPadding), 
                              width - (2 * lrPadding), 
                              upperHalfY + tbPadding, 
                              deviceWindowX + lrPadding);
    wbkgd(gui->deviceStatusWindow, COLOR_PAIR(4));
//    scrollok(gui->deviceStatusWindow, TRUE);
    
    wattron(gui->deviceStatusWindow, COLOR_PAIR(1));
    wprintw(gui->deviceStatusWindow, "DEVICE STATUS\n");
    wattroff(gui->processStatusWindow, COLOR_PAIR(1));
    
    wprintw(gui->deviceStatusWindow, "Keyboard\n");
    wprintw(gui->deviceStatusWindow, "Video\n");
    wprintw(gui->deviceStatusWindow, "Disk\n");
    wprintw(gui->deviceStatusWindow, "Shared mem 1\n");
    wprintw(gui->deviceStatusWindow, "Shared mem 2\n");
    
    wrefresh(gui->deviceStatusWindow);
    
    
    
    int lowerHalfHeight = height - tbPadding;
    
    WINDOW* inputWindowBorder = newwin(lowerHalfHeight, COLS, height, lrPadding);
    wbkgd(inputWindowBorder, COLOR_PAIR(0));
    wrefresh(inputWindowBorder);
    gui->logWindow = newwin(lowerHalfHeight - (2 * tbPadding), 
                              COLS - (2 * lrPadding), 
                              height + tbPadding, 
                              lrPadding + lrPadding);
    wbkgd(gui->logWindow, COLOR_PAIR(2));
    scrollok(gui->logWindow, TRUE);
    wrefresh(gui->logWindow);
    
    return gui;
}