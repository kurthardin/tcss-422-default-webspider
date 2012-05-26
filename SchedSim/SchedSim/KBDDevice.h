//
//  KBDDevice.h
//  SchedSim
//
//  Created by Kurt Hardin on 5/20/12.
//  Copyright (c) 2012 University of Washington at Tacoma. All rights reserved.
//

#ifndef SchedSim_KBDDevice_h
#define SchedSim_KBDDevice_h

#include <pthread.h>

#include "CPU.h"

typedef struct KBDDevice {
    LinkedBlockingQueue *inputBuffer;
    PCBQueue *blockedQueue;
    CPU *cpu;
    pthread_t *tid;
} KBDDevice;

KBDDevice * KBDDevice_init(CPU *);

#endif
