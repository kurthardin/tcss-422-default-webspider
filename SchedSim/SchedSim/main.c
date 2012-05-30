//
//  main.c
//  SchedSim
//
//  Created by Kurt Hardin on 5/9/12.
//  Copyright (c) 2012 University of Washington at Tacoma. All rights reserved.
//

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "Scheduler.h"
#include "SysClock.h"
#include "KBDDevice.h"
#include "GUI.h"

void createIOBoundProcess(CPU *cpu) {
    int stepCount = 1 + (rand() % 5000);
    int ioRequestCount = 1 + (rand() % (stepCount / (10 + (rand() % 15))));
    RequestType *ioRequests = malloc(sizeof(RequestType) * ioRequestCount);
    int j;
    for (j = 0; j < ioRequestCount; j++) {
        RequestType request = {rand() % ioRequestCount, 3 + (rand() % 2)};
        ioRequests[j] = request;
    }
    Process *ioProc = Process_init(stepCount, ioRequestCount, ioRequests);
    Scheduler_newProcess(cpu->scheduler, ioProc, -1);
}

void createProdConsPair(CPU *cpu, int sharedMemIndex) {
    int prodStepCount = 1 + (rand() % 5000);
    int prodRequestCount = 1 + (rand() % (prodStepCount / (10 + (rand() % 15))));
    RequestType *prodRequests = malloc(sizeof(RequestType) * prodRequestCount);
    int j;
    for (j = 0; j < prodRequestCount; j++) {
        RequestType request = {rand() % prodRequestCount, SYSTEM_REQUEST_TYPE_WRITE_SHARED_MEM /*3 + (rand() % 3)*/};
        prodRequests[j] = request;
    }
    Process *prodProc = Process_init(prodStepCount, prodRequestCount, prodRequests);
    Scheduler_newProcess(cpu->scheduler, prodProc, sharedMemIndex);
    
    int consStepCount = 1 + (rand() % 5000);
    int consRequestCount = 1 + (rand() % (consStepCount / (10 + (rand() % 15))));
    RequestType *consRequests = malloc(sizeof(RequestType) * consRequestCount);
    for (j = 0; j < consRequestCount; j++) {
        RequestType request = {rand() % consRequestCount, SYSTEM_REQUEST_TYPE_READ_SHARED_MEM /*2 + (rand() % 3)*/};
        consRequests[j] = request;
    }
    Process *consProc = Process_init(consStepCount, consRequestCount, consRequests);
    Scheduler_newProcess(cpu->scheduler, consProc, sharedMemIndex);
}

int main(int argc, const char * argv[]) {
    
//    getchar();
    
    int calcProcessCount = 2;
    int ioBoundProcessCount = 2;
    int prodConsPairCount = 2;
    
    CPU *cpu = CPU_init();
    
    srand(time(NULL));
    
    // Initialize UI process
    RequestType kbdRequests[] = {{0,SYSTEM_REQUEST_TYPE_KBD}};
    Process *kbdProc = Process_init(1, 1, kbdRequests);
    Scheduler_newProcess(cpu->scheduler, kbdProc, -1);
    
    // Initialize caluclation processes
    int i;
    for (i = 0; i < calcProcessCount; i++) {
        Process *bkgProc = Process_init(5000 + (rand() % 5000), 0, NULL);
        Scheduler_newProcess(cpu->scheduler, bkgProc, -1);
    }
    
    // Initialize IO bound processes
    for (i = 0; i < ioBoundProcessCount; i++) {
        createIOBoundProcess(cpu);
    }
    
    // Initialize producer/consumer process pairs
    for (i = 0; i < prodConsPairCount; i++) {
        createProdConsPair(cpu, i);
    }
    
    SysTimer *timer = SysTimer_init(1000, cpu);
    
    cpu->runningProcess = PCBQueue_dequeue(cpu->scheduler->readyQueue);
    cpu->runningProcess->state = PCB_STATE_RUNNING;
    
    SysClock *clock = SysClock_init(cpu, timer);
    
    while (1) {
        // Run!  Could use field in SysClock to check if still running...
//        
//        char key = getchar();
//        LinkedBlockingQueue_enqueue(cpu->dvcKbd->inputBuffer, &key);
    }
    
    return 0;
}

