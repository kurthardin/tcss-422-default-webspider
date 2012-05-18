//
//  LinkedQueue.h
//  SchedSim
//
//  Created by Kurt Hardin on 5/17/12.
//  Copyright (c) 2012 University of Washington at Tacoma. All rights reserved.
//

#import <pthread.h>

#import "LinkedQueueNode.h"

#ifndef SchedSim_LinkedQueue_h
#define SchedSim_LinkedQueue_h

typedef struct {
    LinkedQueueNode* head;
    LinkedQueueNode* tail;
    int size;
    pthread_mutex_t* mod_mutex;
    pthread_cond_t* mod_sig;
} LinkedBlockingQueue;

LinkedBlockingQueue* linked_blocking_queue_init();
void linked_blocking_queue_enqueue(LinkedBlockingQueue*, void*);
void* linked_blocking_queue_dequeue(LinkedBlockingQueue*);

#endif
