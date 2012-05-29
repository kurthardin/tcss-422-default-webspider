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
#include "KBDDevice.h"
#include "LinkedQueueNode.h"

#define PROCESS_STATE_COL   14
#define PROCESS_PERCENT_COL 39
#define DEVICE_BLOCKED_COL  24

SchedSimGUI * SchedSimGUI_init(CPU *cpu) {
    SchedSimGUI *gui = (SchedSimGUI *) malloc(sizeof(SchedSimGUI));
    
    gui->cpu = cpu;
    gui->updateMutex = malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(gui->updateMutex, NULL);
    
    initscr();                  // Start curses mode
    if(has_colors()) {	
        start_color();          // Start color functionality
	}
	cbreak();                   // Line buffering disabled, pass on everything to me 		
    noecho();
    curs_set(0);
    
    init_pair(1, COLOR_BLACK, COLOR_BLUE);
    init_pair(2, COLOR_WHITE, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);
    init_pair(5, COLOR_RED, COLOR_BLACK);
    init_pair(6, COLOR_BLACK, COLOR_CYAN);
    
    bkgd(COLOR_PAIR(1));
    refresh();
    
    int tbPadding = 1;
    int lrPadding = 2;
    int width = COLS * 0.5;
    int height = LINES * 0.5;
    
    
    
    int processBorderWindowY = tbPadding;
    int processBorderWindowX = lrPadding;
    int processBorderWindowHeight = height - (2 * tbPadding);
    int processBorderWindowWidth = COLS - width - (2 * lrPadding);
    WINDOW* processBorderWindow = newwin(processBorderWindowHeight, processBorderWindowWidth, 
                                         processBorderWindowY, processBorderWindowX);
    wmove(processBorderWindow, 1, (processBorderWindowWidth * 0.5) - 7);
    wprintw(processBorderWindow, "PROCESS STATUS");
    wbkgd(processBorderWindow, COLOR_PAIR(2));
    wrefresh(processBorderWindow);
    
    gui->processWindow = newwin(processBorderWindowHeight - (2 * tbPadding) - 2, 
                                processBorderWindowWidth - (2 * lrPadding), 
                                processBorderWindowY + tbPadding + 2, 
                                processBorderWindowX + lrPadding);
    wbkgd(gui->processWindow, COLOR_PAIR(2));
    
    wprintw(gui->processWindow, "PID");
    wmove(gui->processWindow, getcury(gui->processWindow), PROCESS_STATE_COL);
    wprintw(gui->processWindow, "STATE");
    wmove(gui->processWindow, getcury(gui->processWindow), PROCESS_PERCENT_COL);
    wprintw(gui->processWindow, "%% DONE\n");
    
    wattron(gui->processWindow, COLOR_PAIR(5));
    int i;
    for (i = 0; i < cpu->scheduler->processCount; i++) {
        wprintw(gui->processWindow, "%d", i);
        wmove(gui->processWindow, getcury(gui->processWindow), PROCESS_STATE_COL);
        wprintw(gui->processWindow, "blocked");
        wmove(gui->processWindow, getcury(gui->processWindow), PROCESS_PERCENT_COL);
        wprintw(gui->processWindow, "0%%\n");
    }
    wattroff(gui->processWindow, COLOR_PAIR(5));
    
    wrefresh(gui->processWindow);
    
    
    int deviceBorderWindowY = height;
    int deviceBorderWindowX = processBorderWindowX;
    int deviceBorderWindowHeight = height - tbPadding;
    int deviceBorderWindowWidth = processBorderWindowWidth;
    WINDOW* deviceBorderWindow = newwin(deviceBorderWindowHeight, deviceBorderWindowWidth, deviceBorderWindowY, deviceBorderWindowX);
    wbkgd(deviceBorderWindow, COLOR_PAIR(2));
    wmove(deviceBorderWindow, 1, (deviceBorderWindowWidth * 0.5) - 7);
    wprintw(deviceBorderWindow, "DEVICE STATUS");
    wrefresh(deviceBorderWindow);
    
    gui->deviceWindow = newwin(deviceBorderWindowHeight - (2 * tbPadding) - 2, 
                               deviceBorderWindowWidth - (2 * lrPadding), 
                               deviceBorderWindowY + tbPadding + 2, 
                               deviceBorderWindowX + lrPadding);
    wbkgd(gui->deviceWindow, COLOR_PAIR(2));
    
    wprintw(gui->deviceWindow, "DEVICE");
    wmove(gui->deviceWindow, getcury(gui->deviceWindow), DEVICE_BLOCKED_COL);
    wprintw(gui->deviceWindow, "BLOCKED QUEUE\n\n");
    
    wprintw(gui->deviceWindow, "Keyboard\n");
    wprintw(gui->deviceWindow, "Video\n");
    wprintw(gui->deviceWindow, "Disk\n");
    wprintw(gui->deviceWindow, "Shared mem 1\n");
    wprintw(gui->deviceWindow, "Shared mem 2\n");
    
    wrefresh(gui->deviceWindow);
    
    
    int logBorderWindowY = processBorderWindowY;
    int logBorderWindowX = width + 1;
    int logBorderWindowHeight = LINES - (2 * tbPadding);
    int logBorderWindowWidth = COLS - width - lrPadding - 1;
    WINDOW* logBorderWindow = newwin(logBorderWindowHeight, logBorderWindowWidth, logBorderWindowY, logBorderWindowX);
    wmove(logBorderWindow, 1, (logBorderWindowWidth * 0.5) - 2);
    wprintw(logBorderWindow, "LOG");
    wbkgd(logBorderWindow, COLOR_PAIR(2));
    wrefresh(logBorderWindow);
    gui->logWindow = newwin(logBorderWindowHeight - (2 * tbPadding) - 2, 
                            logBorderWindowWidth - (2 * lrPadding), 
                            logBorderWindowY + tbPadding + 2, 
                            logBorderWindowX + lrPadding);
    wbkgd(gui->logWindow, COLOR_PAIR(2));
    scrollok(gui->logWindow, TRUE);
    wmove(gui->logWindow, 0, 0);
    wrefresh(gui->logWindow);
    
    return gui;
}

