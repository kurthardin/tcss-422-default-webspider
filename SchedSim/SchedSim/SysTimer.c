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

void * SysTimer_run(void *);

SysTimer * SysTimer_init(int delay, CPU *cpu) {
    SysTimer *timer = malloc(sizeof(SysTimer));
    timer->delay = delay;
    timer->cpu = cpu;
    timer->tid = malloc(sizeof(pthread_t));
    pthread_create(timer->tid, NULL, SysTimer_run, timer);
    return timer;
}

void * SysTimer_run(void *arg) {
    
    SysTimer *timer = (SysTimer *)arg;
    
    while (1) {
        sleep(timer->delay);
        CPU_signalInterrupt(timer->cpu, Interrupt_make(INTERRUPT_TYPE_TIMER, timer));
    }
    
    return EXIT_SUCCESS;
}