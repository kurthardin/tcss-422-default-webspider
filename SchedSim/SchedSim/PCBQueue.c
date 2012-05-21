//
//  PCBQueue.c
//  SchedSim
//
//  Created by Kurt Hardin on 5/17/12.
//  Copyright (c) 2012 University of Washington at Tacoma. All rights reserved.
//

#include <stdio.h>

#include "PCBQueue.h"

PCBQueue* PCBQueue_init() {
    return LinkedBlockingQueue_init();
}

void PCBQueue_enqueue(PCBQueue* queue, PCB* pcb) {
    LinkedBlockingQueue_enqueue(queue, (void*) pcb);
}

PCB* PCBQueue_dequeue(PCBQueue* queue) {
    return (PCB*) LinkedBlockingQueue_blockingDequeue(queue);
}

int PCBQueue_getSize(PCBQueue * queue) {
    return LinkedBlockingQueue_getSize(queue);
}