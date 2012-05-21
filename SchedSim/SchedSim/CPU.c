//
//  CPU.c
//  SchedSim
//
//  Created by Kurt Hardin on 5/17/12.
//  Copyright (c) 2012 University of Washington at Tacoma. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

#include "CPU.h"
#include "IODevice.h"
#include "KBDDevice.h"

Interrupt Interrupt_make(int type, void *src) {
    Interrupt interrupt = {type, src};
    return interrupt;
}

CPU * CPU_init() {
    CPU *cpu = malloc(sizeof(CPU));
//    sched->dvcDisk = IODevice_init("Disk", cpu);
//    sched->dvcVid = IODevice_init("Video", cpu);
//    sched->dvcKbd = KBDDevice_init(cpu);
//    sched->sharedMemory = SharedMemory_init();
    return cpu;
}

void CPU_signalInterrupt(CPU *cpu, Interrupt interrupt) {
    // STUB
}