//
//  GUI.h
//  SchedSim
//
//  Created by Kurt Hardin on 5/28/12.
//  Copyright (c) 2012 University of Washington at Tacoma. All rights reserved.
//

#ifndef SchedSim_GUI_h
#define SchedSim_GUI_h

#include <ncurses.h>
#include <pthread.h>

#include "CPU.h"

typedef struct {
    WINDOW* processWindow;
    WINDOW* deviceWindow;
    WINDOW* logWindow;
    CPU *cpu;
    pthread_mutex_t *updateMutex;
} SchedSimGUI;

SchedSimGUI * SchedSimGUI_init(CPU *);

void SchedSimGUI_updateProcessWindow(SchedSimGUI *);
void SchedSimGUI_updateDeviceWindow(SchedSimGUI *);
void SchedSimGUI_printLogMessage(SchedSimGUI *, const char *);

#endif
