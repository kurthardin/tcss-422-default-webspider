//
//  KBDDevice.h
//  SchedSim
//
//  Created by Kurt Hardin on 5/20/12.
//  Copyright (c) 2012 University of Washington at Tacoma. All rights reserved.
//

#include <pthread.h>

#ifndef SchedSim_KBDDevice_h
#define SchedSim_KBDDevice_h

#include "CPU.h"

typedef struct {
    char key;
    CPU *cpu;
    pthread_t *tid;
} KBDDevice;

KBDDevice * KBDDevice_init(CPU *);

#endif
