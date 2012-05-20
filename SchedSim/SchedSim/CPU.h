//
//  CPU.h
//  SchedSim
//
//  Created by Kurt Hardin on 5/17/12.
//  Copyright (c) 2012 University of Washington at Tacoma. All rights reserved.
//

#include "LinkedBlockingQueue.h"

#ifndef SchedSim_Interrupt_h
#define SchedSim_Interrupt_h

#define INTERRUPT_TYPE_IO   1
#define INTERRUPT_TYPE_XXX  2   // Add more as needed

typedef struct {
    int type;
    //int priority;
} Interrupt;

Interrupt Interrupt_make(int type/*, int priority*/);

#endif


#ifndef SchedSim_CPU_h
#define SchedSim_CPU_h

typedef struct {
    LinkedBlockingQueue *interruptQueues[10];
} CPU;

void CPU_signalInterrupt(CPU *, Interrupt);

#endif