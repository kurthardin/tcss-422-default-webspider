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

PCBQueue* PCBQueue_init();
void PCBQueue_enqueue(PCBQueue *, PCB *);
PCB* PCBQueue_blockingDequeue(PCBQueue *);
PCB* PCBQueue_dequeue(PCBQueue *);
PCB* PCBQueue_blockingPeek(PCBQueue *);
int PCBQueue_getSize(PCBQueue *);

#endif
