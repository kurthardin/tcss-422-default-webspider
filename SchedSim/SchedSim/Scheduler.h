//
//  Scheduler.h
//  SchedSim
//
//  Created by Kurt Hardin on 5/17/12.
//  Copyright (c) 2012 University of Washington at Tacoma. All rights reserved.
//

#include "PCBQueue.h"

#ifndef SchedSim_Scheduler_h
#define SchedSim_Scheduler_h

typedef struct {
    PCBQueue *readyQueue;
} Scheduler;

Scheduler * Scheduler_init();

#endif
