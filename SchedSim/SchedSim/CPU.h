//
//  CPU.h
//  SchedSim
//
//  Created by Kurt Hardin on 5/17/12.
//  Copyright (c) 2012 University of Washington at Tacoma. All rights reserved.
//

#ifndef SchedSim_Interrupt_h
#define SchedSim_Interrupt_h

#define INTERRUPT_PRIORITY_COUNT 3

#define INTERRUPT_TYPE_TIMER    0
#define INTERRUPT_TYPE_IO       1
#define INTERRUPT_TYPE_KBD      2
// Add more as needed

typedef struct {
    int type;
    void *src;
} Interrupt;

Interrupt * Interrupt_init(int type, void *);

#endif


#ifndef SchedSim_CPU_h
#define SchedSim_CPU_h

#include <pthread.h>

#include "LinkedBlockingQueue.h"
#include "SharedMem.h"

struct Scheduler;
struct IODevice;
struct KBDDevice;

typedef struct {
    LinkedBlockingQueue *interruptQueues[INTERRUPT_PRIORITY_COUNT];
    struct Scheduler *scheduler;
    struct IODevice *dvcDisk;
    struct IODevice *dvcVid;
    struct KBDDevice *dvcKbd;
    SharedMemory *sharedMemory;
    pthread_mutex_t* modMutex;
} CPU;

void CPU_signalInterrupt(CPU *, Interrupt *);
void CPU_step(CPU *);

#endif