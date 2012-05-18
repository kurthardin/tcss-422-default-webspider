//
//  LinkedQueue.c
//  SchedSim
//
//  Created by Kurt Hardin on 5/17/12.
//  Copyright (c) 2012 University of Washington at Tacoma. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

#include "LinkedBlockingQueue.h"

LinkedBlockingQueue* linked_blocking_queue_init() {
    LinkedBlockingQueue* queue = malloc(sizeof(LinkedBlockingQueue));
    queue->head = NULL;
    queue->tail = NULL;
    queue->size = 0;
    pthread_mutex_init(queue->mod_mutex, NULL);
    pthread_cond_init(queue->mod_sig, NULL);
    return queue;
}

void linked_blocking_queue_enqueue(LinkedBlockingQueue* queue, void* data) {
    
    LinkedQueueNode* new_tail = linked_queue_node_init(data);
    
    pthread_mutex_lock(queue->mod_mutex);
    if (queue->head == NULL) {
        queue->head = new_tail;
    }
    
    if (queue->tail != NULL) {
        queue->tail->next_node = new_tail;
    }
    
    queue->tail = new_tail;
    queue->size++;
    pthread_mutex_unlock(queue->mod_mutex);
    pthread_cond_signal(queue->mod_sig);
}

void* linked_blocking_queue_dequeue(LinkedBlockingQueue* queue) {
    
    pthread_mutex_lock(queue->mod_mutex);
    while (queue->head == NULL) {
        pthread_cond_wait(queue->mod_sig, queue->mod_mutex);
    }
    
    LinkedQueueNode* old_head = queue->head;
    queue->head = old_head->next_node;
    queue->size--;
    pthread_mutex_unlock(queue->mod_mutex);
    
    return old_head->data;
}