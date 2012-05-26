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
    sharedMem->mode = SHARED_MEM_MODE_WRITE;
    sharedMem->owner = NULL;
    sharedMem->mutexReadBlockedQueue = PCBQueue_init();
    sharedMem->mutexWriteBlockedQueue = PCBQueue_init();
    return sharedMem;
}