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

SharedMemory * SharedMemory_init() {
    SharedMemory* sharedMem = malloc(sizeof(SharedMemory));
    sharedMem->mutexOwner = NULL;
    sharedMem->mutexBlockedQueue = pcb_queue_init();
    return sharedMem;
}

int SharedMemory_read(SharedMemory* sharedMemory) {
    // TODO lock
    return sharedMemory->data;
    // TODO unlock
}

void SharedMemory_write(SharedMemory* sharedMemory, int new_data) {
    // TODO lock
    sharedMemory->data = new_data;
    // TODO unlock
}