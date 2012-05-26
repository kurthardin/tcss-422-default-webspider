//
//  Scheduler.h
//  SchedSim
//
//  Created by Kurt Hardin on 5/17/12.
//  Copyright (c) 2012 University of Washington at Tacoma. All rights reserved.
//

#ifndef SchedSim_Scheduler_h
#define SchedSim_Scheduler_h

#include "PCB.h"
#include "PCBQueue.h"
#include "CPU.h"
#include "IODevice.h"

#define MAX_PROCESSES   10

typedef struct Scheduler {
    CPU *cpu;
    PCB **processList;
    int processCount;
    PCBQueue *readyQueue;
} Scheduler;

Scheduler * Scheduler_init(CPU *);

void Scheduler_newProcess(Scheduler *, Process *);

void Scheduler_handleSystemRequest(Scheduler *);
void Scheduler_handleInterrupt(Scheduler *, PCB *, int);

#endif
