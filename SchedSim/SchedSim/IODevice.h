//  Team: default
//  Names: Bartholomew, Dan
//         Choe, Ju
//         Hardin, Kurt
//
//  IODevice.h
//  SchedSim
//
//  Created by Kurt Hardin on 5/17/12.
//  Copyright (c) 2012 University of Washington at Tacoma. All rights reserved.
//

#ifndef SchedSim_IODevice_h
#define SchedSim_IODevice_h

#include <pthread.h>

#include "CPU.h"
#include "PCB.h"
#include "PCBQueue.h"

typedef struct IODevice {
    int type;
    PCBQueue *blockedQueue;
    CPU *cpu;
    pthread_t *tid;
} IODevice;

IODevice * IODevice_init(int, CPU *);

#endif
