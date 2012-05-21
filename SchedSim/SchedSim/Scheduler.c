//
//  Scheduler.c
//  SchedSim
//
//  Created by Kurt Hardin on 5/20/12.
//  Copyright (c) 2012 University of Washington at Tacoma. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

#include "Scheduler.h"

Scheduler * Scheduler_init(CPU *cpu) {
    Scheduler *sched = malloc(sizeof(Scheduler));
    sched->processList = malloc(sizeof(PCB)*MAX_PROCESSES);
    sched->readyQueue = PCBQueue_init();
    return sched;
}

void Scheduler_schedule(Scheduler *scheduler, void *src, int type) {
    // STUB
}