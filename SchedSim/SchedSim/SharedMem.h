//
//  SharedMem.h
//  SchedSim
//
//  Created by Ju Choe on 5/20/12.
//  Copyright (c) 2012 University of Washington at Tacoma. All rights reserved.
//

#ifndef SchedSim_SharedMem_h
#define SchedSim_SharedMem_h

#import <pthread.h>

#include "PCB.h"
#include "PCBQueue.h"

typedef struct {
    int data;
    PCB *mutexOwner;
    PCBQueue *mutexBlockedQueue;
} SharedMemory;

SharedMemory * SharedMemory_init();

int SharedMemory_read(SharedMemory*);
void SharedMemory_write(SharedMemory*, int);

#endif
