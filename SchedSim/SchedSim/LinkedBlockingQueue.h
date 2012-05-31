//
//  LinkedQueue.h
//  SchedSim
//
//  Created by Kurt Hardin on 5/17/12.
//  Copyright (c) 2012 University of Washington at Tacoma. All rights reserved.
//

#include <pthread.h>

#include "LinkedQueueNode.h"

#ifndef SchedSim_LinkedQueue_h
#define SchedSim_LinkedQueue_h

typedef struct {
    LinkedQueueNode *head;
    LinkedQueueNode *tail;
    int size;
    pthread_mutex_t *mod_mutex;
    pthread_cond_t *mod_sig;
} LinkedBlockingQueue;

LinkedBlockingQueue* LinkedBlockingQueue_init();
void LinkedBlockingQueue_enqueue(LinkedBlockingQueue *, void *);
void * LinkedBlockingQueue_dequeue(LinkedBlockingQueue *);
void * LinkedBlockingQueue_blockingDequeue(LinkedBlockingQueue *);
void * LinkedBlockingQueue_blockingPeek(LinkedBlockingQueue *);
int LinkedBlockingQueue_getSize(LinkedBlockingQueue *);

#endif
