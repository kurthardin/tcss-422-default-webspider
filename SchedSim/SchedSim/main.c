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

int main(int argc, const char * argv[]) {
    
    CPU *cpu = CPU_init();
    
    srand(time(NULL));
    
    RequestType kbdRequests[] = {{0,SYSTEM_REQUEST_TYPE_KBD}};
    Process *kbdProc = Process_init(1, 1, kbdRequests);
    Scheduler_newProcess(cpu->scheduler, kbdProc);
    
    Process *bkgProc = Process_init(5000 + (rand() % 5000), 0, NULL);
    Scheduler_newProcess(cpu->scheduler, bkgProc);
    
    int prodStepCount = rand() % 5000;
    int prodRequestCount = rand() % (prodStepCount / (10 + (rand() % 15)));
    RequestType prodRequests[prodRequestCount];
    int i;
    for (i = 0; i < prodRequestCount; i++) {
        RequestType request = {rand() % prodRequestCount, 2 + (rand() % 3)};
        prodRequests[i] = request;
    }
    Process *prodProc = Process_init(prodStepCount, prodRequestCount, prodRequests);
    Scheduler_newProcess(cpu->scheduler, prodProc);
    
    int consStepCount = rand() % 5000;
    int consRequestCount = rand() % (consStepCount / (10 + (rand() % 15)));
    RequestType consRequests[consRequestCount];
    for (i = 0; i < consRequestCount; i++) {
        RequestType request = {rand() % consRequestCount, 1 + (rand() % 3)};
        consRequests[i] = request;
    }
    Process *consProc = Process_init(consStepCount, consRequestCount, consRequests);
    Scheduler_newProcess(cpu->scheduler, consProc);
    
    SysTimer *timer = SysTimer_init(1000, cpu);
    
    cpu->runningProcess = PCBQueue_dequeue(cpu->scheduler->readyQueue);
    cpu->runningProcess->state = PCB_STATE_RUNNING;
    
    SysClock *clock = SysClock_init(cpu, timer);
    
    while (1) {
        // Run!  Could use field in SysClock to check if still running...
    }
    
    return 0;
}

