//
//  SysTimer.h
//  SchedSim
//
//  Created by Kurt Hardin on 5/17/12.
//  Copyright (c) 2012 University of Washington at Tacoma. All rights reserved.
//

#include <pthread.h>

#include "CPU.h"

#ifndef SchedSim_SysTimer_h
#define SchedSim_SysTimer_h

typedef struct {
    int delay;
    CPU *cpu;
    pthread_t *tid;
} SysTimer;

SysTimer * SysTimer_init(int delay, CPU *);

#endif
