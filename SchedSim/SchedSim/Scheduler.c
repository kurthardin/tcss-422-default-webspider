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
#include "GUI.h"

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
    SchedSimGUI_updateProcessWindow((SchedSimGUI *) scheduler->cpu->gui);
    SchedSimGUI_printLogMessage((SchedSimGUI *) scheduler->cpu->gui, 
                                LOG_TYPE_PROC, 
                                pcb->pid, 
                                "added to ready queue");
}

void Scheduler_handleInterrupt(Scheduler *scheduler, PCB *src, int type) {
    PCB *runningPcb = scheduler->cpu->runningProcess;
    switch (type) {
        case INTERRUPT_TYPE_TIMER:
            SchedSimGUI_printLogMessage((SchedSimGUI *) scheduler->cpu->gui, 
                                        LOG_TYPE_PROC, 
                                        runningPcb->pid, 
                                        "time slice up");
            moveToReadyQueue(scheduler, runningPcb);
            loadNextProcess(scheduler);
            break;
            
        case INTERRUPT_TYPE_KBD:
            src->waitingOn = PCB_WAITING_ON_KEYBOARD_INPUT;
            SchedSimGUI_printLogMessage((SchedSimGUI *) scheduler->cpu->gui, 
                                        LOG_TYPE_PROC, 
                                        src->pid, 
                                        "unblocked by keyboard interrupt");
            moveToReadyQueue(scheduler, src);
            break;
            
        case INTERRUPT_TYPE_IO:
            SchedSimGUI_printLogMessage((SchedSimGUI *) scheduler->cpu->gui, 
                                        LOG_TYPE_PROC, 
                                        src->pid, 
                                        "unblocked by IO interrupt");
            moveToReadyQueue(scheduler, src);
            break;
            
        default:
            break;
    }
    SchedSimGUI_updateDeviceWindow((SchedSimGUI *) scheduler->cpu->gui);
}

void loadNextProcess(Scheduler *scheduler) {
    PCB * nextProcess = PCBQueue_dequeue(scheduler->readyQueue);
    scheduler->cpu->runningProcess = nextProcess;
    nextProcess->state = PCB_STATE_RUNNING;
    scheduler->cpu->ip = nextProcess->nextStep;
    SchedSimGUI_updateProcessWindow((SchedSimGUI *) scheduler->cpu->gui);
    
    SchedSimGUI_printLogMessage((SchedSimGUI *)scheduler->cpu->gui, LOG_TYPE_PROC, nextProcess->pid, "switched to running");
    switch (nextProcess->waitingOn) {
        case PCB_WAITING_ON_SHARED_MEM_READ:
            Scheduler_unblockProcessWaitingOnSharedMemery(scheduler, nextProcess->mem_ref);
            break;
            
        case PCB_WAITING_ON_SHARED_MEM_WRITE:
            Scheduler_unblockProcessWaitingOnSharedMemery(scheduler, nextProcess->mem_ref);
            break;
            
        case PCB_WAITING_ON_KEYBOARD_INPUT:
            CPU_getKeyFromKeyboard(scheduler->cpu);
            break;
            
        default:
            break;
    }
    nextProcess->waitingOn = PCB_WAITING_ON_NONE;
    if (scheduler->cpu->timer != NULL) {
        SysTimer_reset((SysTimer *)scheduler->cpu->timer);
    }
    
}

void Scheduler_unblockProcessWaitingOnSharedMemery(Scheduler * scheduler, int memRef) {
    SharedMemory *sharedMemory = scheduler->cpu->sharedMemory[memRef];
    char acquiredMutexMsg[150];
    sprintf(acquiredMutexMsg, "process %d acquired mutex lock", sharedMemory->owner->pid);
    char releasedMutexMsg[150];
    sprintf(releasedMutexMsg, "process %d released mutex lock", sharedMemory->owner->pid);
    if (sharedMemory->mode == SHARED_MEM_MODE_READ) {
        SchedSimGUI_printLogMessage((SchedSimGUI *) scheduler->cpu->gui, 
                                    LOG_TYPE_MEM, 
                                    memRef, 
                                    acquiredMutexMsg);
        SchedSimGUI_printLogMessage((SchedSimGUI *) scheduler->cpu->gui, 
                                    LOG_TYPE_PROC, 
                                    sharedMemory->owner->pid, 
                                    "read from shared memory");
        SchedSimGUI_printLogMessage((SchedSimGUI *) scheduler->cpu->gui, 
                                    LOG_TYPE_MEM, 
                                    memRef, 
                                    releasedMutexMsg);
        sharedMemory->mode = SHARED_MEM_MODE_WRITE;
        sharedMemory->owner = PCBQueue_dequeue(sharedMemory->mutexWriteBlockedQueue);
    } else if (sharedMemory->mode == SHARED_MEM_MODE_WRITE) {
        SchedSimGUI_printLogMessage((SchedSimGUI *) scheduler->cpu->gui, 
                                    LOG_TYPE_MEM, 
                                    memRef, 
                                    acquiredMutexMsg);
        SchedSimGUI_printLogMessage((SchedSimGUI *) scheduler->cpu->gui, 
                                    LOG_TYPE_PROC, 
                                    sharedMemory->owner->pid, 
                                    "wrote to shared memory");
        SchedSimGUI_printLogMessage((SchedSimGUI *) scheduler->cpu->gui, 
                                    LOG_TYPE_MEM, 
                                    memRef, 
                                    releasedMutexMsg);
        sharedMemory->mode = SHARED_MEM_MODE_READ;
        sharedMemory->owner = PCBQueue_dequeue(sharedMemory->mutexReadBlockedQueue);
    }
    
    if (sharedMemory->owner != NULL) {
        sharedMemory->owner->mem_ref = memRef;
        moveToReadyQueue(scheduler, sharedMemory->owner);
    }
}