//
//  LinkedQueue.h
//  SchedSim
//
//  Created by Kurt Hardin on 5/17/12.
//  Copyright (c) 2012 University of Washington at Tacoma. All rights reserved.
//

#import "LinkedQueueNode.h"

#ifndef SchedSim_LinkedQueue_h
#define SchedSim_LinkedQueue_h

typedef struct {
    LinkedQueueNode* head;
    LinkedQueueNode* tail;
    int size;
} LinkedQueue;

LinkedQueue* linked_queue_init();
void linked_queue_enqueue(LinkedQueue*, void*);
void* linked_queue_dequeue(LinkedQueue*);

#endif
