//  Team: default
//  Names: Bartholomew, Dan
//         Choe, Ju
//         Hardin, Kurt
//
//  SysTimer.h
//  SchedSim
//
//  Created by Kurt Hardin on 5/17/12.
//  Copyright (c) 2012 University of Washington at Tacoma. All rights reserved.
//

#ifndef SchedSim_SysTimer_h
#define SchedSim_SysTimer_h

#include <pthread.h>

#include "CPU.h"

typedef struct {
    int counter;
    int quanta;
    CPU *cpu;
    pthread_mutex_t* counterMutex;
} SysTimer;

SysTimer * SysTimer_init(int delay, CPU *);
void SysTimer_increment(SysTimer *);
void SysTimer_reset(SysTimer *);

#endif
