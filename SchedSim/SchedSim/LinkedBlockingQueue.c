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

LinkedBlockingQueue* LinkedBlockingQueue_init() {
    LinkedBlockingQueue* queue = malloc(sizeof(LinkedBlockingQueue));
    queue->head = NULL;
    queue->tail = NULL;
    queue->size = 0;
    queue->mod_mutex = malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(queue->mod_mutex, NULL);
    queue->mod_sig = malloc(sizeof(pthread_cond_t));
    pthread_cond_init(queue->mod_sig, NULL);
    return queue;
}

void LinkedBlockingQueue_enqueue(LinkedBlockingQueue *queue, void *data) {
    
    LinkedQueueNode *new_tail = LinkedQueueNode_init(data);
    
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

void * dequeue(LinkedBlockingQueue *queue) {
    LinkedQueueNode* old_head = queue->head;
    if (old_head != NULL) {
        queue->head = old_head->next_node;
        queue->size--;
    }
    
    void *data = NULL;
    if (old_head != NULL) {
        data = old_head->data;
        free(old_head);
    }
    
    return data;
}

void * LinkedBlockingQueue_dequeue(LinkedBlockingQueue *queue) {
    
    pthread_mutex_lock(queue->mod_mutex);
    void *data = dequeue(queue);
    pthread_mutex_unlock(queue->mod_mutex);
    
    return data;
}

void * LinkedBlockingQueue_blockingDequeue(LinkedBlockingQueue *queue) {
    pthread_mutex_lock(queue->mod_mutex);
    while (queue->head == NULL) {
        pthread_cond_wait(queue->mod_sig, queue->mod_mutex);
    }
    void *data = dequeue(queue);
    pthread_mutex_unlock(queue->mod_mutex);
    return data;
}

void * LinkedBlockingQueue_blockingPeek(LinkedBlockingQueue *queue) {
    pthread_mutex_lock(queue->mod_mutex);
    while (queue->head == NULL) {
        pthread_cond_wait(queue->mod_sig, queue->mod_mutex);
    }
    void *data = queue->head->data;
    pthread_mutex_unlock(queue->mod_mutex);
    return data;
}

int LinkedBlockingQueue_getSize(LinkedBlockingQueue *queue) {
    pthread_mutex_lock(queue->mod_mutex);
    int theSize = queue->size;
    pthread_mutex_unlock(queue->mod_mutex);
    return theSize;
}