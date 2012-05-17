//
//  PCB.h
//  SchedSim
//
//  Created by Kurt Hardin on 5/17/12.
//  Copyright (c) 2012 University of Washington at Tacoma. All rights reserved.
//

#include "Process.h"

#ifndef SchedSim_PCB_h
#define SchedSim_PCB_h

typedef struct {
	int	pid;
	int next_step;		// this is the step count that the CPU had gotten to when this process was prempted (like a PC register value)
	int state;			// e.g. 0=running, 1=ready, 2=interrupted, 3=blocked
	Process proc;       // pointer to the actual process
	int waiting_on;		// which queue is it in if it is waiting on something (blocked)
	int owns;			// which mutex lock does it own
    
    // anything else you need
} PCB;

#endif
