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

Interrupt * Interrupt_init(int type, void *src) {
    Interrupt *interrupt = malloc(sizeof(Interrupt));
    interrupt->type = type;
    interrupt->src = src;
    return interrupt;
}


void checkForInterrupt(CPU *);

CPU * CPU_init() {
    CPU *cpu = malloc(sizeof(CPU));
    int i;
    for (i = 0; i < INTERRUPT_PRIORITY_COUNT; i++) {
        cpu->interruptQueues[i] = LinkedBlockingQueue_init();
    }
    cpu->scheduler = Scheduler_init(cpu);
    cpu->dvcDisk = IODevice_init("Disk", cpu);
    cpu->dvcVid = IODevice_init("Video", cpu);
    cpu->dvcKbd = KBDDevice_init(cpu);
    cpu->sharedMemory = SharedMemory_init();
    return cpu;
}

void CPU_signalInterrupt(CPU *cpu, Interrupt *interrupt) {
    LinkedBlockingQueue_enqueue(cpu->interruptQueues[interrupt->type], interrupt);
}

void CPU_step(CPU *cpu) {
    
    // Do stuff...
    
    checkForInterrupt(cpu);
}

void checkForInterrupt(CPU *cpu) {
    Interrupt *interrupt = NULL;
    int i;
    for (i = 0; i < INTERRUPT_PRIORITY_COUNT; i++) {
        interrupt = LinkedBlockingQueue_dequeue(cpu->interruptQueues[i]);
        if (interrupt != NULL) {
            Scheduler_schedule(cpu->scheduler, interrupt->src, interrupt->type);
            break;
        }
    }
}