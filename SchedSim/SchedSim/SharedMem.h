//
//  SharedMem.h
//  SchedSim
//
//  Created by Ju Choe on 5/20/12.
//  Copyright (c) 2012 University of Washington at Tacoma. All rights reserved.
//

#import <pthread.h>
#include <stdio.h>

#ifndef SchedSim_SharedMem_h
#define SchedSim_SharedMem_h

typedef struct {
    int data;
    pthread_mutex_t* mod_mutex;
    pthread_cond_t* mod_sig;
} SharedMemory;

int SharedMemory_read(SharedMemory*);
void SharedMemory_write(SharedMemory*, int);

#endif
