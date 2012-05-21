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
#include "Scheduler.h"
#include "IODevice.h"
#include "KBDDevice.h"

Interrupt Interrupt_make(int type, void *src) {
    Interrupt interrupt = {type, src};
    return interrupt;
}

CPU * CPU_init() {
    CPU *cpu = malloc(sizeof(CPU));
    int i;
    for (i = 0; i < 10; i++) {
        cpu->interruptQueues[i] = linked_blocking_queue_init();
    }
    cpu->scheduler = Scheduler_init(cpu);
    cpu->dvcDisk = IODevice_init("Disk", cpu);
    cpu->dvcVid = IODevice_init("Video", cpu);
    cpu->dvcKbd = KBDDevice_init(cpu);
    cpu->sharedMemory = SharedMemory_init();
    return cpu;
}

void CPU_signalInterrupt(CPU *cpu, Interrupt interrupt) {
    // STUB
}