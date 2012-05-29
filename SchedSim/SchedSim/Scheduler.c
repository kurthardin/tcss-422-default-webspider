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
    char msg[150];
    sprintf(msg, "Process %d: Added to ready queue", pcb->pid);
    SchedSimGUI_updateProcessWindow((SchedSimGUI *) scheduler->cpu->gui);
    SchedSimGUI_printLogMessage((SchedSimGUI *) scheduler->cpu->gui, msg);
}

void Scheduler_handleInterrupt(Scheduler *scheduler, PCB *src, int type) {
    PCB *runningPcb = scheduler->cpu->runningProcess;
    char *key;
    char msg[150];
    switch (type) {
        case INTERRUPT_TYPE_TIMER:
            sprintf(msg, "Process %d: Time slice up", runningPcb->pid);
            moveToReadyQueue(scheduler, runningPcb);
            loadNextProcess(scheduler);
            break;
            
        case INTERRUPT_TYPE_KBD:
            key = LinkedBlockingQueue_dequeue(scheduler->cpu->dvcKbd->inputBuffer);
            sprintf(msg, "Process %d: recieved character from keyboard (%c)", src->pid, *key);
            moveToReadyQueue(scheduler, src);
            break;
            
        case INTERRUPT_TYPE_IO:
            sprintf(msg, "Process %d: Unblocked by IO interrupt", src->pid);
            moveToReadyQueue(scheduler, src);
            break;
            
        default:
            break;
    }
    SchedSimGUI_printLogMessage((SchedSimGUI *) scheduler->cpu->gui, msg);
}

void loadNextProcess(Scheduler *scheduler) {
    PCB * nextProcess = PCBQueue_dequeue(scheduler->readyQueue);
    scheduler->cpu->runningProcess = nextProcess;
    nextProcess->state = PCB_STATE_RUNNING;
    scheduler->cpu->ip = nextProcess->nextStep;
    char msg[150];
    sprintf(msg, "Process %d: Switched to running", nextProcess->pid);
    SchedSimGUI_printLogMessage((SchedSimGUI *)scheduler->cpu->gui, msg);
    if (nextProcess->waitingOn == SHARED_MEM_MODE_READ) {
        nextProcess->waitingOn = 0;
        sprintf(msg, "Process %d: Read from shared memory", nextProcess->pid);
        SchedSimGUI_printLogMessage((SchedSimGUI *)scheduler->cpu->gui, msg);
    } else if (nextProcess->waitingOn == SHARED_MEM_MODE_WRITE) {
        nextProcess->waitingOn = 0;
        sprintf(msg, "Process %d: Read from shared memory", nextProcess->pid);
        SchedSimGUI_printLogMessage((SchedSimGUI *)scheduler->cpu->gui, msg);
    }
    SchedSimGUI_updateProcessWindow((SchedSimGUI *) scheduler->cpu->gui);
    if (scheduler->cpu->timer != NULL) {
        SysTimer_reset((SysTimer *)scheduler->cpu->timer);
    }
    
}