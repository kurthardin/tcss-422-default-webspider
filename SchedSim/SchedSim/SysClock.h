//
//  SysClock.h
//  SchedSim
//
//  Created by Kurt Hardin on 5/27/12.
//  Copyright (c) 2012 University of Washington at Tacoma. All rights reserved.
//

#ifndef SchedSim_SysClock_h
#define SchedSim_SysClock_h

#include <pthread.h>

#include "CPU.h"
#include "SysTimer.h"

typedef struct {
    CPU *cpu;
    SysTimer *timer;
    pthread_t *tid;
} SysClock;

SysClock * SysClock_init(CPU *, SysTimer *);

#endif
