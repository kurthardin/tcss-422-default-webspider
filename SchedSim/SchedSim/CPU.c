//
//  CPU.c
//  SchedSim
//
//  Created by Kurt Hardin on 5/17/12.
//  Copyright (c) 2012 University of Washington at Tacoma. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

#include "CPU.h"
#include "SysTimer.h"
#include "Scheduler.h"
#include "IODevice.h"
#include "KBDDevice.h"
#include "GUI.h"

Interrupt * Interrupt_init(int type, PCB *src) {
    Interrupt *interrupt = malloc(sizeof(Interrupt));
    interrupt->type = type;
    interrupt->src = src;
    return interrupt;
}


void checkForInterrupt(CPU *);
void checkForSystemRequest(CPU *, int);

CPU * CPU_init() {
    CPU *cpu = malloc(sizeof(CPU));
    cpu->ip = 0;
    cpu->runningProcess = NULL;
    cpu->timer = NULL;
    int i;
    for (i = 0; i < INTERRUPT_PRIORITY_COUNT; i++) {
        cpu->interruptQueues[i] = LinkedBlockingQueue_init();
    }
    cpu->scheduler = Scheduler_init(cpu);
    cpu->dvcDisk = IODevice_init(LOG_TYPE_DISK, cpu);
    cpu->dvcVid = IODevice_init(LOG_TYPE_VID, cpu);
    cpu->dvcKbd = KBDDevice_init(cpu);
    
    for (i = 0; i < NUMBER_SHARED_MEMORY; i++) {
        cpu->sharedMemory[i] = SharedMemory_init();
    }
    
    cpu->gui = (struct SchedSimGUI *) SchedSimGUI_init(cpu);
    
    return cpu;
}

void CPU_signalInterrupt(CPU *cpu, Interrupt *interrupt) {
    LinkedBlockingQueue_enqueue(cpu->interruptQueues[interrupt->type], interrupt);
}

void CPU_step(CPU *cpu) {
    
    
    checkForInterrupt(cpu);
    
    int ip = cpu->ip;
    cpu->ip++;
    if (cpu->ip > cpu->runningProcess->proc->noSteps - 1) {
        cpu->ip = 0;
    }
    checkForSystemRequest(cpu, ip);
    
}

void checkForInterrupt(CPU *cpu) {
    Interrupt *interrupt = NULL;
    int i;
    for (i = 0; i < INTERRUPT_PRIORITY_COUNT; i++) {
        interrupt = LinkedBlockingQueue_dequeue(cpu->interruptQueues[i]);
        if (interrupt != NULL) {
            cpu->runningProcess->nextStep = cpu->ip; //handleInterrupt(cpu, interrupt);
            Scheduler_handleInterrupt(cpu->scheduler, interrupt->src, interrupt->type);
            break;
        }
    }
}

void checkForSystemRequest(CPU *cpu, int ip) {
    Process *proc = cpu->runningProcess->proc;
    RequestType request;
    int i;
    for (i = 0; i < proc->noRequests; i++) {
        request = cpu->runningProcess->proc->requests[i];
        if (request.step == ip) {
            CPU_systemRequest(cpu, request.type);
            break;
        }
    }
}

void handleKeyboardSystemRequest(CPU *cpu) {
    cpu->runningProcess->nextStep = cpu->ip;
    cpu->runningProcess->state = PCB_STATE_BLOCKED;
    PCBQueue_enqueue(cpu->dvcKbd->blockedQueue, cpu->runningProcess);
    SchedSimGUI_printLogMessage((SchedSimGUI *) cpu->gui, LOG_TYPE_KBD, cpu->runningProcess->pid, "blocked on keyboard request");
}

void handleIOSystemRequest(CPU *cpu, int type) {
    cpu->runningProcess->nextStep = cpu->ip;
    cpu->runningProcess->state = PCB_STATE_BLOCKED;
    PCBQueue *blockedQueue = NULL;
    if (type == SYSTEM_REQUEST_TYPE_IO_VID) {
        blockedQueue = cpu->dvcVid->blockedQueue;
    } else if (type == SYSTEM_REQUEST_TYPE_IO_DISK) {
        blockedQueue = cpu->dvcDisk->blockedQueue;
    }
    if (blockedQueue != NULL) {
        PCBQueue_enqueue(blockedQueue, cpu->runningProcess);
    }
    SchedSimGUI_printLogMessage((SchedSimGUI *) cpu->gui, LOG_TYPE_PROC, cpu->runningProcess->pid, "blocked on IO request");
    SchedSimGUI_updateDeviceWindow((SchedSimGUI *) cpu->gui);
}

