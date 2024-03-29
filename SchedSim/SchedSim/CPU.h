//  Team: default
//  Names: Bartholomew, Dan
//         Choe, Ju
//         Hardin, Kurt
//
//  CPU.h
//  SchedSim
//
//  Created by Kurt Hardin on 5/17/12.
//  Copyright (c) 2012 University of Washington at Tacoma. All rights reserved.
//

#include "PCB.h"

#ifndef SchedSim_Interrupt_h
#define SchedSim_Interrupt_h

#define INTERRUPT_PRIORITY_COUNT 3

#define INTERRUPT_TYPE_TIMER     0
#define INTERRUPT_TYPE_IO        1
#define INTERRUPT_TYPE_KBD       2
// Add more as needed

typedef struct {
    int type;
    PCB *src;
} Interrupt;

Interrupt * Interrupt_init(int type, PCB *);

#endif


#ifndef SchedSim_CPU_h
#define SchedSim_CPU_h

#include <pthread.h>

#include "LinkedBlockingQueue.h"
#include "SharedMem.h"
#include "Defines.h"

struct SysTimer;
struct Scheduler;
struct IODevice;
struct KBDDevice;
struct SchedSimGUI;

typedef struct {
    int ip;
    PCB *runningProcess;
    struct SysTimer *timer;
    LinkedBlockingQueue *interruptQueues[INTERRUPT_PRIORITY_COUNT];
    struct Scheduler *scheduler;
    struct IODevice *dvcDisk;
    struct IODevice *dvcVid;
    struct KBDDevice *dvcKbd;
    int sharedMemoryCount;
    SharedMemory **sharedMemory;
    struct SchedSimGUI *gui;
    boolean isRunning;
    pthread_mutex_t* modMutex;
} CPU;

CPU * CPU_init(int);

void CPU_signalInterrupt(CPU *, Interrupt *);
void CPU_systemRequest(CPU *, int);
void CPU_getKeyFromKeyboard(CPU *);

void CPU_step(CPU *);

boolean CPU_isRunning(CPU *);
void CPU_setIsRunning(CPU *, boolean);

#endif