void SchedSimGUI_updateProcessWindow(SchedSimGUI *gui) {
    pthread_mutex_lock(gui->updateMutex);
    int i;
    for (i = 0; i < gui->cpu->scheduler->processCount; i++) {
        PCB *pcb = gui->cpu->scheduler->processList[i];
        char *state;
        switch (pcb->state) {
            case PCB_STATE_RUNNING:
                wattron(gui->processWindow, COLOR_PAIR(3));
                state = "running";
                break;
                
            case PCB_STATE_READY:
                wattron(gui->processWindow, COLOR_PAIR(4));
                state = "ready  ";
                break;
                
            case PCB_STATE_BLOCKED:
                wattron(gui->processWindow, COLOR_PAIR(5));
                state = "blocked";
                break;
                
            default:
                break;
        }
        wmove(gui->processWindow, i + 1, 0);
        wprintw(gui->processWindow, "%d ", i);
        wmove(gui->processWindow, getcury(gui->processWindow), PROCESS_STATE_COL);
        wprintw(gui->processWindow, state);
        wmove(gui->processWindow, getcury(gui->processWindow), PROCESS_PERCENT_COL);
        wprintw(gui->processWindow, "%.0lf%%   ", ((float) pcb->nextStep / (float) pcb->proc->noSteps) * 100);
        switch (pcb->state) {
            case PCB_STATE_RUNNING:
                wattroff(gui->processWindow, COLOR_PAIR(3));
                break;
                
            case PCB_STATE_READY:
                wattroff(gui->processWindow, COLOR_PAIR(4));
                break;
                
            case PCB_STATE_BLOCKED:
                wattroff(gui->processWindow, COLOR_PAIR(5));
                
            default:
                break;
        }
    }
    wrefresh(gui->processWindow);
    pthread_mutex_unlock(gui->updateMutex);    
}

void SchedSimGUI_updateDeviceWindow(SchedSimGUI *gui) {
    pthread_mutex_lock(gui->updateMutex);
    
    PCBQueue *queue;
    
    int i;
    for (i = 0; i < 5; i++) {
        switch (i) {
            case 0:
                queue = gui->cpu->dvcKbd->blockedQueue;
                break;
                
            case 1:
                queue = gui->cpu->dvcVid->blockedQueue;
                break;
                
            case 2:
                queue = gui->cpu->dvcDisk->blockedQueue;
                break;
                
            case 3:
                queue = gui->cpu->sharedMemory[0]->mutexReadBlockedQueue;
                // TODO writeblockedqueue
                break;
                
            case 4:
                queue = gui->cpu->sharedMemory[1]->mutexReadBlockedQueue;
                // TODO
                break;
                
            default:
                break;
        }
        pthread_mutex_lock(queue->mod_mutex);
        int size = PCBQueue_getSize(queue);
        
        LinkedQueueNode *node = queue->head;
        int j;
        for (j = 0; j < size; j++) {
            wmove(gui->deviceWindow, 1, DEVICE_BLOCKED_COL + (j * 5));
            wprintw(gui->deviceWindow, "%d -> ", ((PCB *)node->data)->pid);
            node = node->next_node;
        }
        pthread_mutex_unlock(queue->mod_mutex);
    }
    
    pthread_mutex_unlock(gui->updateMutex);
}

void SchedSimGUI_printLogMessage(SchedSimGUI *gui, int logType, int id, const char *message) {
    pthread_mutex_lock(gui->updateMutex);
    switch (logType) {
        case LOG_TYPE_PROC:
            wprintw(gui->logWindow, "\n[PRO %d] %s", id, message);
            break;
            
        case LOG_TYPE_VID:
            wprintw(gui->logWindow, "\n[VIDEO] %s", message);
            break;
            
        case LOG_TYPE_DISK:
            wprintw(gui->logWindow, "\n[DISK ] %s", message);
            break;
            
        case LOG_TYPE_KBD:
            wprintw(gui->logWindow, "\n[KEYBD] %s", message);
            break;
            
        case LOG_TYPE_MEM:
            wprintw(gui->logWindow, "\n[MEM %d] %s", id, message);
            break;
            
        default:
            break;
    }
    
    wrefresh(gui->logWindow);
    pthread_mutex_unlock(gui->updateMutex);
}