//
//  PCBQueue.h
//  SchedSim
//
//  Created by Kurt Hardin on 5/17/12.
//  Copyright (c) 2012 University of Washington at Tacoma. All rights reserved.
//

#ifndef SchedSim_PCBQueue_h
#define SchedSim_PCBQueue_h

#include "LinkedBlockingQueue.h"
#include "PCB.h"

typedef LinkedBlockingQueue PCBQueue;

PCBQueue* pcb_queue_init();
void PCBQueue_enqueue(PCBQueue*, PCB*);
PCB* PCBQueue_dequeue(PCBQueue*);

#endif
