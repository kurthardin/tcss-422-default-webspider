//  Team: default
//  Names: Bartholomew, Dan
//         Choe, Ju
//         Hardin, Kurt
//
//  PCB.c
//  SchedSim
//
//  Created by Kurt Hardin on 5/20/12.
//  Copyright (c) 2012 University of Washington at Tacoma. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

#include "PCB.h"

// Initializes a PCB
PCB * PCB_init(int pid, Process *proc) {
    PCB *pcb = malloc(sizeof(PCB));
    pcb->pid = pid;
    pcb->proc = proc;
    pcb->nextStep = 0;
    pcb->state = PCB_STATE_READY;
    pcb->owns = 0;
    pcb->waitingOn = PCB_WAITING_ON_NONE;
    pcb->mem_ref = -1;
    return pcb;
}