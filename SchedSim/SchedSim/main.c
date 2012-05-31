//  Team: default
//  Names: Bartholomew, Dan
//         Choe, Ju
//         Hardin, Kurt
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

#define MAX_PROCESSES       10

#define CALC_COUNT_ARG      1
#define IO_COUNT_ARG        2
#define PROD_CONS_COUNT_ARG 3

#define PROD_CONS_MIN_STEP_COUNT    2500
#define PROD_CONS_MAX_STEP_COUNT    5000
#define PROD_CONS_MAX_REQUEST_COUNT 50

// creates IO bound processes
// Random step count range: 1 to 5000
// Random request count range: some random percentage of the step count 
void createIOBoundProcess(CPU *cpu) {
    int stepCount = 1 + (rand() % 5000);
    int ioMaxRequests = 1 + (stepCount / (10 + (rand() % 15)));
    int ioRequestCount = 1 + (rand() % ioMaxRequests);
    RequestType *ioRequests = malloc(sizeof(RequestType) * ioRequestCount);
    int j;
    for (j = 0; j < ioRequestCount; j++) {
        RequestType request = {rand() % ioRequestCount, 3 + (rand() % 2)};
        ioRequests[j] = request;
    }
    Process *ioProc = Process_init(stepCount, ioRequestCount, ioRequests);
    Scheduler_newProcess(cpu->scheduler, ioProc, -1);
}

// creates pairs of consumer and producer processes
// Random step count range: 2500 to 5000
// Random request count range: 1 to 50
void createProdConsPair(CPU *cpu, int sharedMemIndex) {
    int prodStepCount = PROD_CONS_MIN_STEP_COUNT + (rand() % PROD_CONS_MAX_STEP_COUNT - PROD_CONS_MIN_STEP_COUNT);
    int prodRequestCount = 1 + (rand() % PROD_CONS_MAX_REQUEST_COUNT);
    RequestType *prodRequests = malloc(sizeof(RequestType) * prodRequestCount);
    int j;
    for (j = 0; j < prodRequestCount; j++) {
        RequestType request = {rand() % prodRequestCount, SYSTEM_REQUEST_TYPE_WRITE_SHARED_MEM /*3 + (rand() % 3)*/};
        prodRequests[j] = request;
    }
    Process *prodProc = Process_init(prodStepCount, prodRequestCount, prodRequests);
    Scheduler_newProcess(cpu->scheduler, prodProc, sharedMemIndex);
    
    int consStepCount = PROD_CONS_MIN_STEP_COUNT + (rand() % PROD_CONS_MAX_STEP_COUNT - PROD_CONS_MIN_STEP_COUNT);
    int consRequestCount = 1 + (rand() % PROD_CONS_MAX_REQUEST_COUNT);
    RequestType *consRequests = malloc(sizeof(RequestType) * consRequestCount);
    for (j = 0; j < consRequestCount; j++) {
        RequestType request = {rand() % consRequestCount, SYSTEM_REQUEST_TYPE_READ_SHARED_MEM /*2 + (rand() % 3)*/};
        consRequests[j] = request;
    }
    Process *consProc = Process_init(consStepCount, consRequestCount, consRequests);
    Scheduler_newProcess(cpu->scheduler, consProc, sharedMemIndex);
}

// main
int main(int argc, const char * argv[]) {
    
    int calcProcessCount = 2;
    int ioBoundProcessCount = 3;
    int prodConsPairCount = 2;
    
    int remainingProcessCount = MAX_PROCESSES - 1;
    
    int i;
    for (i = 1; i < argc; i++) {
        switch (i) {
            case CALC_COUNT_ARG:
                calcProcessCount = atoi(argv[i]);
                if (calcProcessCount < 1) {
                    calcProcessCount = 1;
                } else if (calcProcessCount > remainingProcessCount) {
                    calcProcessCount = remainingProcessCount;
                }
                remainingProcessCount -= calcProcessCount;
                break;
                
            case IO_COUNT_ARG:
                ioBoundProcessCount = atoi(argv[i]);
                if (ioBoundProcessCount > remainingProcessCount) {
                    ioBoundProcessCount = remainingProcessCount;
                }
                remainingProcessCount -= ioBoundProcessCount;
                break;
                
            case PROD_CONS_COUNT_ARG:
                prodConsPairCount = atoi(argv[i]);
                if (prodConsPairCount * 2 > remainingProcessCount) {
                    prodConsPairCount = remainingProcessCount / 2;
                }
                remainingProcessCount -= (prodConsPairCount * 2);
                break;
                
            default:
                break;
        }
    }
    
    CPU *cpu = CPU_init(prodConsPairCount);
    
    SchedSimGUI_printLogMessage((SchedSimGUI *) cpu->gui, -1, -1, "SchedSim started");
    
    srand(time(NULL));
    
    // Initialize UI process
    RequestType kbdRequests[] = {{0,SYSTEM_REQUEST_TYPE_KBD}};
    Process *kbdProc = Process_init(1, 1, kbdRequests);
    Scheduler_newProcess(cpu->scheduler, kbdProc, -1);
    
    // Initialize caluclation processes
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
    
    SysClock_init(cpu, timer);
    
    while (CPU_isRunning(cpu));
    
    SchedSimGUI_printLogMessage((SchedSimGUI *) cpu->gui, -1, -1, "SchedSim stopped");
    
    endwin();
    
    return 0;
}

