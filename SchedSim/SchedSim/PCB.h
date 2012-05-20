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

#define PCB_STATE_RUNNING   0
#define PCB_STATE_READY     1
#define PCB_STATE_INT       2
#define PCB_STATE_BLOCKED   3

typedef struct {
	int	pid;
	int nextStep;		// this is the step count that the CPU had gotten to when this process was prempted (like a PC register value)
	int state;			// e.g. 0=running, 1=ready, 2=interrupted, 3=blocked
	Process *proc;      // pointer to the actual process
	int waitingOn;		// which queue is it in if it is waiting on something (blocked)
	int owns;			// which mutex lock does it own
} PCB;

PCB * PCB_init(int, Process *);

#endif
