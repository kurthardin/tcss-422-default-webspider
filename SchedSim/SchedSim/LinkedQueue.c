//
//  LinkedQueue.c
//  SchedSim
//
//  Created by Kurt Hardin on 5/17/12.
//  Copyright (c) 2012 University of Washington at Tacoma. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

#include "LinkedQueue.h"

LinkedQueue* linked_queue_init() {
    LinkedQueue* queue = malloc(sizeof(LinkedQueue));
    queue->head = NULL;
    queue->tail = NULL;
    queue->size = 0;
    return queue;
}

void linked_queue_enqueue(LinkedQueue* queue, void* data) {
    
    LinkedQueueNode* new_tail = linked_queue_node_init(data);
    
    if (queue->head == NULL) {
        queue->head = new_tail;
    }
    
    if (queue->tail != NULL) {
        queue->tail->next_node = new_tail;
    }
    
    queue->tail = new_tail;
    queue->size++;
}

void* linked_queue_dequeue(LinkedQueue* queue) {
    
    LinkedQueueNode* old_head = queue->head;
    if (old_head != NULL) {
        queue->head = old_head->next_node;
        queue->size--;
    }
    
    return old_head->data;
}