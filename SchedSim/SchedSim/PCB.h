//  Team: default
//  Names: Bartholomew, Dan
//         Choe, Ju
//         Hardin, Kurt
//
//  PCB.h
//  SchedSim
//
//  Created by Kurt Hardin on 5/17/12.
//  Copyright (c) 2012 University of Washington at Tacoma. All rights reserved.
//

#ifndef SchedSim_PCB_h
#define SchedSim_PCB_h

#include "Process.h"

#define PCB_STATE_RUNNING   0
#define PCB_STATE_READY     1
#define PCB_STATE_INT       2
#define PCB_STATE_BLOCKED   3

#define PCB_WAITING_ON_NONE             0
#define PCB_WAITING_ON_SHARED_MEM_READ  1
#define PCB_WAITING_ON_SHARED_MEM_WRITE 2
#define PCB_WAITING_ON_KEYBOARD_INPUT   3

typedef struct {
	int	pid;
	int nextStep;		// this is the step count that the CPU had gotten to when this process was prempted (like a PC register value)
	int state;			// e.g. 0=running, 1=ready, 2=interrupted, 3=blocked
	Process *proc;      // pointer to the actual process
	int waitingOn;		// which queue is it in if it is waiting on something (blocked)
	int owns;			// which mutex lock does it own
    int mem_ref;        // reference to which shared memory if needed
} PCB;

PCB * PCB_init(int, Process *);

#endif
