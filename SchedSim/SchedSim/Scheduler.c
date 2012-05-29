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
    SchedSimGUI_printLogMessage((SchedSimGUI *) scheduler->cpu->gui, LOG_TYPE_PROC, pcb->pid, "added to ready queue");
}

void Scheduler_handleInterrupt(Scheduler *scheduler, PCB *src, int type) {
    PCB *runningPcb = scheduler->cpu->runningProcess;
    char *key;
    char msg[150];
    int pid;
    switch (type) {
        case INTERRUPT_TYPE_TIMER:
            sprintf(msg, "time slice up");
            pid = runningPcb->pid;
            moveToReadyQueue(scheduler, runningPcb);
            loadNextProcess(scheduler);
            break;
            
        case INTERRUPT_TYPE_KBD:
            key = LinkedBlockingQueue_dequeue(scheduler->cpu->dvcKbd->inputBuffer);
            pid = src->pid;
            sprintf(msg, "recieved character from keyboard (%c)", *key);
            moveToReadyQueue(scheduler, src);
            break;
            
        case INTERRUPT_TYPE_IO:
            sprintf(msg, "unblocked by IO interrupt");
            pid = src->pid;
            moveToReadyQueue(scheduler, src);
            break;
            
        default:
            break;
    }
    SchedSimGUI_printLogMessage((SchedSimGUI *) scheduler->cpu->gui, LOG_TYPE_PROC, pid, msg);
}

void loadNextProcess(Scheduler *scheduler) {
    PCB * nextProcess = PCBQueue_dequeue(scheduler->readyQueue);
    scheduler->cpu->runningProcess = nextProcess;
    nextProcess->state = PCB_STATE_RUNNING;
    scheduler->cpu->ip = nextProcess->nextStep;

    SchedSimGUI_printLogMessage((SchedSimGUI *)scheduler->cpu->gui, LOG_TYPE_PROC, nextProcess->pid, "switched to running");
    if (nextProcess->waitingOn == SHARED_MEM_MODE_READ) {
        nextProcess->waitingOn = 0;
        SchedSimGUI_printLogMessage((SchedSimGUI *)scheduler->cpu->gui, LOG_TYPE_PROC, nextProcess->pid, "read from shared memory");
    } else if (nextProcess->waitingOn == SHARED_MEM_MODE_WRITE) {
        nextProcess->waitingOn = 0;
        SchedSimGUI_printLogMessage((SchedSimGUI *)scheduler->cpu->gui, LOG_TYPE_PROC, nextProcess->pid, "wrote to shared memory");
    }
    SchedSimGUI_updateProcessWindow((SchedSimGUI *) scheduler->cpu->gui);
    if (scheduler->cpu->timer != NULL) {
        SysTimer_reset((SysTimer *)scheduler->cpu->timer);
    }
    
}