void handleSharedMemoryRead(CPU *cpu, int memRef) {
    
    SharedMemory *sharedMemory = cpu->sharedMemory[memRef];
    if (sharedMemory->mode == SHARED_MEM_MODE_READ && 
        PCBQueue_getSize(sharedMemory->mutexReadBlockedQueue) == 0) {
        
        sharedMemory->owner = cpu->runningProcess;

        SchedSimGUI_printLogMessage((SchedSimGUI *) cpu->gui, LOG_TYPE_PROC, cpu->runningProcess->pid, "acquired mutex lock");
        SchedSimGUI_printLogMessage((SchedSimGUI *) cpu->gui, LOG_TYPE_PROC, cpu->runningProcess->pid, "read from shared memory");
        SchedSimGUI_printLogMessage((SchedSimGUI *) cpu->gui, LOG_TYPE_PROC, cpu->runningProcess->pid, "released mutex lock");
        
        Scheduler_unblockProcessWaitingOnSharedMemery(cpu->scheduler, memRef);
    } else {
        PCBQueue_enqueue(sharedMemory->mutexReadBlockedQueue, cpu->runningProcess);
        cpu->runningProcess->state = PCB_STATE_BLOCKED;
        cpu->runningProcess->waitingOn = SHARED_MEM_MODE_READ;
        SchedSimGUI_printLogMessage((SchedSimGUI *) cpu->gui, LOG_TYPE_PROC, cpu->runningProcess->pid, "blocked on shared memory mutex");
    }
}

void handleSharedMemoryWrite(CPU *cpu, int memRef) {
    
    SharedMemory *sharedMemory = cpu->sharedMemory[memRef];
    if (sharedMemory->mode == SHARED_MEM_MODE_WRITE && 
        PCBQueue_getSize(sharedMemory->mutexWriteBlockedQueue) == 0) {
        
        sharedMemory->owner = cpu->runningProcess;
        SchedSimGUI_printLogMessage((SchedSimGUI *) cpu->gui, LOG_TYPE_PROC, sharedMemory->owner->pid, "acquired mutex lock");
        SchedSimGUI_printLogMessage((SchedSimGUI *) cpu->gui, LOG_TYPE_PROC, sharedMemory->owner->pid, "wrote to shared memory");
        SchedSimGUI_printLogMessage((SchedSimGUI *) cpu->gui, LOG_TYPE_PROC, cpu->runningProcess->pid, "released mutex lock");
        
        Scheduler_unblockProcessWaitingOnSharedMemery(cpu->scheduler, memRef);
    } else {
        PCBQueue_enqueue(sharedMemory->mutexWriteBlockedQueue, cpu->runningProcess);
        cpu->runningProcess->state = PCB_STATE_BLOCKED;
        cpu->runningProcess->waitingOn = SHARED_MEM_MODE_WRITE;
        SchedSimGUI_printLogMessage((SchedSimGUI *) cpu->gui, LOG_TYPE_PROC, cpu->runningProcess->pid, "blocked on shared memory mutex");
    }
}

void CPU_systemRequest(CPU *cpu, int type) {
    int index = cpu->runningProcess->mem_ref;
    switch (type) {
        
        case SYSTEM_REQUEST_TYPE_KBD:
            handleKeyboardSystemRequest(cpu);
            break;
            
        case SYSTEM_REQUEST_TYPE_IO_VID:
        case SYSTEM_REQUEST_TYPE_IO_DISK:
            handleIOSystemRequest(cpu, type);
            break;
            
        case SYSTEM_REQUEST_TYPE_READ_SHARED_MEM:
            handleSharedMemoryRead(cpu, index);
            break;
            
        case SYSTEM_REQUEST_TYPE_WRITE_SHARED_MEM:
            handleSharedMemoryWrite(cpu, index);
            break;
            
        default:
            break;
    }
    
    cpu->runningProcess->nextStep = cpu->ip;
    Scheduler_handleSystemRequest(cpu->scheduler);
}