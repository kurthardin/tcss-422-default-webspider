//
//  SharedMem.c
//  SchedSim
//
//  Created by Ju Choe on 5/20/12.
//  Copyright (c) 2012 University of Washington at Tacoma. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

#include "SharedMem.h"

SharedMemory* shared_memory_init() {
    SharedMemory* sharedMem = malloc(sizeof(SharedMemory));
    pthread_mutex_init(sharedMem->mod_mutex, NULL);
    pthread_cond_init(sharedMem->mod_sig, NULL);
    return sharedMem;
}

int SharedMemory_read(SharedMemory* sharedMemory) {
    pthread_mutex_lock(sharedMemory->mod_mutex);
    pthread_cond_wait(sharedMemory->mod_sig, sharedMemory->mod_mutex);
    return sharedMemory->data;
}

void SharedMemory_write(SharedMemory* sharedMemory, int new_data) {
    pthread_mutex_lock(sharedMemory->mod_mutex);
    sharedMemory->data = new_data;
    pthread_mutex_unlock(sharedMemory->mod_mutex);
}