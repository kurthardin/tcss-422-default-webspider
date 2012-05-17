//
//  PCBQueue.h
//  SchedSim
//
//  Created by Kurt Hardin on 5/17/12.
//  Copyright (c) 2012 University of Washington at Tacoma. All rights reserved.
//

#include "LinkedQueue.h"
#include "PCB.h"

#ifndef SchedSim_PCBQueue_h
#define SchedSim_PCBQueue_h

typedef LinkedQueue PCBQueue;

PCBQueue* pcb_queue_init();
void pcb_queue_enqueue(PCBQueue*, PCB*);
PCB* pcb_queue_dequeue(PCBQueue*);

#endif
