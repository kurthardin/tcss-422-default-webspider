//  Team: default
//  Names: Bartholomew, Dan
//         Choe, Ju
//         Hardin, Kurt
//
//  SharedMem.h
//  SchedSim
//
//  Created by Ju Choe on 5/20/12.
//  Copyright (c) 2012 University of Washington at Tacoma. All rights reserved.
//

#ifndef SchedSim_SharedMem_h
#define SchedSim_SharedMem_h

#define SHARED_MEM_MODE_READ    1
#define SHARED_MEM_MODE_WRITE   2

#include <pthread.h>

#include "PCB.h"
#include "PCBQueue.h"

typedef struct {
    int data;
    int mode;
    PCB *owner;
    PCBQueue *mutexReadBlockedQueue;
    PCBQueue *mutexWriteBlockedQueue;
} SharedMemory;

SharedMemory * SharedMemory_init();

#endif
