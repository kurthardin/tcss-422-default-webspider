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

#include "CPU.h"

typedef struct {
    WINDOW* processStatusWindow;
    WINDOW* deviceStatusWindow;
    WINDOW* logWindow;
} SchedSimGUI;

SchedSimGUI * SchedSimGUI_init(CPU *);

#endif
