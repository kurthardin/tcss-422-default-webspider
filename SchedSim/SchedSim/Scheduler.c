//
//  Scheduler.c
//  SchedSim
//
//  Created by Kurt Hardin on 5/20/12.
//  Copyright (c) 2012 University of Washington at Tacoma. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

#include "Scheduler.h"
#include "SysTimer.h"
#include "PCB.h"
#include "KBDDevice.h"

void loadNextProcess(Scheduler *);

Scheduler * Scheduler_init(CPU *cpu) {
    Scheduler *sched = malloc(sizeof(Scheduler));
    sched->cpu = cpu;
    sched->processList = malloc(sizeof(PCB *) * MAX_PROCESSES);
    sched->processCount = 0;
    sched->readyQueue = PCBQueue_init();
    return sched;
}

void Scheduler_newProcess(Scheduler *scheduler, Process *proc, int index) {
    if (scheduler->processCount < MAX_PROCESSES) {
        PCB *newPcb = PCB_init(scheduler->processCount, proc);
        newPcb->mem_ref = index;
        scheduler->processList[scheduler->processCount] = newPcb;
        scheduler->processCount++;
        PCBQueue_enqueue(scheduler->readyQueue, newPcb);
    }
}

void Scheduler_handleSystemRequest(Scheduler *scheduler) {
    if (scheduler->cpu->runningProcess->state == PCB_STATE_BLOCKED) {
        loadNextProcess(scheduler);
    }
}

void moveToReadyQueue(Scheduler *scheduler, PCB *pcb) {
    pcb->state = PCB_STATE_READY;
    PCBQueue_enqueue(scheduler->readyQueue, pcb);
    printf("Process %d: Added to ready queue\n", pcb->pid);
}

void Scheduler_handleInterrupt(Scheduler *scheduler, PCB *src, int type) {
    PCB *runningPcb = scheduler->cpu->runningProcess;
    PCB *unblockedPCB;
    char *key;
    switch (type) {
        case INTERRUPT_TYPE_TIMER:
            printf("Process %d: Time slice up\n", runningPcb->pid);
            moveToReadyQueue(scheduler, runningPcb);
            loadNextProcess(scheduler);
            break;
            
        case INTERRUPT_TYPE_KBD:
            key = LinkedBlockingQueue_dequeue(scheduler->cpu->dvcKbd->inputBuffer);
            unblockedPCB = PCBQueue_dequeue(scheduler->cpu->dvcKbd->blockedQueue);
            printf("Process %d: recieved character from keyboard (%c)\n", unblockedPCB->pid, *key);
            moveToReadyQueue(scheduler, unblockedPCB);
            break;
            
        case INTERRUPT_TYPE_IO:
            unblockedPCB = src;
            printf("Process %d: Unblocked by IO interrupt\n", unblockedPCB->pid);
            moveToReadyQueue(scheduler, unblockedPCB);
            break;
            
        default:
            break;
    }
}

void loadNextProcess(Scheduler *scheduler) {
    PCB * nextProcess = PCBQueue_dequeue(scheduler->readyQueue);
    scheduler->cpu->runningProcess = nextProcess;
    nextProcess->state = PCB_STATE_RUNNING;
    scheduler->cpu->ip = nextProcess->nextStep;
    printf("Process %d: Switched to running\n", nextProcess->pid);
    if (nextProcess->waitingOn == SHARED_MEM_MODE_READ) {
        nextProcess->waitingOn = 0;
        printf("Process %d: Read from shared memory\n", nextProcess->pid);
    } else if (nextProcess->waitingOn == SHARED_MEM_MODE_WRITE) {
        nextProcess->waitingOn = 0;
        printf("Process %d: Read from shared memory\n", nextProcess->pid);
    }
    if (scheduler->cpu->timer != NULL) {
        SysTimer_reset((SysTimer *)scheduler->cpu->timer);
    }
    
}