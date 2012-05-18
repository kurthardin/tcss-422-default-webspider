//
//  IODevice.h
//  SchedSim
//
//  Created by Kurt Hardin on 5/17/12.
//  Copyright (c) 2012 University of Washington at Tacoma. All rights reserved.
//

#include <pthread.h>

#include "PCB.h"
#include "PCBQueue.h"

#ifndef SchedSim_IODevice_h
#define SchedSim_IODevice_h

typedef struct {
    char* type;
    PCBQueue* blocked_queue;
} IODevice;

IODevice* io_device_init(char*);
void io_device_service(IODevice*);

#endif