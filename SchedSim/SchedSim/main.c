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

void createProdConsPair(CPU *cpu, int sharedMemIndex) {
    int prodStepCount = rand() % 5000;
    int prodRequestCount = rand() % (prodStepCount / (10 + (rand() % 15)));
    RequestType *prodRequests = malloc(sizeof(RequestType) * prodRequestCount);
    int j;
    for (j = 0; j < prodRequestCount; j++) {
        RequestType request = {rand() % prodRequestCount, 3 + (rand() % 3)};
        prodRequests[j] = request;
        RequestType *prodRequestsPtr = &prodRequests[j];
        printf("RequestType-type: %d\n", prodRequestsPtr->type);
    }
    Process *prodProc = Process_init(prodStepCount, prodRequestCount, prodRequests);
    Scheduler_newProcess(cpu->scheduler, prodProc, sharedMemIndex);
    
    int consStepCount = rand() % 5000;
    int consRequestCount = rand() % (consStepCount / (10 + (rand() % 15)));
    RequestType *consRequests = malloc(sizeof(RequestType) * consRequestCount);
    for (j = 0; j < consRequestCount; j++) {
        RequestType request = {rand() % consRequestCount, 2 + (rand() % 3)};
        consRequests[j] = request;
    }
    Process *consProc = Process_init(consStepCount, consRequestCount, consRequests);
    Scheduler_newProcess(cpu->scheduler, consProc, sharedMemIndex);
}

int main(int argc, const char * argv[]) {
    
    
    int calcProcessCount = 2;
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
    
    // Initialize producer/consumer process pairs
    for (i = 0; i < prodConsPairCount; i++) {
        createProdConsPair(cpu, i);
    }
//    int prodStepCount = rand() % 5000;
//    int prodRequestCount = rand() % (prodStepCount / (10 + (rand() % 15)));
//    RequestType prodRequests[prodRequestCount];
//    int j;
//    for (j = 0; j < prodRequestCount; j++) {
//        RequestType request = {rand() % prodRequestCount, 3 + (rand() % 3)};
//        prodRequests[j] = request;
//        RequestType *prodRequestsPtr = &prodRequests[j];
//        printf("RequestType-type: %d\n", prodRequestsPtr->type);
//    }
//    Process *prodProc = Process_init(prodStepCount, prodRequestCount, prodRequests);
//    Scheduler_newProcess(cpu->scheduler, prodProc, 0);
//    
//    int consStepCount = rand() % 5000;
//    int consRequestCount = rand() % (consStepCount / (10 + (rand() % 15)));
//    RequestType consRequests[consRequestCount];
//    for (j = 0; j < consRequestCount; j++) {
//        RequestType request = {rand() % consRequestCount, 2 + (rand() % 3)};
//        consRequests[j] = request;
//    }
//    Process *consProc = Process_init(consStepCount, consRequestCount, consRequests);
//    Scheduler_newProcess(cpu->scheduler, consProc, 0);
//    
//    prodStepCount = rand() % 5000;
//    prodRequestCount = rand() % (prodStepCount / (10 + (rand() % 15)));
//    RequestType prodRequests2[prodRequestCount];
//    for (j = 0; j < prodRequestCount; j++) {
//        RequestType request = {rand() % prodRequestCount, 3 + (rand() % 3)};
//        prodRequests2[j] = request;
//        RequestType *prodRequestsPtr = &prodRequests2[j];
//        printf("RequestType-type: %d\n", prodRequestsPtr->type);
//    }
//    prodProc = Process_init(prodStepCount, prodRequestCount, prodRequests);
//    Scheduler_newProcess(cpu->scheduler, prodProc, 1);
//    
//    consStepCount = rand() % 5000;
//    consRequestCount = rand() % (consStepCount / (10 + (rand() % 15)));
//    RequestType consRequests2[consRequestCount];
//    for (j = 0; j < consRequestCount; j++) {
//        RequestType request = {rand() % consRequestCount, 2 + (rand() % 3)};
//        consRequests2[j] = request;
//    }
//    consProc = Process_init(consStepCount, consRequestCount, consRequests2);
//    Scheduler_newProcess(cpu->scheduler, consProc, 1);
    
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

