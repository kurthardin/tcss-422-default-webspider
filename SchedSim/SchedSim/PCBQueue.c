//
//  PCBQueue.c
//  SchedSim
//
//  Created by Kurt Hardin on 5/17/12.
//  Copyright (c) 2012 University of Washington at Tacoma. All rights reserved.
//

#include <stdio.h>

#include "PCBQueue.h"

PCBQueue* pcb_queue_init() {
    return linked_queue_init();
}

void pcb_queue_enqueue(PCBQueue* queue, PCB* pcb) {
    linked_queue_enqueue(queue, linked_queue_node_init((void*)pcb));
}

PCB* pcb_queue_dequeue(PCBQueue* queue) {
    return (PCB*) linked_queue_dequeue(queue);
}