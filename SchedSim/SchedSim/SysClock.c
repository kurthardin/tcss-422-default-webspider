//
//  SysClock.c
//  SchedSim
//
//  Created by Kurt Hardin on 5/27/12.
//  Copyright (c) 2012 University of Washington at Tacoma. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "SysClock.h"
#include "Defines.h"

void * SysClock_run(void *);

SysClock * SysClock_init(CPU *cpu, SysTimer *timer) {
    SysClock *clock = malloc(sizeof(SysClock));
    clock->cpu = cpu;
    clock->timer = timer;
    clock->tid = malloc(sizeof(pthread_t));
    pthread_create(clock->tid, NULL, SysClock_run, clock);
    return clock;
}

void * SysClock_run(void *arg) {
    SysClock *clock = (SysClock *)arg;
    while (CPU_isRunning(clock->cpu)) {
        CPU_step(clock->cpu);
        SysTimer_increment(clock->timer);
        usleep(1000);
    }
    return EXIT_SUCCESS;
}