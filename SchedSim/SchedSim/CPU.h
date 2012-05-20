//
//  CPU.h
//  SchedSim
//
//  Created by Kurt Hardin on 5/17/12.
//  Copyright (c) 2012 University of Washington at Tacoma. All rights reserved.
//

#ifndef SchedSim_Interrupt_h
#define SchedSim_Interrupt_h

#define INTERRUPT_TYPE_IO       1
#define INTERRUPT_TYPE_KBD      2
#define INTERRUPT_TYPE_TIMER    3
// Add more as needed

typedef struct {
    int type;
    //int priority;
    void *src;
} Interrupt;

Interrupt Interrupt_make(int type/*, int priority*/, void *);

#endif






#include "LinkedBlockingQueue.h"
#include "IODevice.h"
#include "KBDDevice.h"
#include "SharedMem.h"

#ifndef SchedSim_CPU_h
#define SchedSim_CPU_h

typedef struct {
    LinkedBlockingQueue *interruptQueues[10];
    IODevice *dvcDisk;
    IODevice *dvcVid;
    KBDDevice *dvcKbd;
    SharedMemory *sharedMemory;
} CPU;

void CPU_signalInterrupt(CPU *, Interrupt);

#endif