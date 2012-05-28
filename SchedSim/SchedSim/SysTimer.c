//
//  SysTimer.c
//  SchedSim
//
//  Created by Kurt Hardin on 5/20/12.
//  Copyright (c) 2012 University of Washington at Tacoma. All rights reserved.
//

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "SysTimer.h"
#include "Defines.h"

SysTimer * SysTimer_init(int quanta, CPU *cpu) {
    SysTimer *timer = malloc(sizeof(SysTimer));
    timer->counter = 0;
    timer->quanta = quanta;
    timer->cpu = cpu;
    timer->counterMutex = malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(timer->counterMutex, NULL);
    cpu->timer = (struct SysTimer *) timer;
    return timer;
}

void SysTimer_increment(SysTimer *timer) {
    pthread_mutex_lock(timer->counterMutex);
    timer->counter++;
    if (timer->counter == timer->quanta) {
        CPU_signalInterrupt(timer->cpu, Interrupt_init(INTERRUPT_TYPE_TIMER, NULL));
    }
    pthread_mutex_unlock(timer->counterMutex);
}

void SysTimer_reset(SysTimer *timer) {
    pthread_mutex_lock(timer->counterMutex);
    timer->counter = 0;
    pthread_mutex_unlock(timer->counterMutex);
}